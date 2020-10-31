#ifndef __PENALTY_COLLISION_HANDLER_H__
#define __PENALTY_COLLISION_HANDLER_H__
#include <Eigen/StdVector>
#include <memory>

class PenaltyCollisionHandler
{
public:
    static void addParticleGroundForceTotal(const Eigen::VectorXd &x, const Eigen::VectorXd &v, int i, Eigen::VectorXd &F);

private:
    inline PenaltyCollisionHandler(double k, double thickness) : m_k(k), m_thickness(thickness) {}

protected:
    double m_thickness;
    double m_k;
    static std::shared_ptr<PenaltyCollisionHandler> m_instance;
};

#endif