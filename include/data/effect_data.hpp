#pragma once

#include <nlohmann/json.hpp>

#include "base_data.hpp"

struct EffectData : public BaseData
{
    std::string func_name;  // effect映射函数名
    int logic_address;      // 逻辑定位参数

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(EffectData, id, name, description, func_name, logic_address);
};
