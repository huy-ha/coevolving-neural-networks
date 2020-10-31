#ifndef __SYMPLECTIC_EULER_H__
#define __SYMPLECTIC_EULER_H__
#include "Scene.h"
class SymplecticEuler
{
public:
    static void StepScene(Scene &scene, double dt);
};
#endif