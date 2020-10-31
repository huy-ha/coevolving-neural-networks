#include "HillClimberSolver.h"
#include <Simulation.h>

HillClimberSolver::HillClimberSolver() : Solver()
{
    SetConfig("population_count", 4);
    m_solvername = "HillClimberSolver";
}

void HillClimberSolver::Iteration()
{
    MutatePopulation();
}