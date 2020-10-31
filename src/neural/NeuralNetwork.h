#ifndef __NEURAL_NETWORK_H__
#define __NEURAL_NETWORK_H__
#include <vector>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/StdVector>
#include <nlohmann/json.hpp>
#include "ActivationFunction.h"
#include <memory>
#include <vector>
#include <limits>
#include <iostream>
using namespace std;
using namespace Eigen;
class NeuralNetwork
{
public:
    NeuralNetwork(int inputDimension, int hiddenLayerDimensions, int outputDimensions, int layers);
    NeuralNetwork(const NeuralNetwork &other);

    VectorXd Forward(const VectorXd &input);
    friend ostream &operator<<(ostream &os, NeuralNetwork &neural);

    void ToJson(nlohmann::json &output);

    static void Load(nlohmann::json &input, NeuralNetwork &output);

    inline void Mutate(double amt)
    {
        for (auto &layer : m_layers)
        {
            layer = layer + MatrixXd::Random(layer.rows(), layer.cols()) * amt;
        }

        for (auto &bias : m_biases)
        {
            bias = bias + VectorXd::Random(bias.size()) * amt;
        }
    }

    static NeuralNetwork Recombine(NeuralNetwork &n1, NeuralNetwork &n2, double t);

private:
    vector<MatrixXd> m_layers;
    vector<VectorXd> m_biases;
    int m_inputDim, m_outputDim, m_hiddenDim, m_layersCount;
    unique_ptr<ActivationFunction> m_activationFunc;
};

#endif