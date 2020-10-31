#include "NeuralNetwork.h"
#include <iostream>
#include "ReLU.h"
#include "ArcTan.h"
#include <fstream>
#include <sstream>
#include <math.h>
#include <random>
#include <mutex>
using namespace Eigen;
using namespace std;

namespace Neural
{
mutex randMut;
uniform_real_distribution<double> unif(0, 1);
default_random_engine re;

double SafeRandom()
{
    lock_guard<mutex> lock(randMut);
    return unif(re);
}
} // namespace Neural

NeuralNetwork::NeuralNetwork(int inputDimension, int hiddenLayerDimensions, int outputDimension, int layers)
{
    m_inputDim = inputDimension;
    m_outputDim = outputDimension;
    m_hiddenDim = hiddenLayerDimensions;
    m_layersCount = layers;

    m_activationFunc = make_unique<ArcTan>();
    for (int i = 0; i < layers; i++)
    {
        int width = hiddenLayerDimensions;
        int height = hiddenLayerDimensions;
        if (i == 0)
        {
            height = inputDimension;
            m_biases.push_back(VectorXd::Random(hiddenLayerDimensions));
        }
        else if (i == layers - 1)
        {
            width = outputDimension;
            m_biases.push_back(VectorXd::Random(outputDimension));
        }
        else
        {
            m_biases.push_back(VectorXd::Random(hiddenLayerDimensions));
        }

        m_layers.push_back(MatrixXd::Random(width, height));
    }
}

NeuralNetwork NeuralNetwork::Recombine(NeuralNetwork &n1, NeuralNetwork &n2, double t)
{
    NeuralNetwork output(n1);
    for (int layer = 0; layer < output.m_layersCount; layer++)
    {
        // do weights
        int colCount = n1.m_layers[layer].cols();
        for (int col = 0; col < colCount; col++)
        {
            if (Neural::SafeRandom() > t)
            {
                output.m_layers[layer].col(col) = n2.m_layers[layer].col(col);
            }
        }

        // do biases
        int size = n1.m_biases[layer].size();
        for (int j = 0; j < size; j++)
        {
            if (Neural::SafeRandom() > t)
            {
                output.m_biases[layer][j] = n2.m_biases[layer][j];
            }
        }
    }
    return output;
}

NeuralNetwork::NeuralNetwork(const NeuralNetwork &other)
{
    m_inputDim = other.m_inputDim;
    m_outputDim = other.m_outputDim;
    m_hiddenDim = other.m_hiddenDim;
    m_layersCount = other.m_layersCount;
    m_activationFunc = make_unique<ArcTan>();
    for (const auto &layer : other.m_layers)
    {
        m_layers.push_back(MatrixXd(layer));
    }
    for (const auto &bias : other.m_biases)
    {
        m_biases.push_back(VectorXd(bias));
    }
}

Eigen::VectorXd NeuralNetwork::Forward(const Eigen::VectorXd &input)
{
    VectorXd hidden = VectorXd::Zero(m_hiddenDim);
    for (int i = 0; i < m_layers.size(); i++)
    {
        if (i == m_layers.size() - 1)
        {
            if (m_layers.size() < 2)
            {
                return (*m_activationFunc)(m_layers[0] * input + m_biases[0]) / M_PI + VectorXd::Ones(m_outputDim) * 0.5;
            }
            else
            {
                return (*m_activationFunc)(m_layers[i] * hidden + m_biases[i]) / M_PI + VectorXd::Ones(m_outputDim) * 0.5;
            }
        }
        else if (i == 0)
        {
            hidden = (*m_activationFunc)(m_layers[0] * input + m_biases[0]);
        }
        else
        {
            hidden = (*m_activationFunc)(m_layers[i] * hidden + m_biases[i]);
        }
    }
}

void NeuralNetwork::ToJson(nlohmann::json &output)
{
    output["input_dim"] = m_inputDim;
    output["output_dim"] = m_outputDim;
    output["hidden_dim"] = m_hiddenDim;
    output["layers_count"] = m_layersCount;
    for (auto &layer : m_layers)
    {
        VectorXd v1(Map<VectorXd>(layer.data(), layer.cols() * layer.rows()));
        vector<double> v2;
        v2.resize(v1.size());
        VectorXd::Map(&v2[0], v1.size()) = v1;
        output["layers"].push_back(v2);
    }

    for (auto &bias : m_biases)
    {
        vector<double> v;
        v.resize(bias.size());
        VectorXd::Map(&v[0], bias.size()) = bias;
        output["biases"].push_back(v);
    }
}

void NeuralNetwork::Load(nlohmann::json &input, NeuralNetwork &output)
{
    output.m_layers.clear();
    output.m_biases.clear();
    output.m_inputDim = -1;
    output.m_outputDim = -1;
    output.m_hiddenDim = -1;
    output.m_activationFunc = make_unique<ArcTan>();

    output.m_inputDim = input["input_dim"];
    output.m_outputDim = input["output_dim"];
    output.m_hiddenDim = input["hidden_dim"];
    output.m_layersCount = input["layers_count"];

    for (auto bias : input["biases"])
    {
        output.m_biases.push_back(
            VectorXd(Map<VectorXd>(
                bias.get<vector<double>>().data(),
                bias.get<vector<double>>().size())));
    }

    for (auto layer : input["layers"])
    {
        if (output.m_layers.size() == 0)
        {
            output.m_layers.push_back(
                MatrixXd(Map<MatrixXd>(
                    layer.get<vector<double>>().data(),
                    output.m_hiddenDim,
                    output.m_inputDim)));
        }
        else if (output.m_layers.size() == output.m_layersCount - 1)
        {
            output.m_layers.push_back(
                MatrixXd(Map<MatrixXd>(
                    layer.get<vector<double>>().data(),
                    output.m_outputDim,
                    output.m_hiddenDim)));
        }
        else
        {
            output.m_layers.push_back(
                MatrixXd(Map<MatrixXd>(
                    layer.get<vector<double>>().data(),
                    output.m_hiddenDim,
                    output.m_hiddenDim)));
        }
    }
}