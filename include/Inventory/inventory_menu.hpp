#pragma once
#include "inventory_component.hpp"
#include "inventory_slot.hpp"
#include<vector>
#include<memory>

struct InventoryMenu
{
    std::weak_ptr<Inventory> inventory;
    std::vector<std::unique_ptr<InventorySlot>> Slots;
    
    void inicialize_menu(std::shared_ptr<Inventory> inv_ptr);
    //TODO:背包打开关闭的处理
    void toggle_menu();
    
    // 全量刷新 (通常只在初次打开时调用)
    void refresh_all_slots();
    
    // 增量刷新：绑定到底层委托的函数，哪个格子变了就只刷新那个格子
    void handle_slot_updated(int slot_index);

    //动态更新容量
    void handle_capacity_updated(int new_capacity);


};