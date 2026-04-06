# 背包系统总览

* 背包系统分为**数据层**和**UI管理层**，完整实现一个背包系统，需要对数据层，主世界交互，UI层面三者之间的通信有一个充分的了解。
* 我将背包简单拆解成了五个头文件，分别是**inventory_component**，背包菜单栏**Menu**，菜单栏管辖的子层级**Slot**，以及负责显示物品描述和处理拖拽行为的**tool_tip**,**drag_drop_operation**


## 1. 数据层 inventory_component

### 1.1 设计逻辑与接口说明
---
本组件主要处理的是数据层面中拾取，丢弃，使用，交换物品；背包扩容，负重逻辑。

定义结构体**ItemStack**，存储物品的**ItemData**和当前格子存储的数量，使用**std::function**，定义委托**OnSlotUpdatedDelegate**，广播背包发生变化的格子索引，由UI层面的**Menu**和**Slot** 收听广播。
#### 1.1.1 add_item
---
当玩家试图从世界中获取物品时，调用本函数，传入物品信息**ItemData**以及存储数量。代码片段如下
```cpp
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
```
通过**std::optional**管理物品，并且通过遍历格子的方法为每一个空格子创建一个空的**ItemStack**，将拾取的物品填充进去。
当然，这是处理可堆叠物品的逻辑，非可堆叠物品的逻辑相似，不再赘述。
#### 1.1.2 remove_item
---
与添加物品的逻辑类似，通过数组**tar_idx**存储物品所在的格子索引，通过格子索引找到对应的物品进行移除操作
```cpp
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
```
#### 1.1.3 use_item
---
在**ItemData**中，我们定义了一个物品枚举类**ItemType**，用于给世界的物品贴标签。对于标签的不同，对应的使用逻辑也会不同

#### 1.1.4 swap_item
---
用于处理玩家拖拽物品到任意格子时的操作，同样通过广播**OnSlotUpdatedDelegate**，通知UI层变化

## 2.UI层 inventory_menu
### 2.1 设计逻辑与接口说明
---
背包菜单的逻辑较为简单，在第一次启动游戏的时候，我们会调用一次**inicialize_menu**，对菜单进行初始化操作，包括初始化指向背包的弱引用指针**std::weak_ptr\<Inventory\> inventory;**，为函数绑定委托，动态生成UI格子。
#### 2.1.1 inicialize_menu
上文我们讲到本函数的逻辑是负责初始化背包，事实也确实如此，故略。
#### 2.1.2 refresh_all_slots
对于每一个格子，调用它自身的接口**refresh_UI**，从数据层中拿到信息，并刷新显示的图片和存储量
#### 2.1.3 handle_slot_updated
接收来自数据层的格子索引广播，在背包初始化的时候本接口已被执行如下操作
```cpp
//当底层背包的物品发生变化时，会自动调用这里的 Lambda 表达式，传入idx到对应的函数中
    inv_ptr->OnSlotUpdatedDelegate = [this](int index) {
        this->handle_slot_updated(index); 
    };
```
然后我们再调用格子的**refresh_UI()**即可刷新格子中的信息。

#### 2.1.4 handle_capacity_updated
字面意思，用于扩容背包。

由于**weak_ptr**无法直接操作，因此需要通过**Lock()**把指向背包的指针升级为**share_ptr**，之后再从原本的大小开始一点点扩容，直到新的容量上限为止。
```cpp
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

```

## 3.UI层 inventory_slot
### 3.1 设计逻辑与接口说明
隶属于**Menu**的层级，负责存储格子的索引，以及指向背包的弱指针，方便随时拿数据。
#### 3.1.1 refresh_UI
三步骤
* 提升背包组件指针为**share_ptr**
* 从数据层拿取本格子的物品数据
* 刷新显示UI

## 碎碎念/建议栏
剩下两个不着急写.jpg，
后续会改进一下处理非法操作的逻辑
可随时提建议~