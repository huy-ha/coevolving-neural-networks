#include "CollisionHandler.h"
#include <Eigen/Core>
#include <Eigen/StdVector>
#include <math.h>

double CollisionHandler::COR = 0.8;
double CollisionHandler::StaticFrictionCoefficient = 1;
double CollisionHandler::KineticFrictionCoefficient = 0.6;

bool CollisionHandler::DetectParticleGround(Scene &scene, int i, Vector3 &n)
{
    Vector3 xi = scene.getX().segment<3>(3 * i);
    Vector3 vi = scene.getV().segment<3>(3 * i);
    n = xi[1] * Vector3(0, 1, 0);
    bool overlapping = n.squaredNorm() < pow(Scene::PARTICLE_RADIUS, 2);
    bool approaching = vi.dot(n) < 0;
    return overlapping && approaching;
}

void CollisionHandler::RespondParticleGround(Scene &scene, int i, Vector3 &n)
{
    Vector3 nhat = n.normalized();
    Vector3 v = scene.getV().segment<3>(3 * i);
    // Get vector parallel to v
    auto up = Vector3(0, 1, 0);
    auto tangent = (v - v.dot(up) * up).normalized();
    // TODO compute actual forces
    auto friction = tangent * tangent.dot(v) * KineticFrictionCoefficient;
    scene.getV().segment<3>(3 * i) -=
        0.5 * (1 + COR) * 2 * (v.dot(nhat)) * nhat + friction;
}