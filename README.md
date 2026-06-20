# 爱吃番茄的勇者 Abel

## 项目简介

**爱吃番茄的勇者 Abel** 起源于 AbelTomato 在面向对象程序设计课程中临时起意的 C++ 文字 RPG 练习项目。当前项目正在从早期命令行文字交互游戏，逐步演进为一个数据驱动的 RPG 原型。

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

项目处于 MVP 链路搭建阶段，当前重点是验证统一 JSON 契约下的最小游戏动作链路。

已具备的能力：

- 已定义第一阶段 API Contract 与 `attack` 示例 JSON。
- 已搭建 FastAPI Mock Backend。
- 已搭建 React + Vite + TypeScript 前端骨架。
- 已实现 React Debug Console，用于发送 action 并查看响应、事件和错误。
- 已完成前后端 Mock 链路验证。

仍在推进的能力：

- Godot 调用 FastAPI Mock API。
- C++ `handle_json()` 与 `abel_core_cli`。
- FastAPI 从 Mock 响应切换到真实 C++ Core。
- React / Godot / FastAPI / C++ 的端到端联调。

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
