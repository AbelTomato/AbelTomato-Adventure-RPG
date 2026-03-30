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
    OnReleaseAttack,
    OnHitTarget,
    OnTurnStart,
    OnTurnEnd,
    OnCritTarget,
    OnUseItem,
    OnBeforeDamaged,
    OnAfterDamaged,
    OnKillTarget,
    OnBeKilled,
    OnMissAttack,
    OnBeMissed
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
#endif