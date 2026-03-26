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

#endif