# 协作规则归档

最后更新：2026-06-27

> 归档说明：本文是 Phase 0-6 期间的两人协作方案草案，保留用于理解职责边界和早期接口优先思路。当前执行入口以 `docs/README.md` 和 `docs/原子化发展路线.md` 为准；具体 JSON/API 契约以 `docs/api_contract.md` 为准。

---

下面按你们新的两人分工，给出一套 **完整、可执行、低返工** 的开发流程。

你们现在的分工是：

```txt
你：FastAPI + React
另一个人：C++ Core + Godot Client
```

这套分工是可行的，但必须明确一点：

> 你们两个人中间的核心交界面不是代码，而是 JSON 接口规约。  
> 你负责把接口、后端、Web 调试工具稳定下来；他负责让 C++ 能处理接口、Godot 能消费接口。

---

# 一、最终目标架构

推荐采用：

```txt
React Portal / Debug Console
        ↓ HTTP JSON
FastAPI Backend
        ↓ subprocess / later pybind11
C++ Core
        ↑
Godot Game Client
        ↓ HTTP JSON
FastAPI Backend
```

更直观地说：

```txt
                 ┌────────────────────────┐
                 │ React Portal / Debug    │
                 │ 调试台 / 存档 / 配置查看 │
                 └───────────┬────────────┘
                             │ HTTP JSON
                             ↓
┌────────────────────┐   ┌────────────────────┐   ┌────────────────────┐
│ Godot Game Client   │ → │ FastAPI Backend     │ → │ C++ Core CLI/Lib    │
│ 场景/动画/表现/输入 │   │ API/校验/存档/桥接   │   │ 规则/战斗/状态推进  │
└────────────────────┘   └────────────────────┘   └────────────────────┘
```

注意：

```txt
React 和 Godot 不直接通信
React 和 Godot 都只通过 FastAPI 通信
FastAPI 统一调用 C++ Core
C++ Core 不知道 React/Godot 的存在
```

---

# 二、两个人的职责边界

## 你：FastAPI + React

你是：

```txt
API Contract Owner
Backend Owner
React Tooling Owner
联调入口负责人
```

你负责：

```txt
1. docs/api_contract.md
2. examples 请求/响应样例
3. FastAPI 项目结构
4. Pydantic 请求/响应校验
5. /api/game/action 接口
6. C++ Core 调用桥接 cpp_core_client.py
7. mock response 机制
8. React Debug Console
9. React Portal，可后置
10. 存档/配置 API，第二阶段再做
```

你不负责：

```txt
Godot 场景
Godot 动画
C++ 伤害公式
C++ Buff 结算
Godot 角色移动
Godot TileMap
```

---

## 另一个人：C++ Core + Godot

他是：

```txt
Game Logic Owner
Game Client Owner
```

他负责：

```txt
1. C++ Core JSON 输入输出
2. C++ CLI stdin/stdout
3. attack/action 逻辑
4. GameState / EntityState / Event 结构处理
5. Godot 最小战斗场景
6. Godot HTTP 请求 FastAPI
7. Godot 根据 events 播放动画/更新血条
8. 后续技能、地图、战斗表现
```

他不负责：

```txt
FastAPI 路由设计
React Debug Console
Pydantic schema
存档 API
React Portal
```

---

# 三、第一原则：先 mock，再接真 C++

不要一开始就三端同时真联调。

正确流程是：

```txt
接口规约
  ↓
FastAPI mock
  ↓
React Debug Console 调 mock
  ↓
Godot 调 mock
  ↓
C++ CLI 独立实现
  ↓
FastAPI 从 mock 切到 C++
  ↓
React/Godot 无需改接口
```

这能最大程度降低互相等待。

---

# 四、阶段 0：接口规约阶段

## 目标

先不写复杂系统，先确定：

```txt
客户端发什么 JSON
后端校验什么 JSON
C++ 吃什么 JSON
C++ 返回什么 JSON
Godot/React 如何消费返回 JSON
```

## 你负责产出

```txt
docs/api_contract.md
examples/requests/attack.json
examples/responses/attack_success.json
examples/responses/attack_error.json
```

可选：

```txt
schemas/game_action_request.schema.json
schemas/game_action_response.schema.json
```

## 第一版请求结构

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

## 第一版响应结构

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
    "entities": []
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
      "tags": ["physical"],
      "message": "Abel 对 史莱姆造成 18 点物理伤害"
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

## 阶段成功标准

```txt
你们两个人都认可 attack.json 和 attack_success.json
C++ 人知道要吃什么、吐什么
Godot 人知道 events 怎么播
你知道 FastAPI 和 React Debug Console 怎么写
```

---

# 五、阶段 1：你搭 FastAPI Mock Backend

## 目标

先不等 C++，你先提供一个可调用的后端接口。

```txt
POST /api/game/action
```

它先直接返回 mock JSON。

## 推荐目录

```txt
backend/
  app/
    main.py
    api/
      game.py
      health.py
    schemas/
      game.py
    services/
      cpp_core_client.py
      mock_service.py
    mock/
      attack_success.json
  requirements.txt
```

## 你要实现的接口

```txt
GET  /api/health
POST /api/game/action
GET  /api/debug/example-action
```

### `/api/game/action`

第一阶段逻辑：

```txt
收到请求
  ↓
Pydantic 基础校验
  ↓
如果 USE_MOCK_CORE=true
  ↓
返回 backend/app/mock/attack_success.json
```

后续切换为：

```txt
收到请求
  ↓
Pydantic 校验
  ↓
cpp_core_client 调 C++ CLI
  ↓
返回 C++ 输出
```

## 阶段成功标准

```txt
curl 可以调通 /api/game/action
React 可以调通 /api/game/action
Godot 可以调通 /api/game/action
此时 C++ 没完成也不影响联调
```

---

# 六、阶段 2：你做 React Debug Console

## 目标

React 第一阶段不做游戏主画面，而做联调工具。

页面：

```txt
/debug/action
```

## 功能

```txt
1. 加载 attack.json 示例
2. 左侧编辑 Request JSON
3. 点击 Send
4. 请求 FastAPI /api/game/action
5. 右侧展示 Response JSON
6. 下方展示 events 列表
7. 错误时展示 error.code / error.message
```

## 推荐目录

```txt
frontend/
  src/
    pages/
      DebugAction.tsx
      Dashboard.tsx
    services/
      apiClient.ts
    types/
      game.ts
    mock/
      attackRequest.ts
```

## 为什么这个很重要？

因为它是你们的联调控制台。

```txt
C++ 输出不对 -> Debug Console 看得见
FastAPI 包装错误 -> Debug Console 看得见
Godot 接不通 -> 先用 Debug Console 验证接口是否正常
```

## 阶段成功标准

```txt
React 页面能发送 attack 请求
能显示 mock response
能把 events 渲染成列表
能显示错误响应
```

---

# 七、阶段 3：另一个人做 Godot Mock 接入

## 目标

Godot 先接你的 FastAPI mock，不等 C++。

## 他要做的东西

```txt
1. Godot 项目初始化
2. 一个战斗测试场景
3. 玩家节点 player_1
4. 怪物节点 slime_1
5. 一个 Attack 按钮
6. 一个事件日志 Label/Panel
7. 一个血条 UI
8. HTTP POST 到 /api/game/action
9. 解析 response.state 和 response.events
10. 根据 events 更新表现
```

## Godot 消费事件规则

```txt
attack_started -> 播放攻击动画或打印“攻击开始”
damage -> 飘字 / 日志显示伤害
hp_changed -> 更新血条
death -> 播放死亡动画，后续再加
```

## 你要提供给他的东西

```txt
docs/godot_integration.md
API 地址
attack 请求示例
attack 响应示例
events 类型表
错误码表
后端启动方式
```

## 阶段成功标准

```txt
Godot 点击 Attack
FastAPI mock 返回 events
Godot 更新血条/日志/简单动画
```

---

# 八、阶段 4：另一个人做 C++ CLI Core

## 目标

C++ 独立完成 JSON 输入输出，不依赖 FastAPI/Godot。

## C++ Core 对外形态

先做 CLI：

```txt
abel_core_cli
```

支持：

```bash
abel_core_cli < examples/requests/attack.json
```

输出：

```json
{
  "ok": true,
  "state": {},
  "events": [],
  "error": null
}
```

## C++ 逻辑

第一版只做：

```txt
action.type = attack
```

规则：

```txt
找到 actor
找到 target
damage = max(1, actor.physical_attack - target.physical_defense)
target.hp = max(0, target.hp - damage)
如果 target.hp == 0，status.alive = false，追加 death event
返回新 state 和 events
```

## 推荐 C++ 入口

```cpp
std::string handle_json(const std::string& input_json);
```

CLI 的 `main()` 只负责：

```txt
读 stdin
调用 handle_json
写 stdout
```

## 阶段成功标准

```txt
C++ CLI 可以吃 attack.json
输出合法 JSON
输出结构符合 api_contract.md
```

---

# 九、阶段 5：你把 FastAPI 从 mock 切到 C++

## 目标

FastAPI 的对外接口不变，只把内部实现换掉。

原来：

```txt
/api/game/action -> mock attack_success.json
```

切换为：

```txt
/api/game/action -> cpp_core_client.py -> abel_core_cli -> 返回 C++ 输出
```

## 你负责的 `cpp_core_client.py`

职责：

```txt
1. 接收 Python dict
2. json.dumps 成字符串
3. subprocess 调用 C++ CLI
4. 设置 timeout
5. 捕获 stderr
6. 校验 stdout 是合法 JSON
7. 返回 dict
8. C++ 崩溃时返回统一错误结构
```

## 统一错误结构

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
    "details": {}
  }
}
```

## 阶段成功标准

```txt
React Debug Console 不改代码，仍能调通
Godot 不改代码，仍能调通
只是后端从 mock 变成真实 C++ 输出
```

---

# 十、阶段 6：端到端联调

## 完整链路

```txt
Godot 点击 Attack
  ↓
FastAPI /api/game/action
  ↓
C++ CLI handle_json
  ↓
返回 state + events
  ↓
FastAPI 包装返回
  ↓
Godot 更新血条/动画
```

同时：

```txt
React Debug Console 也能发送同样请求并查看响应
```

## 联调检查点

```txt
1. request_id 是否原样返回
2. ok 是否正确
3. state.entities 中 HP 是否更新
4. events seq 是否连续
5. damage value 是否正确
6. Godot 是否按 events 顺序播放
7. React 是否能展示完整响应
8. C++ 错误是否被 FastAPI 包装
```

## 阶段成功标准

```txt
同一个 /api/game/action 接口
React 能调
Godot 能调
C++ 真正参与计算
```

这就是第一个真正的系统里程碑。

---

# 十一、阶段 7：加入存档和配置接口

不要第一阶段就做，但 MVP 跑通后应该加。

## 你负责 FastAPI + React

### 存档接口

```txt
GET    /api/saves
POST   /api/saves
GET    /api/saves/{save_id}
PUT    /api/saves/{save_id}
DELETE /api/saves/{save_id}
```

第一版可以用本地 JSON 文件存储：

```txt
backend/storage/saves/*.json
```

后续再换 SQLite/PostgreSQL。

### 配置接口

```txt
GET /api/config/version
GET /api/config/race
GET /api/config/job
GET /api/config/skill
GET /api/config/effect
GET /api/config/buff
```

### React 页面

```txt
/saves       存档管理
/config      配置查看
/debug/action 调试接口
```

---

## 另一个人负责 C++ + Godot

```txt
C++ Core 支持更多 action
Godot 支持加载存档
Godot 支持角色/怪物状态初始化
Godot 支持更多事件表现
```

---

# 十二、阶段 8：扩展游戏动作

按这个顺序扩，不要乱：

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

每新增一个 action，都必须同步改：

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

# 十三、推荐时间线

## 第 1 周：契约 + mock + 双客户端接入

你：

```txt
api_contract.md
attack examples
FastAPI mock
React Debug Console 初版
```

他：

```txt
Godot 战斗测试场景
Godot 调 FastAPI mock
C++ CLI 框架
```

---

## 第 2 周：C++ 真接入

你：

```txt
cpp_core_client.py
FastAPI mock -> C++ 切换
React Debug Console 完善错误展示
```

他：

```txt
C++ attack 真逻辑
Godot 根据真实返回更新表现
```

---

## 第 3 周：战斗闭环

你：

```txt
存档 mock / 本地 JSON 存档
React Save 页面雏形
```

他：

```txt
end_turn
敌人反击
死亡判定
Godot 战斗表现完善
```

---

## 第 4 周：可展示 Demo

共同完成：

```txt
玩家攻击
敌人反击
死亡
事件日志
血条变化
存档读取/写入
React Debug/Save 工具
Godot 简单战斗演示
```

---

# 十四、你们每天/每次开发的协作流程

推荐固定流程：

```txt
1. 先改 api_contract.md
2. 再改 examples
3. 你更新 FastAPI/React
4. 他更新 C++/Godot
5. 用 React Debug Console 测接口
6. 用 Godot 测表现
7. 出问题先看 JSON，不先甩锅代码
```

只要接口变了，必须同步：

```txt
docs
examples
FastAPI schema
C++ parser
Godot parser
React types
```

---

# 十五、最重要的工程纪律

## 1. C++ 不依赖 Godot

C++ Core 不 include Godot，不知道 Godot 节点。

## 2. Godot 不写规则

Godot 不算伤害、不判定 Buff、不结算背包。

## 3. React 不做游戏主画面

React 做调试、门户、存档、配置工具。

## 4. Python 不写游戏规则

FastAPI 只校验、转发、存档、错误包装。

## 5. JSON Contract 优先

任何字段变化先改 contract 和 examples。

---

# 十六、最终执行流程总览

你们新的完整流程就是：

```txt
Phase 0：JSON 接口规约
  你主导，他确认

Phase 1：FastAPI Mock Backend
  你实现

Phase 2：React Debug Console
  你实现

Phase 3：Godot 接 Mock API
  他实现，你提供接口文档

Phase 4：C++ CLI Core
  他实现，你提供 examples

Phase 5：FastAPI 接真实 C++
  你实现，他提供 CLI 路径和运行方式

Phase 6：端到端联调
  两人一起

Phase 7：存档/配置接口
  你主导，他接入 Godot

Phase 8：扩展 action 和游戏系统
  他主导 C++/Godot，你维护 API/React 工具
```

一句话总结：

> 你负责把“通道”和“工具”做稳，他负责把“规则”和“表现”做出来；两个人共同维护 JSON Contract。