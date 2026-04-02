#pragma once

#include <optional>

#include "base_data.hpp"
#include "components/behavior_component.hpp"
#include "definitions/attribute_id.hpp"
#include "definitions/item_type.hpp"
#include "nlohmann/json.hpp"

struct ItemData : BaseData
{
    ItemType type;
    double weight;   // 重量
    bool stackable;  // 是否可堆叠

    struct Rarity
    {
        RarityType rarity_type;  // 品质类型
        int level;               // 装备等级
        int hidden_points;       // 隐藏分

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Rarity, rarity_type, level, hidden_points);
    };

    Rarity default_rarity;  // 默认品质

    std::optional<int> usage_count;  // 可使用次数

    struct AttrRange
    {
        Attr::ID id;              // 对应属性ID
        double min_val, max_val;  // 属性范围的最小值和最大值
        bool is_percent;          // 是否为百分比加成

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(AttrRange, id, min_val, max_val, is_percent);
    };

    // 装备类型的基础属性池（池中的每一个词条都抽出来）
    std::optional<std::vector<AttrRange>> base_attr_pool;

    // 装备类型的随机属性池（从池中随机抽n个）
    std::optional<std::vector<AttrRange>> random_affix_pool;

    // 物品的行为池
    std::optional<BehaviorComponent> behaviors;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(ItemData, id, name, description, type, weight, stackable,
                                   default_rarity, usage_count, base_attr_pool, random_affix_pool,
                                   behaviors);
};
