#pragma once

#include "nlohmann/json.hpp"
enum class ItemType
{
    Weapon,      // 武器
    Armor,       // 防具
    Accessory,   // 饰品
    Comsumable,  // 消耗品
    Material,    // 材料
    Quest        // 任务物品
};

NLOHMANN_JSON_SERIALIZE_ENUM(ItemType, {{ItemType::Weapon, "weapon"},
                                        {ItemType::Armor, "armor"},
                                        {ItemType::Accessory, "accessory"},
                                        {ItemType::Comsumable, "comsumable"},
                                        {ItemType::Material, "material"},
                                        {ItemType::Quest, "quest"}});
enum class RarityType
{
    Junk,       // 垃圾，灰色
    Common,     // 基础，白色
    Uncommon,   // 精良，绿色
    Rare,       // 稀有，蓝色
    Epic,       // 史诗，紫色
    Legendary,  // 传说，橙色
    Artifact    // 神话，红色
};

NLOHMANN_JSON_SERIALIZE_ENUM(RarityType, {{RarityType::Junk, "junk"},
                                          {RarityType::Common, "common"},
                                          {RarityType::Uncommon, "uncommon"},
                                          {RarityType::Rare, "rare"},
                                          {RarityType::Epic, "epic"},
                                          {RarityType::Legendary, "legendary"},
                                          {RarityType::Artifact, "artifact"}});
