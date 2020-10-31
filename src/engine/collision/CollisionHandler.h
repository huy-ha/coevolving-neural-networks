#ifndef __COLLISION_HANDLER_H__
#define __COLLISION_HANDLER_H__
#include "../core/Utils.h"
#include "../core/Scene.h"
class CollisionHandler
{
public:
    static bool DetectParticleGround(Scene &scene, int i, Vector3 &n);
    static void RespondParticleGround(Scene &scene, int i, Vector3 &n);

    static double COR; // Coefficient of Restitution
    static double StaticFrictionCoefficient;
    static double KineticFrictionCoefficient;
};
#endif