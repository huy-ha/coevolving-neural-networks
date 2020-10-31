#ifndef __RELU_ACTIVATION_FUNCTION__
#define __RELU_ACTIVATION_FUNCTION__
#include "ActivationFunction.h"

class ReLU : public ActivationFunction
{
public:
    inline ReLU() : ActivationFunction([](Eigen::VectorXd v) {
                        for (int i = 0; i < v.size(); i++)
                        {
                            v[i] = v[i] > 0 ? v[i] : 0;
                        }
                        return v;
                    }) {}
};

#endif