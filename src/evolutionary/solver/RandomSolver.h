#ifndef __RANDOM_SOLVER_H__
#define __RANDOM_SOLVER_H__
#include "Solver.h"

class RandomSolver : public Solver
{
public:
    inline RandomSolver()
    {
        SetConfig("population_count", 4);
        m_solvername = "RandomSolver";
    }
    inline virtual void Iteration() override
    {
        InitializePopulation();
    }
};
#endif