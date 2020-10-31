#ifndef __REPRODUCER_H__
#define __REPRODUCER_H__
#include <Creature.h>
#include <memory>
class Reproducer
{
public:
    virtual shared_ptr<Creature> Reproduce(shared_ptr<Creature> p1, shared_ptr<Creature> p2) = 0;
};

#endif