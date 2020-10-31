#include "Creature.h"
#include <chrono>
#include <Eigen/Core>
#include <Eigen/StdVector>
#include <fstream>
#include <string>
#include <Voxelize.hpp>
#include "../engine/core/Simulation.h"
#include "../build.h"
#include <math.h>

using namespace std;
using namespace std::chrono;

#pragma region Initialization

atomic_uint Creature::next_id(0);

Creature::Creature() : m_morphologyNetwork(3, 10, 1, 10),
                       m_policyNetwork(3, 6, MUSCLE_ORDER * 2, 6)
{
    m_meshReady = false;

    m_name = GenerateName();
    m_domainBounds = DEFAULT_DOMAIN_SIZE;
    m_resolution = DEFAULT_RESOLUTION;
    m_id = next_id++;

    m_spring_k = DEFAULT_SPRING_K;
    m_spring_dampening = DEFAULT_SPRING_DAMP;
    m_frequency = 15;
    m_maxAmplitude = 0.25;
    m_minAmplitude = -0.1;
    m_maxPhase = 4;
}

Creature::Creature(NeuralNetwork &morphology) : m_morphologyNetwork(morphology),
                                                m_policyNetwork(3, 6, MUSCLE_ORDER * 2, 6)
{
    m_meshReady = false;

    m_name = GenerateName();
    m_domainBounds = DEFAULT_DOMAIN_SIZE;
    m_resolution = DEFAULT_RESOLUTION;
    m_id = next_id++;

    m_spring_k = DEFAULT_SPRING_K;
    m_spring_dampening = DEFAULT_SPRING_DAMP;
    m_frequency = 15;
    m_maxAmplitude = 0.25;
    m_minAmplitude = -0.1;
    m_maxPhase = 4;
}

Creature::Creature(NeuralNetwork &morphology, NeuralNetwork &policy) : m_morphologyNetwork(morphology),
                                                                       m_policyNetwork(policy)
{
    m_meshReady = false;

    m_name = GenerateName();
    m_domainBounds = DEFAULT_DOMAIN_SIZE;
    m_resolution = DEFAULT_RESOLUTION;
    m_id = next_id++;

    m_spring_k = DEFAULT_SPRING_K;
    m_spring_dampening = DEFAULT_SPRING_DAMP;
    m_frequency = 15;
    m_maxAmplitude = 0.25;
    m_minAmplitude = -0.1;
    m_maxPhase = 4;
}

Creature::Creature(const Creature &other) : m_morphologyNetwork(other.m_morphologyNetwork),
                                            m_policyNetwork(other.m_policyNetwork)
{
    m_meshReady = false;
    m_name = other.m_name;
    m_id = next_id++;
    m_domainBounds = other.m_domainBounds;
    m_resolution = other.m_resolution;

    m_spring_k = other.m_spring_k;
    m_spring_dampening = other.m_spring_dampening;
    m_frequency = other.m_frequency;
    m_maxAmplitude = other.m_maxAmplitude;
    m_minAmplitude = other.m_minAmplitude;
    m_maxPhase = other.m_maxPhase;
}

void Creature::RunSimulation(bool saveSimulation)
{
    m_simulationRunning = true;
    m_simulation = async(launch::async, [&, saveSimulation]() {
        Simulation sim(*this, "./");
        auto simFinish = sim.Play();
        simFinish.wait();
        simFinish.get();
        m_simulationRunning = false;
        m_dirty = false;
        m_fitness = sim.AverageVelocity();
        m_simulationOutput = sim.GetOutput();
        if (saveSimulation)
        {
            sim.SaveOutput();
        }
        return m_fitness;
    });
}

string Creature::GenerateName()
{
    std::array<char, 128> buffer;
    string result;
    unique_ptr<FILE, decltype(&pclose)> pipe(popen("names", "r"), pclose);
    if (!pipe)
    {
        throw runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
    {
        result += buffer.data();
    }
    result.erase(std::remove(result.begin(), result.end(), '\n'), result.end());
    if (result.size() == 0)
    {
        cout << "Please install names using \'pip install names\'" << endl;
        exit(0);
    }
    return result;
}

void Creature::Get(nlohmann::json &output)
{
    if (!m_meshReady)
    {
        GenerateMesh();
    }
    output["mesh"] = m_mesh;
    output["name"] = m_name;
    output["center"]["x"] = m_center[0];
    output["center"]["y"] = m_center[1];
    output["center"]["z"] = m_center[2];
}

void Creature::NormalizePoint(int x, int y, int z, Vector3 &output)
{
    output = Vector3::Zero();
    double norm = (m_resolution - 1) / 2;
    output[0] = m_domainBounds * (x - norm) / norm;
    output[1] = m_domainBounds * (y - norm) / norm;
    output[2] = m_domainBounds * (z - norm) / norm;
}

void Creature::SampleEdgeParameters()
{
    for (auto &edge : m_mesh["edges"])
    {
        int i = edge["i"];
        int j = edge["j"];
        auto p1 = m_mesh["vertices"][i];
        auto p2 = m_mesh["vertices"][j];
        Vector3 v1(p1["x"], p1["y"], p1["z"]);
        Vector3 v2(p2["x"], p2["y"], p2["z"]);
        auto val = m_policyNetwork.Forward((v1 + v2) / 2);
        for (int i = 0; i < MUSCLE_ORDER; i++)
        {
            edge["amplitudes"].push_back(val[i * 2]);
            edge["phases"].push_back(val[i * 2 + 1]);
        }
        edge["length"] = (v1 - v2).norm();
    }
}

static inline double gaussian(const Vector3 &point, double norm)
{
    return exp(-(point / norm).dot(point / norm) * 0.2);
}

void Creature::GenerateMesh()
{
    VoxelMask mask(m_resolution);

    vector<double> networkOutput;

    mask.ForEachVoxel([&](int x, int y, int z, bool val) {
        Vector3 point;
        NormalizePoint(x, y, z, point);
        networkOutput.push_back(
#ifdef USE_GAUSSIAN
            gaussian(point, m_domainBounds) *
#endif
            m_morphologyNetwork.Forward(point)[0]);
    });

    sort(networkOutput.begin(), networkOutput.end());
    auto it = networkOutput.begin();
    advance(it, networkOutput.size() * (1 - SIZE_FACTOR));
    double cutoff = *it;

    mask.ForEachVoxel([&](int x, int y, int z, bool val) {
        Vector3 point;
        NormalizePoint(x, y, z, point);
        mask.set(x, y, z,
#ifdef USE_GAUSSIAN
                 gaussian(point, m_domainBounds) *
#endif
                         m_morphologyNetwork.Forward(point)[0] >
                     cutoff);
    });
    Voxelize(mask, m_mesh);
    m_center = Vector3::Zero();
    for (auto &vertex : m_mesh["vertices"])
    {
        double x = vertex["x"];
        double y = vertex["y"];
        double z = vertex["z"];
        vertex["x"] = x / m_resolution;
        vertex["y"] = y / m_resolution;
        vertex["z"] = z / m_resolution;
        m_center += Vector3(vertex["x"], vertex["y"], vertex["z"]);
    }
    m_center = m_center / m_mesh["vertices"].size();
    SampleEdgeParameters();
}
#pragma endregion

#pragma region File IO

void Creature::ToJson(nlohmann::json &output)
{
    output["name"] = Name();
    output["domain_bound"] = m_domainBounds;
    output["resolution"] = m_resolution;

    output["spring_k"] = m_spring_k;
    output["spring_damp"] = m_spring_dampening;
    output["frequency"] = m_frequency;
    output["max_amplitude"] = m_maxAmplitude;
    output["min_amplitude"] = m_minAmplitude;
    output["max_phase"] = m_maxPhase;

    nlohmann::json morphologyOutput;
    m_morphologyNetwork.ToJson(morphologyOutput);
    nlohmann::json policyOutput;
    m_policyNetwork.ToJson(policyOutput);
    output["morphology"] = morphologyOutput;
    output["policy"] = policyOutput;
}

void Creature::Save(string path)
{
    ofstream f;
    f.open(path + Name() + ".creature");
    if (f.is_open())
    {
        nlohmann::json output;
        ToJson(output);
        f << output.dump(4) << endl;
    }
    else
        cout << "Error opening " << Name() + ".creature" << endl;
    f.close();

    ofstream simulationOutput;
    simulationOutput.open(path + Name() + ".json");
    if (simulationOutput.is_open())
    {
        simulationOutput << m_simulationOutput << endl;
    }
    else
        cout << "Error opening " << Name() + ".creature" << endl;
    simulationOutput.close();
}

shared_ptr<Creature> Creature::Load(string path, nlohmann::json config)
{
    auto creature = make_shared<Creature>();
    creature->m_meshReady = false;

    ifstream f;
    f.open(path);
    f.precision(std::numeric_limits<double>::max_digits10);
    if (f.is_open())
    {
        nlohmann::json input;
        f >> input;
        creature->m_name = input["name"];

        creature->m_domainBounds = input["domain_bound"];
        creature->m_resolution = input["resolution"];

        creature->m_spring_k = input["spring_k"];
        creature->m_spring_dampening = input["spring_damp"];
        creature->m_frequency = input["frequency"];
        creature->m_maxAmplitude = input["max_amplitude"];
        creature->m_minAmplitude = input["min_amplitude"];
        creature->m_maxPhase = input["max_phase"];
        NeuralNetwork::Load(input["morphology"], creature->m_morphologyNetwork);
        NeuralNetwork::Load(input["policy"], creature->m_policyNetwork);
    }

    for (auto it = config.begin(); it != config.end(); it++)
    {
        if (it.key() == "resolution")
        {
            creature->m_resolution = (*it).get<int>();
        }
        else if (it.key() == "spring_k")
        {
            creature->m_spring_k = (*it).get<int>();
        }
    }

    return creature;
}

#pragma endregion
