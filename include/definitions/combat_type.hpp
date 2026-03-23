#ifndef COMBAT_TYPE_HPP
#define COMBAT_TYPE_HPP

#include <nlohmann/json.hpp>

class Creature;

enum class battle_result
{
    VICTORY,
    DEFEAT,
    ESCAPED
};

enum class HitResult  // 攻击结果类型
{
    Miss,
    Hit,
    Critical,
    Block
};

enum class DamageType  // 伤害类型
{
    Physical,
    Magical,
    True
};

NLOHMANN_JSON_SERIALIZE_ENUM(DamageType, {{DamageType::Physical, "physical"},
                                          {DamageType::Magical, "magical"},
                                          {DamageType::True, "true"}});

#endif