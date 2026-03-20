#include "stat_calculator.hpp"

int StatCalculator::apply_standard(double base, double pct, int flat)
{
    return static_cast<int>(base * (1.0 + pct) + flat);
}

FinalAttributes StatCalculator::make_final_attrs(const Attributes &attr, const BonusStats &bonus)
{
    return {
        apply_standard(attr.strength, bonus.strength.pct, bonus.strength.flat),
        apply_standard(attr.physique, bonus.physique.pct, bonus.physique.flat),
        apply_standard(attr.dexterity, bonus.dexterity.pct, bonus.dexterity.flat),
        apply_standard(attr.esthesia, bonus.esthesia.pct, bonus.esthesia.flat),
        apply_standard(attr.bewitchment, bonus.bewitchment.pct, bonus.bewitchment.flat),
        apply_standard(attr.willpower, bonus.willpower.pct, bonus.willpower.flat),
        apply_standard(attr.life_growth, bonus.life_growth.pct, bonus.life_growth.flat),
        apply_standard(attr.magic_growth, bonus.magic_growth.pct, bonus.magic_growth.flat),
        apply_standard(attr.speed, bonus.speed.pct, bonus.speed.flat),
        apply_standard(attr.luck, bonus.luck.pct, bonus.luck.flat)};
}

int StatCalculator::calc_max_hp(const FinalAttributes &f, int level, const BonusStats &bonus, const int race_hp)
{
    using namespace Config::Attr;
    using namespace Config::Growth;

    double base_hp = (f.strength * ATTR_HP_PER_STRENGTH) +
                     (f.physique * ATTR_HP_PER_PHYSIQUE) +
                     (f.willpower * ATTR_HP_PER_WILLPOWER) +
                     (f.life_growth * level * GROWTH_HP_PER_LIFE_GROWTH_PER_LEVEL) +
                     race_hp;

    return apply_standard(base_hp, bonus.max_hp.pct, bonus.max_hp.flat);
}

int StatCalculator::calc_max_mp(const FinalAttributes &f, int level, const BonusStats &bonus, const int race_mp)
{
    using namespace Config::Attr;
    using namespace Config::Growth;

    double base_mp = (f.esthesia * ATTR_MP_PER_ESTHESIA) +
                     (f.bewitchment * ATTR_MP_PER_BEWITCHMENT) +
                     (f.willpower * ATTR_MP_PER_WILLPOWER) +
                     (f.magic_growth * level * GROWTH_MP_PER_MAGIC_GROWTH_PER_LEVEL) +
                     race_mp;

    return apply_standard(base_mp, bonus.max_mp.pct, bonus.max_mp.flat);
}

int StatCalculator::calc_max_sp(const FinalAttributes &f, const BonusStats &bonus, const int race_sp)
{
    using namespace Config::Attr;

    return (f.strength * ATTR_SP_PER_STRENGTH) +
           (f.physique * ATTR_SP_PER_PHYSIQUE) +
           (f.willpower * ATTR_SP_PER_WILLPOWER) +
           race_sp;
}

int StatCalculator::calc_evasion_value(const FinalAttributes &f, const BonusStats &bonus, const int race_evasion_value)
{
    using namespace Config::Attr;

    double base_evasion_value = (f.dexterity * ATTR_EVASION_PER_DEXTERITY) +
                                (f.esthesia * ATTR_EVASION_PER_ESTHESIA) +
                                (f.speed * ATTR_EVASION_PER_SPEED) +
                                (f.luck * ATTR_EVASION_PER_LUCK) +
                                race_evasion_value;

    return apply_standard(base_evasion_value, bonus.evasion_value.pct, bonus.evasion_value.flat);
}

int StatCalculator::calc_magical_hit_value(const FinalAttributes &f, const BonusStats &bonus, const int race_magical_hit_value)
{
    using namespace Config::Attr;

    const int base_magical_hit_value = (f.esthesia * ATTR_MAGICAL_HIT_PER_ESTHESIA) +
                                       (f.bewitchment * ATTR_MAGICAL_HIT_PER_BEWITCHMENT) +
                                       (f.luck * ATTR_MAGICAL_HIT_PER_LUCK) +
                                       race_magical_hit_value;

    return apply_standard(base_magical_hit_value, bonus.magical_hit_value.pct, bonus.magical_hit_value.flat);
}

int StatCalculator::calc_speed(const FinalAttributes &f, const BonusStats &bonus, int race_speed)
{
    using namespace Config::Attr;

    double base_speed = (f.dexterity * ATTR_SPEED_PER_DEXTERITY) +
                        race_speed;

    return apply_standard(base_speed, bonus.speed.pct, bonus.speed.flat);
}

int StatCalculator::calc_defense(const FinalAttributes &f, const BonusStats &bonus)
{
    using namespace Config::Attr;

    double base_defense = (f.physique * ATTR_DEFENSE_PER_PHYSIQUE);

    return apply_standard(base_defense, bonus.defense.pct, bonus.defense.flat);
}

double StatCalculator::calc_ignore_defense_rate(const FinalAttributes &f, const BonusStats &bonus, double race_ignore_defense_rate)
{
    using namespace Config::Attr;

    double base_ignore_defense_rate = (f.esthesia * ATTR_MAGICAL_CRIT_RATE_PER_ESTHESIA) +
                                      (f.luck * ATTR_IGNORE_DEFENSE_RATE_PER_LUCK) +
                                      race_ignore_defense_rate;

    return base_ignore_defense_rate + bonus.ignore_defense_rate_bonus;
}

double StatCalculator::calc_physical_crit_rate(const FinalAttributes &f, const BonusStats &bonus, double race_physical_crit_rate)
{
    using namespace Config::Attr;

    double base_physical_crit_rate = (f.esthesia * ATTR_PHYSICAL_CRIT_RATE_PER_ESTHESIA) +
                                     (f.luck * ATTR_MAGICAL_CRIT_RATE_PER_LUCK) +
                                     race_physical_crit_rate;

    return base_physical_crit_rate + bonus.physical_crit_rate_bonus;
}

double StatCalculator::calc_magical_crit_rate(const FinalAttributes &f, const BonusStats &bonus, double race_magical_crit_rate)
{
    using namespace Config::Attr;

    double base_magical_crit_rate = (f.esthesia * ATTR_MAGICAL_CRIT_RATE_PER_ESTHESIA) +
                                    (f.luck * ATTR_MAGICAL_CRIT_RATE_PER_LUCK) +
                                    race_magical_crit_rate;

    return base_magical_crit_rate + bonus.magical_crit_rate_bonus;
}

double StatCalculator::calc_true_crit_rate(const BonusStats &bonus, double race_true_crit_rate)
{
    return bonus.true_crit_rate_bonus + race_true_crit_rate;
}

int StatCalculator::calc_base_physical_attack_power(const FinalAttributes &f, const BonusStats &bonus)
{
    using namespace Config::Battle;

    double base_base_physical_attack_power = (f.strength * BATTLE_BASE_PHYSICAL_ATTACK_POWER_PER_STRENGTH) +
                                             (f.physique * BATTLE_BASE_PHYSICAL_ATTACK_POWER_PER_PHYSIQUE);

    return apply_standard(base_base_physical_attack_power, bonus.base_physical_attack_power.pct, bonus.base_physical_attack_power.flat);
}

int StatCalculator::calc_base_magical_attack_power(const FinalAttributes &f, const BonusStats &bonus)
{
    using namespace Config::Battle;

    double base_base_magical_attack_power = (f.bewitchment * BATTLE_BASE_MAGICAL_ATTACK_POWER_PER_BEWITCHMENT) +
                                            (f.willpower * BATTLE_BASE_MAGICAL_ATTACK_POWER_PER_WILLPOWER);

    return apply_standard(base_base_magical_attack_power, bonus.base_magical_attack_power.pct, bonus.base_magical_attack_power.flat);
}

int StatCalculator::calc_base_true_attack_power(const FinalAttributes &f, const BonusStats &bonus)
{
    using namespace Config::Battle;

    double base_base_true_attack_power = (f.strength * BATTLE_BASE_TRUE_ATTACK_POWER_PER_STRENGTH) +
                                         (f.bewitchment * BATTLE_BASE_TRUE_ATTACK_POWER_PER_BEWITCHMENT);

    return apply_standard(base_base_true_attack_power, bonus.base_true_attack_power.pct, bonus.base_true_attack_power.flat);
}

double StatCalculator::calc_physical_damage_reduction(const BonusStats &bonus, double race_physical_damage_reduction)
{
    return bonus.physical_damage_reduction_bonus + race_physical_damage_reduction;
}

double StatCalculator::calc_magical_damage_reduction(const BonusStats &bonus, double race_magical_damage_reduction)
{
    return bonus.magical_damage_reduction_bonus + race_magical_damage_reduction;
}

double StatCalculator::calc_true_damage_reduction(const BonusStats &bonus, double race_true_damage_reduction)
{
    return bonus.true_damage_reduction_bonus + race_true_damage_reduction;
}

double StatCalculator::calc_block_rate(const BonusStats &bonus)
{
    return bonus.block_rate_bonus;
}

double StatCalculator::calc_physical_crit_damage(const BonusStats &bonus)
{
    return bonus.physical_critical_damage_bonus;
}

double StatCalculator::calc_magical_crit_damage(const BonusStats &bonus)
{
    return bonus.magical_critical_damage_bonus;
}

double StatCalculator::calc_true_crit_damage(const BonusStats &bonus)
{
    return bonus.true_critical_damage_bonus;
}

double StatCalculator::calc_physical_damage_increase(const BonusStats &bonus)
{
    return bonus.physical_damage_increase_bonus;
}

double StatCalculator::calc_magical_damage_increase(const BonusStats &bonus)
{
    return bonus.magical_damage_increase_bonus;
}