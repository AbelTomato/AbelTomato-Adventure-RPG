#pragma once

#include <nlohmann/json.hpp>

enum class ActionTriggerType
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

NLOHMANN_JSON_SERIALIZE_ENUM(ActionTriggerType,
                             {{ActionTriggerType::OnReleaseAttack, "on_release_attack"},
                              {ActionTriggerType::OnHitTarget, "on_hit_target"},
                              {ActionTriggerType::OnTurnStart, "on_turn_start"},
                              {ActionTriggerType::OnTurnEnd, "on_turn_end"},
                              {ActionTriggerType::OnCritTarget, "on_crit_target"},
                              {ActionTriggerType::OnUseItem, "on_use_item"},
                              {ActionTriggerType::OnBeforeDamaged, "on_before_damaged"},
                              {ActionTriggerType::OnAfterDamaged, "on_after_damaged"},
                              {ActionTriggerType::OnKillTarget, "on_kill_target"},
                              {ActionTriggerType::OnBeKilled, "on_be_killed"},
                              {ActionTriggerType::OnMissAttack, "on_miss_attack"},
                              {ActionTriggerType::OnBeMissed, "on_be_missed"}});

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
