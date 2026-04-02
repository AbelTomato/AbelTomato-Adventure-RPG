#pragma once

#include <cstdint>
#include <nlohmann/json.hpp>

using ValueType = int64_t;

enum class DamageType  // 伤害类型
{
    Physical,  // 物理
    Magical,   // 魔法
    True       // 真实
};

NLOHMANN_JSON_SERIALIZE_ENUM(DamageType, {{DamageType::Physical, "physical"},
                                          {DamageType::Magical, "magical"},
                                          {DamageType::True, "true"}});
