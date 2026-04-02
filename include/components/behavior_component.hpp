#pragma once

#include <vector>

#include "data/condition_data.hpp"
#include "nlohmann/json.hpp"

struct PassiveAction
{
    std::string trigger_type;               // 触发时机
    double chance;                          // 触发几率
    std::vector<ConditionData> conditions;  // 触发条件
    std::string action_type;                // 触发动作类型
    int action_id;                          // 触发动作id
    std::map<std::string, double> params;   // 参数列表

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(PassiveAction, trigger_type, chance, conditions, action_type,
                                   action_id, params);
};

struct BehaviorComponent
{
    std::vector<PassiveAction> actions;  // 行为列表

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(BehaviorComponent, actions);
};
