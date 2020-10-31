#include "Simulation.h"
#include "../force/GravityForce.h"
#include "../force/SpringForce.h"
#include "../force/DragForce.h"
#include "../force/MuscleForce.h"
#include "../force/MultiMuscleForce.h"
#include "ExplicitEuler.h"
#include "SymplecticEuler.h"
#include "Utils.h"
#include "../collision/CollisionHandler.h"
#include <memory>
#include <thread>
#include <chrono>
#include <math.h>
#include <algorithm>
#include <fstream>
#include "Utils.h"
#include <Eigen/Core>
#include <Eigen/StdVector>
#include "../../build.h"
#include <Creature.h>

using namespace std;
using namespace std::chrono;
double Simulation::PLAYBACK_FRAMERATE = 60;
double Simulation::PARTICLE_MASS = 1;
atomic<int> Simulation::evaluations(0);

Simulation::Simulation(int particleCount, string name, string outputPath) : m_scene(particleCount)
{
    m_output.clear();
    m_output["particleCount"] = particleCount;
    m_output["particleRadius"] = Scene::PARTICLE_RADIUS;
    m_output["name"] = name;
    m_name = name;
    m_outputPath = outputPath;
}

Simulation::Simulation(nlohmann::json &mesh, string name, string outputPath) : m_scene(mesh["vertices"].size())
{
    int particleCount = mesh["vertices"].size();
    m_output.clear();
    m_output["particleCount"] = particleCount;
    m_output["particleRadius"] = Scene::PARTICLE_RADIUS;
    m_output["mesh"] = mesh;
    m_output["name"] = name;
    m_name = name;
    m_outputPath = outputPath;
    for (int i = 0; i < particleCount; i++)
    {
        auto vertex = mesh["vertices"][i];
        double y = vertex["y"];
        y += 0.3;
        double x = vertex["x"];
        x -= 3;
        m_scene.SetPosition(i, Vector3(x, y, vertex["z"]));
        m_scene.SetVelocity(i, Vector3(6, 0, -1));
    }
    double spring_k = 100 * particleCount;
    double spring_dampening = 10;

    for (auto &edge : mesh["edges"])
    {
        int i = edge["i"];
        int j = edge["j"];
        m_scene.AddEdge(i, j);
        m_scene.AddForce(
            make_shared<SpringForce>(spring_k, edge["length"], spring_dampening, i, j));
    }

    m_scene.AddForce(make_shared<GravityForce>(Vector3(0, -9.81, 0)));
    m_scene.AddForce(make_shared<DragForce>(1));
    dt = 0.00001;
    maxT = 3;
}

Simulation::Simulation(Creature &creature, string outputPath)
{
    nlohmann::json creatureOutput;
    creature.Get(creatureOutput);

    nlohmann::json mesh = creatureOutput["mesh"];
    int particleCount = mesh["vertices"].size();
    m_output.clear();
    m_output["particleCount"] = particleCount;
    m_scene = Scene(particleCount);
    m_output["particleRadius"] = Scene::PARTICLE_RADIUS;
    m_output["mesh"] = mesh;
    m_output["center"] = creatureOutput["center"];
    m_output["name"] = creature.Name();
    m_name = creature.Name();
    m_outputPath = outputPath;
    double lowest_y = 1000;
    for (int i = 0; i < particleCount; i++)
    {
        auto vertex = mesh["vertices"][i];
        lowest_y = min(lowest_y, vertex["y"].get<double>());
        m_scene.SetPosition(i, Vector3(
                                   vertex["x"].get<double>(),
                                   vertex["y"].get<double>(),
                                   vertex["z"].get<double>()));
    }
    Vector3 translation(0, -lowest_y + Scene::PARTICLE_RADIUS + 0.1, 0);
    m_scene.TranslateAllPoints(translation);

    double spring_k = creature.m_spring_k;
    double spring_dampening = creature.m_spring_dampening;
    double frequency = creature.m_frequency;
    double maxAmplitude = creature.m_maxAmplitude;
    double minAmplitude = creature.m_minAmplitude;
    double maxPhase = creature.m_maxPhase;

    for (auto &edge : mesh["edges"])
    {
        int i = edge["i"];
        int j = edge["j"];
        m_scene.AddEdge(i, j);
        vector<double> frequencies = {15};
        vector<double> amplitudes = edge["amplitudes"];
        vector<double> phases = edge["phases"];
        for (int i = 0; i < MUSCLE_ORDER; i++)
        {
            amplitudes[i] = minAmplitude + amplitudes[i] * (maxAmplitude - minAmplitude);
            phases[i] *= maxPhase;
        }
        m_scene.AddForce(
            make_shared<MultiMuscleForce>(
                spring_k, edge["length"], spring_dampening,
                i, j,
                frequencies,
                amplitudes,
                phases, this));
    }

    m_scene.AddForce(make_shared<GravityForce>(Vector3(0, -9.81, 0)));
    m_scene.AddForce(make_shared<DragForce>(1));

    dt = 0.00001;
    maxT = 0.1;
}

void Simulation::SaveOutput()
{
    ofstream f;
    if (!CheckOrCreateDirectory(m_outputPath))
    {
        cout << "ERROR: Can't create output directory " << m_outputPath << endl;
        return;
    }
    f.open(m_outputPath + m_name + ".sim");
    if (f.is_open())
    {
        f << m_output;
    }
    else
        cout << "Error opening " << m_outputPath + m_name + ".sim" << endl;
    f.close();
    cout << "Saved " << m_outputPath + m_name + ".sim" << endl;
}

void Simulation::Step()
{
    ExplicitEuler::StepScene(m_scene, dt);
    t += dt;
}
// #define LOG_SIMULATION
future<void> Simulation::Play()
{
    shouldStop = false;
    t = 0;
    return async(launch::async, [&]() {
#ifdef LOG_SIMULATION
        cout << m_output["name"] << ": Starting Simulation" << endl;
#endif
        double saveInterval = 1 / PLAYBACK_FRAMERATE;
        double nextSaveTime = 0;
#ifdef LOG_SIMULATION
        double logInterval = 1.0 / 10.0;
        double nextLogTime = 0;
#endif
        startingPos = m_scene.GetCenter();
        while (t < maxT && !shouldStop)
        {
            Step();
            if (t > nextSaveTime)
            {
                nlohmann::json frame;
                frame["t"] = t;
                vector<double> pos;
                auto &scenePos = m_scene.getX();
                pos.resize(scenePos.size());
                VectorXd::Map(&pos[0], scenePos.size()) = scenePos;
                frame["positions"] = pos;
#ifdef LOG_ENERGY
                frame["kinetic_energy"] = m_scene.ComputeKineticEnergy();
                frame["gravity_potential_energy"] = m_scene.ComputeEnergy("12GravityForce");
                frame["spring_potential_energy"] = m_scene.ComputeEnergy("11SpringForce");
#endif
                m_output["simulation"].push_back(frame);
                nextSaveTime += saveInterval;
            }
#ifdef LOG_SIMULATION
            if (t > nextLogTime)
            {
                cout << m_output["name"] << ": " << t * 100 / maxT << endl;
                nextLogTime += logInterval;
            }
#endif
        }
        finalPos = m_scene.GetCenter();
        evaluations++;
#ifdef LOG_SIMULATION
        if (t >= maxT)
            cout
                << m_name << ": Done" << endl;
        else if (shouldStop)
            cout << m_name << ": Stopped because of SIGKILL" << endl;
#endif
    });
}

string Simulation::ToString() const
{
    return m_scene.ToString();
}

string Simulation::SetUpToString() const
{
    return m_scene.SetUpToString();
}