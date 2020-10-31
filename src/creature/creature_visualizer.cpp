#include <iostream>
#include <Creature.h>
#include <nlohmann/json.hpp>
using namespace std;

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cout << "Please provide a path " << endl;
        return -1;
    }
    string path = argv[1];
    nlohmann::json config;
    config["resolution"] = 12;
    auto creature = Creature::Load(path, config);
    creature->RunSimulation(true);
}