#ifndef __CREATURE_H__
#define __CREATURE_H__
#include "../neural/NeuralNetwork.h"
#include <nlohmann/json.hpp>
#include <string>
#include "../engine/core/Utils.h"
#include <memory>
#include <future>
#include <atomic>

#define DEFAULT_RESOLUTION 20
#define DEFAULT_DOMAIN_SIZE 5
#define DEFAULT_SPRING_K 40000
#define DEFAULT_SPRING_DAMP 40
#define SIZE_FACTOR 0.1
// #define USE_GAUSSIAN
using namespace std;

class Creature
{
public:
    friend class Simulation;

    Creature();
    Creature(const Creature &other);
    Creature(NeuralNetwork &morphology);
    Creature(NeuralNetwork &morphology, NeuralNetwork &policy);
    void Get(nlohmann::json &output);
    void GenerateMesh();
    static string GenerateName();
    inline string Name() const { return m_name; }
    void Save(string path);
    void ToJson(nlohmann::json &output);
    static shared_ptr<Creature> Load(string path, nlohmann::json config = {});

    inline NeuralNetwork GetPolicy() const
    {
        return m_policyNetwork;
    }

    inline NeuralNetwork GetMorphology() const
    {
        return m_morphologyNetwork;
    }

    inline double Fitness()
    {
        if (!m_dirty)
        {
            return m_fitness;
        }
        else if (m_simulationRunning)
        {
            m_simulation.wait();
            return m_fitness;
        }
        RunSimulation();
        m_simulation.wait();
        return m_fitness;
    }

    inline void RunSimulation()
    {
        RunSimulation(false);
    }

    void RunSimulation(bool saveSimulation);

    static bool PtrCompare(const shared_ptr<Creature> c1, const shared_ptr<Creature> c2)
    {
        return c1->Fitness() > c2->Fitness();
    }

public:
    inline void MutateMorphology(double amt)
    {
        m_morphologyNetwork.Mutate(amt);
        m_dirty = true;
    }

    inline void MutatePolicy(double amt)
    {
        m_policyNetwork.Mutate(amt);
        m_dirty = true;
    }

    inline int Id() const
    {
        return m_id;
    }

private:
    void NormalizePoint(int x, int y, int z, Vector3 &output);
    void SampleEdgeParameters();
    static atomic_uint next_id;

protected:
    NeuralNetwork m_morphologyNetwork;
    NeuralNetwork m_policyNetwork;

private:
    bool m_meshReady;
    nlohmann::json m_mesh;
    string m_name;
    double m_domainBounds;
    double m_resolution;
    Vector3 m_center;
    int m_id;

private:
    bool m_dirty = true; // whether or not fitness needs to be recalculated
    bool m_simulationRunning = false;
    double m_fitness = -1;
    future<double> m_simulation;
    nlohmann::json m_simulationOutput;

private:
    double m_spring_k;
    double m_spring_dampening;
    double m_frequency;
    double m_maxAmplitude;
    double m_minAmplitude;
    double m_maxPhase;
};

#endif