#include "Creature.h"
#include <Eigen/Core>
#include <iostream>
#include <time.h>

using namespace std;

int main()
{
    Creature c1;

    Creature c2 = c1;
    c2.MutatePolicy(0.05);

    Creature c3 = c1;
    c3.MutatePolicy(0.1);
    c1.RunSimulation();
    c2.RunSimulation();
    c3.RunSimulation();
    double c1_fitness = c1.Fitness();
    double c2_fitness = c2.Fitness();
    double c3_fitness = c3.Fitness();
    cout << "Original creature achieved a fitness of " << c1_fitness << endl;
    cout << "Mutating that creature's policy a bit gave " << c2_fitness << endl;
    cout << "A bit more mutation gave " << c3_fitness << endl;
}