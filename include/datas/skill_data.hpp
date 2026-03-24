#ifndef SKILL_DATA_HPP
#define SKILL_DATA_HPP

#include <definitions/combat_type.hpp>
#include <nlohmann/json.hpp>
#include <vector>

#include "base_data.hpp"
#include "definitions/base_type.hpp"

struct SkillData : public BaseData
{
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

    std::string display_effect;

    int cost_hp, cost_mp, cost_sp;
    int cooldown;  // 冷却时间
    TargetType target_type;
    double hit_rate_bonus = 0.0;   // 命中修正
    double crit_rate_bonus = 0.0;  // 暴击修正

    std::vector<DamageInfo> damages;
    std::vector<EffectTrigger> effect_triggers;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(SkillData, id, name, description, display_effect, cost_hp,
                                   cost_mp, cost_sp, cooldown, target_type, hit_rate_bonus,
                                   crit_rate_bonus, damages, effect_triggers);
};

#endif