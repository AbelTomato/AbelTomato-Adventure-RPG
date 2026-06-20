# 项目结构与 Git 工作流调整方案

最后更新：2026-06-20

本文根据 `docs/` 目录中的现有设计文档，整理当前项目从单体 C++ 原型向多端协作 Monorepo 演进时的目录结构、职责边界与 Git 工作流建议。

参考文档：

- `docs/game_design_spec.md`
- `docs/development_path.md`
- `docs/exposed_interface_designs.md`
- `docs/current_development_plan.md`
- `docs/inventory_design.md`

---

## 1. 调整目标

当前项目应从“单体 C++ 命令行游戏原型”调整为“数据驱动 RPG 多端协作原型”。

目标架构为：

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

本次结构调整的核心目标：

1. 明确 `C++ Core`、`FastAPI`、`Godot`、`React` 的职责边界。
2. 建立统一 JSON Contract，作为各端协作契约。
3. 让两人团队可以并行开发，减少互相阻塞。
4. 保留当前 C++ 代码可运行性，避免一次性大规模重构破坏现有构建。
5. 为后续 `attack` MVP、Mock Backend、Debug Console 和端到端联调预留清晰目录。

---

## 2. 当前结构判断

当前仓库主要结构为：

```txt
Abel-Tomato-Soul/
  CMakeLists.txt
  include/
  src/
  data/
  docs/
  third_party/
```

这说明当前仓库仍然偏向单体 C++ 项目。

但根据 `docs/current_development_plan.md`，项目已经明确进入以下多端协作方向：

```txt
cpp-core/
backend/
frontend/
godot-client/
data/
docs/
examples/
```

因此当前主要问题是：

```txt
文档架构已经升级为多模块协作，仓库结构仍停留在单体 C++ 原型阶段。
```

---

## 3. 推荐总体结构

推荐采用 Monorepo，即一个仓库内同时管理 C++ Core、FastAPI Backend、React 工具端、Godot 客户端、数据配置、文档和示例 JSON。

长期推荐结构：

```txt
Abel-Tomato-Soul/
  cpp-core/
    CMakeLists.txt
    include/
    src/
    tests/
    third_party/
    README.md

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
    README.md

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
    README.md

  godot-client/
    project.godot
    scenes/
    scripts/
    assets/
    README.md

  data/
    config.json
    race.json
    job.json
    skill.json
    effect.json
    buff.json

  docs/
    game_design_spec.md
    development_path.md
    exposed_interface_designs.md
    current_development_plan.md
    inventory_design.md
    api_contract.md
    project_structure_and_git_workflow.md

  examples/
    requests/
      attack.json
    responses/
      attack_success.json
      attack_error.json
```

---

## 4. 渐进式迁移策略

不建议立即把现有 `include/`、`src/`、`third_party/` 全部移动到 `cpp-core/`。

原因：

1. 当前 C++ 构建入口仍在根目录 `CMakeLists.txt`。
2. 一次性移动容易破坏 include 路径和已有 CMake 配置。
3. 当前优先目标是跑通 JSON Contract 和多端协作链路，而不是目录重构本身。

建议分两阶段处理。

### 4.1 阶段一：新增多端目录骨架

第一阶段只新增协作所需目录：

```txt
backend/
frontend/
godot-client/
examples/
```

同时新增或补齐：

```txt
docs/api_contract.md
examples/requests/attack.json
examples/responses/attack_success.json
examples/responses/attack_error.json
```

现有 C++ 目录暂时保持：

```txt
include/
src/
third_party/
CMakeLists.txt
```

这样可以让 FastAPI、React、Godot 的开发先启动，而不影响现有 C++ 构建。

### 4.2 阶段二：C++ Core 迁移

等以下能力稳定后，再迁移到 `cpp-core/`：

```txt
handle_json(input_json)
abel_core_cli
attack action handler
stdin/stdout JSON 支持
基础 C++ 测试样例
```

迁移后建议采用：

```txt
CMakeLists.txt
cpp-core/CMakeLists.txt
```

其中根目录 `CMakeLists.txt` 只做顶层编排：

```cmake
add_subdirectory(cpp-core)
```

实际 C++ 构建逻辑放到 `cpp-core/CMakeLists.txt`。

---

## 5. 模块职责边界

### 5.1 `cpp-core/`

`cpp-core/` 是规则核心层，只负责游戏规则计算。

负责内容：

- 属性系统
- 战斗系统
- 技能系统
- Buff 系统
- 背包规则
- 回合推进
- 实体状态变更
- `handle_json(const std::string& input_json)`
- `abel_core_cli` stdin/stdout JSON 入口

禁止内容：

- HTTP API
- FastAPI 依赖
- React/Godot 依赖
- 数据库访问
- UI 表现逻辑
- 游戏主循环 `while(true)`

### 5.2 `backend/`

`backend/` 是 FastAPI 服务编排层。

负责内容：

- HTTP API
- Pydantic schema
- JSON Contract 校验
- Mock Response
- C++ CLI 调用
- 超时处理
- 错误包装
- 存档接口
- 配置接口

禁止内容：

- 伤害公式
- Buff 结算
- 技能效果计算
- 背包核心规则
- 回合推进规则

### 5.3 `frontend/`

`frontend/` 是 React Web 工具层，不是主游戏客户端。

第一阶段重点：

- Debug Console
- 请求 JSON 编辑
- 响应 JSON 展示
- events 列表展示
- error 信息展示
- 配置查看
- 存档管理页面

禁止内容：

- 重新计算伤害
- 重新结算 Buff
- 重新判断死亡
- 绕过 FastAPI 直接调用 C++

### 5.4 `godot-client/`

`godot-client/` 是主游戏表现层。

负责内容：

- 地图
- 角色节点
- 动画
- 血条
- 飘字
- 音效
- HTTP 请求 FastAPI
- 按 `events.seq` 顺序播放表现

禁止内容：

- 自行计算伤害
- 自行计算命中、Buff、死亡
- 直接访问 C++ Core

### 5.5 `examples/`

`examples/` 是 JSON Contract 的样例库，也是跨端联调的基础。

第一阶段至少包含：

```txt
examples/requests/attack.json
examples/responses/attack_success.json
examples/responses/attack_error.json
```

这些文件应同时服务于：

- C++ CLI 输入输出测试
- FastAPI Mock 返回
- React Debug Console 默认请求
- Godot HTTP 联调样例

---

## 6. Git 工作流建议

当前仓库只有 `main` 分支，且已有未跟踪文档文件。两人并行开发时，不建议继续直接在 `main` 上开发。

推荐采用轻量 GitHub Flow。

### 6.1 分支模型

```txt
main
  ↑
feature/project-structure
feature/docs-api-contract
feature/examples-attack-json
feature/backend-mock-action
feature/frontend-debug-console
feature/cpp-handle-json
feature/cpp-attack-action
feature/godot-http-action
feature/backend-real-cpp-core
feature/e2e-attack-flow
```

当前阶段不建议引入复杂 Git Flow，例如：

```txt
develop
release/*
hotfix/*
```

两人团队使用 GitHub Flow 更轻、更直观。

### 6.2 `main` 分支规则

`main` 分支必须保持：

1. 可构建。
2. 文档与 examples 一致。
3. 不提交半成品实验代码。
4. 每次合并都对应一个明确小里程碑。
5. 不直接 push，所有功能通过 feature 分支合并。

推荐规则：

```txt
禁止直接 push main
所有改动通过 feature 分支提交 PR
PR 至少由另一名成员检查一次
```

### 6.3 分支命名规范

按职责域命名：

```txt
feature/docs-api-contract
feature/examples-attack-json
feature/backend-mock-action
feature/frontend-debug-console
feature/cpp-handle-json
feature/cpp-attack-action
feature/godot-http-action
feature/project-structure

fix/cpp-invalid-json
fix/backend-error-wrapper
fix/godot-event-order

docs/update-contract-attack
docs/git-workflow
```

### 6.4 提交信息规范

建议使用简化 Conventional Commits：

```txt
feat(cpp-core): add handle_json action entry
feat(backend): add mock game action endpoint
feat(frontend): add debug action console
feat(godot): request mock action api
feat(examples): add attack request and response samples

docs(contract): define attack json contract
docs(workflow): add project structure and git workflow
fix(backend): wrap cpp core errors consistently
refactor(cpp-core): move attack logic into action handler
test(cpp-core): add attack action fixture test
chore(structure): add backend frontend godot directories
```

### 6.5 PR 合并顺序

推荐按以下顺序合并：

```txt
1. feature/project-structure
2. feature/docs-api-contract
3. feature/examples-attack-json
4. feature/backend-mock-action
5. feature/frontend-debug-console
6. feature/godot-http-action
7. feature/cpp-handle-json
8. feature/cpp-attack-action
9. feature/backend-real-cpp-core
10. feature/e2e-attack-flow
```

这样可以保证接口契约先稳定，再进行各端实现。

---

## 7. 协作纪律

### 7.1 JSON Contract 优先

任何新增 action 或字段变更，必须按以下顺序：

```txt
1. 更新 docs/api_contract.md
2. 更新 examples/requests/*.json
3. 更新 examples/responses/*.json
4. 更新 backend schema/mock
5. 更新 cpp-core handler
6. 更新 frontend/godot 展示或调用逻辑
```

不要先改代码再补文档。

### 7.2 跨模块改动拆 PR

不建议一个 PR 同时包含：

```txt
C++ 规则 + FastAPI + React UI + Godot 场景 + 文档
```

推荐拆分为：

```txt
PR 1：docs + examples
PR 2：backend schema/mock
PR 3：cpp-core handler
PR 4：frontend 或 godot 接入
```

### 7.3 examples 作为契约测试

`examples/requests/attack.json` 应作为统一测试输入。

它至少应被以下模块复用：

- `abel_core_cli`
- FastAPI mock/backend 测试
- React Debug Console
- Godot 接口联调

### 7.4 每个模块维护自己的 README

建议后续新增：

```txt
cpp-core/README.md
backend/README.md
frontend/README.md
godot-client/README.md
```

每个 README 只写必要内容：

- 如何安装依赖
- 如何启动
- 如何测试
- 与 JSON Contract 的关系

---

## 8. `.gitignore` 调整建议

后续 `.gitignore` 建议覆盖：

```gitignore
# C++
build/
out/
*.exe
*.obj
*.pdb
*.ilk

# Python
backend/.venv/
backend/__pycache__/
backend/.pytest_cache/
*.pyc

# Node
frontend/node_modules/
frontend/dist/
frontend/.vite/

# Godot
.godot/
godot-client/.godot/
godot-client/export_presets.cfg

# Local saves
backend/app/storage/saves/*.json
!backend/app/storage/saves/.gitkeep

# IDE
.vscode/
```

注意：以下文件必须进入 Git：

```txt
data/*.json
examples/**/*.json
docs/*.md
```

---

## 9. 建议落地顺序

### 9.1 第一批：结构与契约

分支：

```txt
feature/project-structure
feature/docs-api-contract
feature/examples-attack-json
```

交付物：

```txt
backend/
frontend/
godot-client/
examples/
docs/api_contract.md
examples/requests/attack.json
examples/responses/attack_success.json
examples/responses/attack_error.json
```

成功标准：

```txt
两名成员都能根据同一份 JSON Contract 开始开发。
```

### 9.2 第二批：Mock 链路

分支：

```txt
feature/backend-mock-action
feature/frontend-debug-console
feature/godot-http-action
```

成功标准：

```txt
React 和 Godot 都能调用同一个 FastAPI mock 接口。
```

### 9.3 第三批：真实 C++ Core

分支：

```txt
feature/cpp-handle-json
feature/cpp-attack-action
feature/backend-real-cpp-core
```

成功标准：

```txt
FastAPI 从 mock 切换到 C++ CLI 后，React 和 Godot 不需要修改请求代码。
```

### 9.4 第四批：端到端联调

分支：

```txt
feature/e2e-attack-flow
```

成功标准：

```txt
Godot 点击 Attack
  ↓
FastAPI /api/game/action
  ↓
C++ CLI handle_json
  ↓
返回 state + events
  ↓
Godot 更新表现

React Debug Console 可调同一接口。
```

---

## 10. 最终建议

当前最优先的不是重写代码，而是先稳定协作骨架：

1. 新增 `backend/`、`frontend/`、`godot-client/`、`examples/`。
2. 新增 `docs/api_contract.md`。
3. 新增 attack 请求和响应示例 JSON。
4. 从现在开始使用 `feature/*` 分支开发。
5. 暂缓移动现有 C++ 目录，等 `handle_json()` 和 `abel_core_cli` 稳定后再迁移到 `cpp-core/`。

一句话总结：

> 先用 Monorepo 骨架和 JSON Contract 把协作边界定住，再逐步迁移 C++ Core 和接入 FastAPI、React、Godot，避免一开始就陷入大规模目录重构。