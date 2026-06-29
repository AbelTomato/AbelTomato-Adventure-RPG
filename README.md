# 番茄战魂

## 项目简介

**番茄战魂** 起源于 AbelTomato 在面向对象程序设计课程中临时起意的 C++ 文字 RPG 练习项目。当前项目正在从早期命令行文字交互游戏，逐步演进为一个数据驱动的 RPG 原型。

项目希望把游戏规则、服务接口和客户端表现拆分开来，让核心玩法可以被命令行、Web 调试工具和图形化客户端共同复用。

---

## 项目形态

当前仓库包含以下主要部分：

| 模块            | 说明                                                                |
| --------------- | ------------------------------------------------------------------- |
| C++ Core        | 游戏规则、实体、配置、战斗和背包等核心逻辑                          |
| FastAPI Backend | 面向客户端的 HTTP API、JSON 校验、Mock 响应和后续 C++ Core 调用桥接 |
| React Frontend  | Web 调试工具与后续项目 Portal                                       |
| Godot Client    | 后续图形化游戏客户端                                                |
| Docs / Examples | 接口契约、开发规划、示例请求和示例响应                              |
| Data            | 种族、职业、技能、Buff、效果等数据配置                              |

当前整体协作方向：

```txt
React / Godot
    ↓
FastAPI Backend
    ↓
C++ Core
```

---

## 当前状态

项目已完成 `attack` MVP 端到端链路验证，当前处于 **Phase 7：存档与配置接口开发中途**。

当前优先级不是继续扩展玩法，而是先恢复后端质量闸门：修复 `backend/tests/test_save_service.py` 的测试收集错误、修复配置接口路径判断问题，再继续实现 saves API。

已具备的能力：

- 已定义第一阶段 API Contract 与 `attack` 示例 JSON。
- 已搭建 FastAPI Mock Backend。
- 已搭建 React + Vite + TypeScript 前端骨架。
- 已实现 React Debug Console，用于发送 action 并查看响应、事件和错误。
- 已完成前后端 Mock 链路验证。

当前推进的能力：

- Phase 7 本地 JSON 存档接口。
- Phase 7 配置只读接口稳定化。
- React 存档/配置调试页面。
- Godot 保存/加载最小接入。

---

## 开发环境与依赖版本

当前仓库主要依赖版本如下。若依赖声明和锁定版本不同，以锁定文件或项目配置文件为准。

| 模块         | 依赖 / 工具              | 当前版本 / 约束        | 来源                            |
| ------------ | ------------------------ | ---------------------- | ------------------------------- |
| C++ Core     | CMake                    | `>= 3.10`              | `CMakeLists.txt`                |
| C++ Core     | C++ Standard             | `C++17`                | `CMakeLists.txt`                |
| C++ Core     | 项目版本                 | `Abel-Tomato-Soul 1.0` | `CMakeLists.txt`                |
| C++ Core     | nlohmann/json            | `3.12.0`               | `third_party/nlohmann/json.hpp` |
| Backend      | FastAPI                  | `>= 0.111.0`           | `backend/requirements.txt`      |
| Backend      | Uvicorn                  | `>= 0.30.0`            | `backend/requirements.txt`      |
| Backend      | Pydantic                 | `>= 2.7.0`             | `backend/requirements.txt`      |
| Backend      | httpx                    | `>= 0.28.0`            | `backend/requirements.txt`      |
| Backend      | pytest                   | `>= 8.2.0`             | `backend/requirements.txt`      |
| Frontend     | pnpm lockfile            | `9.0`                  | `frontend/pnpm-lock.yaml`       |
| Frontend     | React / React DOM        | `19.2.7`               | `frontend/pnpm-lock.yaml`       |
| Frontend     | Vite                     | `8.0.16`               | `frontend/pnpm-lock.yaml`       |
| Frontend     | TypeScript               | `6.0.3`                | `frontend/pnpm-lock.yaml`       |
| Frontend     | Tailwind CSS             | `4.3.1`                | `frontend/pnpm-lock.yaml`       |
| Frontend     | @tailwindcss/vite        | `4.3.1`                | `frontend/pnpm-lock.yaml`       |
| Frontend     | @vitejs/plugin-react     | `6.0.2`                | `frontend/pnpm-lock.yaml`       |
| Frontend     | @radix-ui/react-slot     | `1.3.0`                | `frontend/pnpm-lock.yaml`       |
| Frontend     | class-variance-authority | `0.7.1`                | `frontend/pnpm-lock.yaml`       |
| Frontend     | clsx                     | `2.1.1`                | `frontend/pnpm-lock.yaml`       |
| Frontend     | lucide-react             | `1.21.0`               | `frontend/pnpm-lock.yaml`       |
| Frontend     | tailwind-merge           | `3.6.0`                | `frontend/pnpm-lock.yaml`       |
| Frontend     | @types/node              | `26.0.0`               | `frontend/pnpm-lock.yaml`       |
| Frontend     | @types/react             | `19.2.17`              | `frontend/pnpm-lock.yaml`       |
| Frontend     | @types/react-dom         | `19.2.3`               | `frontend/pnpm-lock.yaml`       |
| Godot Client | Godot                    | `4.6`                  | `godot-client/project.godot`    |
| Godot Client | Renderer                 | `Forward Plus`         | `godot-client/project.godot`    |
| Godot Client | Physics                  | `Jolt Physics`         | `godot-client/project.godot`    |
| Godot Client | Windows Rendering Driver | `d3d12`                | `godot-client/project.godot`    |

注意：`frontend/package.json` 当前大量使用 `latest`，实际可复现版本应以 `frontend/pnpm-lock.yaml` 为准。

---

## 核心约定

- C++ Core 负责游戏规则，不依赖 UI、HTTP 或具体客户端。
- FastAPI 负责接口编排、请求校验、错误包装和后续 C++ Core 调用。
- React 主要承担 Web 调试工具和开发 Portal。
- Godot 主要承担图形化游戏表现。
- `state` 表示游戏结算后的最终状态，`events` 表示过程事件。
- `docs/api_contract.md` 是跨模块协作的统一接口契约。

---

## 主要文档

- `docs/README.md`：当前文档入口与真实项目状态。
- `docs/原子化发展路线.md`：面向边学边写的原子化切片路线。
- `docs/代码评审.md`：当前全量审阅问题清单和验证结果。
- `docs/current_development_plan.md`：当前总体规划与架构边界。
- `docs/api_contract.md`：第一阶段 JSON API Contract。
- `docs/development_logs/README.md`：当前开发状态与恢复入口。
- `docs/game_design_spec.md`：游戏设计说明。
- `docs/inventory_design.md`：背包系统设计。
- `backend/README.md`：后端说明。
- `frontend/README.md`：前端说明。

---

## 历史说明

项目早期运行环境为：

```txt
C++11，MinGW（UTF-8 编码）
```

早期玩法为命令行文字交互。当前仓库仍保留 C++ 原始代码与数据配置，并在此基础上逐步扩展为多端协作的 RPG 原型。

历史更新：

- 2026-03-12：上传了初版内容。
- 2026-03-19：重写了参数传递方式，将生物属性归为结构体管理。
- 2026-06-20：开始搭建 API Contract、FastAPI Mock Backend 与 React Debug Console。
