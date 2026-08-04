# 文档入口

最后更新：2026-08-04

本文是 `docs/` 的当前入口。新对话、恢复任务或人工继续开发时，优先从这里开始，而不是直接阅读历史日志。

## 1. 当前项目状态

当前真实状态：**Phase 7 存档与配置接口开发中途，质量闸门需要先恢复**。

已确认：

- Phase 6 端到端 `attack` MVP 链路曾完成验证。
- 当前 C++ CLI `build-mingw/abel_core_cli.exe` 可构建，验证脚本 8/8 通过。
- 当前前端 `pnpm -C frontend typecheck` 通过。
- 当前后端全量测试失败，原因是 `backend/tests/test_save_service.py` 语法不完整。
- 当前 `backend/app/api/config.py` 存在路径安全判断方向错误。
- 当前 Phase 7 saves API 尚未完整接入。

因此当前不要继续扩展技能、地图、复杂背包或新 action。先按 `docs/原子化发展路线.md` 从 A1 开始。

## 2. 必读文档顺序

### 当前开发必读

1. `docs/README.md`：文档入口和当前状态。
2. `docs/原子化发展路线.md`：未来原子化切片路线。
3. `docs/代码评审.md`：当前问题清单和验证结果。
4. `docs/api_contract.md`：跨端 JSON 契约。
5. `docs/development_logs/README.md`：历史日志入口。

### 设计参考

- `docs/game_design_spec.md`：游戏设计总说明。
- `docs/inventory_design.md`：背包系统设计。
- `docs/exposed_interface_designs.md`：接口暴露方案。
- `docs/project_structure_and_git_workflow.md`：仓库结构和 Git 工作流建议。

### 历史/归档参考

- `docs/CODEX_Memory.md`
- `docs/CODEX_Memory_CN.md`
- `docs/cooperation_rule.md`
- `docs/bug_list.md`
- `docs/development_logs/`：按日期归档的开发日志；从 `docs/development_logs/README.md` 的日期索引进入。

这些文档保留历史上下文，但不作为当前执行入口。若与 `docs/README.md`、`docs/原子化发展路线.md`、`docs/代码评审.md` 冲突，以后三者为准。

## 3. 当前下一步

立即执行：

```txt
A1 修复 backend/tests/test_save_service.py 语法错误
```

验证命令：

```powershell
backend/.venv/Scripts/python.exe -m pytest backend/tests/test_save_service.py
```

完成后再进入：

```txt
A2 修复 Config API 路径判断方向
```

## 4. 文档维护规则

1. 当前状态只维护在 `docs/README.md` 和 `docs/原子化发展路线.md`。
2. 详细问题清单维护在 `docs/代码评审.md`；后续修复后可同步更新状态。
3. API 字段变化必须先更新 `docs/api_contract.md` 和 `examples/`。
4. 历史日志不回写当前状态，只追加新阶段记录。
5. 明确过时且内容已被新文档覆盖的小文档可以删除；大型历史文档先保留。

## 5. 已删除/合并的过时文档

- `docs/work_rules.md`：内容已被 `docs/README.md`、`docs/原子化发展路线.md`、`docs/development_logs/README.md` 覆盖。
- `docs/exposed_interface_designs_Misuzu.md`：内容与 `docs/exposed_interface_designs.md`、`docs/api_contract.md` 重复；当前正式契约以 `docs/api_contract.md` 为准。
