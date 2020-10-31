#ifndef __SOLVER_H__
#define __SOLVER_H__
#include <Creature.h>
#include <memory>
#include <chrono>
#include <ctime>
#include <time.h>
#include <vector>
#include <fstream>
#include <list>
#include <nlohmann/json.hpp>

using namespace std;
using namespace std::chrono;
class Solver
{
public:
    Solver();

    void Run();
    string StrTime();
    void SaveOutput();
    const nlohmann::json &GetConfig(string field) const;

protected:
    void SortPopulation();
    void UpdateRecordHolder();
    void MutatePopulation();
    void InitializePopulation();
    void MutatePopulation(double mutationRate);
    inline void PrintPopulation()
    {
        cout << endl;
        for_each(m_population.begin(), m_population.end(), [](auto creature) {
            cout << creature->Name() << ":" << creature->Fitness() << endl;
        });
        cout << endl;
    }
    void CalculatePopulationFitness();

protected:
    void LogDotplot();
    void LogLearningCurve(int evals, double learningCurve);

    template <typename T>
    inline void SetConfig(string field, T val)
    {
        m_output["config"][field] = val;
    }

protected:
    const unsigned int seed = (unsigned int)time(0);
    virtual void Iteration() = 0;
    inline int GetCurrentIteration() { return m_iteration; }

protected:
    list<shared_ptr<Creature>> m_population;
    double m_recordFitness = -1;
    shared_ptr<Creature> m_recordHolder;
    string m_solvername;

private:
    nlohmann::json m_output;
    int m_iteration = 0;
    bool optimizePolicyOnly;
    shared_ptr<NeuralNetwork> m_policyOnlyMorphology;
};

#endif