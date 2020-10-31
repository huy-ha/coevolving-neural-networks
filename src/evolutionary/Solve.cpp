#include <HillClimberSolver.h>
#include <EvolveSolver.h>
#include <RandomSolver.h>
#include <Solver.h>
#include <memory>
#include <string>
using namespace std;

int main(int argc, char **argv)
{
    shared_ptr<Solver> solver;
    if (argc > 1)
    {
        string solverType(argv[1]);
        if (solverType == "e")
        {
            solver = make_shared<EvolveSolver>();
        }
        else if (solverType == "hc")
        {
            solver = make_shared<HillClimberSolver>();
        }
        else if (solverType == "r")
        {
            solver = make_shared<RandomSolver>();
        }
        else
        {
            cout << "Please input a valid solver type:" << endl;
            cout << "\te: EvolveSolver" << endl;
            cout << "\thc: HillClimberSolver" << endl;
            cout << "\tr: RandomSolver" << endl;
            exit(-1);
        }
    }
    else
    {
        solver = make_shared<EvolveSolver>();
    }

    solver->Run();
}