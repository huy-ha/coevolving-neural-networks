#ifndef __SIMULATION_H__
#define __SIMULATION_H__
#include "Scene.h"
#include <string>
#include <future>
#include <atomic>
#include <memory>
#include <nlohmann/json.hpp>

class Creature;

using namespace std;
class Simulation
{
public:
    Simulation(int particleCount, string name, string outputPath);
    Simulation(nlohmann::json &mesh, string name, string outputPath);
    Simulation(Creature &creature, string outputPath);
    future<void> Play();
    inline void Stop() { shouldStop = true; }
    string ToString() const;
    string SetUpToString() const;
    void SaveOutput();
    inline string GetName() const { return m_name; }
    double GetT() const { return t; }
    double GetDt() const { return dt; }
    double GetMaxT() const { return maxT; }

    inline double AverageVelocity()
    {
        return (startingPos - finalPos).norm() / maxT;
    }
    inline nlohmann::json GetOutput()
    {
        return m_output;
    }
    static atomic<int> evaluations;

private:
    void Step();

private:
    Scene m_scene;
    double dt, maxT, t;
    atomic<bool> shouldStop;
    nlohmann::json m_output;
    string m_name, m_outputPath;
    static double PLAYBACK_FRAMERATE;
    static double PARTICLE_MASS;
    Vector3 startingPos;
    Vector3 finalPos;
};

#endif