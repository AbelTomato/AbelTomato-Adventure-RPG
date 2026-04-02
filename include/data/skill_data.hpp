#pragma once

#include <definitions/combat_type.hpp>
#include <nlohmann/json.hpp>

#include "base_data.hpp"
#include "components/behavior_component.hpp"
#include "definitions/base_type.hpp"

struct SkillData : public BaseData  // 技能仅用来触发效果，由效果决定强度
{
    std::string display_effect;  // 对于技能在数据方面的描述

    BehaviorComponent behaviors;  // 技能行为列表

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(SkillData, id, name, description, display_effect, behaviors);
};
