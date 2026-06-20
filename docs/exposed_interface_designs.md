# 对外暴露 JSON 接口设计候选

最后更新：2026-06-20

本文整理本项目三种可行的对外暴露接口设计。这里的“对外暴露”分两层：

```txt
React / Godot
    ↓ HTTP JSON
FastAPI 对外 HTTP 接口
    ↓ JSON stdin/stdout 或后续 pybind11
C++ Core 对外逻辑接口
```

建议 FastAPI 和 C++ Core 尽量共用同一套 JSON Contract。FastAPI 负责 HTTP、校验、错误包装、存档和调用 C++；C++ Core 负责规则结算，并且只接收 JSON、返回 JSON。

---

## 共同设计原则

无论采用哪一种方案，都建议遵守以下规则：

1. C++ Core 不持有 `while(true)` 游戏循环。
2. C++ Core 不依赖 React、Godot、FastAPI、HTTP、数据库或 UI。
3. 请求应表达“当前状态 + 本次动作”。
4. 响应应表达“新状态 + 发生事件”。
5. `state` 表示最终结果，`events` 表示过程。
6. 前端和 Godot 只做展示，不重新计算伤害、Buff、命中、死亡等规则。
7. 错误结构必须统一，不能一会儿返回 `msg`，一会儿返回 `error` 字符串。
8. 所有接口应带 `version`，方便后续兼容。

---

## 方案一：统一 Action 接口，全量 State 返回

### 设计概述

这是最推荐的 MVP 方案。

所有游戏动作都通过一个统一接口进入：

```txt
POST /api/game/action
```

C++ Core 对应一个统一入口：

```cpp
std::string handle_json(const std::string& input_json);
```

调用模型：

```txt
旧 state + action
        ↓
C++ Core
        ↓
新 state + events
```

### 适用场景

- 项目早期 MVP。
- 两人并行开发。
- 需要 React Debug Console 和 Godot 共用同一套接口。
- 状态规模暂时不大。
- 更重视清晰、可调试、低返工，而不是流量最小化。

### 优点

- 结构最清晰。
- 前端、Godot、FastAPI、C++ 都只认一套请求和响应。
- 很容易做 mock。
- 很容易保存样例 JSON 作为测试输入输出。
- C++ CLI 可以直接执行：

```bash
abel_core_cli < examples/requests/attack.json
```

### 缺点

- 每次返回完整 `state`，状态变大后响应体会膨胀。
- 后续多人、多地图、大背包时可能需要引入差量更新。

### 请求样例

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

### 成功响应样例

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

### 错误响应样例

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

## 方案二：统一 Action 接口，差量 Patch 返回

### 设计概述

这个方案仍然只有一个统一接口：

```txt
POST /api/game/action
```

但响应不返回完整 `state`，而是返回 `state_patch`，表示本次动作导致哪些字段发生了变化。

调用模型：

```txt
旧 state + action
        ↓
C++ Core
        ↓
state_patch + events
        ↓
客户端把 patch 应用到本地 state
```

### 适用场景

- 游戏状态已经比较大。
- 客户端希望长期持有本地状态。
- 网络传输量需要控制。
- Godot 或 React 已经有可靠的状态管理层。

### 优点

- 响应体更小。
- 适合后续地图、背包、任务、多个实体同时存在的场景。
- 可以清楚表达“哪些字段变了”。

### 缺点

- 客户端必须正确应用 patch。
- 调试复杂度比全量 state 高。
- 一旦客户端本地 state 和服务端逻辑预期不同，问题更难排查。
- MVP 阶段不建议优先采用。

### 请求样例

请求结构可以与方案一完全相同：

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
    "client": "godot",
    "debug": false
  }
}
```

### 成功响应样例

这里使用一种简单的 patch 格式：

```json
{
  "ok": true,
  "version": 1,
  "request_id": "req_attack_001",
  "state_patch": [
    {
      "op": "replace",
      "path": "/entities/slime_1/attrs/hp",
      "from": 50,
      "value": 32
    },
    {
      "op": "replace",
      "path": "/entities/slime_1/status/alive",
      "from": true,
      "value": true
    }
  ],
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
  "state": null,
  "error": null
}
```

### 错误响应样例

```json
{
  "ok": false,
  "version": 1,
  "request_id": "req_attack_001",
  "state_patch": [],
  "events": [],
  "state": null,
  "error": {
    "code": "PATCH_CONFLICT",
    "message": "Client state does not match expected source value",
    "details": {
      "path": "/entities/slime_1/attrs/hp",
      "expected": 50,
      "actual": 40
    }
  }
}
```

### 关键约定

为了让 patch 方案可靠，需要额外约定：

```txt
1. path 如何定位实体。
2. 客户端本地 state 是否必须和请求 state 一致。
3. patch 应用失败时是否重新请求完整 state。
4. 是否采用标准 JSON Patch，还是项目自定义 patch。
```

如果采用这个方案，建议 `entities` 在客户端内部转成 map：

```json
{
  "entities": {
    "player_1": {},
    "slime_1": {}
  }
}
```

否则数组路径会因为顺序变化而不稳定。

---

## 方案三：按领域或动作拆分接口

### 设计概述

这个方案不只暴露一个 `/api/game/action`，而是按领域或动作拆成多个 HTTP 接口。

示例：

```txt
POST /api/combat/attack
POST /api/combat/cast-skill
POST /api/inventory/use-item
POST /api/inventory/equip-item
POST /api/world/move
```

C++ Core 内部仍然可以保持统一入口，也可以分成多个 handler：

```cpp
std::string handle_attack_json(const std::string& input_json);
std::string handle_cast_skill_json(const std::string& input_json);
std::string handle_use_item_json(const std::string& input_json);
```

更推荐的折中方式是：HTTP endpoint 拆分，但 FastAPI 内部统一转换为方案一的 action 格式，然后仍然调用：

```cpp
std::string handle_json(const std::string& input_json);
```

### 适用场景

- 后端 API 想对外表现得更 REST 风格。
- 不同领域的请求结构差异非常大。
- FastAPI 未来要给外部工具、管理后台、配置后台使用。
- 项目已经有稳定领域边界，例如 combat、inventory、world、save。

### 优点

- HTTP 语义更直观。
- 每个接口可以有更精确的 Pydantic schema。
- 文档对外部调用者更友好。
- 适合后期扩展管理接口、存档接口、配置接口。

### 缺点

- 接口数量会变多。
- 每新增一个 action 可能要新增 endpoint。
- React、Godot、FastAPI、C++ 更容易出现重复映射逻辑。
- 早期容易过度设计。

### 请求样例：普通攻击

```http
POST /api/combat/attack
Content-Type: application/json
```

```json
{
  "version": 1,
  "request_id": "req_attack_001",
  "actor_id": "player_1",
  "target_id": "slime_1",
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
    "client": "debug"
  }
}
```

FastAPI 可以把它转换成统一 action 请求：

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
    "entities": []
  },
  "meta": {
    "client": "debug"
  }
}
```

### 成功响应样例：普通攻击

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
      "target_id": "slime_1"
    },
    {
      "seq": 2,
      "type": "damage",
      "source_id": "player_1",
      "target_id": "slime_1",
      "value": 18,
      "damage_type": "physical"
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

### 请求样例：释放技能

```http
POST /api/combat/cast-skill
Content-Type: application/json
```

```json
{
  "version": 1,
  "request_id": "req_skill_001",
  "actor_id": "player_1",
  "target_id": "slime_1",
  "skill_id": 200001,
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
          "mp": 30,
          "max_mp": 30,
          "magic_attack": 18
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
          "magic_defense": 3
        },
        "status": {
          "alive": true
        }
      }
    ]
  },
  "meta": {
    "client": "godot"
  }
}
```

### 成功响应样例：释放技能

```json
{
  "ok": true,
  "version": 1,
  "request_id": "req_skill_001",
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
          "mp": 20,
          "max_mp": 30,
          "magic_attack": 18
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
          "hp": 35,
          "max_hp": 50,
          "magic_defense": 3
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
      "type": "skill_cast_started",
      "source_id": "player_1",
      "target_id": "slime_1",
      "skill_id": 200001
    },
    {
      "seq": 2,
      "type": "resource_changed",
      "entity_id": "player_1",
      "resource": "mp",
      "from": 30,
      "to": 20
    },
    {
      "seq": 3,
      "type": "damage",
      "source_id": "player_1",
      "target_id": "slime_1",
      "value": 15,
      "damage_type": "magic"
    },
    {
      "seq": 4,
      "type": "hp_changed",
      "entity_id": "slime_1",
      "from": 50,
      "to": 35
    }
  ],
  "error": null
}
```

### 错误响应样例

```json
{
  "ok": false,
  "version": 1,
  "request_id": "req_skill_001",
  "state": null,
  "events": [],
  "error": {
    "code": "INSUFFICIENT_RESOURCE",
    "message": "Not enough mp to cast skill: 200001",
    "details": {
      "entity_id": "player_1",
      "resource": "mp",
      "required": 10,
      "current": 5,
      "skill_id": 200001
    }
  }
}
```

---

## 三种方案对比

| 方案 | 接口形态 | 推荐阶段 | 核心优点 | 核心风险 |
| --- | --- | --- | --- | --- |
| 方案一：统一 Action + 全量 State | `POST /api/game/action` | MVP 首选 | 清晰、易 mock、易联调 | 状态大后响应变大 |
| 方案二：统一 Action + Patch | `POST /api/game/action` | 中后期优化 | 响应小、适合大状态 | 客户端状态同步复杂 |
| 方案三：按领域拆接口 | `/api/combat/*`、`/api/inventory/*` | 后期或管理 API | HTTP 语义清楚、schema 精细 | 接口数量多，早期易过度设计 |

---

## 推荐结论

当前项目最适合采用：

```txt
方案一：统一 Action 接口，全量 State 返回
```

理由：

1. 当前首要目标是跑通链路，而不是优化流量。
2. C++ Core、FastAPI、React Debug Console、Godot 都可以围绕同一份 JSON Contract 并行开发。
3. 全量 `state` 更容易排查问题。
4. `events` 可以直接服务 Godot 动画、React 日志和调试工具。
5. 后续如果状态膨胀，可以在保持顶层结构不变的情况下演进到方案二。

建议第一阶段固定产出：

```txt
docs/api_contract.md
examples/requests/attack.json
examples/responses/attack_success.json
examples/responses/attack_error.json
```

第一阶段统一顶层响应结构：

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

第一阶段统一顶层请求结构：

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
