#ifndef __CROSSOVER_REPRODUCER_H__
#define __CROSSOVER_REPRODUCER_H__
#include "Reproducer.h"

class CrossoverReproducer : public Reproducer
{
public:
    virtual shared_ptr<Creature> Reproduce(shared_ptr<Creature> p1, shared_ptr<Creature> p2) override;
};

#endif