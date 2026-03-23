#ifndef RACE_DATA_HPP
#define RACE_DATA_HPP

#include <string>

#include "base_data.hpp"
#include "definitions/core_attributes.hpp"

class RaceData : public BaseData
{
private:
    Attributes base_attr;           // 基本属性
    int init_hp, init_mp, init_sp;  // 初始生命、法力、精力
    std::vector<int> race_skills;   // 种族技能

public:
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(RaceData, id, name, base_attr, init_hp, init_mp, init_sp,
                                   race_skills);
};

#endif