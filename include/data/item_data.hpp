#ifndef ITEM_DATA_HPP
#define ITEM_DATA_HPP

#include <optional>

#include "base_data.hpp"
#include "components/behavior_component.hpp"
#include "definitions/attribute_id.hpp"
#include "definitions/item_type.hpp"
#include "nlohmann/json.hpp"

struct ItemData : BaseData
{
    ItemType type;
    double weight;  // 重量
    int rarity;     // 品质

    struct AttrRange
    {
        Attr::ID id;              // 对应属性ID
        double min_val, max_val;  // 属性范围的最小值和最大值
        bool is_percent;          // 是否为百分比加成

        NLOHMANN_JSON_SERIALIZE_ENUM(AttrRange, id, min_val, max_val, is_percent);
    };

    // 装备类型的基础属性池（池中的每一个词条都抽出来）
    std::optional<std::vector<AttrRange>> base_attr_pool;

    // 装备类型的随机属性池（从池中随机抽n个）
    std::optional<std::vector<AttrRange>> random_affix_pool;

    // 物品的行为池
    std::optional<std::vector<BehaviorComponent>> behavior_pool;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(ItemData, id, name, description, type, weight, rarity,
                                   base_attr_pool, random_affix_pool, behavior_pool);
};

#endif