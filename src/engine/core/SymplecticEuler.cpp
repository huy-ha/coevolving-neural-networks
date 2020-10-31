#include "SymplecticEuler.h"
#include <Eigen/Core>

void SymplecticEuler::StepScene(Scene &scene, double dt)
{
    auto &x = scene.getX();
    auto &v = scene.getV();
    VectorXd F = VectorXd::Zero(x.size());
    scene.AccumulateForce(F);
    v = v + dt * F;
    x = x + dt * v;
}
