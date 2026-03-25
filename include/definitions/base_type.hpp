#ifndef BASE_TYPE_HPP
#define BASE_TYPE_HPP

#include <nlohmann/json.hpp>

enum class TargetType  // 目标类型
{
    Self,          // 自身
    SingleEnemy,   // 单个敌人
    MultiEnemies,  // 多个敌人
    SingleAlly,    // 单个友方
    MultiAllies,   // 多个友方
    All            // 全体
};

NLOHMANN_JSON_SERIALIZE_ENUM(TargetType, {{TargetType::Self, "self"},
                                          {TargetType::SingleEnemy, "single_enemy"},
                                          {TargetType::MultiEnemies, "multi_enemies"},
                                          {TargetType::SingleAlly, "single_ally"},
                                          {TargetType::MultiAllies, "multi_allies"},
                                          {TargetType::All, "all"}});

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