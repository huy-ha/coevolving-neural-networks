#ifndef __EVOLVE_SOLVER_H__
#define __EVOLVE_SOLVER_H__
#include "Solver.h"
#include <list>
#include <Selector.h>
#include <Reproducer.h>

using namespace std;
class EvolveSolver : public Solver
{
public:
    EvolveSolver();
    virtual void Iteration() override;
    shared_ptr<Creature> CreateOffspring();

protected:
    int m_targetPopulationSize;
    unique_ptr<Selector> m_selector;
    unique_ptr<Reproducer> m_reproducer;
    int m_iterOffspringsCount;
};

#endif