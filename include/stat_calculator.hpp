#ifndef STAT_CALCULATOR_HPP
#define STAT_CALCULATOR_HPP
#include "definitions.hpp"
#include "config.hpp"

class StatCalculator
{
public:
    static int apply_standard(double base, double pct, int flat);

    static FinalAttributes make_final_attrs(const Attributes &attr, const BonusStats &bonus);

    static int calc_max_hp(const FinalAttributes &f, int level, const BonusStats &bonus, const int race_hp);

    static int calc_max_mp(const FinalAttributes &f, int level, const BonusStats &bonus, const int race_mp);

    static int calc_max_sp(const FinalAttributes &f, const BonusStats &bonus, const int race_sp);

    static int calc_evasion_value(const FinalAttributes &f, const BonusStats &bonus, const int race_evasion_value);

    static int calc_physical_hit_value(const FinalAttributes &f, const BonusStats &bonus, const int race_physical_hit_value);

    static int calc_magical_hit_value(const FinalAttributes &f, const BonusStats &bonus, const int race_magical_hit_value);

    static int calc_speed(const FinalAttributes &f, const BonusStats &bonus, int race_speed);

    static int calc_defense(const FinalAttributes &f, const BonusStats &bonus);

    static double calc_ignore_defense_rate(const FinalAttributes &f, const BonusStats &bonus, double race_ignore_defense_rate);

    static double calc_physical_crit_rate(const FinalAttributes &f, const BonusStats &bonus, double race_physical_crit_rate);

    static double calc_magical_crit_rate(const FinalAttributes &f, const BonusStats &bonus, double race_magical_crit_rate);

    static double calc_true_crit_rate(const BonusStats &bonus, double race_true_crit_rate);

    static int calc_base_physical_attack_power(const FinalAttributes &f, const BonusStats &bonus);

    static int calc_base_magical_attack_power(const FinalAttributes &f, const BonusStats &bonus);

    static int calc_base_true_attack_power(const FinalAttributes &f, const BonusStats &bonus);

    static double calc_physical_damage_reduction(const BonusStats &bonus, double race_physical_damage_reduction);

    static double calc_magical_damage_reduction(const BonusStats &bonus, double race_magical_damage_reduction);

    static double calc_true_damage_reduction(const BonusStats &bonus, double race_true_damage_reduction);

    static double calc_block_rate(const BonusStats &bonus);

    static double calc_physical_crit_damage(const BonusStats &bonus);

    static double calc_magical_crit_damage(const BonusStats &bonus);

    static double calc_true_crit_damage(const BonusStats &bonus);

    static double calc_physical_damage_increase(const BonusStats &bonus);

    static double calc_magical_damage_increase(const BonusStats &bonus);
};

#endif