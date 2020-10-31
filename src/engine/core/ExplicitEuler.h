#ifndef __EXPLICIT_EULER_H__
#define __EXPLICIT_EULER_H__
#include "Scene.h"
class ExplicitEuler
{
public:
    static void StepScene(Scene &scene, double dt);
};
#endif