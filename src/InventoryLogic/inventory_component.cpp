#include "inventory/inventory_component.hpp"

void Inventory::add_item(const ItemData& item, int in_count)
{
    // 缓存当前需要存储量
    int left_over_count = in_count;
    if (item.stackable)
    {
        for (int i = 0; i < current_capacity; ++i)
        {
            ItemStack& I = items[i];

            if (I.data.has_value() && I.data->id == item.id && I.count < max_stack)
            {
                int res_place = max_stack - I.count;
                int can_add = std::min(left_over_count, res_place);
                I.count += can_add;
                left_over_count -= can_add;

                // 数据改变了，通知 UI 刷新这个格子
                if (OnSlotUpdatedDelegate) OnSlotUpdatedDelegate(i);

                if (left_over_count <= 0) return;
            }
        }

        if (left_over_count > 0)
        {
            for (int i = 0; i < current_capacity; ++i)
            {
                ItemStack& I = items[i];

                // 寻找完全为空的格子
                if (!I.data.has_value())
                {
                    int res_place = max_stack;
                    int can_add = std::min(left_over_count, res_place);

                    I.count += can_add;
                    left_over_count -= can_add;
                    I.data = item;

                    if (OnSlotUpdatedDelegate) OnSlotUpdatedDelegate(i);

                    if (left_over_count <= 0) return;
                }
            }
        }
    }
    // 不可堆叠之物
    else
    {
        while (left_over_count > 0)
        {
            bool placed = false;
            for (int i = 0; i < current_capacity; ++i)
            {
                ItemStack& I = items[i];
                if (!I.data.has_value())
                {
                    I.count = 1;
                    I.data = item;
                    left_over_count -= 1;
                    placed = true;

                    if (OnSlotUpdatedDelegate) OnSlotUpdatedDelegate(i);
                    break;
                }
            }
            // 如果遍历完一圈发现没放进去（没空位了），就跳出循环防止死循环
            if (!placed) break;
        }
    }
    if (left_over_count > 0)
    {
        // TODO:背包已满
    }
}
void Inventory::remove_item(const ItemStack& item, int out_count)
{
    if (out_count <= 0 || item.data.has_value()) return;

    int left_to_remove = out_count;
    // 存储当前物品的索引
    std::vector<int> tar_idx;

    for (int i = 0; i < current_capacity; i++)
    {
        ItemStack& I = items[i];
        if (I.data->id == item.data->id)
        {
            tar_idx.push_back(i);
            left_to_remove -= items[i].count;
            if (left_to_remove <= 0) break;  // 提前结束遍历，因为数量已经确认足够了
        }
    }
    if (left_to_remove > 0)
    {
        // TODO:打印不足以扣除
        return;
    }

    for (const int& num : tar_idx)
    {
        int now_to_remove = std::min(left_to_remove, items[num].count);

        items[num].count -= now_to_remove;
        left_to_remove -= now_to_remove;

        // 清空本格子
        if (left_to_remove <= 0)
        {
            items[num].data = std::nullopt;
        }

        // 广播
        if (OnSlotUpdatedDelegate) OnSlotUpdatedDelegate(num);
    }
}

void Inventory::swap_item(int slot_index1, int slot_index2)
{
    if (slot_index1 < 0 || slot_index1 >= current_capacity || slot_index2 < 0 ||
        slot_index2 >= current_capacity)
        return;
    std::swap(items[slot_index1], items[slot_index2]);
}

void Inventory::use_item(int slot_index1)
{
    if (slot_index1 < 0 || slot_index1 >= current_capacity) return;

    auto& item = items[slot_index1];
    if (!item.data.has_value()) return;
    // TODO:完善消耗逻辑
    switch (item.data->type)
    {
        case ItemType::Weapon:
            break;
        case ItemType::Armor:
            break;
        case ItemType::Comsumable:
            break;
        case ItemType::Accessory:
            break;
        case ItemType::Quest:
            break;
        case ItemType::Material:
            break;
        default:
            break;
    }
}
