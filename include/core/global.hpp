#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <random>

enum class battle_result
{
    VICTORY,
    DEFEAT,
    ESCAPED
};

namespace Global
{
    extern std::mt19937 rng;

    int get_random_int(int min, int max);

    double get_random_double();
}

#endif