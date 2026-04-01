#pragma once
#include"data/item_data.hpp"
#include"inventory_tool_tip.hpp"
#include"inventory_component.hpp"
#include"inventory_component.hpp"
#include"inventory_drag_drop_operation.hpp"
#include<functional>
#include<memory>

struct InventorySlot
{
    void set_item(const ItemStack& item);
    //格子的索引
    int slot_index;
    
    //弱指针随时查看背包组件
    std::weak_ptr<Inventory>Inventory_Ref;
    
    //TODO::图形化之后 声明图片 文本 按钮 
    
    //数据层收到外界的快递 调用本函数 刷新背包内显示信息
    void refresh_UI();
    //处理鼠标事件
	//TODO:依旧图形化再处理
    void on_mouse_enter(); 										// 触发 Tooltip
    void on_mouse_leave(); 										// 隐藏 Tooltip
    void on_drag_detected(); 									// 生成 InventoryDragDropOperation
    //bool on_drop(const InventoryDragDropOperation& drop_op); 	// 处理丢弃事件
    
};