#ifndef _SELECTOR_H_
#define _SELECTOR_H_
#include <memory>
#include <tuple>
#include <Creature.h>
#include <list>

using namespace std;
class Selector
{
public:
    virtual tuple<shared_ptr<Creature>, shared_ptr<Creature>> Select(
        const list<shared_ptr<Creature>> &population) = 0;
};

#endif