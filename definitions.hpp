#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

#include <string>

struct Attributes
{                    // 基本属性
    int strength;    // 力量
    int physique;    // 体质
    int dexterity;   // 灵巧
    int esthesia;    // 感知
    int bewitchment; // 魔力
    int willpower;   // 意志

    int life_growth;  // 生命成长
    int magic_growth; // 法力成长
    int speed;        // 速度
    int luck;         // 幸运
};

struct RaceData // TODO:增加初始数据
{
    std::string race_name;
    Attributes base_attr;
    int init_hp, init_mp;
    int init_evasion_value, init_hit_value;
    double init_physical_crit_rate, init_magical_crit_rate;
    double init_physical_damage_reduction, init_magical_damage_reduction;
    double init_ignore_defense_rate;
};

struct DerivedStats
{
    int max_hp;                       // 最大生命值
    int max_mp;                       // 最大法力值
    int max_sp;                       // 最大精力值
    int base_physical_attack_power;   // 基础物理攻击力
    int base_magical_attack_power;    // 基础魔法攻击力
    double block_rate;                // 格挡率
    int evasion_value;                // 闪避值
    int hit_value;                    // 命中值
    double physical_crit_rate;        // 物理暴击率
    double magical_crit_rate;         // 魔法暴击率
    int defense;                      // 防御力
    double physical_damage_reduction; // 物理减伤
    double magical_damage_reduction;  // 魔法减伤
    double ignore_defense_rate;       // 物理无视防御几率
};

struct Mod
{
    double pct = 0.0;
    int flat = 0;

    void clear()
    {
        pct = 0.0;
        flat = 0;
    }

    Mod &operator+=(const Mod &other)
    {
        pct += other.pct;
        flat += other.flat;

        return *this;
    }
};

struct FinalAttributes
{
    void clear()
    {
        *this = {};
    }

    int strength, physique, dexterity, esthesia, bewitchment, willpower;
    int life_growth, magic_growth;
    int speed, luck;
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
    Mod evasion_value, hit_value;
    Mod defense;
    Mod physical_damage, magical_damage;

    double physical_critical_damage_bonus;  // 物理爆伤加成
    double magical_critical_damage_bonus;   // 魔法爆伤加成
    double physical_crit_rate_bonus;        // 物理暴击率加成
    double magical_crit_rate_bonus;         // 魔法暴击率加成
    double block_rate_bonus;                // 格挡率加成
    double physical_damage_reduction_bonus; // 物理减伤加成
    double magical_damage_reduction_bonus;  // 魔法减伤加成
    double ignore_defense_rate_bonus;       // 物理无视防御几率加成
};

namespace Config
{
    namespace Attr
    { // 属性换算
        static constexpr int ATTR_HP_PER_STRENGTH = 8;
        static constexpr int ATTR_HP_PER_PHYSIQUE = 15;
        static constexpr int ATTR_HP_PER_WILLPOWER = 3;

        static constexpr int ATTR_MP_PER_ESTHESIA = 3;
        static constexpr int ATTR_MP_PER_BEWITCHMENT = 12;
        static constexpr int ATTR_MP_PER_WILLPOWER = 7;

        static constexpr double ATTR_SP_PER_STRENGTH = 0.3;
        static constexpr double ATTR_SP_PER_PHYSIQUE = 0.5;
        static constexpr double ATTR_SP_PER_WILLPOWER = 0.2;

        static constexpr int ATTR_EVASION_PER_DEXTERITY = 2;
        static constexpr int ATTR_EVASION_PER_ESTHESIA = 1;
        static constexpr int ATTR_EVASION_PER_SPEED = 3;
        static constexpr int ATTR_EVASION_PER_LUCK = 5;

        static constexpr int ATTR_HIT_PER_DEXTERITY = 1;
        static constexpr int ATTR_HIT_PER_ESTHESIA = 2;
        static constexpr int ATTR_HIT_PER_SPEED = 2;
        static constexpr int ATTR_HIT_PER_LUCK = 3;

        static constexpr double ATTR_SPEED_PER_DEXTERITY = 0.2;

        static constexpr double ATTR_DEFENSE_PER_PHYSIQUE = 3;

        static constexpr double ATTR_IGNORE_DEFENSE_RATE_PER_ESTHESIA = 0.0007;
        static constexpr double ATTR_IGNORE_DEFENSE_RATE_PER_LUCK = 0.001;

        static constexpr double ATTR_PHYSICAL_CRIT_RATE_PER_ESTHESIA = 0.0002;
        static constexpr double ATTR_PHYSICAL_CRIT_RATE_PER_LUCK = 0.0005;

        static constexpr double ATTR_MAGICAL_CRIT_RATE_PER_ESTHESIA = 0.0001;
        static constexpr double ATTR_MAGICAL_CRIT_RATE_PER_LUCK = 0.0003;
    }

    namespace Growth
    { // 成长与级别
        static constexpr int GROWTH_HP_PER_LIFE_GROWTH_PER_LEVEL = 5;
        static constexpr int GROWTH_MP_PER_MAGIC_GROWTH_PER_LEVEL = 4;

        static constexpr int GROWTH_FREE_ATTRIBUTE_PER_LEVEL = 10;
    }

    namespace Battle
    { // 战斗机制
        static constexpr int BATTLE_BASE_PHYSICAL_ATTACK_POWER_PER_STRENGTH = 5;
        static constexpr int BATTLE_BASE_PHYSICAL_ATTACK_POWER_PER_PHYSIQUE = 1;

        static constexpr int BATTLE_BASE_MAGICAL_ATTACK_POWER_PER_BEWITCHMENT = 6;
        static constexpr int BATTLE_BASE_MAGICAL_ATTACK_POWER_PER_WILLPOWER = 1;
    }

    namespace Initial
    { // 初始状态

    }
}

#endif