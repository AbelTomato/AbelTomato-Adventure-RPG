#ifndef PROPERTY_CONTAINERS_HPP
#define PROPERTY_CONTAINERS_HPP

#include <definitions/core_attributes.hpp>

struct DerivedStats
{
    int max_hp;                           // 最大生命值
    int max_mp;                           // 最大法力值
    int max_sp;                           // 最大精力值
    int base_physical_attack_power;       // 基础物理攻击力
    int base_magical_attack_power;        // 基础魔法攻击力
    double block_rate;                    // 格挡率
    int evasion_value;                    // 闪避值
    int physical_hit_value;               // 物理命中值
    int magical_hit_value;                // 法术命中值
    double physical_crit_rate;            // 物理暴击率
    double magical_crit_rate;             // 魔法暴击率
    int physical_defense;                 // 物理防御力
    int magical_defense;                  // 魔法防御力
    double physical_damage_reduction;     // 物理减伤
    double magical_damage_reduction;      // 魔法减伤
    double true_damage_reduction;         // 真实减伤
    double ignore_physical_defense_rate;  // 无视物理防御几率
    double ignore_magical_defense_rate;   // 无视魔法防御几率
    double physical_crit_damage;          // 物理爆伤
    double magical_crit_damage;           // 魔法爆伤
    double physical_damage_increase;      // 物理增伤
    double magical_damage_increase;       // 魔法增伤
};

struct BonusStats
{
    void clear()
    {
        *this = {};
    }

    Mod strength, physique, dexterity, esthesia, bewitchment, willpower;
    Mod life_growth, magic_growth, speed, luck;
    Mod max_hp, max_mp, base_physical_attack_power, base_magical_attack_power;
    Mod evasion_value, physical_hit_value, magical_hit_value;
    Mod physical_defense, magical_defense;

    double physical_crit_damage;          // 物理爆伤加成
    double magical_crit_damage;           // 魔法爆伤加成
    double physical_crit_rate;            // 物理暴击率加成
    double magical_crit_rate;             // 魔法暴击率加成
    double block_rate;                    // 格挡率加成
    double physical_damage_reduction;     // 物理减伤加成
    double magical_damage_reduction;      // 魔法减伤加成
    double true_damage_reduction;         // 真实减伤加成
    double ignore_physical_defense_rate;  // 无视物理防御几率加成
    double ignore_magical_defense_rate;   // 无视魔法防御几率加成
    double physical_damage_increase;      // 物理增伤加成
    double magical_damage_increase;       // 魔法增伤加成
};

#endif