#include "CrossoverReproducer.h"
#include <NeuralNetwork.h>

shared_ptr<Creature> CrossoverReproducer::Reproduce(shared_ptr<Creature> p1, shared_ptr<Creature> p2)
{
    // get neural network from both parent
    auto p1Policy = p1->GetPolicy();
    auto p1Morphology = p1->GetMorphology();

    auto p2Policy = p2->GetPolicy();
    auto p2Morphology = p2->GetMorphology();

    // recombine the neural networks
    auto childPolicy = NeuralNetwork::Recombine(p1Policy, p2Policy, 0.7);
    auto childMorphology = NeuralNetwork::Recombine(p1Morphology, p2Morphology, 0.7);
    // return child
    auto child = make_shared<Creature>(childMorphology, childPolicy);
    child->RunSimulation();
    return child;
}