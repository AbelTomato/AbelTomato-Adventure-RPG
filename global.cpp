#include "global.hpp"
#include <ctime>

namespace Global
{
    std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));

    int get_random_int(int min, int max)
    {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(rng);
    }

    double get_random_double()
    {
        std::uniform_real_distribution<double> dist(0.0, 1.0);
        return dist(rng);
    }
}