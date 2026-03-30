#ifndef STAT_CALCULATOR_HPP
#define STAT_CALCULATOR_HPP

#include <array>

#include "config_cache.hpp"
#include "definitions/attribute_id.hpp"
#include "systems/config_cache.hpp"

class StatCalculator
{
public:
    // 基础加成公式：(基础 + 固定) * (1 + 百分比)
    static double compute_additive_multiplicative(double base, double flat, double pct);

    // 计算二级属性
    static void compute_derived_bases(std::array<double, Attr::MaxCount>& data,
                                      const CalcCoefficients& coeffs);
};

#endif