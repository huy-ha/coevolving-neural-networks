#include "GravityForce.h"
#include <cassert>
#include <iostream>
using namespace std;
void GravityForce::addForceToTotal(const VectorXd &x, const VectorXd &v, VectorXd &f)
{
    assert(x.size() % 3 == 0);
    auto N = x.size() / 3;
    for (int i = 0; i < N; i++)
    {
        f.segment<3>(3 * i) += m_gravityForce;
    }
}

#ifdef LOG_ENERGY
void GravityForce::addEnergyToTotal(const VectorXd &x, const VectorXd &v, double &E)
{
    for (int i = 0; i < x.size() / 3; i++)
    {
        E += 9.81 * x[3 * i + 1];
    }
}
#endif