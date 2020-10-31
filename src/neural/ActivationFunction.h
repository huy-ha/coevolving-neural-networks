#ifndef __ACTIVATION_FUNCTION__
#define __ACTIVATION_FUNCTION__
#include <functional>
#include <Eigen/Core>
#include <Eigen/StdVector>
class ActivationFunction
{
public:
    inline ActivationFunction(std::function<Eigen::VectorXd(Eigen::VectorXd)> f) : m_func(f) {}

    inline Eigen::VectorXd operator()(Eigen::VectorXd input)
    {
        return m_func(input);
    }

protected:
    std::function<Eigen::VectorXd(Eigen::VectorXd)> m_func;
};

#endif