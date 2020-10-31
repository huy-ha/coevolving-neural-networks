#ifndef __EVOLVER_UTILS_H__
#define __EVOLVER_UTILS_H__
#include <mutex>

double SafeRandom();
double SafeRandom(double min, double max);
int SafeRandomInt(int min, int max);

#endif