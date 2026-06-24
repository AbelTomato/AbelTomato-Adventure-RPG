# 对外暴露 JSON 接口设计：统一 Action + 全量 State

最后更新：2026-06-20

本文记录本项目当前决定采用的对外暴露接口方案：

```txt
方案一：统一 Action 接口，全量 State 返回
```

这份文档同时约束两层接口：

```txt
React / Godot
    ↓ HTTP JSON
FastAPI 对外 HTTP 接口
    ↓ JSON stdin/stdout，后续可替换为 pybind11
C++ Core 对外逻辑接口
```

核心原则是：FastAPI 和 C++ Core 尽量共用同一套 JSON Contract。FastAPI 负责 HTTP、校验、错误包装、存档和调用 C++；C++ Core 负责规则结算，并且只接收 JSON、返回 JSON。

---

## 1. 设计目标

本接口服务于请求响应式游戏逻辑：

```txt
旧 GameState + ActionRequest
        ↓
C++ Core 计算
        ↓
新 GameState + GameEvents
```

C++ Core 不应持有传统游戏主循环：

```cpp
while (true) {
    game.update();
}
```

而应暴露为一个可被外部调用的纯逻辑入口：

```cpp
std::string handle_json(const std::string& input_json);
```

CLI 阶段可以包装成：

```bash
abel_core_cli < examples/requests/attack.json
```

---

## 2. 共同设计原则

1. C++ Core 不持有 `while(true)` 游戏循环。
2. C++ Core 不依赖 React、Godot、FastAPI、HTTP、数据库或 UI。
3. 请求表达“当前状态 + 本次动作”。
4. 响应表达“新状态 + 发生事件”。
5. `state` 表示最终结果。
6. `events` 表示过程，用于动画、日志、飘字、调试。
7. React 和 Godot 只展示，不重新计算伤害、Buff、命中、死亡等规则。
8. 错误结构必须统一。
9. 所有接口必须带 `version`。
10. 响应必须原样返回 `request_id`，方便链路排查。

---

## 3. HTTP 接口

第一阶段只暴露一个游戏动作接口：

```txt
POST /api/game/action
```

所有动作都通过 `action.type` 区分，例如：

```txt
attack
end_turn
cast_skill
use_item
equip_item
move
interact
```

MVP 阶段只实现：

```txt
attack
```

---

## 4. C++ Core 接口

C++ Core 对外保持一个稳定入口：

```cpp
std::string handle_json(const std::string& input_json);
```

推荐内部拆成两层：

```cpp
nlohmann::json handle_request(const nlohmann::json& input);
std::string handle_json(const std::string& input_json);
```

这样可以同时服务三种调用方式：

```txt
CLI：读取 stdin，调用 handle_json，写 stdout
FastAPI：subprocess 调用 CLI
未来 pybind11：直接绑定 handle_json
```

---

## 5. 请求结构

统一请求顶层结构：

```json
{
  "version": 1,
  "request_id": "req_xxx",
  "action": {},
  "state": {},
  "meta": {}
}
```

字段说明：

| 字段 | 类型 | 必填 | 说明 |
| --- | --- | --- | --- |
| `version` | number | 是 | 接口协议版本，第一版固定为 `1` |
| `request_id` | string | 是 | 请求 ID，响应必须原样返回 |
| `action` | object | 是 | 本次要执行的游戏动作 |
| `state` | object | 是 | 当前游戏状态 |
| `meta` | object | 否 | 调试、客户端类型、随机种子等附加信息 |

### Action 结构

```json
{
  "type": "attack",
  "actor_id": "player_1",
  "target_id": "slime_1",
  "params": {}
}
```

字段说明：

| 字段 | 类型 | 必填 | 说明 |
| --- | --- | --- | --- |
| `type` | string | 是 | 动作类型 |
| `actor_id` | string | 是 | 动作发起者 |
| `target_id` | string | 视动作而定 | 动作目标 |
| `params` | object | 否 | 动作扩展参数 |

### State 结构

MVP 阶段使用以下结构：

```json
{
  "turn": {
    "round": 1,
    "active_entity_id": "player_1"
  },
  "entities": []
}
```

`entities` 使用数组，不写死 `player` 和 `monster` 字段。这样后续可以自然扩展到多个怪物、NPC、召唤物、队友。

实体结构：

```json
{
  "id": "player_1",
  "name": "Abel",
  "type": "player",
  "attrs": {},
  "status": {}
}
```

---

## 6. 成功响应结构

统一成功响应顶层结构：

```json
{
  "ok": true,
  "version": 1,
  "request_id": "req_xxx",
  "state": {},
  "events": [],
  "error": null
}
```

字段说明：

| 字段 | 类型 | 必填 | 说明 |
| --- | --- | --- | --- |
| `ok` | boolean | 是 | 是否成功 |
| `version` | number | 是 | 接口协议版本 |
| `request_id` | string | 是 | 原样返回请求 ID |
| `state` | object | 是 | 结算后的完整新状态 |
| `events` | array | 是 | 本次动作产生的事件列表 |
| `error` | null | 是 | 成功时固定为 `null` |

---

## 7. 错误响应结构

统一错误响应顶层结构：

```json
{
  "ok": false,
  "version": 1,
  "request_id": "req_xxx",
  "state": null,
  "events": [],
  "error": {
    "code": "ERROR_CODE",
    "message": "Human readable error message",
    "details": {}
  }
}
```

字段说明：

| 字段 | 类型 | 必填 | 说明 |
| --- | --- | --- | --- |
| `ok` | boolean | 是 | 错误时固定为 `false` |
| `version` | number | 是 | 接口协议版本 |
| `request_id` | string | 是 | 能识别时原样返回，不能识别时可为空字符串 |
| `state` | null | 是 | 错误时第一阶段固定为 `null` |
| `events` | array | 是 | 错误时第一阶段固定为空数组 |
| `error.code` | string | 是 | 稳定错误码 |
| `error.message` | string | 是 | 可读错误信息 |
| `error.details` | object | 否 | 调试用结构化细节 |

第一批错误码：

```txt
INVALID_JSON
INVALID_SCHEMA
UNSUPPORTED_VERSION
INVALID_ACTION
ENTITY_NOT_FOUND
INVALID_ACTOR
INVALID_TARGET
ENTITY_DEAD
NOT_ACTIVE_TURN
INSUFFICIENT_RESOURCE
CPP_CORE_ERROR
INTERNAL_ERROR
```

---

## 8. Event 结构

`events` 是给 React / Godot 使用的过程信息。客户端应按 `seq` 顺序消费事件。

通用事件结构：

```json
{
  "seq": 1,
  "type": "damage",
  "source_id": "player_1",
  "target_id": "slime_1",
  "value": 18,
  "message": "Abel 对史莱姆造成 18 点物理伤害"
}
```

MVP 阶段推荐事件类型：

```txt
attack_started
damage
hp_changed
death
turn_started
turn_ended
resource_changed
```

关键约定：

```txt
state 表示结算后的最终事实
events 表示本次动作发生过什么
```

例如怪物血量从 50 变为 32：

```txt
state 里写 hp = 32
events 里写 hp_changed from 50 to 32
```

---

## 9. Attack 请求样例

```http
POST /api/game/action
Content-Type: application/json
```

```json
{
  "version": 1,
  "request_id": "req_attack_001",
  "action": {
    "type": "attack",
    "actor_id": "player_1",
    "target_id": "slime_1",
    "params": {}
  },
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
      },
      {
        "id": "slime_1",
        "name": "史莱姆",
        "type": "monster",
        "attrs": {
          "hp": 50,
          "max_hp": 50,
          "physical_attack": 8,
          "physical_defense": 2
        },
        "status": {
          "alive": true
        }
      }
    ]
  },
  "meta": {
    "client": "debug",
    "debug": true
  }
}
```

---

## 10. Attack 成功响应样例

```json
{
  "ok": true,
  "version": 1,
  "request_id": "req_attack_001",
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
      },
      {
        "id": "slime_1",
        "name": "史莱姆",
        "type": "monster",
        "attrs": {
          "hp": 32,
          "max_hp": 50,
          "physical_attack": 8,
          "physical_defense": 2
        },
        "status": {
          "alive": true
        }
      }
    ]
  },
  "events": [
    {
      "seq": 1,
      "type": "attack_started",
      "source_id": "player_1",
      "target_id": "slime_1",
      "tags": ["physical", "melee"]
    },
    {
      "seq": 2,
      "type": "damage",
      "source_id": "player_1",
      "target_id": "slime_1",
      "value": 18,
      "damage_type": "physical",
      "message": "Abel 对史莱姆造成 18 点物理伤害"
    },
    {
      "seq": 3,
      "type": "hp_changed",
      "entity_id": "slime_1",
      "from": 50,
      "to": 32
    }
  ],
  "error": null
}
```

---

## 11. Attack 错误响应样例

```json
{
  "ok": false,
  "version": 1,
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

---

## 12. FastAPI 调用 C++ 的包装规则

FastAPI 收到 `/api/game/action` 请求后：

```txt
1. 读取 HTTP JSON。
2. 用 Pydantic 或 JSON Schema 做基础校验。
3. 将请求 JSON 原样传给 C++ Core。
4. 等待 C++ Core 返回 JSON。
5. 校验 C++ 返回是否是合法响应结构。
6. 返回给 React / Godot。
```

如果 C++ Core 崩溃、超时、返回非法 JSON，FastAPI 应包装为统一错误：

```json
{
  "ok": false,
  "version": 1,
  "request_id": "req_attack_001",
  "state": null,
  "events": [],
  "error": {
    "code": "CPP_CORE_ERROR",
    "message": "C++ core execution failed",
    "details": {
      "stderr": "optional stderr text",
      "timeout_ms": 3000
    }
  }
}
```

---

## 13. 第一阶段实现边界

MVP 阶段只实现：

```txt
action.type = attack
```

推荐攻击规则先保持简单：

```txt
damage = max(1, actor.physical_attack - target.physical_defense)
target.hp = max(0, target.hp - damage)
if target.hp == 0:
    target.status.alive = false
    append death event
```

MVP 阶段暂不实现：

```txt
技能
Buff
装备
背包
地图移动
命中
闪避
暴击
格挡
多目标
敌人反击
存档数据库
```

这些后续都可以继续通过同一个 `action` 结构扩展。

---

## 14. 后续扩展方式

### 技能

```json
{
  "type": "cast_skill",
  "actor_id": "player_1",
  "target_id": "slime_1",
  "params": {
    "skill_id": 200001
  }
}
```

### 使用物品

```json
{
  "type": "use_item",
  "actor_id": "player_1",
  "target_id": "player_1",
  "params": {
    "slot_index": 3
  }
}
```

### 移动

```json
{
  "type": "move",
  "actor_id": "player_1",
  "params": {
    "direction": "north_east"
  }
}
```

---

## 15. 最终结论

当前项目正式采用：

```txt
POST /api/game/action
```

请求顶层结构：

```json
{
  "version": 1,
  "request_id": "req_xxx",
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

响应顶层结构：

```json
{
  "ok": true,
  "version": 1,
  "request_id": "req_xxx",
  "state": {},
  "events": [],
  "error": null
}
```

这一版接口优先保证：

```txt
清晰
稳定
易 mock
易联调
易被 C++ CLI 测试
易被 React Debug Console 和 Godot 同时消费
```
