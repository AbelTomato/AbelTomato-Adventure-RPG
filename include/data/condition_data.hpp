#pragma once

#include <map>
#include <string>

#include "base_data.hpp"
#include "nlohmann/json.hpp"

struct ConditionData : BaseData  // 条件数据
{
    std::string type;                    // 条件类型
    std::map<std::string, double> args;  // 判断条件映射

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(ConditionData, id, name, description, type, args);
};