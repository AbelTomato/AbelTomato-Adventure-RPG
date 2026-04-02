#pragma once

#include <vector>

#include "definitions/attribute_id.hpp"
#include "definitions/core_attributes.hpp"
#include "nlohmann/json.hpp"

struct AttrModifier  // 单条属性加成信息
{
    Attr::ID id;
    Mod mod;

    NLOHMANN_JSON_SERIALIZE_ENUM(AttrModifier, id, mod);
};

struct StatBonusComponent  // 属性加成列表组件
{
    std::vector<AttrModifier> modifiers;

    NLOHMANN_JSON_SERIALIZE_ENUM(StatBonusComponent, modifiers);
};
