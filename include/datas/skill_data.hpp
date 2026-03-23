#ifndef SKILL_DATA_HPP
#define SKILL_DATA_HPP

#include <definitions/combat_type.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include "definitions/base_type.hpp"


class SkillData
{
private:
    struct DamageInfo
    {
        DamageType type;
        double multiplier;
        int fixed_bonus;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(DamageInfo, type, multiplier, fixed_bonus);
    };
    struct EffectTrigger
    {
        int effect_id;
        double chance;
        int power;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(EffectTrigger, effect_id, chance, power);
    };

    int id = 0;
    std::string skill_name;
    std::string description;

    int cost_hp, cost_mp, cost_sp;  // 消耗
    int cooldown;                   // 冷却

    TargetType target_type;        // 技能目标
    double hit_rate_bonus = 0.0;   // 命中修正
    double crit_rate_bonus = 0.0;  // 暴击修正

    std::vector<DamageInfo> damages;

    std::vector<EffectTrigger> effect_triggers;

public:
    int get_id()
    {
        return id;
    }

    const std::string& get_name()
    {
        return skill_name;
    }

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(SkillData, id, skill_name, description, cost_hp, cost_mp,
                                   cost_sp, cooldown, target_type, hit_rate_bonus, crit_rate_bonus,
                                   damages, effect_triggers);
};

#endif