# 当前项目 Bug 列表

最后更新：2026-05-25

本文档记录当前项目中已经发现的 bug、构建阻塞、数据契约不一致和高风险缺陷。  
状态约定：

- `Open`：尚未修复。
- `Blocked`：被更底层问题阻塞，暂时无法验证。
- `Needs Verify`：需要补测试或运行场景确认。

严重级别约定：

- `P0`：项目无法构建、无法启动或核心数据无法加载。
- `P1`：核心玩法/系统逻辑会产生错误结果或崩溃。
- `P2`：接口未完成、架构风险或后续功能会踩坑。
- `P3`：命名、文档、工程一致性问题。

## 构建阻塞

### BUG-001：C++ 标准与实际代码不匹配

- 状态：`Open`
- 严重级别：`P0`
- 位置：
  - `CMakeLists.txt:4`
  - `include/systems/data_manager.hpp:39`
  - `README.md:9`
- 现象：
  - `CMakeLists.txt` 设置 `CMAKE_CXX_STANDARD 17`。
  - `DataRepository<T>::load()` 使用了 `std::map::contains`。
  - `std::map::contains` 是 C++20 API，C++17 下无法编译。
  - README 仍写运行环境为 C++11。
- 影响：
  - 当前项目在 C++17 配置下无法完整编译。
  - 项目文档、构建配置、代码实际需求三者不一致。
- 建议：
  - 如果决定使用 C++20：将 CMake 标准改为 C++20，并同步 README。
  - 如果坚持 C++17：将 `temp_map.contains(id)` 改为 `temp_map.find(id) != temp_map.end()`。

### BUG-002：`ItemData` 的 `std::optional` 字段缺少 JSON 序列化支持

- 状态：`Open`
- 严重级别：`P0`
- 位置：
  - `include/data/item_data.hpp:28`
  - `include/data/item_data.hpp:40`
  - `include/data/item_data.hpp:43`
  - `include/data/item_data.hpp:46`
  - `include/data/item_data.hpp:48`
- 现象：
  - `ItemData` 中包含多个 `std::optional<T>` 字段。
  - 这些字段被直接放进 `NLOHMANN_DEFINE_TYPE_INTRUSIVE`。
  - 当前项目未提供 `std::optional` 与 nlohmann/json 的适配序列化。
- 影响：
  - 编译 `DataRepository<ItemData>` 时会报 `get_to(std::optional<...>)` 相关错误。
  - 物品数据仓库无法参与构建。
- 建议：
  - 为 `std::optional<T>` 添加统一 `to_json/from_json` 适配。
  - 或为 `ItemData` 手写 `from_json`，使用 `contains()`/`find()` 判断可选字段是否存在。

### BUG-003：默认 CMake 生成器依赖不存在的 NMake

- 状态：`Open`
- 严重级别：`P2`
- 位置：
  - 工程构建流程
- 现象：
  - 执行 `cmake -S . -B build` 时，当前 Windows 环境默认选择 `NMake Makefiles`。
  - 本机没有 `nmake`，导致配置失败。
- 影响：
  - 新开发者直接按常规 CMake 命令可能无法配置项目。
- 建议：
  - 文档中明确推荐 MinGW + Ninja。
  - 或提供 `CMakePresets.json`，固定常用生成器和编译器。

## JSON 数据契约问题

### BUG-004：`data/config.json` 与 `ConfigManager::sync_cache()` 键名不匹配

- 状态：`Open`
- 严重级别：`P0`
- 位置：
  - `data/config.json:2`
  - `data/config.json:3`
  - `src/systems/load_configs.cpp:50`
- 现象：
  - JSON 使用 `Settings`，代码结构字段是 `settings`。
  - JSON 使用 `Attr`，代码读取 `conf.get("attr", "...")`。
  - JSON 中大量 key 使用 `HpPerStrength` 形式，代码读取 `HPPerStrength` 形式。
  - 成长配置位于 `Growth`，但代码从 `attr` 分类读取成长参数。
- 影响：
  - 配置加载或缓存同步会失败。
  - 即使加载成功，属性计算缓存也可能保持默认零值，导致所有派生属性错误。
- 建议：
  - 统一 JSON 顶层字段、分类名和 key 大小写。
  - 明确 `Growth`、`Battle` 是否进入 `CalcCoefficients`。
  - 为配置加载加一组最小单元测试。

### BUG-005：`data/skill.json` 与当前 `SkillData/PassiveAction` 结构不一致

- 状态：`Open`
- 严重级别：`P0`
- 位置：
  - `data/skill.json:13`
  - `data/skill.json:40`
  - `include/data/skill_data.hpp`
  - `include/components/behavior_component.hpp`
- 现象：
  - 第一条技能使用 `actions_id`，但 `PassiveAction` 需要 `action_id`。
  - 后续技能仍使用旧字段：`cost`、`cooldown`、`target_type`、`effect_triggers`。
  - 当前 `SkillData` 只定义了 `display_effect` 和 `behaviors`。
- 影响：
  - 技能 JSON 不能稳定反序列化为当前 C++ 数据结构。
  - 后续技能系统无法建立可靠的数据驱动链路。
- 建议：
  - 决定技能数据最终 schema。
  - 若采用 `BehaviorComponent` 路线，迁移所有技能 JSON。
  - 若保留 `cost/cooldown` 等字段，需要扩展 `SkillData`。

### BUG-006：`data/buff.json` 与当前 `BuffData` 结构不一致

- 状态：`Open`
- 严重级别：`P0`
- 位置：
  - `data/buff.json:6`
  - `include/data/buff_data.hpp`
- 现象：
  - 当前 `BuffData` 只定义 `BehaviorComponent behaviors`。
  - JSON 中使用 `trigger_type`、`max_stack`、`dispellable`、`stance`、`tick_effect_ids`。
- 影响：
  - Buff JSON 无法按当前 `BuffData` 结构加载。
  - Buff 系统的数据契约尚未定型。
- 建议：
  - 明确 Buff 是直接拥有 `duration/max_stack/stance` 等字段，还是统一走 `BehaviorComponent`。
  - 更新 C++ 结构或迁移 JSON。

### BUG-007：`DataManager::load_all_data()` 期望的物品 JSON 文件缺失

- 状态：`Open`
- 严重级别：`P0`
- 位置：
  - `src/systems/data_manager.cpp`
  - `data/`
- 现象：
  - `DataManager::load_all_data()` 会加载 `weapon.json`、`armor.json`、`accessory.json`、`comsumable.json`、`material.json`、`quest.json`。
  - 当前 `data/` 目录没有这些文件。
- 影响：
  - 即使编译问题解决，调用 `load_all_data()` 也会在物品数据阶段失败。
- 建议：
  - 先提供空数组文件作为占位，例如 `[]`。
  - 或让 `DataManager` 支持可选数据表。
  - 或在开发阶段拆分 `load_core_data()` 与 `load_item_data()`。

## 背包系统问题

### BUG-008：`Inventory::remove_item()` 可能扣错数量

- 状态：`Open`
- 严重级别：`P1`
- 位置：
  - `src/InventoryLogic/inventory_component.cpp:79`
  - `src/InventoryLogic/inventory_component.cpp:83`
  - `src/InventoryLogic/inventory_component.cpp:93`
  - `src/InventoryLogic/inventory_component.cpp:105`
- 现象：
  - 第一轮查找目标格子时，代码已经修改 `left_to_remove`。
  - 第二轮实际扣除物品时，又继续使用这个已经被修改过的 `left_to_remove`。
  - 当第一轮确认数量足够后，`left_to_remove` 往往已经小于等于 0。
- 影响：
  - 可能扣除 0 个、扣错数量，甚至因为负数参与计算导致数量增加。
  - 背包核心数据不可信。
- 建议：
  - 第一轮只统计候选格和总数量，不修改真正用于扣除的剩余数量。
  - 第二轮重新以 `out_count` 作为剩余扣除量。
  - 为跨多个格子的堆叠删除写单元测试。

### BUG-009：`Inventory::remove_item()` 可能解引用空物品格

- 状态：`Open`
- 严重级别：`P1`
- 位置：
  - `src/InventoryLogic/inventory_component.cpp:90`
- 现象：
  - 遍历背包时直接访问 `I.data->id`。
  - 没有先检查 `I.data.has_value()`。
- 影响：
  - 遍历到空格时可能触发未定义行为或崩溃。
- 建议：
  - 改为 `if (I.data.has_value() && I.data->id == item.data->id)`。

### BUG-010：`Inventory::add_item()` 背包满时没有返回失败信息

- 状态：`Open`
- 严重级别：`P2`
- 位置：
  - `src/InventoryLogic/inventory_component.cpp:76`
- 现象：
  - 背包容量不足时只留下 TODO。
  - `add_item()` 返回类型为 `void`，调用方无法知道是否完全添加成功。
- 影响：
  - 拾取、奖励、商店购买等系统无法可靠处理“部分添加”或“添加失败”。
- 建议：
  - 返回剩余数量，或返回结构体 `{ added_count, leftover_count, error }`。

### BUG-011：`Inventory::use_item()` 只有类型分支，没有实际使用逻辑

- 状态：`Open`
- 严重级别：`P2`
- 位置：
  - `src/InventoryLogic/inventory_component.cpp:134`
  - `src/InventoryLogic/inventory_component.cpp:140`
- 现象：
  - 函数只判断物品类型，没有消耗、装备、触发行为、更新数量或广播刷新。
- 影响：
  - 背包中的物品无法真正使用。
- 建议：
  - 先实现消耗品数量递减与清空格子。
  - 装备类应交给装备系统或返回使用意图，不建议在背包里硬编码全部战斗逻辑。

### BUG-012：背包扩容接口声明未实现

- 状态：`Open`
- 严重级别：`P2`
- 位置：
  - `include/inventory/inventory_component.hpp:37`
- 现象：
  - `Inventory::update_inventory()` 只有声明，没有实现。
- 影响：
  - 一旦被调用会链接失败。
  - `InventoryMenu::handle_capacity_updated()` 缺少数据层配套逻辑。
- 建议：
  - 实现明确的 `resize_capacity(int new_capacity)`。
  - 扩容后广播容量变化，而不仅是格子变化。

### BUG-013：背包菜单开关接口声明未实现

- 状态：`Open`
- 严重级别：`P2`
- 位置：
  - `include/inventory/inventory_menu.hpp:14`
- 现象：
  - `InventoryMenu::toggle_menu()` 只有声明，没有实现。
- 影响：
  - 一旦 UI 层调用会链接失败。
- 建议：
  - 如果当前没有真实 UI，可以先删除声明或实现一个状态布尔值。

### BUG-014：`InventoryToolTip::hide_tool_tip()` 实现成了自由函数

- 状态：`Open`
- 严重级别：`P1`
- 位置：
  - `include/inventory/inventory_tool_tip.hpp:9`
  - `src/InventoryLogic/inventory_tool_tip.cpp:8`
- 现象：
  - 头文件声明的是成员函数 `InventoryToolTip::hide_tool_tip()`。
  - cpp 中实现的是自由函数 `void hide_tool_tip()`。
- 影响：
  - 如果代码调用 `InventoryToolTip::hide_tool_tip()`，会出现链接错误。
- 建议：
  - 将实现改为 `void InventoryToolTip::hide_tool_tip()`。

## 属性系统问题

### BUG-015：属性计算依赖硬编码偏移，未来容易静默算错

- 状态：`Open`
- 严重级别：`P2`
- 位置：
  - `src/systems/attribute_manager.cpp:29`
  - `src/systems/attribute_manager.cpp:30`
- 现象：
  - 一级属性最终值通过 `Attr::DerivedCount + i`、`Attr::ModCount + i` 推导索引。
  - 该逻辑强依赖 `Attr::ID` 的具体数值布局。
- 影响：
  - 一旦属性 ID 调整，代码可能仍能编译但计算错误。
- 建议：
  - 使用显式映射表描述 base/mod/final 三者关系。
  - 或增加静态断言和单元测试锁定 ID 布局。

### BUG-016：比例属性没有执行设计规范中的上限钳制

- 状态：`Open`
- 严重级别：`P1`
- 位置：
  - `src/systems/attribute_manager.cpp:58`
  - `src/systems/attribute_manager.cpp:64`
- 现象：
  - 暴击率、格挡率、减伤、无视防御等属性直接相加。
  - 设计规范中存在上限，例如暴击率 95%、格挡率 50%、减伤 50%、无视防御 60%。
- 影响：
  - 数值可能突破设计上限，导致战斗系统失衡。
- 建议：
  - 在最终属性写入时统一 clamp。
  - 上限应从配置读取，而不是散落硬编码。

### BUG-017：部分派生属性尚未计算默认值

- 状态：`Open`
- 严重级别：`P2`
- 位置：
  - `src/systems/stat_calculator.cpp`
  - `src/systems/attribute_manager.cpp:64`
- 现象：
  - `StatCalculator::compute_derived_bases()` 目前没有完整计算：
    - 格挡率
    - 物理/魔法/真实减伤
    - 物理/魔法暴击伤害
    - 物理/魔法增伤
  - 但 `AttributeManager::apply_secondary_mods()` 会读取这些派生值。
- 影响：
  - 最终属性可能使用默认零值，战斗结果不完整。
- 建议：
  - 明确每个派生属性的基础公式和默认值。
  - 没有公式的属性也应显式初始化默认基准。

### BUG-018：配置缓存未同步时属性计算会静默错误

- 状态：`Open`
- 严重级别：`P1`
- 位置：
  - `src/systems/attribute_manager.cpp:14`
  - `src/systems/load_configs.cpp`
- 现象：
  - `AttributeManager` 直接使用全局 `G_Cache`。
  - 如果外部忘记先调用 `ConfigManager::sync_cache()`，缓存保持零值。
- 影响：
  - 派生属性会被算成错误结果，且不一定有错误提示。
- 建议：
  - 给 `ConfigManager` 增加 `is_cache_ready` 标记。
  - `AttributeManager` 计算前断言或返回错误。
  - 更理想的方式是通过依赖注入传入 `CalcCoefficients`。

## 组件与序列化问题

### BUG-019：`StatBonusComponent` 使用了不合适的 JSON 宏

- 状态：`Open`
- 严重级别：`P1`
- 位置：
  - `include/components/stat_bonus_component.hpp:14`
  - `include/components/stat_bonus_component.hpp:21`
- 现象：
  - 普通结构体 `AttrModifier`、`StatBonusComponent` 使用了 `NLOHMANN_JSON_SERIALIZE_ENUM`。
  - 该宏用于枚举映射，不适合普通 struct 字段序列化。
- 影响：
  - 一旦相关组件被实际反序列化或更严格编译检查，可能出错。
- 建议：
  - 改用 `NLOHMANN_DEFINE_TYPE_INTRUSIVE`。
  - 同时为 `Mod` 和 `Attr::ID` 确认 JSON 适配。

### BUG-020：`include/definitions/buff_type.hpp` 为空

- 状态：`Open`
- 严重级别：`P3`
- 位置：
  - `include/definitions/buff_type.hpp`
- 现象：
  - 文件只有 `#pragma once`，没有定义内容。
- 影响：
  - Buff 类型边界不清晰。
  - 当前 `buff.json` 和 `BuffData` 不一致的问题更难收敛。
- 建议：
  - 删除空文件，或补充 Buff stance、duration policy、stack policy 等类型定义。

## 架构与事件系统风险

### BUG-021：`OnSlotUpdatedDelegate` 是单播委托，后续监听者会互相覆盖

- 状态：`Open`
- 严重级别：`P2`
- 位置：
  - `include/inventory/inventory_component.hpp`
  - `src/InventoryLogic/inventory_menu.cpp`
- 现象：
  - `Inventory` 只有一个 `OnSlotUpdatedDelegate`。
  - `InventoryMenu::inicialize_menu()` 会直接覆盖该委托。
- 影响：
  - 当前只有一个 UI 菜单时可用。
  - 未来如果日志、任务、装备面板、存档系统也监听背包变化，会互相覆盖。
- 建议：
  - 短期保留单播，但文档注明约束。
  - 中期改为观察者列表或事件总线。

### BUG-022：`InventoryDragDropOperation` 仍为空

- 状态：`Open`
- 严重级别：`P2`
- 位置：
  - `include/inventory/inventory_drag_drop_operation.hpp`
  - `src/InventoryLogic/inventory_drag_drop_operation.cpp`
- 现象：
  - 拖拽操作类没有携带源格子、物品栈、目标信息等数据。
- 影响：
  - 背包拖拽交换、丢弃、拆分堆叠等 UI 行为无法落地。
- 建议：
  - 至少定义 `source_slot_index`。
  - 后续根据 UI 需求添加拖拽数量、源背包 ID、操作类型。

## 文档与命名一致性

### BUG-023：`Comsumable` 拼写与常见英文 `Consumable` 不一致

- 状态：`Open`
- 严重级别：`P3`
- 位置：
  - `include/definitions/item_type.hpp`
  - `src/systems/data_manager.cpp`
  - `data/`
- 现象：
  - 当前代码和设计中使用 `Comsumable` / `comsumable`。
  - 常见拼写应为 `Consumable` / `consumable`。
- 影响：
  - 如果这是有意约定，必须全项目统一。
  - 如果不是有意，后续数据、UI、文档会持续混乱。
- 建议：
  - 在早期统一修正拼写。
  - 如果为了兼容已有数据，可在 JSON 层临时支持两个字符串。

### BUG-024：README、CMake、代码实际标准三者不一致

- 状态：`Open`
- 严重级别：`P3`
- 位置：
  - `README.md:9`
  - `CMakeLists.txt:4`
  - `include/systems/data_manager.hpp:39`
- 现象：
  - README 写 C++11。
  - CMake 写 C++17。
  - 代码使用 C++20 API。
- 影响：
  - 新开发者无法判断应该使用哪个标准。
  - CI 或本地构建容易出现环境差异问题。
- 建议：
  - 在修复 BUG-001 时同步更新 README。

## 建议修复顺序

1. 先修构建阻塞：
   - BUG-001
   - BUG-002
2. 再修配置与数据契约：
   - BUG-004
   - BUG-005
   - BUG-006
   - BUG-007
3. 然后修背包核心逻辑：
   - BUG-008
   - BUG-009
   - BUG-014
4. 接着补属性系统正确性：
   - BUG-016
   - BUG-017
   - BUG-018
5. 最后处理架构和一致性问题：
   - BUG-019 到 BUG-024

## 后续维护建议

- 每修复一个 bug，在本文档中将状态从 `Open` 改为 `Fixed`，并记录修复提交或修复日期。
- 对 P0/P1 问题，修复时应同时补最小单元测试。
- 未来如果引入 CI，本文件中的 P0 问题应全部转化为自动化检查。
