#include "EvolutionaryUtils.h"
#include <random>

using namespace std;

mutex randMut;
uniform_real_distribution<double> unif(0, 1);
default_random_engine re;

double SafeRandom()
{
    lock_guard<mutex> lock(randMut);
    return unif(re);
}

double SafeRandom(double min, double max)
{
    return min + (max - min) * SafeRandom();
}

int SafeRandomInt(int min, int max)
{
    return (int)SafeRandom(min, max);
}
