#ifndef RACE_DATA_HPP
#define RACE_DATA_HPP

#include "base_data.hpp"
#include "definitions/core_attributes.hpp"

struct RaceData : public BaseData
{
    Attributes base_attr;           // 基本属性
    int init_hp, init_mp, init_sp;  // 初始生命、法力、精力
    std::vector<int> race_skills;   // 种族技能

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(RaceData, id, name, description, base_attr, init_hp, init_mp,
                                   init_sp, race_skills);
};

#endif