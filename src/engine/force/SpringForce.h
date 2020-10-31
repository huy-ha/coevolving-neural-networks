#ifndef __SPRING_FORCE_H__
#define __SPRING_FORCE_H__
#include "Force.h"
#include <Eigen/StdVector>
#include "../core/Utils.h"

class SpringForce : public Force
{
public:
    inline SpringForce(double k, double l0, double b, int i, int j)
        : m_k(k), m_l0(l0), m_b(b), m_i(i), m_j(j) {}
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

protected:
    int m_i, m_j; // Two endpoints
    double m_k;   // stiffness
    double m_l0;  // original length
    double m_b;   // dampening
};
#endif