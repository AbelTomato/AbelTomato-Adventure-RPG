#ifndef EFFECT_TYPE_HPP
#define EFFECT_TYPE_HPP

#include <nlohmann/json.hpp>

enum class EffectType
{
    AttributeChange,  // 瞬时改变属性（加血，加魔）
    ApplyBuff,        // 施加持续性buff
    RemoveBuff,       // 驱散buff
    Displace,         // 位移
    SpeicialScript    // 触发特殊剧情/脚本
};

NLOHMANN_JSON_SERIALIZE_ENUM(EffectType, {{EffectType::AttributeChange, "attribute_change"},
                                          {EffectType::ApplyBuff, "apply_buff"},
                                          {EffectType::RemoveBuff, "remove_buff"},
                                          {EffectType::Displace, "displace"},
                                          {EffectType::SpeicialScript, "special_script"}});

enum class EffectTriggerType
{
    OnReleaseAttack,  // 释放攻击时
    OnHitTarget,      // 命中目标时
    OnTurnStart,      // 回合开始时
    OnTurnEnd,        // 回合结束时
    OnCritTarget,     // 暴击时
    OnUseItem,        // 使用物品时
    OnBeforeDamaged,  // 受击前
    OnAfterDamaged,   // 受击后
    OnKillTarget,     // 击杀目标后
    OnBeKilled,       // 被击杀后
    OnMissAttack,     // 闪避攻击时
    OnBeMissed        // 攻击被闪避时
};

NLOHMANN_JSON_SERIALIZE_ENUM(EffectTriggerType,
                             {{EffectTriggerType::OnReleaseAttack, "on_release_attack"},
                              {EffectTriggerType::OnHitTarget, "on_hit_target"},
                              {EffectTriggerType::OnTurnStart, "on_turn_start"},
                              {EffectTriggerType::OnTurnEnd, "on_turn_end"},
                              {EffectTriggerType::OnCritTarget, "on_crit_target"},
                              {EffectTriggerType::OnUseItem, "on_use_item"},
                              {EffectTriggerType::OnBeforeDamaged, "on_before_damaged"},
                              {EffectTriggerType::OnAfterDamaged, "on_after_damaged"},
                              {EffectTriggerType::OnKillTarget, "on_kill_target"},
                              {EffectTriggerType::OnBeKilled, "on_be_killed"},
                              {EffectTriggerType::OnMissAttack, "on_miss_attack"},
                              {EffectTriggerType::OnBeMissed, "on_be_missed"}});

enum class ActionTargetType
{
    None = 0,
    Effect,
    Skill,
    Buff
};

NLOHMANN_JSON_SERIALIZE_ENUM(ActionTargetType, {{ActionTargetType::None, "none"},
                                                {ActionTargetType::Effect, "effect"},
                                                {ActionTargetType::Skill, "skill"},
                                                {ActionTargetType::Buff, "buff"}});
#endif