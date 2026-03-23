#ifndef BUFF_TYPE_HPP
#define BUFF_TYPE_HPP

#include <nlohmann/json.hpp>

enum class TriggerType
{
    On_Turn_Start,  // 回合开始时触发
    On_Turn_End,    // 回合结束时触发
    On_Action,      // 每次行动时触发
    Passive_Attr    // 持有即触发
};

NLOHMANN_JSON_SERIALIZE_ENUM(TriggerType, {{TriggerType::On_Turn_Start, "on_turn_start"},
                                           {TriggerType::On_Turn_End, "on_turn_end"},
                                           {TriggerType::On_Action, "on_action"},
                                           {TriggerType::Passive_Attr, "passive_attr"}});

#endif