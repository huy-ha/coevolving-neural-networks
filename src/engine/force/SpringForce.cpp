#include "SpringForce.h"
#include "../core/Utils.h"
void SpringForce::addForceToTotal(const VectorXd &x, const VectorXd &v, VectorXd &f)
{
    Vector3 springForce = Vector3::Zero();

    const Vector3 xi = x.segment<3>(3 * m_i);
    const Vector3 xj = x.segment<3>(3 * m_j);
    const Vector3 vi = v.segment<3>(3 * m_i);
    const Vector3 vj = v.segment<3>(3 * m_j);

    Vector3 n = (xi - xj).normalized();
    double l = (xi - xj).norm();
    // spring force
    springForce += m_k * (l - m_l0) * n;
    // spring dampening
    springForce += m_b * n.dot(vi - vj) * n;
    f.segment<3>(3 * m_i) -= springForce;
    f.segment<3>(3 * m_j) += springForce;
}

#ifdef LOG_ENERGY
void SpringForce::addEnergyToTotal(const VectorXd &x, const VectorXd &v, double &E)
{
    const Vector3 xi = x.segment<3>(3 * m_i);
    const Vector3 xj = x.segment<3>(3 * m_j);
    double displacement = (xi - xj).norm() - m_l0;
    E += 0.5 * m_k * displacement * displacement;
}
#endif