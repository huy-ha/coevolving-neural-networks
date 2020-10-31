#ifndef __ARCTAN_ACTIVATION_FUNCTION__
#define __ARCTAN_ACTIVATION_FUNCTION__
#include "ActivationFunction.h"
#include <math.h>

class ArcTan : public ActivationFunction
{
public:
    inline ArcTan() : ActivationFunction([](Eigen::VectorXd v) {
                          for (int i = 0; i < v.size(); i++)
                          {
                              v[i] = atan(v[i]);
                          }
                          return v;
                      }) {}
};

#endif