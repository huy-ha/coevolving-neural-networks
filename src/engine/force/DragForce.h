#ifndef __DAMPENING_FORCE_H__
#define __DAMPENING_FORCE_H__
#include "Force.h"
#include <Eigen/StdVector>
#include "../core/Utils.h"

class DragForce : public Force
{
public:
    inline DragForce(double b)
        : m_b(b) {}
    virtual void addForceToTotal(
        const VectorXd &x,
        const VectorXd &v,
        VectorXd &f);

#ifdef LOG_ENERGY
    inline void addEnergyToTotal(
        const VectorXd &x,
        const VectorXd &v,
        double &E){};
#endif

protected:
    double m_b; // dampening
};
#endif