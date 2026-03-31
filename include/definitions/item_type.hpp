#ifndef ITEM_TYPE_HPP
#define ITEM_TYPE_HPP

#include "nlohmann/json.hpp"
enum class ItemType
{
    Weapon,       // 武器
    Armor,        // 防具
    Accessories,  // 饰品
    Comsumable,   // 消耗品
    Material,     // 材料
    Quest         // 任务物品
};

NLOHMANN_JSON_SERIALIZE_ENUM(ItemType, {{ItemType::Weapon, "weapon"},
                                        {ItemType::Armor, "armor"},
                                        {ItemType::Accessories, "accessories"},
                                        {ItemType::Comsumable, "comsumable"},
                                        {ItemType::Material, "material"},
                                        {ItemType::Quest, "quest"}});

#endif