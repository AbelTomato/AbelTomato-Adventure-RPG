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
    PHYSICAL_ATTACK_POWER,  // 基于物理攻击
    MAGICAL_ATTACK_POWER,   // 基于魔法攻击
    TRUE_ATTACK_POWER,      // 基于真实攻击
    MAX_HP,                 // 基于最大生命
    CURRENT_HP,             // 基于当前生命
    MAX_MP,                 // 基于最大法力
    CURRENT_MP,             // 基于当前法力
    DEFENSE                 // 基于防御
};

NLOHMANN_JSON_SERIALIZE_ENUM(EffectSettlementType,
                             {{EffectSettlementType::PHYSICAL_ATTACK_POWER,
                               "physical_attack_power"},
                              {EffectSettlementType::MAGICAL_ATTACK_POWER, "magical_attack_power"},
                              {EffectSettlementType::TRUE_ATTACK_POWER, "true_attack_power"},
                              {EffectSettlementType::MAX_HP, "max_hp"},
                              {EffectSettlementType::CURRENT_HP, "cur_hp"},
                              {EffectSettlementType::MAX_MP, "max_mp"},
                              {EffectSettlementType::CURRENT_MP, "cur_mp"},
                              {EffectSettlementType::DEFENSE, "defense"}});

enum class CostMethod  // 消耗类型
{
    FLAT,         // 固定值
    PERCENT_MAX,  // 最大百分比
    PERCENT_CUR   // 当前百分比
};

NLOHMANN_JSON_SERIALIZE_ENUM(CostMethod, {{CostMethod::FLAT, "flat"},
                                          {CostMethod::PERCENT_MAX, "percent_max"},
                                          {CostMethod::PERCENT_CUR, "percent_cur"}});

#endif