# 当前开发规划

最后更新：2026-06-20

本文档用于承接当前阶段的项目规划，重点说明两人团队在 **C++ Core + FastAPI + Godot + React** 架构下的职责边界、接口选择、执行顺序和近期交付物。

本文参考：

- `docs/game_design_spec.md`
- `docs/development_path.md`
- `docs/exposed_interface_designs.md`
- `docs/inventory_design.md`

---

## 1. 项目当前定位

**爱吃番茄的勇者 Abel** 当前应定位为一个数据驱动 RPG 原型项目，而不是传统单体 C++ 命令行游戏。

项目核心目标是构建一套可扩展、可调试、可多端接入的游戏逻辑与表现架构：

```txt
Godot Game Client
    ↓ HTTP JSON
FastAPI Backend
    ↓ JSON stdin/stdout，后续可替换为 pybind11
C++ Core

React Portal / Debug Console
    ↓ HTTP JSON
FastAPI Backend
```

当前阶段的重点不是一次性完成完整 RPG，而是优先跑通最小可行链路：

```txt
客户端发送 Action JSON
    ↓
FastAPI 校验和转发
    ↓
C++ Core 执行规则计算
    ↓
返回新 state + events
    ↓
Godot / React 根据结果展示
```

---

## 2. 总体架构

### 2.1 推荐架构

当前阶段采用四层架构：

| 层级       | 模块     | 主要职责                                                |
| ---------- | -------- | ------------------------------------------------------- |
| 游戏客户端 | Godot    | 地图、角色、动画、战斗表现、HUD、玩家输入               |
| Web 工具层 | React    | Debug Console、Portal、存档管理、配置查看               |
| 服务编排层 | FastAPI  | HTTP API、JSON 校验、调用 C++、错误包装、存档与配置接口 |
| 规则核心层 | C++ Core | 属性、战斗、技能、Buff、背包、回合推进等核心规则        |

### 2.2 核心约束

1. C++ Core 不持有 `while(true)` 游戏循环。
2. C++ Core 不依赖 React、Godot、FastAPI、HTTP、数据库或 UI。
3. FastAPI 不实现游戏规则，只负责校验、转发、存档、错误包装和接口编排。
4. Godot 不重新计算伤害、Buff、命中、死亡等规则。
5. React 不承担主游戏画面，只做 Web Portal 和开发调试工具。
6. React 和 Godot 不直接通信，都通过 FastAPI 访问游戏逻辑。
7. `state` 表示最终状态，`events` 表示过程事件。
8. JSON Contract 是 C++、FastAPI、Godot、React 的共同协作契约。

---

## 3. 两人团队分工

当前团队分工：

```txt
成员 A：FastAPI + React
成员 B：C++ Core + Godot Client
```

### 3.1 成员 A：FastAPI + React

成员 A 是接口、后端和 Web 工具链负责人。

负责内容：

- `docs/api_contract.md`
- 示例请求和响应 JSON
- FastAPI 项目结构
- Pydantic 请求/响应模型
- `/api/game/action` 接口
- C++ Core 调用桥接 `cpp_core_client.py`
- Mock Response 机制
- React Debug Console
- React Portal 后续页面
- 存档接口和配置接口

不负责内容：

- Godot 场景和动画
- Godot TileMap、Sprite、节点脚本
- C++ 伤害公式
- C++ Buff、技能、背包规则

### 3.2 成员 B：C++ Core + Godot Client

成员 B 是游戏规则和游戏客户端负责人。

负责内容：

- C++ Core JSON 输入输出
- C++ CLI stdin/stdout
- `handle_json()` 统一入口
- `attack` 等 action 的核心逻辑
- `GameState`、`EntityState`、`GameEvent` 的解析和生成
- Godot 最小战斗场景
- Godot HTTP 请求 FastAPI
- Godot 根据 `events` 播放动画、飘字、更新血条
- 后续地图、技能、战斗表现和客户端交互

不负责内容：

- FastAPI 路由设计
- Pydantic schema
- React Debug Console
- React Portal
- 存档 API 的后端实现

### 3.3 共同负责

两人共同维护：

- JSON Contract
- `examples/requests/*.json`
- `examples/responses/*.json`
- 接口变更记录
- 每个 action 的输入输出约定

任何接口字段变化，都必须同步更新文档和示例。

---

## 4. 接口方案选择

参考 `docs/exposed_interface_designs.md`，当前阶段采用：

```txt
方案一：统一 Action 接口，全量 State 返回
```

### 4.1 对外 HTTP 接口

第一阶段只暴露一个核心游戏动作接口：

```txt
POST /api/game/action
```

所有动作都通过统一 action 格式进入：

```json
{
  "type": "attack",
  "actor_id": "player_1",
  "target_id": "slime_1",
  "params": {}
}
```

### 4.2 C++ Core 对外入口

C++ 第一阶段提供统一字符串接口：

```cpp
std::string handle_json(const std::string& input_json);
```

CLI 程序只负责：

```txt
读取 stdin
    ↓
调用 handle_json
    ↓
输出 stdout
```

### 4.3 为什么第一阶段返回完整 state

第一阶段返回完整 `state`，不采用 `state_patch`。

原因：

1. 当前首要目标是跑通链路，不是优化响应体大小。
2. 完整 `state` 更容易调试。
3. React Debug Console 可以直接展示完整状态。
4. Godot 初期不需要实现复杂 patch 应用逻辑。
5. 后续状态膨胀后，可以在保持顶层结构的基础上演进到 `state_patch`。

---

## 5. 第一阶段 JSON Contract

### 5.1 请求结构

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
    "client": "debug",
    "debug": true
  }
}
```

字段说明：

| 字段         | 作用                             |
| ------------ | -------------------------------- |
| `version`    | 协议版本，第一阶段固定为 `1`     |
| `request_id` | 请求 ID，用于日志和联调排错      |
| `action`     | 本次动作描述                     |
| `state`      | 当前游戏状态                     |
| `meta`       | 客户端来源、debug 标记等附加信息 |

### 5.2 响应结构

```json
{
  "ok": true,
  "version": 1,
  "request_id": "req_attack_001",
  "state": {},
  "events": [],
  "error": null
}
```

字段说明：

| 字段         | 作用                      |
| ------------ | ------------------------- |
| `ok`         | 本次请求是否成功          |
| `version`    | 协议版本                  |
| `request_id` | 回传请求 ID               |
| `state`      | 新游戏状态                |
| `events`     | 本次动作产生的过程事件    |
| `error`      | 错误信息，成功时为 `null` |

### 5.3 错误结构

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

第一阶段建议错误码：

| 错误码                  | 含义               |
| ----------------------- | ------------------ |
| `INVALID_JSON`          | JSON 无法解析      |
| `INVALID_SCHEMA`        | 请求结构不符合协议 |
| `UNSUPPORTED_VERSION`   | 协议版本不支持     |
| `INVALID_ACTION`        | 未知或非法 action  |
| `ENTITY_NOT_FOUND`      | 实体不存在         |
| `INVALID_TARGET`        | 目标非法           |
| `INSUFFICIENT_RESOURCE` | 资源不足           |
| `CPP_CORE_ERROR`        | C++ Core 执行失败  |
| `INTERNAL_ERROR`        | 未分类内部错误     |

### 5.4 事件结构

事件用于 Godot 动画表现和 React 调试展示。

```json
{
  "seq": 1,
  "type": "damage",
  "source_id": "player_1",
  "target_id": "slime_1",
  "value": 18,
  "damage_type": "physical",
  "message": "Abel 对史莱姆造成 18 点物理伤害"
}
```

第一阶段建议事件类型：

| 事件类型         | 用途                       |
| ---------------- | -------------------------- |
| `attack_started` | 攻击开始，用于播放攻击动画 |
| `damage`         | 造成伤害，用于飘字和日志   |
| `hp_changed`     | HP 改变，用于更新血条      |
| `death`          | 实体死亡，用于播放死亡动画 |
| `turn_started`   | 回合开始                   |
| `turn_ended`     | 回合结束                   |

Godot 应按 `seq` 顺序播放事件。未知事件不应导致客户端崩溃，应忽略或输出调试日志。

---

## 6. MVP Action：attack

第一阶段只实现一个动作：

```txt
action.type = attack
```

### 6.1 输入要求

`state.entities` 中至少存在：

- `actor_id` 对应实体
- `target_id` 对应实体

实体至少需要：

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

### 6.2 第一版规则

```txt
damage = max(1, actor.physical_attack - target.physical_defense)
target.hp = max(0, target.hp - damage)
如果 target.hp == 0，则 target.status.alive = false
```

### 6.3 输出事件

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

## 7. 完整执行流程

### Phase 0：接口规约

负责人：成员 A 主导，成员 B 确认。

交付物：

```txt
docs/api_contract.md
examples/requests/attack.json
examples/responses/attack_success.json
examples/responses/attack_error.json
```

成功标准：

```txt
两人都认可 attack 请求和响应结构。
C++ 知道要吃什么、吐什么。
Godot 知道 events 怎么消费。
FastAPI 和 React 知道如何校验和展示。
```

### Phase 1：FastAPI Mock Backend

负责人：成员 A。

交付物：

```txt
backend/app/main.py
backend/app/api/game.py
backend/app/api/health.py
backend/app/schemas/game.py
backend/app/services/mock_service.py
backend/app/mock/attack_success.json
backend/requirements.txt
```

接口：

```txt
GET  /api/health
POST /api/game/action
GET  /api/debug/example-action
```

成功标准：

```txt
curl、React、Godot 都可以调用 /api/game/action。
即使 C++ 未完成，也能返回 mock response。
```

### Phase 2：React Debug Console

负责人：成员 A。

交付物：

```txt
frontend/src/pages/DebugAction.tsx
frontend/src/services/apiClient.ts
frontend/src/types/game.ts
```

页面功能：

```txt
加载 attack 示例请求。
编辑 Request JSON。
发送到 FastAPI。
展示 Response JSON。
展示 events 列表。
展示 error 信息。
```

成功标准：

```txt
React Debug Console 可以完整调试 /api/game/action。
```

### Phase 3：Godot 接 Mock API

负责人：成员 B。

交付物：

```txt
Godot 最小战斗场景
玩家节点 player_1
怪物节点 slime_1
Attack 按钮
事件日志 UI
血条 UI
HTTP 请求脚本
events 解析和播放逻辑
```

成功标准：

```txt
Godot 点击 Attack 后请求 FastAPI mock。
Godot 根据 events 更新血条、日志和简单动画。
```

### Phase 4：C++ CLI Core

负责人：成员 B。

交付物：

```txt
abel_core_cli
handle_json(input_json)
attack action handler
stdin/stdout JSON 支持
```

运行方式：

```bash
abel_core_cli < examples/requests/attack.json
```

成功标准：

```txt
C++ CLI 输出合法 JSON。
输出结构符合 docs/api_contract.md。
```

### Phase 5：FastAPI 切真实 C++

负责人：成员 A。

交付物：

```txt
backend/app/services/cpp_core_client.py
USE_MOCK_CORE 开关
C++ CLI 路径配置
统一 C++ 错误包装
```

成功标准：

```txt
React 和 Godot 不改请求代码。
FastAPI 从 mock 切换到 C++ CLI 后仍能正常返回。
```

### Phase 6：端到端联调

负责人：两人共同。

完整链路：

```txt
Godot 点击 Attack
    ↓
FastAPI /api/game/action
    ↓
C++ CLI handle_json
    ↓
返回 state + events
    ↓
FastAPI 返回响应
    ↓
Godot 更新表现

React Debug Console 可调同一接口。
```

联调检查点：

```txt
request_id 是否原样返回。
ok 是否正确。
state.entities 中 HP 是否更新。
events.seq 是否连续。
damage value 是否正确。
Godot 是否按 events 顺序播放。
React 是否能展示完整响应。
C++ 错误是否被 FastAPI 包装。
```

### Phase 7：存档与配置接口

负责人：成员 A 主导，成员 B 接入 Godot。

建议接口：

```txt
GET    /api/saves
POST   /api/saves
GET    /api/saves/{save_id}
PUT    /api/saves/{save_id}
DELETE /api/saves/{save_id}

GET /api/config/version
GET /api/config/race
GET /api/config/job
GET /api/config/skill
GET /api/config/effect
GET /api/config/buff
```

第一版存档可以使用本地 JSON 文件：

```txt
backend/storage/saves/*.json
```

后续再迁移到 SQLite 或 PostgreSQL。

### Phase 8：扩展 Action 和游戏系统

扩展顺序建议：

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

每新增一个 action，都必须同步更新：

```txt
docs/api_contract.md
examples/requests/{action}.json
examples/responses/{action}_success.json
FastAPI schema，如需要
C++ action handler
Godot event handler
React Debug Console 示例
```

---

## 8. 推荐仓库结构

长期建议结构：

```txt
Abel-Tomato-Soul/
  cpp-core/
    CMakeLists.txt
    include/
    src/
    tests/

  backend/
    app/
      main.py
      api/
        game.py
        health.py
        saves.py
        config.py
      schemas/
        game.py
        save.py
      services/
        cpp_core_client.py
        mock_service.py
        save_service.py
      mock/
        attack_success.json
      storage/
        saves/
    requirements.txt

  frontend/
    package.json
    src/
      pages/
        Dashboard.tsx
        DebugAction.tsx
        SaveList.tsx
        ConfigViewer.tsx
      services/
        apiClient.ts
      types/
        game.ts

  godot-client/
    project.godot
    scenes/
    scripts/
    assets/

  data/
    config.json
    race.json
    job.json
    skill.json
    effect.json
    buff.json

  docs/
    game_design_spec.md
    exposed_interface_designs.md
    current_development_plan.md
    api_contract.md
    godot_integration.md

  examples/
    requests/
      attack.json
    responses/
      attack_success.json
      attack_error.json
```

当前阶段可以不立刻重构已有 C++ 目录，但新模块建议按上述结构新增。

---

## 9. MVP 成功标准

第一阶段 MVP 完成的判断标准：

```txt
1. docs/api_contract.md 明确 attack 请求和响应。
2. examples 中存在 attack 请求、成功响应、错误响应。
3. FastAPI 能提供 /api/game/action。
4. FastAPI 能在 mock 和 C++ CLI 之间切换。
5. React Debug Console 能发送请求并展示响应。
6. Godot 能调用同一接口。
7. C++ CLI 能处理 attack.json。
8. Godot 能根据 events 更新血条、日志和动画。
9. C++ 错误能被 FastAPI 包装为统一错误响应。
10. React、Godot、FastAPI、C++ 都围绕同一份 JSON Contract 工作。
```

---

## 10. 开发纪律

### 10.1 接口优先

任何新增 action 或字段变更，先改：

```txt
docs/api_contract.md
examples/requests/*.json
examples/responses/*.json
```

再改代码。

### 10.2 规则归 C++

以下逻辑只能在 C++ Core 中实现：

- 伤害计算
- Buff 结算
- 技能效果
- 背包规则
- 死亡判定
- 回合推进

### 10.3 表现归 Godot

以下逻辑归 Godot：

- 动画播放
- 飘字
- 血条更新
- 地图渲染
- 角色节点表现
- 摄像机和音效

### 10.4 编排归 FastAPI

以下逻辑归 FastAPI：

- HTTP API
- Pydantic 校验
- C++ 调用
- 超时处理
- 错误包装
- 存档读写
- 配置分发

### 10.5 工具归 React

以下逻辑归 React：

- Debug Console
- 存档管理页面
- 配置查看页面
- 项目 Portal
- 接口响应可视化

### 10.6 小步验证与单元测试优先

后续开发默认采用“小步验证单元测试”模式：每次只推进一个可验证的小切片，先明确输入输出和测试方式，再写实现代码。

核心原则：

1. 每个功能切片必须能被独立验证。
2. 能写自动化测试的地方优先写自动化测试。
3. 涉及 JSON Contract 的改动，先更新文档和 examples，再写 schema、mock、业务代码和测试。
4. 不把多个层的大改动混在同一个切片里，例如不要同时改 C++ 规则、FastAPI 路由、React UI 和 Godot 场景。
5. 每完成一个切片，必须运行该切片对应的最小测试集。
6. 如果最小测试失败，先修复当前切片，不继续叠加新功能。

推荐开发循环：

```txt
定义小切片
  ↓
确认契约和 examples
  ↓
编写或更新测试
  ↓
实现最小代码
  ↓
运行最小测试
  ↓
必要时运行模块级测试
  ↓
更新文档或 README
  ↓
进入下一个小切片
```

不同模块的最小验证方式：

| 模块                  | 小切片示例              | 最小验证方式                                                        |
| --------------------- | ----------------------- | ------------------------------------------------------------------- |
| `docs/` + `examples/` | 新增 action contract    | JSON 格式校验，人工审查字段是否与文档一致                           |
| `backend/`            | 新增接口或 schema       | `python -m pytest`，必要时补 curl 验证                              |
| `frontend/`           | 新增 Debug Console 功能 | 组件测试或手动调用后端，确认 request/response 展示正确              |
| `godot_client/`       | 新增事件消费逻辑        | 使用 mock response 验证动画、血条、日志是否按 `events.seq` 执行     |
| C++ Core              | 新增 action handler     | 使用 `abel_core_cli < examples/requests/{action}.json` 验证输出结构 |

后端当前测试命令：

```bash
cd backend
.venv\Scripts\activate
python -m pytest
```

新增后端功能时，至少补充以下测试之一：

```txt
1. 正常请求返回 ok=true。
2. 非法输入返回统一 error.code。
3. response 符合 docs/api_contract.md。
4. mock response 与 examples/responses/*.json 保持一致。
```

新增 action 时的固定切片顺序：

```txt
1. 更新 docs/api_contract.md。
2. 新增 examples/requests/{action}.json。
3. 新增 examples/responses/{action}_success.json。
4. 新增 examples/responses/{action}_error.json。
5. 补 backend schema / mock 测试。
6. 实现 backend mock 或 C++ handler。
7. 运行最小测试。
8. 再接 React / Godot 展示。
```

### 10.7 Agent 小阶段交付规则

“小步验证单元测试”不是只给出测试命令，而是默认由 Agent 在每个小阶段中主动完成实现、主动运行验证，并在阶段结束时向用户汇报结果。

执行规则：

1. 每个小阶段开始前，Agent 应明确本阶段目标、交付物和最小验证方式。
2. 每个小阶段由 Agent 负责完成实现和验证，不应只让用户自行验证。
3. 若当前环境允许运行测试、构建或 curl，Agent 必须主动执行最小验证命令。
4. 若验证失败，Agent 必须优先修复当前小阶段，不进入下一阶段。
5. 若因环境、权限、依赖或外部服务限制无法验证，Agent 必须说明原因，并给出用户可手动执行的替代命令。
6. 每个小阶段通过验证后，Agent 再建议进入下一个小阶段。

每个小阶段结束时，Agent 的汇报必须包含：

```txt
本阶段目标：做什么。
修改文件：改了哪些路径。
实现内容：核心改动是什么。
验证方式：运行了什么命令或检查了什么行为。
验证结果：通过/失败，包含关键输出，例如 5 passed、HTTP 200、JSON parse ok。
文档回填：是否更新 docs/README/examples；若未更新，说明 NOT BACKFILLED。
下一步建议：建议继续做哪个最小切片。
```

推荐阶段汇报格式：

```txt
本阶段完成：
- 目标：...
- 修改文件：...
- 实现内容：...
- 验证方式：...
- 验证结果：...
- 文档回填：...
- 下一步建议：...
```

示例：

```txt
本阶段完成：
- 目标：为 /api/game/action 增加 mock 测试。
- 修改文件：backend/tests/test_game_action.py、backend/requirements.txt。
- 实现内容：覆盖 attack 成功响应、非法 version、非法 action。
- 验证方式：运行 backend\.venv\Scripts\python.exe -m pytest backend\tests。
- 验证结果：5 passed。
- 文档回填：已更新 backend/README.md。
- 下一步建议：开始 React Debug Console。
```

---

## 11. 近期执行清单

优先级从高到低：

1. 创建 `docs/api_contract.md`。
2. 创建 `examples/requests/attack.json`。
3. 创建 `examples/responses/attack_success.json`。
4. 创建 `examples/responses/attack_error.json`。
5. 搭建 FastAPI mock backend。
6. 实现 `/api/game/action` mock。
7. 搭建 React Debug Console。
8. Godot 接入 mock API。
9. C++ 实现 `abel_core_cli`。
10. FastAPI 从 mock 切换到真实 C++ CLI。

---

## 12. 总结

当前阶段的核心不是先完成完整 RPG，而是先跑通稳定协作链路：

```txt
JSON Contract
    ↓
FastAPI Mock
    ↓
React Debug Console
    ↓
Godot Mock 接入
    ↓
C++ CLI Core
    ↓
端到端真实联调
```

一句话总结：

> 成员 A 把“通道”和“工具”做稳，成员 B 把“规则”和“表现”做出来，两人共同维护 JSON Contract。
