#ifndef __MULTI_MUSCLE_FORCE_H__
#define __MULTI_MUSCLE_FORCE_H__
#include "MuscleForce.h"
#include "../core/Simulation.h"
#include <vector>
#include <functional>
#include <math.h>
#include "../../build.h"

using namespace std;

class MultiMuscleForce : public SpringForce
{
public:
    inline MultiMuscleForce(
        double k, double l0, double b,
        int i, int j,
        vector<double> &frequencies,
        vector<double> &amplitudes,
        vector<double> &phases,
        Simulation *sim)
        : SpringForce(k, l0, b, i, j),
          m_frequencies(frequencies),
          m_amplitudes(amplitudes),
          m_phases(phases), m_sim(sim)
    {
        m_targetLength = [&](double t) {
            double l = 1;
            for (int i = 0; i < MUSCLE_ORDER; i++)
            {
                l += m_amplitudes[i] * sin(m_frequencies[i] * (t + m_phases[i]));
            }
            l *= m_l0;
            return l;
        };
    }
    virtual void addForceToTotal(
        const VectorXd &x,
        const VectorXd &v,
        VectorXd &f) override;

protected:
    vector<double> m_frequencies;
    vector<double> m_amplitudes;
    vector<double> m_phases;
    Simulation *m_sim;
    function<double(double)> m_targetLength;
};

#endif