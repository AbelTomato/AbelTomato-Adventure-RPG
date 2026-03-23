#ifndef GAME_CONFIG_HPP
#define GAME_CONFIG_HPP

#include <definitions/core_attributes.hpp>
#include <nlohmann/json.hpp>

struct Config
{
    struct AttrConfig
    {
        int hp_per_strength;
        int hp_per_physique;
        int hp_per_willpower;

        int mp_per_esthesia;
        int mp_per_bewitchment;
        int mp_per_willpower;

        double sp_per_strength;
        double sp_per_physique;
        double sp_per_willpower;

        int evasion_per_dexterity;
        int evasion_per_esthesia;
        int evasion_per_speed;
        int evasion_per_luck;

        int physical_hit_per_dexterity;
        int physical_hit_per_esthesia;
        int physical_hit_per_speed;
        int physical_hit_per_luck;

        int magical_hit_per_esthesia;
        int magical_hit_per_bewitchment;
        int magical_hit_per_luck;

        double speed_per_dexterity;
        double defense_per_physique;

        double ignore_defense_rate_per_esthesia;
        double ignore_defense_rate_per_luck;

        double physical_crit_rate_per_esthesia;
        double physical_crit_rate_per_luck;

        double magical_crit_rate_per_esthesia;
        double magical_crit_rate_per_luck;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(
            AttrConfig, hp_per_strength, hp_per_physique, hp_per_willpower, mp_per_esthesia,
            mp_per_bewitchment, mp_per_willpower, sp_per_strength, sp_per_physique,
            sp_per_willpower, evasion_per_dexterity, evasion_per_esthesia, evasion_per_speed,
            evasion_per_luck, physical_hit_per_dexterity, physical_hit_per_esthesia,
            physical_hit_per_speed, physical_hit_per_luck, magical_hit_per_esthesia,
            magical_hit_per_bewitchment, magical_hit_per_luck, speed_per_dexterity,
            defense_per_physique, ignore_defense_rate_per_esthesia, ignore_defense_rate_per_luck,
            physical_crit_rate_per_esthesia, physical_crit_rate_per_luck,
            magical_crit_rate_per_esthesia, magical_crit_rate_per_luck);
    };

    struct GrowthConfig
    {
        int hp_per_life_growth_per_level;
        int mp_per_magic_growth_per_level;
        int free_attribute_per_level;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(GrowthConfig, hp_per_life_growth_per_level,
                                       mp_per_magic_growth_per_level, free_attribute_per_level);
    };

    struct BattleConfig
    {
        int physical_attack_per_strength;
        int physical_attack_per_physique;

        int magical_attack_per_bewitchment;
        int magical_attack_per_willpower;

        int true_attack_per_strength;
        int true_attack_per_bewitchment;

        double hit_attr_baseline;
        double hit_grade_difference_weight;
        double hit_baseline_hit;
        double hit_upper_boundary;
        double hit_bottom_boundary;

        double block_damage_reduction_ratio;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(BattleConfig, physical_attack_per_strength,
                                       physical_attack_per_physique, magical_attack_per_bewitchment,
                                       magical_attack_per_willpower, true_attack_per_strength,
                                       true_attack_per_bewitchment, hit_attr_baseline,
                                       hit_grade_difference_weight, hit_baseline_hit,
                                       hit_upper_boundary, hit_bottom_boundary,
                                       block_damage_reduction_ratio);
    };

    struct InitialConfig
    {
    };

    AttrConfig attr;
    GrowthConfig growth;
    BattleConfig battle;
    InitialConfig initial;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Config, attr, growth, battle, initial);
};

#endif