#include "inventory/inventory_menu.hpp"

void InventoryMenu::inicialize_menu(std::shared_ptr<Inventory> inv_ptr)
{
    if (!inv_ptr) return;

    //保存底层背包的弱引用（不增加引用计数，防止循环引用导致内存泄漏）
    inventory = inv_ptr;

    //绑定底层数据的委托
    //当底层背包的物品发生变化时，会自动调用这里的 Lambda 表达式，传入idx到对应的函数中
    inv_ptr->OnSlotUpdatedDelegate = [this](int index) {
        this->handle_slot_updated(index); 
    };

    //根据底层的容量，动态生成 UI 格子
    int capacity = inv_ptr->return_capacity();
    Slots.clear();
    Slots.reserve(capacity); // 预分配内存

    for (int i = 0; i < capacity; i++)
    {
        // 使用 unique_ptr 独占管理 UI 格子的生命周期
        auto new_slot = std::make_unique<InventorySlot>();
        new_slot->slot_index = i;             // 给打工人贴上工号
        new_slot->Inventory_Ref = inventory;  // 告诉打工人去哪里找数据

        Slots.push_back(std::move(new_slot));
    }

    //初次生成完毕后，执行一次全量刷新，让 UI 和数据同步
    refresh_all_slots();
    
}

void InventoryMenu::refresh_all_slots()
{
    // 遍历所有 UI 格子，命令它们各自去底层拉取最新数据
    for (auto& slot : Slots)
    {
        if (slot)
        {
            slot->refresh_UI();
        }
    }
}

void InventoryMenu::handle_slot_updated(int slot_index)
{
    //防越界
    if (slot_index < 0 || slot_index >= Slots.size()) return;

    if (Slots[slot_index])
    {
        Slots[slot_index]->refresh_UI();
    }
}

void InventoryMenu::handle_capacity_updated(int new_capacity)
{
    
    if (new_capacity <= Slots.size()) return; 

    // 若此处获得的背包指针合法
    if (auto inv_ptr = inventory.lock())
    {
        for (int i = Slots.size(); i < new_capacity; ++i)
        {
            auto new_slot = std::make_unique<InventorySlot>();
            new_slot->slot_index = i;
            //刷新Ref指向的组件
            new_slot->Inventory_Ref = inventory;
            //InventorySlot 被 std::unique_ptr 包裹，它不能被复制，只能使用 std::move 将所有权转移给 std::vector
            Slots.push_back(std::move(new_slot));
        }
        
        // 追加完毕后，让新生成的格子拿数据
        refresh_all_slots();
    }
}