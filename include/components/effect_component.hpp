#ifndef EFFECT_COMPONENT_HPP
#define EFFECT_COMPONENT_HPP

#include <vector>

#include "definitions/effect_type.hpp"
#include "nlohmann/json.hpp"

struct PassiveAction
{
    // 触发时机类型
    EffectTriggerType trigger_type;

    // 触发几率
    double chance;

    // 触发动作id，可以是effect_id，也可以是skill_id
    int action_id;

    // 标记触发类型是否为技能
    bool is_skill;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(PassiveAction, trigger_type, chance, action_id, is_skill);
};

struct EffectComponent
{
    std::vector<PassiveAction> actions;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(EffectComponent, actions);
};

#endif