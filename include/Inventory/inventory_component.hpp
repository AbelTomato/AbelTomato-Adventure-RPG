#pragma once
#include <algorithm>
#include <cmath>
#include <functional>
#include <optional>
#include <vector>

#include "data/item_data.hpp"
#include "definitions/item_type.hpp"


struct ItemStack
{
    int count = 0;
    std::optional<ItemData> data;
};

struct Inventory
{
    std::vector<ItemStack> items;
    // 当前背包格子数
    int current_capacity = 114;

    int max_stack = 114514;

    // TODO：请你定义背包的负重上限

    void remove_item(const ItemStack& item, int out_count);

    void add_item(const ItemData& item, int in_count);

    void swap_item(int slot_index1, int slot_index2);

    void use_item(int item_index);

    // TODO:完善扩容逻辑
    void update_inventory();

    using OnSlotUpdated = std::function<void(int slot_index)>;
    OnSlotUpdated OnSlotUpdatedDelegate;

    int return_item_count(int slot_index)
    {
        if (slot_index < 0 || slot_index >= current_capacity) return 0;

        return items[slot_index].count;
    }
    int return_capacity() const
    {
        return current_capacity;
    }

    // 游戏第一次启动时调用的初始化背包函数
    Inventory(int initial_size = 114) : current_capacity(initial_size)
    {
        // 初始化时直接填满空格子
        items.assign(current_capacity, {ItemStack()});
    }

    const ItemStack& get_item_at(int slot_index) const
    {
        return items[slot_index];
    }
};