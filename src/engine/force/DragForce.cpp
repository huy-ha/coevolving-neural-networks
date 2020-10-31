#include "DragForce.h"
#include "../core/Utils.h"

void DragForce::addForceToTotal(const VectorXd &x, const VectorXd &v, VectorXd &f)
{
    f -= v * m_b;
}