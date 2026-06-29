# API Contract

最后更新：2026-06-29

> 当前执行入口见 `docs/README.md`。本文只维护跨端 JSON/API 契约；开发顺序见 `docs/原子化发展路线.md`。

本文定义 **番茄战魂** 第一阶段的 JSON API Contract。该契约用于统一 `C++ Core`、`FastAPI Backend`、`Godot Client` 与 `React Debug Console` 之间的数据结构和协作边界。

---

## 1. 目标与适用范围

`api_contract` 是跨端协作契约，不是某一端的内部接口说明。

它用于规定：

1. 客户端向 FastAPI 发送什么 JSON。
2. FastAPI 向 C++ Core 转发什么 JSON。
3. C++ Core 返回什么 JSON。
4. Godot / React 如何消费 `state`、`events`、`error`。
5. `examples/requests/*.json` 与 `examples/responses/*.json` 应遵守什么结构。

第一阶段只覆盖 MVP 动作：

```txt
action.type = attack
```

后续扩展 `cast_skill`、`use_item`、`equip_item`、`move`、`interact` 等动作时，必须先更新本文档与 examples，再修改代码。

---

## 2. 总体调用链路

```txt
React Debug Console / Godot Client
    ↓ HTTP JSON
FastAPI Backend
    ↓ JSON stdin/stdout，后续可替换为 pybind11
C++ Core
```

第一阶段统一 HTTP 入口：

```txt
POST /api/game/action
```

C++ Core 第一阶段统一入口：

```cpp
std::string handle_json(const std::string& input_json);
```

调用模型：

```txt
旧 state + action
    ↓
C++ Core 规则结算
    ↓
新 state + events
```

---

## 3. 通用设计原则

1. C++ Core 不持有 `while(true)` 游戏主循环。
2. C++ Core 不依赖 React、Godot、FastAPI、HTTP、数据库或 UI。
3. FastAPI 不实现伤害、Buff、命中、死亡等游戏规则。
4. Godot 和 React 不重新计算游戏规则，只消费返回结果。
5. 请求表达“当前状态 + 本次动作”。
6. 响应表达“新状态 + 发生事件”。
7. `state` 表示最终结果。
8. `events` 表示过程事件。
9. 错误结构必须统一。
10. 所有 API 请求和响应必须带 `contract_version`；存档落盘结构必须带 `save_format_version`。

---

## 4. 版本约定

第一阶段 API 契约版本固定为：

```json
"contract_version": 1
```

第一阶段存档格式版本固定为：

```json
"save_format_version": 1
```

当请求中的 `contract_version` 不被支持时，应返回：

```txt
UNSUPPORTED_VERSION
```

---

## 5. 请求顶层结构

```json
{
  "contract_version": 1,
  "request_id": "req_attack_001",
  "action": {
    "type": "attack",
    "actor_id": "player_1",
    "target_id": "slime_1",
    "params": {}
  },
  "state": {},
  "meta": {}
}
```

| 字段         | 类型        | 必填 | 说明                             |
| ------------ | ----------- | ---- | -------------------------------- |
| `contract_version` | `int`       | 是   | API 契约版本，第一阶段固定为 `1` |
| `request_id` | `string`    | 是   | 请求追踪 ID，响应必须原样返回    |
| `action`     | `Action`    | 是   | 本次动作描述                     |
| `state`      | `GameState` | 是   | 当前游戏状态                     |
| `meta`       | `object`    | 否   | 客户端来源、debug 标记等附加信息 |

---

## 6. 响应顶层结构

成功响应：

```json
{
  "ok": true,
  "contract_version": 1,
  "request_id": "req_attack_001",
  "state": {},
  "events": [],
  "error": null
}
```

错误响应：

```json
{
  "ok": false,
  "contract_version": 1,
  "request_id": "req_attack_001",
  "state": null,
  "events": [],
  "error": {
    "code": "ENTITY_NOT_FOUND",
    "message": "Target entity not found: slime_1",
    "details": {
      "target_id": "slime_1"
    }
  }
}
```

| 字段         | 类型             | 必填 | 说明                              |
| ------------ | ---------------- | ---- | --------------------------------- |
| `ok`         | `bool`           | 是   | 本次请求是否成功                  |
| `contract_version` | `int`            | 是   | API 契约版本                      |
| `request_id` | `string`         | 是   | 回传请求 ID                       |
| `state`      | `GameState/null` | 是   | 成功时为新状态，失败时为 `null`   |
| `events`     | `GameEvent[]`    | 是   | 本次动作产生的过程事件            |
| `error`      | `GameError/null` | 是   | 成功时为 `null`，失败时为错误对象 |

---

## 7. GameState 结构

第一阶段采用“战斗最小可用状态”，暂不暴露完整属性系统。

```json
{
  "turn": {
    "round": 1,
    "active_entity_id": "player_1"
  },
  "entities": []
}
```

| 字段       | 类型            | 必填 | 说明                     |
| ---------- | --------------- | ---- | ------------------------ |
| `turn`     | `TurnState`     | 是   | 当前回合状态             |
| `entities` | `EntityState[]` | 是   | 当前场景内参与结算的实体 |

### 7.1 TurnState

```json
{
  "round": 1,
  "active_entity_id": "player_1"
}
```

| 字段               | 类型     | 必填 | 说明            |
| ------------------ | -------- | ---- | --------------- |
| `round`            | `int`    | 是   | 当前回合数      |
| `active_entity_id` | `string` | 是   | 当前行动实体 ID |

---

## 8. EntityState 结构

```json
{
  "id": "player_1",
  "name": "Abel",
  "type": "player",
  "attrs": {
    "hp": 100,
    "max_hp": 100,
    "physical_attack": 20,
    "physical_defense": 5
  },
  "status": {
    "alive": true
  }
}
```

| 字段     | 类型           | 必填 | 说明                                      |
| -------- | -------------- | ---- | ----------------------------------------- |
| `id`     | `string`       | 是   | 实体唯一 ID                               |
| `name`   | `string`       | 是   | 展示名称                                  |
| `type`   | `string`       | 是   | 实体类型，例如 `player`、`monster`、`npc` |
| `attrs`  | `EntityAttrs`  | 是   | 第一阶段结算所需属性                      |
| `status` | `EntityStatus` | 是   | 第一阶段实体状态                          |

### 8.1 EntityAttrs

```json
{
  "hp": 100,
  "max_hp": 100,
  "physical_attack": 20,
  "physical_defense": 5
}
```

| 字段               | 类型  | 必填 | 说明               |
| ------------------ | ----- | ---- | ------------------ |
| `hp`               | `int` | 是   | 当前生命值         |
| `max_hp`           | `int` | 是   | 最大生命值         |
| `physical_attack`  | `int` | 是   | 第一阶段物理攻击力 |
| `physical_defense` | `int` | 是   | 第一阶段物理防御力 |

### 8.2 EntityStatus

```json
{
  "alive": true
}
```

| 字段    | 类型   | 必填 | 说明         |
| ------- | ------ | ---- | ------------ |
| `alive` | `bool` | 是   | 实体是否存活 |

---

## 9. Action 结构

第一阶段 action 统一结构：

```json
{
  "type": "attack",
  "actor_id": "player_1",
  "target_id": "slime_1",
  "params": {}
}
```

| 字段        | 类型     | 必填       | 说明            |
| ----------- | -------- | ---------- | --------------- |
| `type`      | `string` | 是         | 动作类型        |
| `actor_id`  | `string` | 是         | 动作发起实体 ID |
| `target_id` | `string` | 视动作而定 | 目标实体 ID     |
| `params`    | `object` | 否         | 动作附加参数    |

第一阶段只支持：

```txt
attack
```

后续建议扩展顺序：

```txt
attack
  ↓
end_turn
  ↓
cast_skill
  ↓
apply_buff
  ↓
use_item
  ↓
equip_item
  ↓
move
  ↓
interact
```

---

## 10. Event 结构

事件用于 Godot 动画表现和 React 调试展示。

通用事件约定：

```json
{
  "seq": 1,
  "type": "damage"
}
```

| 字段   | 类型     | 必填 | 说明                          |
| ------ | -------- | ---- | ----------------------------- |
| `seq`  | `int`    | 是   | 事件顺序，从 `1` 开始连续递增 |
| `type` | `string` | 是   | 事件类型                      |

Godot 应按 `seq` 顺序播放事件。未知事件不应导致客户端崩溃，应忽略或输出调试日志。

第一阶段事件类型：

| 事件类型         | 用途                       |
| ---------------- | -------------------------- |
| `attack_started` | 攻击开始，用于播放攻击动画 |
| `damage`         | 造成伤害，用于飘字和日志   |
| `hp_changed`     | HP 改变，用于更新血条      |
| `death`          | 实体死亡，用于播放死亡动画 |
| `turn_started`   | 回合开始，后续扩展         |
| `turn_ended`     | 回合结束，后续扩展         |

---

## 11. Error 结构

```json
{
  "code": "ENTITY_NOT_FOUND",
  "message": "Target entity not found: slime_1",
  "details": {
    "target_id": "slime_1"
  }
}
```

| 字段      | 类型     | 必填 | 说明             |
| --------- | -------- | ---- | ---------------- |
| `code`    | `string` | 是   | 机器可读错误码   |
| `message` | `string` | 是   | 人类可读错误说明 |
| `details` | `object` | 否   | 错误上下文       |

第一阶段错误码：

| 错误码                  | 主要来源      | 含义                 |
| ----------------------- | ------------- | -------------------- |
| `INVALID_JSON`          | FastAPI / C++ | JSON 无法解析        |
| `INVALID_SCHEMA`        | FastAPI       | 请求结构不符合协议   |
| `UNSUPPORTED_VERSION`   | FastAPI / C++ | 协议版本不支持       |
| `INVALID_ACTION`        | C++           | 未知或非法 action    |
| `ENTITY_NOT_FOUND`      | C++           | 实体不存在           |
| `INVALID_TARGET`        | C++           | 目标非法             |
| `INSUFFICIENT_RESOURCE` | C++           | 资源不足，给技能预留 |
| `CPP_CORE_ERROR`        | FastAPI       | C++ Core 执行失败    |
| `INTERNAL_ERROR`        | FastAPI / C++ | 未分类内部错误       |

---

## 12. MVP Action: attack

### 12.1 输入要求

`state.entities` 中至少存在：

1. `action.actor_id` 对应实体。
2. `action.target_id` 对应实体。

两个实体至少需要以下字段：

```json
{
  "id": "player_1",
  "name": "Abel",
  "type": "player",
  "attrs": {
    "hp": 100,
    "max_hp": 100,
    "physical_attack": 20,
    "physical_defense": 5
  },
  "status": {
    "alive": true
  }
}
```

### 12.2 第一版规则

```txt
damage = max(1, actor.physical_attack - target.physical_defense)
target.hp = max(0, target.hp - damage)
如果 target.hp == 0，则 target.status.alive = false
```

### 12.3 成功事件

成功时至少输出：

```txt
attack_started
damage
hp_changed
```

如果目标死亡，追加：

```txt
death
```

---

## 13. 示例 JSON

第一阶段固定维护以下示例：

```txt
examples/requests/attack.json
examples/responses/attack_success.json
examples/responses/attack_error.json
```

这些 examples 同时服务于：

1. C++ CLI 输入输出测试。
2. FastAPI Mock 返回。
3. React Debug Console 默认请求。
4. Godot HTTP 联调样例。

---

## 14. 扩展规范

每新增一个 action，都必须同步更新：

```txt
docs/api_contract.md
examples/requests/{action}.json
examples/responses/{action}_success.json
examples/responses/{action}_error.json
backend schema / mock
C++ action handler
Godot event consumer
React Debug Console 示例
```

扩展时优先保持顶层结构稳定，不要随意修改：

```txt
request:  contract_version + request_id + action + state + meta
response: ok + contract_version + request_id + state + events + error
```

---

## 15. 兼容性与变更规则

1. 新字段应尽量后向兼容。
2. 删除字段或改变字段含义时必须提升 `contract_version`。
3. 字段变更必须同步更新文档、examples、schema、mock 和测试。
4. `events` 可以新增类型，但客户端遇到未知事件必须可忽略。
5. 第一阶段返回完整 `state`，后续状态膨胀后再考虑 `state_patch`。

---

## 16. Phase 7：存档接口 Contract

Phase 7 增加本地存档能力。第一版存档使用后端本地 JSON 文件，不引入数据库、账号系统、云同步或自动存档调度器。

存档接口只负责保存和读取完整 `GameState`，不参与伤害、Buff、回合推进等游戏规则计算。

### 16.1 存档接口列表

```txt
GET    /api/saves
POST   /api/saves
GET    /api/saves/{save_id}
PUT    /api/saves/{save_id}
DELETE /api/saves/{save_id}
```

### 16.2 创建存档请求

```txt
POST /api/saves
```

客户端只传可控业务字段。`save_id`、`created_at`、`updated_at` 必须由后端生成。

```json
{
  "contract_version": 1,
  "request_id": "req_create_save_001",
  "name": "测试存档 1",
  "state": {
    "turn": {
      "round": 1,
      "active_entity_id": "player_1"
    },
    "entities": [
      {
        "id": "player_1",
        "name": "Abel",
        "type": "player",
        "attrs": {
          "hp": 100,
          "max_hp": 100,
          "physical_attack": 20,
          "physical_defense": 5
        },
        "status": {
          "alive": true
        }
      }
    ]
  },
  "meta": {
    "source": "debug_console",
    "client": "react",
    "note": "第一次测试存档"
  }
}
```

| 字段         | 类型        | 必填 | 说明                                     |
| ------------ | ----------- | ---- | ---------------------------------------- |
| `contract_version` | `int`       | 是   | API 契约版本，第一版固定为 `1`           |
| `request_id` | `string`    | 是   | 请求追踪 ID，响应原样返回                |
| `name`       | `string`    | 是   | 玩家可见的存档名                         |
| `state`      | `GameState` | 是   | 当前完整游戏状态，复用动作接口状态结构   |
| `meta`       | `object`    | 否   | 来源、客户端、备注、debug 信息等附加字段 |

### 16.3 创建存档响应

```json
{
  "ok": true,
  "contract_version": 1,
  "request_id": "req_create_save_001",
  "save": {
    "save_id": "save_20260626_140000_a1b2c3d4",
    "name": "测试存档 1",
    "created_at": "2026-06-26T14:00:00+08:00",
    "updated_at": "2026-06-26T14:00:00+08:00",
    "contract_version": 1,
    "state": {
      "turn": {
        "round": 1,
        "active_entity_id": "player_1"
      },
      "entities": []
    },
    "meta": {
      "source": "debug_console",
      "client": "react",
      "note": "第一次测试存档"
    }
  },
  "error": null
}
```

### 16.4 存档列表响应

```txt
GET /api/saves
```

列表接口只返回摘要，不返回完整 `state`。

```json
{
  "ok": true,
  "contract_version": 1,
  "request_id": null,
  "saves": [
    {
      "save_id": "save_20260626_140000_a1b2c3d4",
      "name": "测试存档 1",
      "created_at": "2026-06-26T14:00:00+08:00",
      "updated_at": "2026-06-26T14:00:00+08:00",
      "save_format_version": 1
    }
  ],
  "error": null
}
```

### 16.5 读取单个存档响应

```txt
GET /api/saves/{save_id}
```

```json
{
  "ok": true,
  "contract_version": 1,
  "request_id": null,
  "save": {
    "save_id": "save_20260626_140000_a1b2c3d4",
    "name": "测试存档 1",
    "created_at": "2026-06-26T14:00:00+08:00",
    "updated_at": "2026-06-26T14:00:00+08:00",
    "contract_version": 1,
    "state": {
      "turn": {
        "round": 1,
        "active_entity_id": "player_1"
      },
      "entities": []
    },
    "meta": {}
  },
  "error": null
}
```

### 16.6 更新存档请求

```txt
PUT /api/saves/{save_id}
```

更新语义采用整体覆盖：使用请求中的 `name`、`state`、`meta` 覆盖旧存档；保留原 `save_id`、`created_at`；刷新 `updated_at`。

```json
{
  "contract_version": 1,
  "request_id": "req_update_save_001",
  "name": "测试存档 1 - 战斗后",
  "state": {
    "turn": {
      "round": 2,
      "active_entity_id": "slime_1"
    },
    "entities": []
  },
  "meta": {
    "source": "godot",
    "client": "godot",
    "note": "攻击后自动保存"
  }
}
```

更新成功响应与创建成功响应结构一致，返回更新后的完整 `save`。

### 16.7 删除存档响应

```txt
DELETE /api/saves/{save_id}
```

```json
{
  "ok": true,
  "contract_version": 1,
  "request_id": null,
  "deleted": true,
  "save_id": "save_20260626_140000_a1b2c3d4",
  "error": null
}
```

### 16.8 后端落盘结构

第一版存档文件路径：

```txt
backend/storage/saves/{save_id}.json
```

落盘 JSON：

```json
{
  "save_id": "save_20260626_140000_a1b2c3d4",
  "name": "测试存档 1",
  "created_at": "2026-06-26T14:00:00+08:00",
  "updated_at": "2026-06-26T14:00:00+08:00",
  "contract_version": 1,
  "state": {},
  "meta": {}
}
```

`save_id` 只允许字母、数字、下划线和连字符：

```regex
^[a-zA-Z0-9_-]+$
```

后端必须拒绝包含路径分隔符、相对路径或扩展路径语义的 `save_id`。

### 16.9 存档错误结构

```json
{
  "ok": false,
  "contract_version": 1,
  "request_id": "req_create_save_001",
  "save": null,
  "error": {
    "code": "INVALID_SAVE_SCHEMA",
    "message": "Invalid save payload",
    "details": {
      "field": "state.entities"
    }
  }
}
```

存档接口错误码：

| 错误码                | 含义                                     |
| --------------------- | ---------------------------------------- |
| `INVALID_SAVE_SCHEMA` | 存档请求结构非法                         |
| `SAVE_NOT_FOUND`      | 存档不存在                               |
| `INVALID_SAVE_ID`     | `save_id` 包含非法字符或存在路径穿越风险 |
| `SAVE_IO_ERROR`       | 存档文件读写失败                         |
| `UNSUPPORTED_VERSION` | API 契约版本不支持                           |
| `INTERNAL_ERROR`      | 未分类内部错误                           |

### 16.10 Phase 7 实施步骤

1. 在 `docs/api_contract.md` 维护存档接口 Contract。
2. 在 `backend/app/schemas/save.py` 定义 `SaveCreateRequest`、`SaveUpdateRequest`、`SaveRecord`、`SaveSummary`。
3. 在 `backend/app/services/save_service.py` 实现本地 JSON 文件 CRUD、`save_id` 生成、时间戳生成和路径安全校验。
4. 在 `backend/app/api/saves.py` 暴露 `/api/saves` 五个 HTTP 接口。
5. 在 `backend/app/main.py` 注册 saves router。
6. 在 `backend/tests/test_save_api.py` 覆盖创建、列表、读取、更新、删除、不存在存档和非法 `save_id`。
7. 在 `examples/requests/save_create.json` 增加创建存档请求样例。
8. 在 `examples/responses/save_create_success.json` 增加创建成功响应样例。
9. 在 `examples/responses/save_error.json` 增加存档错误响应样例。
10. 在 React 侧新增或扩展存档列表页面，调用 `GET /api/saves` 和 `GET /api/saves/{save_id}`。
11. 在 Godot 侧新增保存和加载逻辑：保存当前 `state`，加载存档后替换当前 `state`。
