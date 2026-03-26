# Data Schema

## 1.Base Definition

### 1.1.Base Structs

#### 1.1.1.Core Attributes

##### 1.1.1.1.Attributes

|      属性      | 类型  |   作用   | 备注  |
| :------------: | :---: | :------: | :---: |
|   `strength`   | `int` |   力量   |   -   |
|   `physique`   | `int` |   体质   |   -   |
|  `dexterity`   | `int` |   灵巧   |   -   |
|   `esthesia`   | `int` |   感知   |   -   |
| `bewitchment`  | `int` |   魔力   |   -   |
|  `willpower`   | `int` |   意志   |   -   |
| `life_growth`  | `int` | 生命成长 |   -   |
| `magic_growth` | `int` | 法力成长 |   -   |
|    `speed`     | `int` |   速度   |   -   |
|     `luck`     | `int` |   幸运   |   -   |

##### 1.1.1.2.Mod

|  属性  |   类型   |    作用    | 备注  |
| :----: | :------: | :--------: | :---: |
| `pct`  | `double` | 百分比加成 |   -   |
| `flat` |  `int`   | 固定值加成 |   -   |

#### 1.1.2.Property Containers

##### 1.1.2.1.DerivedStats

|              属性              |   类型   |       作用       |  备注   |
| :----------------------------: | :------: | :--------------: | :-----: |
|            `max_hp`            |  `int`   |    最大生命值    |    -    |
|            `max_mp`            |  `int`   |    最大法力值    |    -    |
|            `max_sp`            |  `int`   |    最大精力值    |    -    |
|  `base_physical_attack_power`  |  `int`   |  基础物理攻击力  |    -    |
|  `base_magical_attack_power`   |  `int`   |  基础魔法攻击力  |    -    |
|          `block_rate`          | `double` |      格挡率      | 上限50% |
|        `evasion_value`         |  `int`   |      闪避值      |    -    |
|      `physical_hit_value`      |  `int`   |    物理命中值    |    -    |
|      `magical_hit_value`       |  `int`   |    法术命中值    |    -    |
|      `physical_crit_rate`      | `double` |    物理暴击率    | 上限95% |
|      `magical_crit_rate`       | `double` |    魔法暴击率    | 上限95% |
|           `defense`            |  `int`   |      防御力      |    -    |
|  `physical_damage_reduction`   | `double` |     物理减伤     | 上限50% |
|   `magical_damage_reduction`   | `double` |     魔法减伤     | 上限50% |
|    `true_damage_reduction`     | `double` |     真实减伤     | 上限50% |
| `ignore_physical_defense_rate` | `double` | 无视物理防御几率 | 上限60% |
| `ignore_magical_defense_rate`  | `double` | 无视魔法防御几率 | 上限60% |
|     `physical_crit_damage`     | `double` |     物理爆伤     |    -    |
|     `magical_crit_damage`      | `double` |     魔法爆伤     |    -    |
|   `physical_damage_increase`   | `double` |     物理增伤     |    -    |
|   `magical_damage_increase`    | `double` |     魔法增伤     |    -    |

##### 1.1.2.2.BonusStats

|              属性              |       类型        |         作用         | 备注  |
| :----------------------------: | :---------------: | :------------------: | :---: |
|           `strength`           | [`Mod`](#1112mod) |       力量加成       |   -   |
|           `physique`           | [`Mod`](#1112mod) |       体质加成       |   -   |
|          `dexterity`           | [`Mod`](#1112mod) |       灵巧加成       |   -   |
|           `esthesia`           | [`Mod`](#1112mod) |       感知加成       |   -   |
|         `bewitchment`          | [`Mod`](#1112mod) |       魔力加成       |   -   |
|          `willpower`           | [`Mod`](#1112mod) |       意志加成       |   -   |
|         `life_growth`          | [`Mod`](#1112mod) |     生命成长加成     |   -   |
|         `magic_growth`         | [`Mod`](#1112mod) |     法力成长加成     |   -   |
|            `speed`             | [`Mod`](#1112mod) |       速度加成       |   -   |
|             `luck`             | [`Mod`](#1112mod) |       幸运加成       |   -   |
|            `max_hp`            | [`Mod`](#1112mod) |    最大生命值加成    |   -   |
|            `max_mp`            | [`Mod`](#1112mod) |    最大法力值加成    |   -   |
|  `base_physical_attack_power`  | [`Mod`](#1112mod) |  基础物理攻击力加成  |   -   |
|  `base_magical_attack_power`   | [`Mod`](#1112mod) |  基础魔法攻击力加成  |   -   |
|        `evasion_value`         | [`Mod`](#1112mod) |      闪避值加成      |   -   |
|      `physical_hit_value`      | [`Mod`](#1112mod) |    物理命中值加成    |   -   |
|      `magical_hit_value`       | [`Mod`](#1112mod) |    法术命中值加成    |   -   |
|       `physical_defense`       | [`Mod`](#1112mod) |    物理防御力加成    |   -   |
|       `Magical_defense`        | [`Mod`](#1112mod) |    魔法防御力加成    |   -   |
|     `physical_crit_damage`     |     `double`      |     物理爆伤加成     |   -   |
|     `magical_crit_damage`      |     `double`      |     魔法爆伤加成     |   -   |
|      `physical_crit_rate`      |     `double`      |    物理暴击率加成    |   -   |
|      `magical_crit_rate`       |     `double`      |    魔法暴击率加成    |   -   |
|          `block_rate`          |     `double`      |      格挡率加成      |   -   |
|  `physical_damage_reduction`   |     `double`      |     物理减伤加成     |   -   |
|   `magical_damage_reduction`   |     `double`      |     魔法减伤加成     |   -   |
|    `true_damage_reduction`     |     `double`      |     真实减伤加成     |   -   |
| `ignore_physical_defense_rate` |     `double`      | 无视物理防御几率加成 |   -   |
| `ignore_magical_defense_rate`  |     `double`      | 无视魔法防御几率加成 |   -   |
|   `physical_damage_increase`   |     `double`      |     物理增伤加成     |   -   |
|   `magical_damage_increase`    |     `double`      |     魔法增伤加成     |   -   |

### 1.2.Base Types

#### 1.2.1.Base Type

##### 1.2.1.1.DamageType

|   类型名   |   含义   | 备注  |   JSON名   |
| :--------: | :------: | :---: | :--------: |
| `Physical` | 物理伤害 |   -   | "physical" |
| `Magical`  | 魔法伤害 |   -   | "magical"  |
|   `True`   | 真实伤害 |   -   |   "true"   |

##### 1.2.1.2.StatusType

| 类型名 |  含义  | 备注  | JSON名 |
| :----: | :----: | :---: | :----: |
|  `HP`  |  生命  |   -   |  "hp"  |
|  `MP`  |  法力  |   -   |  "mp"  |
|  `SP`  |  精力  |   -   |  "sp"  |
| `EXP`  | 经验值 |   -   | "exp"  |

#### 1.2.2.Buff Type

##### 1.2.2.1.TriggerType

|    类型名     |      含义      | 备注  |     JSON名      |
| :-----------: | :------------: | :---: | :-------------: |
| `OnTurnStart` | 回合开始时触发 |   -   | "on_turn_start" |
|  `OnTurnEnd`  | 回合结束时触发 |   -   |  "on_turn_end"  |
|  `OnAction`   | 每次行动时触发 |   -   |   "on_action"   |
| `PassiveAttr` |    持续生效    |   -   | "passive_attr"  |

##### 1.2.3.Combat Type

##### 1.2.3.1.HitResult

|   类型名   | 含义  | 备注  | JSON名 |
| :--------: | :---: | :---: | :----: |
|   `Miss`   | 闪避  |   -   |   -    |
|   `Hit`    | 命中  |   -   |   -    |
| `Critical` | 暴击  |   -   |   -    |
|  `Block`   | 格挡  |   -   |   -    |

#### 1.2.4.Effect Type

##### 1.2.4.1.EffectType

|      类型名       |     含义     |             备注              |       JSON名       |
| :---------------: | :----------: | :---------------------------: | :----------------: |
| `AttributeChange` |  属性的改变  | 如回血，受到伤害，六维降低20% | "attribute_change" |
|    `ApplyBuff`    |   施加buff   |               -               |    "apply_buff"    |
|   `RemoveBuff`    |   驱散buff   |               -               |   "remove_buff"    |
|    `Displace`     |     位移     |               -               |     "displace"     |
|  `SpecialScript`  | 触发特殊效果 |               -               |  "special_script"  |

#### 1.2.5.Skill Type

##### 1.2.5.1.SkillType

|    类型名    |   含义   | 备注  |    JSON名    |
| :----------: | :------: | :---: | :----------: |
| `Initiative` | 主动技能 |   -   | "initiative" |
|  `Passive`   | 被动技能 |   -   |  "passive"   |

##### 1.2.5.2.EffectSettlementType

|        类型名         |     含义     | 备注  |         JSON名          |
| :-------------------: | :----------: | :---: | :---------------------: |
| `PhysicalAttackPower` | 基于物理攻击 |   -   | "physical_attack_power" |
| `MagicalAttackPower`  | 基于魔法攻击 |   -   | "magical_attack_power"  |
|        `MaxHP`        | 基于最大生命 |   -   |        "max_hp"         |
|      `CurrentHP`      | 基于当前生命 |   -   |        "cur_hp"         |
|        `MaxMP`        | 基于最大法力 |   -   |        "max_mp"         |
|      `CurrentMP`      | 基于当前法力 |   -   |        "cur_mp"         |
|   `PhysicalDefense`   | 基于物理防御 |   -   |   "physical_defense"    |
|   `MagicalDefense`    | 基于魔法防御 |   -   |    "magical_defense"    |

##### 1.2.5.3.CostMethod

|    类型名    |    含义    | 备注  |    JSON名     |
| :----------: | :--------: | :---: | :-----------: |
|    `Flat`    |   固定值   |   -   |    "flat"     |
| `PercentMax` | 最大百分比 |   -   | "percent_max" |
| `PercentCur` | 当前百分比 |   -   | "percent_cur" |

##### 1.2.5.4.ShapeType

|   类型名    | 含义  | 备注  |   JSON名    |
| :---------: | :---: | :---: | :---------: |
|   `Point`   | 单点  |   -   |   "point"   |
| `Rectangle` | 矩形  |   -   | "rectangle" |
|  `Circle`   | 圆形  |   -   |  "circle"   |
|   `Cross`   | 十字  |   -   |   "cross"   |
|   `Line`    | 直线  |   -   |   "line"    |
|  `Sector`   | 扇形  |   -   |  "sector"   |

##### 1.2.5.5.TargetFilter

| 类型名  |    含义    | 备注  | JSON名  |
| :-----: | :--------: | :---: | :-----: |
|  `All`  |   无差别   |   -   |  "all"  |
| `Ally`  | 仅友方生效 |   -   | "ally"  |
| `Enemy` | 仅敌方生效 |   -   | "enemy" |

## 2.Data Struct

### 2.1.Race Data

#### 2.1.1.RaceData Definition

|    属性名     |              类型               |            作用            |         备注          |
| :-----------: | :-----------------------------: | :------------------------: | :-------------------: |
|     `id`      |              `int`              |            标识            | 10000 - 11999表示种族 |
|    `name`     |            `string`             |            名称            |           -           |
| `description` |            `string`             |          描述文本          |           -           |
|  `base_attr`  | [`Attributes`](#1111attributes) | 为生物提供初始种族奖励属性 |           -           |
| `race_skills` |             `list`              |   为生物提供初始种族技能   |           -           |

#### 2.1.2.RaceData Example

### 2.2.Job Data

|    属性名     |              类型               |            作用            |          备注          |
| :-----------: | :-----------------------------: | :------------------------: | :--------------------: |
|     `id`      |              `int`              |            标识            | 12000 - 14999 表示职业 |
|    `name`     |            `string`             |            名称            |           -            |
| `description` |            `string`             |          描述文字          |           -            |
|  `base_attr`  | [`Attributes`](#1111attributes) | 为生物提供初始职业奖励属性 |           -            |
| `job_skills`  |             `list`              |   为生物提供初始职业技能   |           -            |

### 2.3.Skill Data

#### 2.3.1.SkillData

|      属性名       |        类型        |            作用            |                         备注                         |
| :---------------: | :----------------: | :------------------------: | :--------------------------------------------------: |
|       `id`        |       `int`        |            标识            |                20000 - 39999 表示技能                |
|      `name`       |      `string`      |            名称            |                          -                           |
|   `description`   |      `string`      |          描述文字          |                          -                           |
| `display_effect`  |      `string`      | 在数据和技能效果方面的描述 |                          -                           |
|      `cost`       |       `list`       |        技能代价列表        |         `list`元素类型为[`Costs`](#232costs)         |
|    `cool_down`    |       `int`        |  描述技能冷却所需基础回合  |                          -                           |
| `hit_correction`  |      `double`      |        技能命中修正        |                          -                           |
| `crit_correction` |      `double`      |        技能暴击修正        |                          -                           |
|      `cast`       | [`Cast`](#233cast) |      描述施法相关属性      |                          -                           |
| `effect_triggers` |       `list`       |      包含技能触发效果      | `list`元素类型为[`EffectTrigger`](#234effecttrigger) |

#### 2.3.2.Costs

|     属性      |              类型               |     作用     |         备注         |
| :-----------: | :-----------------------------: | :----------: | :------------------: |
| `status_type` | [`StatusType`](#1212statustype) | 消耗代价类型 | 如血量，蓝量，精力等 |
| `cost_method` | [`CostMethod`](#1253costmethod) |   消耗方式   | 为固定值或百分比消耗 |
|    `value`    |            `double`             | 消耗代价参数 |          -           |

#### 2.3.3.Cast

|          属性           |                类型                 |       作用       |                               备注                                |
| :---------------------: | :---------------------------------: | :--------------: | :---------------------------------------------------------------: |
|       `min_range`       |                `int`                |   最小施法距离   |                                 -                                 |
|       `max_range`       |                `int`                |   最大施法距离   |                                 -                                 |
|      `shape_type`       |    [`ShapeType`](#1254shapetype)    |     形状类型     |                                 -                                 |
| `is_relative_to_caster` |               `bool`                | 是否与施法者关联 | 如若`shape_type`=`LINE`，则该值决定线段是以自身为端点还是自由放置 |
|  `is_target_selected`   |               `bool`                | 是否必须选中实体 |                                 -                                 |
|      `cast_params`      |               `list`                |   范围参数列表   |                     `list`元素类型为`double`                      |
|     `target_filter`     | [`TargetFilter`](#1255targetfilter) |    目标筛选器    |               定义技能是否仅对敌方有效或是其他情况                |

#### 2.3.4.EffectTrigger

|           属性           |                        类型                         |       作用       | 备注  |
| :----------------------: | :-------------------------------------------------: | :--------------: | :---: |
|       `effect_id`        |                        `int`                        |   触发效果的id   |   -   |
|         `chance`         |                      `double`                       |     触发几率     |   -   |
| `effect_settlement_type` | [`EffectSettlementType`](#1252effectsettlementtype) | 触发强度结算方式 |   -   |
|     `magnification`      |                      `double`                       |     效果倍率     |   -   |

### 2.4.Effect Data

|        属性名        |              类型               |     作用     |                                            备注                                            |
| :------------------: | :-----------------------------: | :----------: | :----------------------------------------------------------------------------------------: |
|         `id`         |              `int`              |     标识     |                                   40000 - 49999表示效果                                    |
|        `name`        |            `string`             |     名称     |                                             -                                              |
|    `description`     |            `string`             |   描述文字   |                                             -                                              |
|    `effect_type`     | [`EffectType`](#1241effecttype) | 描述效果类型 |                                             -                                              |
| `target_sub_type_id` |              `int`              | 表示子类型id | 由 `effect_type` 决定具体含义；如若`effect_type`=`AttributeChange`，则对应id为加血、扣蓝等 |
|  `power_efficiency`  |            `double`             | 强度转化效率 |                                             -                                              |

### 2.5.Buff Data

#### 2.5.1.基本设定

|      属性名       |               类型                |          作用          |         备注          |
| :---------------: | :-------------------------------: | :--------------------: | :-------------------: |
|       `id`        |               `int`               |          标识          | 50000 - 59999表示buff |
|      `name`       |             `string`              |          名称          |           -           |
|   `description`   |             `string`              |        描述文字        |           -           |
|  `trigger_type`   | [`TriggerType`](#1221triggertype) |      描述触发时机      |           -           |
|    `max_stack`    |               `int`               |      最大叠加层数      |           -           |
|   `dispellable`   |              `bool`               |      是否可被驱散      |           -           |
|     `stance`      |             `string`              | 技能立场，正面或是负面 |           -           |
| `tick_effect_ids` |            `list[int]`            |      触发效果列表      |           -           |
