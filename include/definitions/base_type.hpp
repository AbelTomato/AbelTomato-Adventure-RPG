#ifndef BASE_TYPE_HPP
#define BASE_TYPE_HPP

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

enum class StatusType  // 状态类型
{
    HP,
    MP,
    SP,
    EXP
};

NLOHMANN_JSON_SERIALIZE_ENUM(StatusType, {{StatusType::HP, "hp"},
                                          {StatusType::MP, "mp"},
                                          {StatusType::SP, "sp"},
                                          {StatusType::EXP, "exp"}});

#endif