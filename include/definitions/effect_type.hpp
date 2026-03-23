#ifndef EFFECT_TYPE_HPP
#define EFFECT_TYPE_HPP

#include <nlohmann/json.hpp>

enum class EffectType
{
    Attribute_Change,  // 瞬时改变属性（加血，加魔）
    Apply_Buff,        // 施加持续性buff
    Remove_Buff,       // 驱散buff
    Displace,          // 位移
    Speicial_Script    // 触发特殊剧情/脚本
};

NLOHMANN_JSON_SERIALIZE_ENUM(EffectType, {{EffectType::Attribute_Change, "attribute_change"},
                                          {EffectType::Apply_Buff, "apply_buff"},
                                          {EffectType::Remove_Buff, "remove_buff"},
                                          {EffectType::Displace, "displace"},
                                          {EffectType::Speicial_Script, "special_script"}});

#endif