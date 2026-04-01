#include "inventory/inventory_slot.hpp"

void InventorySlot::refresh_UI()
{
    //weak_ptr 必须使用 lock() 提升为 shared_ptr 才能安全访问
    if (auto inv_ptr = Inventory_Ref.lock()) 
    {
        // 确保 slot_index 没有越界
        if (slot_index < 0 || slot_index >= inv_ptr->return_capacity()) return;

        // 实时从底层获取真正的数据
        const ItemStack& item = inv_ptr->get_item_at(slot_index);
        
        if (item.data.has_value())
        {
            // TODO: 设置图标、设置数量、显现图标组件
        }
        else
        {
            // TODO: 隐藏图标组件，清空文本
        }
    }
    else
    {
        // 底层 Inventory 被销毁了，UI 应该进行自我销毁或重置处理
    }
}