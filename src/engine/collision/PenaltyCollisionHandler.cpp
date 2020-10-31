#include "PenaltyCollisionHandler.h"
#include "../core/Scene.h"

using namespace Eigen;
std::shared_ptr<PenaltyCollisionHandler> PenaltyCollisionHandler::m_instance;

double StaticFrictionCoefficient = 1;
double KineticFrictionCoefficient = 0.8;

void PenaltyCollisionHandler::addParticleGroundForceTotal(const VectorXd &x, const VectorXd &v, int i, VectorXd &F)
{
    // your implementation here
    Vector3d px = Vector3d::Zero();
    double r1 = Scene::PARTICLE_RADIUS;

    Vector3d nh;
    nh << 0, 1, 0;
    nh.normalize();
    VectorXd n = (px - x.segment<3>(3 * i)).dot(nh) * nh;
    double dist = n.norm();
    if (!m_instance)
    {
        auto *pch = new PenaltyCollisionHandler(40000, 0.2);
        m_instance = shared_ptr<PenaltyCollisionHandler>(pch);
    }
    if (dist <= (r1 + m_instance->m_thickness)) // overlap
    {
        // Handle Friction
        Vector3 weight = m_instance->m_k * (dist - r1 - m_instance->m_thickness) * nh;
        Vector3d horizontalVelocity = v.segment<3>(3 * i);
        horizontalVelocity[1] = 0;
        Vector3d friction = -horizontalVelocity * KineticFrictionCoefficient * weight.norm() / 3;
        // Vector3d friction = F.segment<3>(3 * i);
        // friction = friction - friction.dot(nh) * nh;
        // friction *= KineticFrictionCoefficient;

        F.segment<3>(3 * i) -= weight - friction;
    }
}