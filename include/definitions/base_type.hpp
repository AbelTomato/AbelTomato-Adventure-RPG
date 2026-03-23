#ifndef BASE_TYPE_HPP
#define BASE_TYPE_HPP

#include <nlohmann/json.hpp>

enum class TargetType  // 目标类型
{
    Self,
    SingleEnemy,
    MultiEnemies,
    SingleAlly,
    MultiAllies
};

NLOHMANN_JSON_SERIALIZE_ENUM(TargetType, {{TargetType::Self, "self"},
                                          {TargetType::SingleEnemy, "single_enemy"},
                                          {TargetType::MultiEnemies, "multi_enemies"},
                                          {TargetType::SingleAlly, "single_ally"},
                                          {TargetType::MultiAllies, "multi_allies"}});

#endif