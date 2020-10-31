#ifndef __FORCE_H__
#define __FORCE_H__
#include <Eigen/Dense>
#include "../../build.h"
using namespace Eigen;
class Force
{
public:
    virtual void addForceToTotal(
        const VectorXd &x,
        const VectorXd &v,
        VectorXd &f) = 0;

#ifdef LOG_ENERGY
    virtual void addEnergyToTotal(
        const VectorXd &x,
        const VectorXd &v,
        double &E) = 0;
#endif
};
#endif