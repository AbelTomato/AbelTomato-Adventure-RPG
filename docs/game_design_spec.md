# Game Desigh Spec

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
|       `physical_defense`       |  `int`   |    物理防御力    |    -    |
|       `magical_defense`        |  `int`   |    魔法防御力    |    -    |
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

## 1.2.Attribute Map

约定了在生物实例内，定义的一级属性、二级属性、实时状态数组下标与属性之间的映射关系。

此表定义了 `Attr::ID` 枚举值及其对应的数组索引，用于 `AttributeManager` 的统一结算。

| 对应枚举名 (`Attr::ID::...`) |   类型   | 描述 (Category)              | 数组索引 |
| :--------------------------- | :------: | :--------------------------- | :------: |
| **BaseStrength**             |  `int`   | 基础一级属性                 |   $1$    |
| **BasePhysique**             |  `int`   | 基础一级属性                 |   $2$    |
| **BaseDexterity**            |  `int`   | 基础一级属性                 |   $3$    |
| **BaseEsthesia**             |  `int`   | 基础一级属性                 |   $4$    |
| **BaseBewitchment**          |  `int`   | 基础一级属性                 |   $5$    |
| **BaseWillpower**            |  `int`   | 基础一级属性                 |   $6$    |
| **BaseLifeGrowth**           |  `int`   | 基础一级属性                 |   $7$    |
| **BaseMagicGrowth**          |  `int`   | 基础一级属性                 |   $8$    |
| **BaseSpeed**                |  `int`   | 基础一级属性                 |   $9$    |
| **BaseLuck**                 |  `int`   | 基础一级属性                 |   $10$   |
| **DerivedPhysAtk**           |  `int`   | 二级属性基准 (公式产出)      |  $101$   |
| **DerivedMagAtk**            |  `int`   | 二级属性基准 (公式产出)      |  $102$   |
| **DerivedBlockRate**         | `double` | 二级属性基准 (公式产出)      |  $103$   |
| **DerivedEvasion**           |  `int`   | 二级属性基准 (公式产出)      |  $104$   |
| **DerivedPhysHit**           |  `int`   | 二级属性基准 (公式产出)      |  $105$   |
| **DerivedMagHit**            |  `int`   | 二级属性基准 (公式产出)      |  $106$   |
| **DerivedPhysCritRate**      | `double` | 二级属性基准 (公式产出)      |  $107$   |
| **DerivedMagCritRate**       | `double` | 二级属性基准 (公式产出)      |  $108$   |
| **DerivedPhysDef**           |  `int`   | 二级属性基准 (公式产出)      |  $109$   |
| **DerivedMagDef**            |  `int`   | 二级属性基准 (公式产出)      |  $110$   |
| **DerivedPhysDamageRed**     | `double` | 二级属性基准 (公式产出)      |  $111$   |
| **DerivedMagDamageRed**      | `double` | 二级属性基准 (公式产出)      |  $112$   |
| **DerivedTrueDamageRed**     | `double` | 二级属性基准 (公式产出)      |  $113$   |
| **DerivedIgnorePhysDef**     | `double` | 二级属性基准 (公式产出)      |  $114$   |
| **DerivedIgnoreMagDef**      | `double` | 二级属性基准 (公式产出)      |  $115$   |
| **DerivedPhysCritDmg**       | `double` | 二级属性基准 (公式产出)      |  $117$   |
| **DerivedMagCritDmg**        | `double` | 二级属性基准 (公式产出)      |  $118$   |
| **DerivedPhysDamageInc**     | `double` | 二级属性基准 (公式产出)      |  $119$   |
| **DerivedMagDamageInc**      | `double` | 二级属性基准 (公式产出)      |  $120$   |
| **ModStrength**              |  `Mod`   | 属性加成 (支持固定值/百分比) |  $301$   |
| **ModPhysique**              |  `Mod`   | 属性加成 (支持固定值/百分比) |  $302$   |
| **ModDexterity**             |  `Mod`   | 属性加成 (支持固定值/百分比) |  $303$   |
| **ModEsthesia**              |  `Mod`   | 属性加成 (支持固定值/百分比) |  $304$   |
| **ModBewitchment**           |  `Mod`   | 属性加成 (支持固定值/百分比) |  $305$   |
| **ModWillpower**             |  `Mod`   | 属性加成 (支持固定值/百分比) |  $306$   |
| **ModLifeGrowth**            |  `Mod`   | 属性加成 (支持固定值/百分比) |  $307$   |
| **ModMagicGrowth**           |  `Mod`   | 属性加成 (支持固定值/百分比) |  $308$   |
| **ModSpeed**                 |  `Mod`   | 属性加成 (支持固定值/百分比) |  $309$   |
| **ModLuck**                  |  `Mod`   | 属性加成 (支持固定值/百分比) |  $310$   |
| **ModMaxHP**                 |  `Mod`   | 属性加成 (支持固定值/百分比) |  $311$   |
| **ModMaxMP**                 |  `Mod`   | 属性加成 (支持固定值/百分比) |  $312$   |
| **ModPhysAtk**               |  `Mod`   | 属性加成 (支持固定值/百分比) |  $313$   |
| **ModMagAtk**                |  `Mod`   | 属性加成 (支持固定值/百分比) |  $314$   |
| **ModEvasion**               |  `Mod`   | 属性加成 (支持固定值/百分比) |  $315$   |
| **ModPhysHit**               |  `Mod`   | 属性加成 (支持固定值/百分比) |  $316$   |
| **ModMagHit**                |  `Mod`   | 属性加成 (支持固定值/百分比) |  $317$   |
| **ModPhysDef**               |  `Mod`   | 属性加成 (支持固定值/百分比) |  $318$   |
| **ModMagDef**                |  `Mod`   | 属性加成 (支持固定值/百分比) |  $319$   |
| **ModPhysCritDmg**           | `double` | 率加成 (直接加法结算)        |  $320$   |
| **ModMagCritDmg**            | `double` | 率加成 (直接加法结算)        |  $321$   |
| **ModPhysCritRate**          | `double` | 率加成 (直接加法结算)        |  $322$   |
| **ModMagCritRate**           | `double` | 率加成 (直接加法结算)        |  $323$   |
| **ModBlockRate**             | `double` | 率加成 (直接加法结算)        |  $324$   |
| **ModPhysDamageRed**         | `double` | 率加成 (直接加法结算)        |  $325$   |
| **ModMagDamageRed**          | `double` | 率加成 (直接加法结算)        |  $326$   |
| **ModTrueDamageRed**         | `double` | 率加成 (直接加法结算)        |  $327$   |
| **ModIgnorePhysDef**         | `double` | 率加成 (直接加法结算)        |  $328$   |
| **ModIgnoreMagDef**          | `double` | 率加成 (直接加法结算)        |  $329$   |
| **ModPhysDamageInc**         | `double` | 率加成 (直接加法结算)        |  $330$   |
| **ModMagDamageInc**          | `double` | 率加成 (直接加法结算)        |  $331$   |
| **FinalStrength**            |  `int`   | 最终属性 (用于战斗/逻辑)     |  $501$   |
| **FinalPhysique**            |  `int`   | 最终属性 (用于战斗/逻辑)     |  $502$   |
| **FinalDexterity**           |  `int`   | 最终属性 (用于战斗/逻辑)     |  $503$   |
| **FinalEsthesia**            |  `int`   | 最终属性 (用于战斗/逻辑)     |  $504$   |
| **FinalBewitchment**         |  `int`   | 最终属性 (用于战斗/逻辑)     |  $505$   |
| **FinalWillpower**           |  `int`   | 最终属性 (用于战斗/逻辑)     |  $506$   |
| **FinalLifeGrowth**          |  `int`   | 最终属性 (用于战斗/逻辑)     |  $507$   |
| **FinalMagicGrowth**         |  `int`   | 最终属性 (用于战斗/逻辑)     |  $508$   |
| **FinalSpeed**               |  `int`   | 最终属性 (用于战斗/逻辑)     |  $509$   |
| **FinalLuck**                |  `int`   | 最终属性 (用于战斗/逻辑)     |  $510$   |
| **FinalPhysAtk**             |  `int`   | 最终属性 (用于战斗/逻辑)     |  $511$   |
| **FinalMagAtk**              |  `int`   | 最终属性 (用于战斗/逻辑)     |  $512$   |
| **FinalBlockRate**           | `double` | 最终属性 (用于战斗/逻辑)     |  $513$   |
| **FinalEvasion**             |  `int`   | 最终属性 (用于战斗/逻辑)     |  $514$   |
| **FinalPhysHit**             |  `int`   | 最终属性 (用于战斗/逻辑)     |  $515$   |
| **FinalMagHit**              |  `int`   | 最终属性 (用于战斗/逻辑)     |  $516$   |
| **FinalPhysCritRate**        | `double` | 最终属性 (用于战斗/逻辑)     |  $517$   |
| **FinalMagCritRate**         | `double` | 最终属性 (用于战斗/逻辑)     |  $518$   |
| **FinalPhysDef**             |  `int`   | 最终属性 (用于战斗/逻辑)     |  $519$   |
| **FinalMagDef**              |  `int`   | 最终属性 (用于战斗/逻辑)     |  $520$   |
| **FinalPhysDamageRed**       | `double` | 最终属性 (用于战斗/逻辑)     |  $521$   |
| **FinalMagDamageRed**        | `double` | 最终属性 (用于战斗/逻辑)     |  $522$   |
| **FinalTrueDamageRed**       | `double` | 最终属性 (用于战斗/逻辑)     |  $523$   |
| **FinalIgnorePhysDef**       | `double` | 最终属性 (用于战斗/逻辑)     |  $524$   |
| **FinalIgnoreMagDef**        | `double` | 最终属性 (用于战斗/逻辑)     |  $525$   |
| **FinalPhysCritDmg**         | `double` | 最终属性 (用于战斗/逻辑)     |  $526$   |
| **FinalMagCritDmg**          | `double` | 最终属性 (用于战斗/逻辑)     |  $527$   |
| **FinalPhysDamageInc**       | `double` | 最终属性 (用于战斗/逻辑)     |  $528$   |
| **FinalMagDamageInc**        | `double` | 最终属性 (用于战斗/逻辑)     |  $529$   |
| **CurMaxHP**                 |  `int`   | 实时状态 (当前上限)          |  $701$   |
| **CurMaxMP**                 |  `int`   | 实时状态 (当前上限)          |  $702$   |
| **CurMaxSP**                 |  `int`   | 实时状态 (当前上限)          |  $703$   |
| **CurLevel**                 |  `int`   | 实时状态                     |  $704$   |
| **CurExp**                   |  `int`   | 实时状态                     |  $705$   |

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

## 3.Core Game Design

### 3.1.Skill Release & Effect Application Process

#### 3.1.1.Process Definition

定义技能释放和效果应用为如下核心流程：

$$
生物 \\
\Downarrow \\
释放技能 \\
\Downarrow \\
触发效果(\text{effect}) \\
\Downarrow \\
根据生物属性、技能效果倍率、爆伤等加成计算强度(\text{power}) \\
\Downarrow \\
将对应\text{effect}[\text{power}]施加至作用对象 \\
\Downarrow \\
作用对象接收信息 \\
\Downarrow \\
触发抵抗效果 \\
\Downarrow \\
根据作用对象抵抗属性等因素计算强度(\text{power}) \\
\Downarrow \\
两\text{effect}拮抗抵消 \\
\Downarrow \\
触发减益效果 \\
\Downarrow \\
根据两者强度计算减益效果强度 \\
\Downarrow \\
施加至作用对象属性 \\
\Downarrow \\
作用对象属性变化
$$

#### 3.1.2.Process Example

以最简单的普通攻击为例：

释放「普通攻击」，触发「物理伤害」效果，根据角色基础物理攻击力，物理伤害加成，物理暴击伤害加成，计算得到强度 $\text{power}_{1} = 120$，传参至受击目标。

受击目标接收「物理伤害」效果，触发「物理减伤」效果，根据受击目标物理防御力，物理伤害减免，真实伤害减免，计算得到强度 $\text{power}_{2} = 30$，调用自身$\text{hp\_decrease(\text{power}\_{1}, \, \text{power}\_{2})}$函数。

$\text{hp\_decrease(\text{power}\_{1}, \, \text{power}\_{2})}$内，触发「生命降低」效果，根据$\text{power}_{1}, \, \text{power}_{2}$，计算得到$\text{power}_{3} = 90$(这里以减法模拟)，受击目标$\text{hp}$降低$90$点。

### 3.2.Base Formula & Parameter Setting

#### 3.2.1.Primary To Derived

##### 3.2.1.1.BaseMaxHP

$$
\text{BaseMaxHP = Strength} \times \text{HPPerStrength} + \text{Physique} \times \text{HPPerPhysique} + \text{Willpower} \times \text{HPPerWillpower}
$$

|          参数           |  值   |
| :---------------------: | :---: |
| $\text{HPPerStrength}$  |  $8$  |
| $\text{HPPerPhysique}$  | $15$  |
| $\text{HPPerWillpower}$ |  $3$  |

##### 3.2.1.2.BaseMaxMP

$$
\text{BaseMaxMP} = \text{Esthesia} \times \text{MPPerEsthesia} + \text{Bewitchment} \times \text{MPPerBewitchment} + \text{Willpower} \times \text{MPPerWillpower}
$$

|           参数            |  值   |
| :-----------------------: | :---: |
|  $\text{MPPerEsthesia}$   |  $3$  |
| $\text{MPPerBewitchment}$ | $12$  |
|  $\text{MPPerWillpower}$  |  $7$  |

##### 3.2.1.3.BaseMaxSP

$$
\text{BaseMaxSP} = \text{Strength} \times \text{SPPerStrength} + \text{Physique} \times \text{SPPerPhysique} + \text{Willpower} \times \text{SPPerWillpower}
$$

|          参数           |  值   |
| :---------------------: | :---: |
| $\text{SPPerStrength}$  | $0.3$ |
| $\text{SPPerPhysique}$  | $0.5$ |
| $\text{SPPerWillpower}$ | $0.2$ |

##### 3.2.1.4.Evasion

$$
\text{Evasion} = \text{Dexterity} \times \text{EvasionPerDexterity} + \text{Esthesia} \times \text{EvasionPerEsthesia} + \text{Speed} \times \text{EvasionPerSpeed} + \text{Luck} \times \text{EvasionPerLuck}
$$

|             参数             |  值   |
| :--------------------------: | :---: |
| $\text{EvasionPerDexterity}$ |  $2$  |
| $\text{EvasionPerEsthesia}$  |  $1$  |
|   $\text{EvasionPerSpeed}$   |  $3$  |
|   $\text{EvasionPerLuck}$    |  $5$  |

##### 3.2.1.5.PhysicalHit

$$
\text{PhysicalHit} = \text{Dexterity} \times \text{PhysicalHitPerDexterity} + \text{Esthesia} \times \text{PhysicalHitPerEsthesia} + \text{Speed} \times \text{PhysicalHitPerSpeed} + \text{Luck} \times \text{PhysicalHitPerLuck}
$$

|               参数               |  值   |
| :------------------------------: | :---: |
| $\text{PhysicalHitPerDexterity}$ |  $1$  |
| $\text{PhysicalHitPerEsthesia}$  |  $2$  |
|   $\text{PhysicalHitPerSpeed}$   |  $2$  |
|   $\text{PhysicalHitPerLuck}$    |  $3$  |

##### 3.2.1.6.MagicalHit

$$
\text{MagicalHit} = \text{Esthesia} \times \text{MagicalHitPerEsthesia} + \text{Bewitchment} \times \text{MagicalHitPerBewitchment} + \text{Luck} \times \text{MagicalHitPerLuck}
$$

|               参数                |  值   |
| :-------------------------------: | :---: |
|  $\text{MagicalHitPerEsthesia}$   |  $1$  |
| $\text{MagicalHitPerBewitchment}$ |  $2$  |
|    $\text{MagicalHitPerLuck}$     |  $2$  |

##### 3.2.1.7.PhysicalDefense

$$
\text{PhysicalDefense} = \text{Physique} \times \text{PhysicalDefensePerPhysique}
$$

|                参数                 |  值   |
| :---------------------------------: | :---: |
| $\text{PhysicalDefensePerPhysique}$ |  $3$  |

##### 3.2.1.8.MagicalDefense

$$
\text{MagicalDefense} = \text{Physique} \times \text{MagicalDefensePerPhysique} + \text{Bewitchment} \times \text{MagicalDefensePerBewitchment}
$$

|                 参数                  |  值   |
| :-----------------------------------: | :---: |
|  $\text{MagicalDefensePerPhysique}$   |  $1$  |
| $\text{MagicalDefensePerBewitchment}$ |  $2$  |

##### 3.2.1.9.IgnorePhysicalDefenseRate

$$
\text{IgnorePhysicalDefenseRate} = \text{Strength} \times \text{IgnorePhysicalDefenseRatePerStrength} + \text{Esthesia} \times \text{IgnorePhysicalDefenseRatePerEsthesia} + \text{Luck} \times \text{IgnorePhysicalDefenseRatePerLuck}
$$

|                     参数                      |         值         |
| :-------------------------------------------: | :----------------: |
| $\text{IgnorePhysicalDefenseRatePerStrength}$ | $3 \times 10^{-5}$ |
| $\text{IgnorePhysicalDefenseRatePerEsthesia}$ | $7 \times 10^{-5}$ |
|   $\text{IgnorePhysicalDefenseRatePerLuck}$   | $1 \times 10^{-4}$ |

##### 3.2.1.10.IgnoreMagicalDefenseRate

$$
\text{IgnoreMagicalDefenseRate} = \text{Bewitchment} \times \text{IgnoreMagicalDefenseRatePerBewitchment} + \text{Esthesia} \times \text{IgnoreMagicalDefenseRatePerEsthesia} + \text{Luck} \times \text{IgnoreMagicalDefenseRatePerLuck}
$$

|                      参数                       |         值         |
| :---------------------------------------------: | :----------------: |
| $\text{IgnoreMagicalDefenseRatePerBewitchment}$ | $4 \times 10^{-5}$ |
|  $\text{IgnoreMagicalDefenseRatePerEsthesia}$   | $5 \times 10^{-5}$ |
|    $\text{IgnoreMagicalDefenseRatePerLuck}$     | $1 \times 10^{-4}$ |

##### 3.2.1.11.PhysicalCritRate

$$
\text{PhysicalCritRate} = \text{Esthesia} \times \text{PhysicalCritRatePerEsthesia} + \text{Luck} \times \text{PhysicalCritRatePerLuck}
$$

|                 参数                 |         值         |
| :----------------------------------: | :----------------: |
| $\text{PhysicalCritRatePerEsthesia}$ | $2 \times 10^{-4}$ |
|   $\text{PhysicalCritRatePerLuck}$   | $5 \times 10^{-4}$ |

##### 3.2.1.12.MagicalCritRate

$$
\text{MagicalCritRate} = \text{Esthesia} \times \text{MagicalCritRatePerEsthesia} + \text{Luck} \times \text{MagicalCritRatePerLuck}
$$

|                参数                 |         值         |
| :---------------------------------: | :----------------: |
| $\text{MagicalCritRatePerEsthesia}$ | $1 \times 10^{-4}$ |
|   $\text{MagicalCritRatePerLuck}$   | $3 \times 10^{-4}$ |

##### 3.2.1.13.GrowthHPPerLevel

$$
\text{GrowthHPPerLevel} = \text{LifeGrowth} \times \text{HPPerLifeGrowthPerLevel}
$$

|               参数               |  值   |
| :------------------------------: | :---: |
| $\text{HPPerLifeGrowthPerLevel}$ |  $5$  |

##### 3.2.1.14.GrowthMPPerLevel

$$
\text{GrowthMPPerLevel} = \text{MagicGrowth} \times \text{MPPerMagicGrowthPerLevel}
$$

|               参数                |  值   |
| :-------------------------------: | :---: |
| $\text{MPPerMagicGrowthPerLevel}$ |  $4$  |

##### 3.2.1.15.BasePhysicalAttackPower

$$
\text{BasePhysicalAttackPower} = \text{Strength} \times \text{BasePhysicalAttackPowerPerStrength} + \text{Physique} \times \text{BasePhysicalAttackPowerPerPhysique}
$$

|                    参数                     |  值   |
| :-----------------------------------------: | :---: |
| $\text{BasePhysicalAttackPowerPerStrength}$ |  $5$  |
| $\text{BasePhysicalAttackPowerPerPhysique}$ |  $1$  |

##### 3.2.1.16.BaseMagicalAttackPower

$$
\text{BaseMagicalAttackPower} = \text{Bewitchment} \times \text{BaseMagicalAttackPowerPerBewitchment} + \text{Willpower} \times \text{BaseMagicalAttackPowerPerWillpower}
$$

|                     参数                      |  值   |
| :-------------------------------------------: | :---: |
| $\text{BaseMagicalAttackPowerPerBewitchment}$ |  $6$  |
|  $\text{BaseMagicalAttackPowerPerWillpower}$  |  $1$  |
