#ifndef __MUSCLE_FORCE_H__
#define __MUSCLE_FORCE_H__
#include "SpringForce.h"
#include "../core/Simulation.h"

class MuscleForce : public SpringForce
{
public:
    inline MuscleForce(
        double k, double l0, double b,
        int i, int j, double omega,
        double alpha, double c, Simulation *sim)
        : SpringForce(k, l0, b, i, j),
          m_omega(omega), m_alpha(alpha), m_c(c), m_sim(sim) {}
    virtual void addForceToTotal(
        const VectorXd &x,
        const VectorXd &v,
        VectorXd &f) override;

protected:
    double m_omega; // frequency
    double m_alpha; // Amplitude
    double m_c;     // phase shift
    Simulation *m_sim;
};

#endif