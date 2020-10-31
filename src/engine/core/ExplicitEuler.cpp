#include "ExplicitEuler.h"
#include <Eigen/Core>
#include "../collision/PenaltyCollisionHandler.h"

void ExplicitEuler::StepScene(Scene &scene, double dt)
{
    auto &x = scene.getX();
    auto &v = scene.getV();
    VectorXd F = VectorXd::Zero(x.size());
    scene.AccumulateForce(F);
    for (int i = 0; i < scene.GetParticleCount(); i++)
    {
        if (x[3 * i + 1] < 0.1)
        {
            PenaltyCollisionHandler::addParticleGroundForceTotal(x, v, i, F);
        }
    }
    x = x + dt * v;
    v = v + dt * F;
}
