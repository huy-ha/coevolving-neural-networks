#include "EvolveSolver.h"
#include <TournamentSelector.h>
#include <CrossoverReproducer.h>

EvolveSolver::EvolveSolver() : Solver()
{
    m_targetPopulationSize = GetConfig("population_count").get<int>();
    m_selector = make_unique<TournamentSelector>(m_targetPopulationSize * GetConfig("selection_pressure").get<double>());
    m_reproducer = make_unique<CrossoverReproducer>();

    m_iterOffspringsCount = GetConfig("reproduction_factor").get<double>() * m_targetPopulationSize;
    m_solvername = "EvolveSolver";
}

shared_ptr<Creature> EvolveSolver::CreateOffspring()
{
    auto [p1, p2] = m_selector->Select(m_population);
    return m_reproducer->Reproduce(p1, p2);
}

void EvolveSolver::Iteration()
{
    list<shared_ptr<Creature>> offsprings;
    // create new offsprings add new offsprings to population
    for (int i = 0; i < m_iterOffspringsCount; i++)
    {
        offsprings.emplace_front(CreateOffspring());
    }

    for (auto offspring : offsprings)
    {
        m_population.emplace_front(offspring);
    }

    CalculatePopulationFitness();
    // mutate everyone
    MutatePopulation();
    // remove worse
    SortPopulation();
    while (m_population.size() > m_targetPopulationSize)
    {
        m_population.pop_back();
    }
}
