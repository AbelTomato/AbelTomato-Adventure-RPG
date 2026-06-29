# CODEX 项目记忆 中文版

最后更新：2026-06-27

> 归档说明：本文是早期 C++ 单体原型阶段的项目记忆，保留用于理解历史设计、数据结构和旧 C++ 风险。当前执行状态以 `docs/README.md`、`docs/原子化发展路线.md`、`docs/代码评审.md` 为准。

这份文档是根据 `docs/CODEX_Memory.md` 整理出的中文阅读版，用来帮助人类开发者快速理解当前项目状态、架构意图、已有代码边界和主要风险。

## 1. 项目定位

- 项目名：`Abel-Tomato-Soul`，README 中称为 `番茄战魂`。
- 当前形态：C++ 控制台/文字 RPG 原型，使用 CMake、MinGW 和项目内置的 `nlohmann/json`。
- 设计目标：数据驱动 RPG 核心，包括属性、种族、职业、技能、效果、Buff、物品、背包、本地 JSON 存档、网格地图探索、基于速度比的半即时回合制。
- 当前实现程度：设计文档已经覆盖中型 RPG 架构，但可执行程序还很薄。`src/main.cpp` 基本为空，代码主要集中在数据结构、配置加载、属性计算、背包雏形。

## 2. 设计规范理解

- 核心一级属性：
  - 力量 `strength`
  - 体质 `physique`
  - 灵巧 `dexterity`
  - 感知 `esthesia`
  - 魔力 `bewitchment`
  - 意志 `willpower`
  - 生命成长 `life_growth`
  - 法力成长 `magic_growth`
  - 速度 `speed`
  - 幸运 `luck`
- 属性体系分为：
  - 基础属性
  - 派生属性
  - 属性加成
  - 最终属性
  - 当前运行时状态
- 属性 ID 由 `Attr::ID` 统一管理，本质上是整个数值系统的核心契约。
- 属性加成有两类：
  - `Mod { pct, flat }`：百分比加成和固定值加成。
  - `double`：暴击率、格挡率、减伤、增伤、无视防御等比例属性的直接加法加成。
- 核心玩法方向：
  - 半即时回合制，依据速度比决定敌我行动频率。
  - 遵循“玩家不动，世界静止”的交互节奏。
  - 二维方格地图，支持八方向移动。
  - 碰撞、事件、环境效果、技能范围都基于网格判断。
  - 世界循环是“大地图探索 - 资源采集 - 动态遇敌 - 阶梯式开荒”。
  - 存档采用本地 JSON 快照。
- 数据导入约定：JSON 中的枚举值应使用字符串表示。

## 3. 目录结构认知

- `data/`：当前 JSON 数据样例和配置，包括 `config.json`、`race.json`、`job.json`、`skill.json`、`effect.json`、`buff.json`。
- `docs/`：设计文档和项目记忆。
- `include/definitions/`：基础枚举、属性 ID、属性结构、物品类型、技能类型等。
- `include/data/`：可序列化的数据结构，例如 `RaceData`、`JobData`、`SkillData`、`EffectData`、`BuffData`、`ItemData`。
- `include/components/`：可复用玩法组件，例如 `BehaviorComponent`、`StatBonusComponent`。
- `include/systems/`：配置加载、数据仓库、属性管理、数值计算。
- `include/inventory/`：背包数据和 UI 协作层头文件。
- `src/systems/`：属性计算、配置加载、数据加载的实现。
- `src/InventoryLogic/`：背包逻辑和 UI 占位实现。
- `third_party/nlohmann/json.hpp`：项目内置 JSON 库。

## 4. 当前运行时架构

### 4.1 定义层

- `include/definitions/attribute_id.hpp` 中的 `Attr::ID` 是属性系统的中心。
- `include/definitions/core_attributes.hpp` 定义了：
  - `Attributes`
  - `FinalAttributes`
  - `Mod`
- `include/definitions/property_containers.hpp` 定义了：
  - `DerivedStats`
  - `BonusStats`
- 当前真正参与计算的不是 `DerivedStats`/`BonusStats` 容器，而是 `AttributeManager` 内部的数组索引系统。
- 多个枚举已经绑定了 nlohmann/json 字符串序列化，例如：
  - `DamageType`
  - `ActionTriggerType`
  - `ActionTargetType`
  - `EffectType`
  - `ItemType`
  - `RarityType`
  - `SkillType`
  - `CostMethod`
  - `ShapeType`
  - `TargetFilter`

### 4.2 数据层

- 多数数据结构都基于 `BaseData { id, name, description }`。
- `RaceData`：
  - 包含 `Attributes base_attr`
  - 包含 `race_skills`
- `JobData`：
  - 包含 `Attributes base_attr`
  - 包含 `job_skills`
- `SkillData`：
  - 当前期望字段为 `display_effect`
  - 当前期望字段为 `BehaviorComponent behaviors`
- `BehaviorComponent`：
  - 内部是 `std::vector<PassiveAction> actions`
- `PassiveAction` 当前代码期望字段：
  - `trigger_type`
  - `chance`
  - `conditions`
  - `action_type`
  - `action_id`
  - `params`
- `EffectData`：
  - 用 `func_name` 映射未来的效果函数。
  - 用 `logic_address` 指向相关属性或逻辑参数。
  - 这暗示后续应设计一个注册表式 `EffectEngine`。
- `ItemData`：
  - 支持物品类型、重量、是否可堆叠、默认品质。
  - 支持可选使用次数。
  - 支持基础属性池、随机词条池、行为组件。

### 4.3 配置与数据加载

- `ConfigManager` 是单例，负责：
  - 保存 `Config configs`
  - 保存计算缓存 `CalcCoefficients cache`
  - 从 JSON 加载配置
  - 把配置同步到缓存
- `Config` 内部是嵌套 map：
  - `category -> key -> double`
- `DataRepository<T>` 是通用数据仓库：
  - 从 JSON 数组加载数据。
  - 保存 `id -> data`。
  - 保存 `name -> id`。
- `DataManager` 是单例，负责所有数据仓库：
  - 种族
  - 技能
  - 效果
  - Buff
  - 职业
  - 武器
  - 防具
  - 饰品
  - 消耗品
  - 材料
  - 任务物品
- `DataManager::load_all_data()` 当前期望存在以下物品文件：
  - `weapon.json`
  - `armor.json`
  - `accessory.json`
  - `comsumable.json`
  - `material.json`
  - `quest.json`
- 这些物品 JSON 文件目前还不存在。

### 4.4 属性系统

- `AttributeManager` 使用：
  - `std::array<double, Attr::MaxCount> _data`
  - `std::array<Mod, Attr::MaxCount> _mods`
- 属性重算采用脏标记 `_is_dirty`，只有读取属性时才触发重算。
- 重算流程：
  1. `calculate_primary_finals()`：计算一级属性最终值。
  2. `StatCalculator::compute_derived_bases()`：计算派生属性基准值。
  3. `apply_secondary_mods()`：应用二级属性加成，得到最终属性。
- `StatCalculator` 已实现一部分设计规范中的公式：
  - HP
  - MP
  - SP
  - 物理攻击
  - 魔法攻击
  - 闪避
  - 物理命中
  - 魔法命中
  - 物理防御
  - 魔法防御
  - 无视物理防御
  - 无视魔法防御
  - 物理暴击率
  - 魔法暴击率
- 还没有完整实现或钳制的内容：
  - 格挡率
  - 伤害减免
  - 真实伤害减免
  - 暴击伤害
  - 增伤
  - 战斗属性上限，例如暴击率 95%、格挡率 50%。

### 4.5 背包系统

- `docs/inventory_design.md` 中的目标分层很清楚：
  - `Inventory` / `InventoryComponent`：数据层。
  - `InventoryMenu`：背包 UI 管理者。
  - `InventorySlot`：单个格子 UI。
  - `InventoryToolTip`：物品说明 UI。
  - `InventoryDragDropOperation`：拖拽操作数据。
- 当前 `Inventory` 拥有：
  - `std::vector<ItemStack> items`
  - `current_capacity`
  - `max_stack`
  - 单播事件 `OnSlotUpdatedDelegate`
- `ItemStack` 是：
  - `int count`
  - `std::optional<ItemData> data`
- `Inventory::add_item()`：
  - 已支持可堆叠物品添加。
  - 已支持不可堆叠物品逐格添加。
  - 每次格子变化会广播格子索引。
- `Inventory::swap_item()`：
  - 交换两个格子。
  - 广播两个格子的刷新事件。
- `Inventory::use_item()`：
  - 当前只是按 `ItemType` switch 的占位逻辑。
- `InventoryMenu`：
  - 使用 `std::weak_ptr<Inventory>` 指向背包数据层。
  - 使用 `std::vector<std::unique_ptr<InventorySlot>>` 管理 UI 格子。
- `InventorySlot::refresh_UI()`：
  - 从背包数据层拉取当前格子的真实数据。
  - 图形化显示逻辑尚未实现。

## 5. 当前数据状态

- `data/race.json` 基本匹配 `RaceData`。
- `data/job.json` 基本匹配 `JobData`。
- `data/effect.json` 基本匹配当前 `EffectData`。
- `data/config.json` 和当前 `ConfigManager::sync_cache()` 不匹配：
  - JSON 使用 `Settings`，代码期望 `settings`。
  - JSON 使用 `Attr`、`Growth`、`Battle`，代码读取 `attr`。
  - 多个键名大小写不同，例如 `HpPerStrength` 和 `HPPerStrength`。
  - 成长相关配置放在 `Growth` 下，但代码从 `attr` 中读取。
- `data/skill.json` 是新旧结构混合：
  - 第一个技能中使用 `actions_id`，但代码期望 `action_id`。
  - 后续技能使用 `cost`、`cooldown`、`target_type`、`effect_triggers`，但当前 `SkillData` 期望 `behaviors`。
  - 部分技能 ID 例如 `5001` 不符合设计规范中的技能 ID 区间。
- `data/buff.json` 当前也是旧结构：
  - 代码中的 `BuffData` 期望 `behaviors`。
  - JSON 里是 `trigger_type`、`max_stack`、`dispellable`、`stance`、`tick_effect_ids` 等字段。
- `DataManager` 期望的物品分类 JSON 文件目前缺失。

## 6. 构建验证状态

- 执行 `cmake -S . -B build` 失败：
  - CMake 默认选择 `NMake Makefiles`。
  - 当前环境没有 `nmake`。
- 使用 Ninja + MinGW 可以配置成功：
  - MinGW GCC 版本为 13.1.0。
  - 配置命令可用，但编译还不通过。
- 编译失败的主要原因：
  - `DataRepository<T>::load()` 使用了 `std::map::contains`。
  - 但 `CMakeLists.txt` 设置的是 C++17，`contains` 是 C++20 API。
  - `ItemData` 中有 `std::optional<T>` 字段，但当前 nlohmann/json 宏没有对应 optional 序列化支持。
- README 写的是 C++11，CMake 写的是 C++17，代码实际已经使用 C++17 和部分 C++20 能力。需要统一标准。

## 7. 重要风险与已发现问题

- 当前不能假设 JSON 数据可以直接加载成功，数据结构和代码结构还没有对齐。
- `Inventory::remove_item()` 有明显逻辑问题：
  - 第一轮查找目标格子时已经修改了 `left_to_remove`。
  - 第二轮真正扣除时又复用这个已被修改的值。
  - 这会导致扣除数量错误，甚至出现负数逻辑。
  - 遍历时还直接访问 `I.data->id`，没有先判断 `I.data.has_value()`。
- `InventoryToolTip::hide_tool_tip()`：
  - 头文件中声明为成员函数。
  - cpp 中实现成了自由函数。
- `InventoryDragDropOperation` 当前为空。
- `InventoryMenu::toggle_menu()` 只声明未实现。
- `Inventory::update_inventory()` 只声明未实现。
- `OnSlotUpdatedDelegate` 当前是单播委托：
  - 一个菜单监听没有问题。
  - 未来如果有多个监听者，后绑定者会覆盖前一个监听者。
- `AttributeManager::calculate_primary_finals()` 强依赖枚举值和偏移常量：
  - `DerivedCount`
  - `ModCount`
  - 如果属性 ID 布局变动，这里很容易静默出错。
- `AttributeManager::apply_secondary_mods()` 暂未执行战斗属性上限钳制。
- `ConfigManager::sync_cache()` 必须在 `load_all_configs()` 后调用，否则 `G_Cache` 会保持默认零值，属性计算会全部失真。
- `include/definitions/buff_type.hpp` 当前为空。
- `StatBonusComponent` 对普通 struct 使用了 `NLOHMANN_JSON_SERIALIZE_ENUM`，这看起来不符合预期，后续需要检查。

## 8. 应保留的架构方向

- 核心游戏逻辑应保持和 UI、渲染解耦。
- 当前比较合理的边界是：
  - 数据与配置定义层
  - 纯逻辑系统层，例如 `AttributeManager`、`StatCalculator`，以及未来的战斗、效果、Buff、网格系统
  - 背包数据操作层
  - UI/Menu/Slot 适配层
- 优先使用 JSON 数据驱动内容，不要把种族、职业、技能、物品硬编码进逻辑。
- `Attr::ID` 应被视为稳定契约，不要随意改数值。
- 如果必须修改属性 ID，需要同步迁移：
  - 设计文档
  - C++ 枚举
  - JSON 数据
  - 属性计算代码
  - 效果逻辑地址

## 9. 建议的下一步优先级

1. 统一 C++ 标准：
   - 要么把 CMake 提升到 C++20。
   - 要么把 `std::map::contains` 改为 C++17 可用的 `find`。
2. 修复 JSON 序列化：
   - 尤其是 `std::optional` 字段。
   - 明确可选字段缺失时的默认行为。
3. 对齐配置文件：
   - 统一 `settings` / `Settings`。
   - 统一 `attr` / `Attr`。
   - 统一所有配置键大小写。
   - 明确 `Growth` 和 `Battle` 是否进入 `CalcCoefficients`。
4. 对齐数据 schema：
   - `skill.json`
   - `buff.json`
   - 未来的 item JSON。
5. 修复背包核心逻辑：
   - 先修 `remove_item()`。
   - 再补 `use_item()` 和扩容逻辑。
6. 给核心系统加单元测试：
   - `StatCalculator`
   - `AttributeManager`
   - `DataRepository`
   - `Inventory`
7. 再开始扩展战斗系统、Buff 系统、EffectEngine 和地图系统。

## 10. 未来 EffectEngine 的自然设计

从当前设计文档和代码看，未来最自然的效果系统应当是注册表驱动：

```txt
SkillData / BuffData / ItemData
        |
BehaviorComponent
        |
PassiveAction
        |
ConditionData
        |
EffectData { func_name, logic_address }
        |
EffectRegistry
        |
具体 C++ 效果函数
```

推荐方向：

- `EffectContext` 显式保存施法者、目标、技能、命中结果、临时伤害值等上下文。
- `EffectRegistry` 根据 `func_name` 找到对应函数。
- `EffectData::logic_address` 可以继续指向 `Attr::ID` 或其他逻辑参数。
- 每个效果函数只做一件事，例如：
  - 计算物理基础伤害。
  - 判断物理暴击。
  - 应用防御减免。
  - 扣血。
  - 治疗。
  - 添加 Buff。
  - 移除 Buff。

## 11. 本地工作树注意事项

- `include/definitions/core_attributes.hpp` 在写入英文记忆前就已经处于修改状态，不要随手覆盖。
- 构建验证产生的临时构建目录已经清理。
- 不要提交构建产物。
- 当前新增的中文阅读版文件是：
  - `docs/CODEX_Memory_CN.md`
