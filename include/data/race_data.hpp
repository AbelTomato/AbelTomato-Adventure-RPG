#pragma once

#include "base_data.hpp"
#include "definitions/core_attributes.hpp"

struct RaceData : public BaseData
{
    Attributes base_attr;          // 基本属性
    std::vector<int> race_skills;  // 种族技能

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(RaceData, id, name, description, base_attr, race_skills);
};
