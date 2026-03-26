#ifndef SKILL_TYPE_HPP
#define SKILL_TYPE_HPP

#include "nlohmann/json.hpp"

enum class SkillType  // 技能类型
{
    Initiative,  // 主动
    Passive      // 被动
};

NLOHMANN_JSON_SERIALIZE_ENUM(SkillType, {{SkillType::Initiative, "initiative"},
                                         {SkillType::Passive, "passive"}});

enum class EffectSettlementType  // 技能触发效果强度结算方式
{
    PhysicalAttackPower,  // 基于物理攻击
    MagicalAttackPower,   // 基于魔法攻击
    MaxHP,                // 基于最大生命
    CurrentHP,            // 基于当前生命
    MaxMP,                // 基于最大法力
    CurrentMP,            // 基于当前法力
    PhysicalDefense,      // 基于物理防御
    MagicalDefense        // 基于魔法防御
};

NLOHMANN_JSON_SERIALIZE_ENUM(EffectSettlementType,
                             {{EffectSettlementType::PhysicalAttackPower, "physical_attack_power"},
                              {EffectSettlementType::MagicalAttackPower, "magical_attack_power"},
                              {EffectSettlementType::MaxHP, "max_hp"},
                              {EffectSettlementType::CurrentHP, "cur_hp"},
                              {EffectSettlementType::MaxMP, "max_mp"},
                              {EffectSettlementType::CurrentMP, "cur_mp"},
                              {EffectSettlementType::PhysicalDefense, "physical_defense"},
                              {EffectSettlementType::MagicalDefense, "magical_defense"}});

enum class CostMethod  // 消耗类型
{
    Flat,        // 固定值
    PercentMax,  // 最大百分比
    PercentCur   // 当前百分比
};

NLOHMANN_JSON_SERIALIZE_ENUM(CostMethod, {{CostMethod::Flat, "flat"},
                                          {CostMethod::PercentMax, "percent_max"},
                                          {CostMethod::PercentCur, "percent_cur"}});

enum class ShapeType
{
    Point,
    Rectangle,
    Circle,
    Cross,
    Line,
    Sector
};

NLOHMANN_JSON_SERIALIZE_ENUM(ShapeType, {{ShapeType::Point, "point"},
                                         {ShapeType::Rectangle, "rectangle"},
                                         {ShapeType::Circle, "circle"},
                                         {ShapeType::Cross, "cross"},
                                         {ShapeType::Line, "line"},
                                         {ShapeType::Sector, "sector"}});

enum class TargetFilter
{
    All,
    Ally,
    Enemy
};

NLOHMANN_JSON_SERIALIZE_ENUM(TargetFilter, {{TargetFilter::All, "all"},
                                            {TargetFilter::Ally, "ally"},
                                            {TargetFilter::Enemy, "enemy"}});
#endif