#ifndef __HILL_CLIMBER_SOLVER_H__
#define __HILL_CLIMBER_SOLVER_H__
#include "Solver.h"

class HillClimberSolver : public Solver
{
public:
    HillClimberSolver();
    virtual void Iteration() override;
};
#endif