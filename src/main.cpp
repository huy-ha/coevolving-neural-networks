#include "Creature.h"
#include <fstream>
#include <time.h>
#include <chrono>
#include <iostream>
#include "engine/core/Simulation.h"
#include "engine/core/Utils.h"

using namespace std;
using namespace std::chrono;
using namespace Eigen;

int main()
{
    unsigned int seed = (unsigned int)time(0);
    srand(seed);

    auto start = high_resolution_clock::now();
    Creature creature;
    auto end = high_resolution_clock::now();
    cout << "Creation of " << creature.Name() << " took " << duration_cast<duration<double>>(end - start).count() << " seconds" << endl;

    start = high_resolution_clock::now();
    Simulation sim(creature, "./");
    cout << "Created simulation" << endl;
    auto simFinish = sim.Play();
    simFinish.wait();
    end = high_resolution_clock::now();
    cout << "Simulation of " << creature.Name() << " took " << duration_cast<duration<double>>(end - start).count() << " seconds" << endl;
    sim.SaveOutput();
    cout << "Saved simulation JSON file to "
         << "./" + creature.Name() + ".sim" << endl;
}