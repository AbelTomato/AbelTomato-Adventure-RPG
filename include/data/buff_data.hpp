#pragma once

#include "base_data.hpp"
#include "components/behavior_component.hpp"

struct BuffData : public BaseData
{
    BehaviorComponent behaviors;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(BuffData, id, name, description, behaviors);
};
