#ifndef BUFF_TYPE_HPP
#define BUFF_TYPE_HPP

#include <nlohmann/json.hpp>

enum class TriggerType
{
    OnTurnStart,  // 回合开始时触发
    OnTurnEnd,    // 回合结束时触发
    OnAction,     // 每次行动时触发
    PassiveAttr   // 持续生效
};

NLOHMANN_JSON_SERIALIZE_ENUM(TriggerType, {{TriggerType::OnTurnStart, "on_turn_start"},
                                           {TriggerType::OnTurnEnd, "on_turn_end"},
                                           {TriggerType::OnAction, "on_action"},
                                           {TriggerType::PassiveAttr, "passive_attr"}});

#endif