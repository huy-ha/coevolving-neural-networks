#include "Solver.h"
#include <Simulation.h>
#include <algorithm>
#include <EvolutionaryUtils.h>

Solver::Solver()
{
    srand(seed);
    SetConfig("population_count", 30);
    SetConfig("mutation_magnitude", 0.3);
    SetConfig("selection_pressure", 0.2);
    SetConfig("reproduction_factor", 0.4);
    // Probabilities
    SetConfig("prob_policy_mutation", 0.7);
    if (GetConfig("prob_policy_mutation").get<double>() == 1)
    {
        optimizePolicyOnly = true;
        srand(24);
        m_policyOnlyMorphology = make_shared<NeuralNetwork>(3, 10, 1, 10);
        srand(seed);
        cout << "Optimizing policy only" << endl;
    }
    else
    {
        optimizePolicyOnly = false;
        cout << "Optimizing policy and morphology" << endl;
    }
}

void Solver::InitializePopulation()
{
    m_population.clear();
    int populationCount = GetConfig("population_count").get<int>();
    for (int i = 0; i < populationCount; i++)
    {
        if (optimizePolicyOnly)
        {
            m_population.emplace_front(
                make_shared<Creature>(*m_policyOnlyMorphology));
        }
        else
        {
            m_population.emplace_front(make_shared<Creature>());
        }
        m_population.front()->RunSimulation();
    }
}

const nlohmann::json &Solver::GetConfig(string field) const
{
    return m_output["config"][field];
}

void Solver::MutatePopulation()
{
    MutatePopulation(GetConfig("mutation_magnitude").get<double>());
}

void Solver::MutatePopulation(double mutationRate)
{
    const double probabilityMutatePolicy = GetConfig("prob_policy_mutation").get<double>();
    const double mutationMagnitude = GetConfig("mutation_magnitude").get<double>();
    list<shared_ptr<Creature>> mutants;

    for_each(m_population.begin(), m_population.end(), [&](shared_ptr<Creature> &creature) {
        auto mutant = make_shared<Creature>(*creature);
        if (SafeRandom() < probabilityMutatePolicy)
        {
            mutant->MutatePolicy(mutationMagnitude);
        }
        else
        {
            mutant->MutateMorphology(mutationMagnitude);
        }
        mutant->RunSimulation();
        mutants.emplace_back(mutant);
    });

    for_each(mutants.begin(), mutants.end(), [&](shared_ptr<Creature> &creature) {
        creature->Fitness();
    });

    for_each(mutants.begin(), mutants.end(), [&](shared_ptr<Creature> &creature) {
        auto it = find_if(m_population.begin(), m_population.end(), [&](auto other) {
            return other->Name() == creature->Name();
        });

        if (it != m_population.end())
        {
            if ((*it)->Fitness() < creature->Fitness())
            {
                m_population.erase(it);
                m_population.emplace_back(creature);
            }
        }
        else
        {
            m_population.emplace_back(creature);
        }
    });
}

void Solver::UpdateRecordHolder()
{
    SortPopulation();
    if (m_population.front()->Fitness() > m_recordFitness)
    {
        int evals = Simulation::evaluations;
        m_recordFitness = m_population.front()->Fitness();
        m_recordHolder = m_population.front();
        m_recordHolder->Save("./");
        cout << m_recordHolder->Name() << ":" << m_recordFitness << " after "
             << evals << " evaluations" << endl;
    }
}

void Solver::SortPopulation()
{
    m_population.sort(Creature::PtrCompare);
}

void Solver::CalculatePopulationFitness()
{
    for_each(m_population.begin(), m_population.end(), [](auto creature) {
        creature->Fitness();
    });
}

void Solver::Run()
{
    cout << "Solving with " << m_solvername << endl;
    InitializePopulation();
    UpdateRecordHolder();
    int evals = Simulation::evaluations;
    LogLearningCurve(evals, m_recordFitness);
    LogDotplot();
    while (true)
    {
        cout << "Iteration " << m_iteration << " | " << StrTime() << endl;
        Iteration();
        UpdateRecordHolder();
        evals = Simulation::evaluations;
        LogLearningCurve(evals, m_recordFitness);
        LogDotplot();
        SaveOutput();
        m_iteration += 1;
    }
}

void Solver::SaveOutput()
{
    ofstream output("./output.json");
    if (output.is_open())
    {
        output << m_output.dump(4) << endl;
        output.close();
    }
    else
    {
        cout << "failed to save output" << endl;
    }
}

string Solver::StrTime()
{
    auto end = system_clock::now();
    time_t end_time = system_clock::to_time_t(end);
    string output(ctime(&end_time));
    output.erase(std::remove(output.begin(), output.end(), '\n'), output.end());
    return output;
}

void Solver::LogDotplot()
{
    int evals = Simulation::evaluations;
    vector<double> fitnesses;
    for_each(m_population.begin(), m_population.end(),
             [&](const shared_ptr<Creature> &creature) {
                 fitnesses.push_back(creature->Fitness());
             });
    m_output["dotplot"].push_back(
        {{"evals", evals},
         {"fitnesses", fitnesses}});
}

void Solver::LogLearningCurve(int evals, double learningCurve)
{
    m_output["learning_curve"].push_back(
        {{"evals", evals},
         {"fitness", learningCurve}});
}