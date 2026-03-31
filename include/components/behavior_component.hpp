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

    // 触发动作目标类型
    ActionTargetType action_type;

    // 触发动作目标id
    int target_id;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(PassiveAction, trigger_type, chance, action_type, target_id);
};

struct BehaviorComponent
{
    std::vector<PassiveAction> behaviors;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(BehaviorComponent, behaviors);
};

#endif