# 成员 B 开发日志：C++ Core + Godot

最后更新：2026-06-21 00:15

负责人范围：

```txt
C++ Core JSON 输入输出
abel_core_cli
attack action handler
GameState / EntityState / GameEvent 解析与生成
Godot HTTP 请求 FastAPI
Godot events 消费与表现
```

---

## 2026-06-21 00:15 - 学习型协作流程启用

### 本阶段目标

将成员 B 后续 C++ Core + Godot 开发方式调整为“开发者先尝试，Agent 指导纠错”的学习型流程。

### 学习目标

理解后续 C++ Core / Godot 小切片应先明确输入输出契约、最小实现范围、验证命令和预期结果，再通过手动尝试推进。

### 你先尝试的内容

本阶段为协作规则整理，暂未进行 C++ 或 Godot 代码尝试。

### 修改文件

```txt
docs/learning_collaboration_workflow.md
docs/development_logs/README.md
docs/development_logs/member_a_fastapi_react.md
docs/development_logs/member_b_cpp_godot.md
```

### 验证方式

读取文档，确认学习流程、日志模板和恢复入口已更新。

### 验证结果

```txt
PASSED
```

### 遇到的问题

原有成员 B 日志只记录了可接手方向，还没有说明后续如何在 C++ Core / Godot 开发中保持学习节奏。

### 纠错与解释

在成员 B 日志中补充学习型协作入口，后续做 `handle_json()`、`abel_core_cli` 或 Godot HTTP 接入时，先讲概念和验证方式，再由开发者尝试。

### 学到的东西

C++ Core / Godot 的学习切片应围绕可观察输入输出推进，例如 JSON 文件、CLI stdout、Godot 请求日志和事件展示。

### 仍然不理解的问题

暂无。

### 当前状态

成员 B 后续 C++ Core + Godot 小切片默认采用学习型协作流程。

### 下一步建议

优先从 C++ CLI Core 最小切片开始：先理解 `docs/api_contract.md` 与 `examples/requests/attack.json`，再设计 `handle_json(input_json)` 的最小输入输出。

---

## 2026-06-20 18:22 - 准备状态记录

### 本阶段目标

记录成员 B 当前可接手的开发入口和依赖文档。

### 修改文件

```txt
docs/development_logs/member_b_cpp_godot.md
```

### 实现内容

当前尚未开始 C++ Core 与 Godot 侧实现，但依赖契约已经准备完成。

可依赖文件：

```txt
docs/api_contract.md
examples/requests/attack.json
examples/responses/attack_success.json
examples/responses/attack_error.json
backend/app/api/game.py
frontend/src/pages/DebugAction.tsx
```

### 验证方式

暂无成员 B 侧实现，因此暂无 C++ / Godot 验证。

### 验证结果

```txt
NOT STARTED
```

### 当前状态

成员 B 可以从以下两个方向选择一个小切片开始：

```txt
方向 A：C++ CLI Core
方向 B：Godot 接 FastAPI Mock API
```

### 下一步建议

优先建议先做 C++ CLI Core 的最小切片：

```txt
1. 提供 handle_json(input_json) 统一入口。
2. 提供 abel_core_cli stdin/stdout。
3. 读取 examples/requests/attack.json。
4. 输出符合 docs/api_contract.md 的 JSON。
5. 第一版只支持 action.type = attack。
```

目标验证方式：

```bash
abel_core_cli < examples/requests/attack.json
```

目标验证结果：

```txt
输出合法 JSON。
ok=true。
events 中包含 attack_started、damage、hp_changed。
damage value=18。
target slime_1 hp 从 50 变为 32。
```

如果先做 Godot Mock API 接入，则目标验证方式：

```txt
1. 启动 FastAPI backend。
2. Godot 点击 Attack。
3. Godot 请求 /api/game/action。
4. Godot 按 events.seq 展示日志、血条变化和简单动画。
```
