#ifndef __GRAVITY_FORCE_H__
#define __GRAVITY_FORCE_H__
#include "Force.h"
#include <Eigen/StdVector>
#include "../core/Utils.h"

class GravityForce : public Force
{
public:
    inline GravityForce(Vector3 gravityForce) : m_gravityForce(gravityForce) {}
    virtual void addForceToTotal(
        const VectorXd &x,
        const VectorXd &v,
        VectorXd &f);

#ifdef LOG_ENERGY
    void addEnergyToTotal(
        const VectorXd &x,
        const VectorXd &v,
        double &E);
#endif

private:
    Vector3 m_gravityForce;
};
#endif