#ifndef ITEM_TYPE_HPP
#define ITEM_TYPE_HPP

#include "nlohmann/json.hpp"
enum class ItemType
{
    Weapon,
    Armor,
    Accessories,
    Comsumable,
    Material,
    Quest
};

NLOHMANN_JSON_SERIALIZE_ENUM(ItemType, {{ItemType::Weapon, "weapon"},
                                        {ItemType::Armor, "armor"},
                                        {ItemType::Accessories, "accessories"},
                                        {ItemType::Comsumable, "comsumable"},
                                        {ItemType::Material, "material"},
                                        {ItemType::Quest, "quest"}});

#endif