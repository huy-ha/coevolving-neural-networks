#include "NeuralNetwork.h"
#include <Eigen/Core>
#include <iostream>
#include <Voxelize.hpp>
#include <nlohmann/json.hpp>
#include "../engine/core/Utils.h"
#include <chrono>
#include <fstream>
#include <time.h>

using namespace std;
using namespace std::chrono;
using namespace Eigen;

int main()
{
    NeuralNetwork n1(2, 2, 2, 3);
    NeuralNetwork n2(2, 2, 2, 3);
    NeuralNetwork n3 = NeuralNetwork::Recombine(n1, n2, 1);
}