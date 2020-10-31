#include "MuscleForce.h"
#include "../core/Utils.h"
#include <math.h>

void MuscleForce::addForceToTotal(const VectorXd &x, const VectorXd &v, VectorXd &f)
{
    Vector3 springForce = Vector3::Zero();
    double targetLength = m_l0 * (1 + m_alpha * sin(m_omega * (m_sim->GetT() + m_c)));

    const Vector3 xi = x.segment<3>(3 * m_i);
    const Vector3 xj = x.segment<3>(3 * m_j);
    const Vector3 vi = v.segment<3>(3 * m_i);
    const Vector3 vj = v.segment<3>(3 * m_j);

    Vector3 n = (xi - xj).normalized();
    double l = (xi - xj).norm();
    // spring force
    springForce += m_k * (l - targetLength) * n;
    // spring dampening
    springForce += m_b * n.dot(vi - vj) * n;
    f.segment<3>(3 * m_i) -= springForce;
    f.segment<3>(3 * m_j) += springForce;
}