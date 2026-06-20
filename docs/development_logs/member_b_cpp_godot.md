# 成员 B 开发日志：C++ Core + Godot

最后更新：2026-06-20 18:22

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
