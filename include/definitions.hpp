#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

#include <string>
class Creature;

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

struct RaceData
{
    RaceData()
    {
        race_name = "生物";
        base_attr = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        init_hp = init_mp = init_sp = 0;
        init_evasion_value = init_physical_hit_value = init_magical_hit_value = 0;
        init_physical_crit_rate = init_magical_crit_rate = init_true_crit_rate = 0.0;
        init_physical_damage_reduction = init_magical_damage_reduction = init_true_damage_reduction = 0.0;
        init_ignore_defense_rate = 0.0;
    }
    std::string race_name;
    Attributes base_attr;
    int init_hp, init_mp, init_sp;
    int init_evasion_value, init_physical_hit_value, init_magical_hit_value;
    double init_physical_crit_rate, init_magical_crit_rate, init_true_crit_rate;
    double init_physical_damage_reduction, init_magical_damage_reduction, init_true_damage_reduction;
    double init_ignore_defense_rate;
};

struct DerivedStats
{
    int max_hp;                       // 最大生命值
    int max_mp;                       // 最大法力值
    int max_sp;                       // 最大精力值
    int base_physical_attack_power;   // 基础物理攻击力
    int base_magical_attack_power;    // 基础魔法攻击力
    int base_true_attack_power;       // 基础真实攻击力
    double block_rate;                // 格挡率
    int evasion_value;                // 闪避值
    int physical_hit_value;           // 物理命中值
    int magical_hit_value;            // 法术命中值
    double physical_crit_rate;        // 物理暴击率
    double magical_crit_rate;         // 魔法暴击率
    double true_crit_rate;            // 真实暴击率
    int defense;                      // 防御力
    double physical_damage_reduction; // 物理减伤
    double magical_damage_reduction;  // 魔法减伤
    double true_damage_reduction;     // 真实减伤
    double ignore_defense_rate;       // 物理无视防御几率
    double physical_crit_damage;      // 物理爆伤
    double magical_crit_damage;       // 魔法爆伤
    double true_crit_damage;          // 真实爆伤
    double physical_damage_increase;  // 物理增伤
    double magical_damage_increase;   // 魔法增伤
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
    Mod max_hp, max_mp, base_physical_attack_power, base_magical_attack_power, base_true_attack_power;
    Mod evasion_value, physical_hit_value, magical_hit_value;
    Mod defense;
    Mod physical_damage, magical_damage;

    double physical_critical_damage_bonus;  // 物理爆伤加成
    double magical_critical_damage_bonus;   // 魔法爆伤加成
    double true_critical_damage_bonus;      // 真实爆伤加成
    double physical_crit_rate_bonus;        // 物理暴击率加成
    double magical_crit_rate_bonus;         // 魔法暴击率加成
    double true_crit_rate_bonus;            // 真实暴击率加成
    double block_rate_bonus;                // 格挡率加成
    double physical_damage_reduction_bonus; // 物理减伤加成
    double magical_damage_reduction_bonus;  // 魔法减伤加成
    double true_damage_reduction_bonus;     // 真实减伤加成
    double ignore_defense_rate_bonus;       // 物理无视防御几率加成
    double physical_damage_increase_bonus;  // 物理增伤加成
    double magical_damage_increase_bonus;   // 魔法增伤加成
};

enum class HitResult
{
    Miss,
    Hit,
    Critical,
    Block
};

struct AttackResult
{
    HitResult type;
    int damage;
};

enum class DamageType
{
    Physical,
    Magical,
    True
};

struct SpecialEffects
{
};

enum class SkillType
{
    CombatSkill,
    Magic
};

struct SkillData
{
    SkillData()
    {
        id = 0;
        skill_name = "普通攻击";
        description = "技能";
        damage_attribute = DamageType::Physical;
        skill_type = SkillType::CombatSkill;
        consume_hp = consume_mp = consume_sp = 0;
        damage_multiplier = 1.0;
        effects = {};
    }

    int id = 0;
    std::string skill_name;
    std::string description;
    DamageType damage_attribute;
    SkillType skill_type;
    int consume_hp, consume_mp, consume_sp;
    double damage_multiplier;
    SpecialEffects effects;
};

struct CombatIntent
{
    SkillData skill;
    Creature *attacker = nullptr;
    Creature *target = nullptr;
};

#endif