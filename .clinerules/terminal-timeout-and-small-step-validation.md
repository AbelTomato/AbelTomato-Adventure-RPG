# 终端命令超时与小步验证规则

适用范围：本项目所有后续开发任务。

## 1. 核心问题

本项目曾多次因为终端命令执行失败、输出捕获异常、长时间下载依赖或路径转义问题导致 Agent 卡死或长时间思考。

因此后续必须采用“短命令、小步验证、失败即停”的终端执行方式。

---

## 2. 命令执行规则

1. 不把多个耗时命令串成一条长命令。
2. 验证命令必须拆分为小步骤执行。
3. 每条命令只验证一个明确目标。
4. 命令失败后，必须先分析失败原因，不继续执行后续命令。
5. 如果命令输出无法捕获，但工具报告执行成功，可以再运行一个更短的确认命令。
6. 如果命令超时进入后台，不要立刻继续叠加新命令；先等待或改用更短命令确认状态。
7. 避免在 Git Bash 中使用 Windows 反斜杠路径，优先使用正斜杠路径。
8. 避免使用 `> NUL` 这类在 Git Bash 下可能生成 `NUL` 文件的重定向。

---

## 3. 推荐命令拆分方式

### JSON 校验

不要这样：

```bash
python -m json.tool examples/requests/attack.json > NUL && python -m json.tool examples/responses/attack_success.json > NUL && python -m json.tool examples/responses/attack_error.json > NUL
```

应该拆成：

```bash
python -m json.tool examples/requests/attack.json
python -m json.tool examples/responses/attack_success.json
python -m json.tool examples/responses/attack_error.json
```

### Backend 测试

使用：

```bash
backend/.venv/Scripts/python.exe -m pytest backend/tests
```

不要和 frontend typecheck 串在同一条命令里。

### Frontend 类型检查

使用：

```bash
pnpm -C frontend typecheck
```

如果输出捕获异常但命令成功，可以再运行：

```bash
pnpm -C frontend exec tsc -b --pretty false
```

---

## 4. 超时处理规则

当命令超时或进入后台时：

1. 停止继续执行依赖该结果的新命令。
2. 向用户说明当前命令正在后台运行或输出未捕获。
3. 若可能，改用更短的验证命令重新确认。
4. 若仍无法确认，标记该验证为 `UNKNOWN`，不要声称通过。
5. 给出用户可手动执行的替代命令。

---

## 5. 阶段汇报要求

每个小阶段结束时，必须明确写出：

```txt
验证方式：运行了哪些具体命令。
验证结果：通过/失败/UNKNOWN。
失败原因：如有失败，说明根因。
下一步：只给一个最小可执行方向。
```

如果某条命令失败但后续用修正命令验证通过，也必须在汇报中说明失败原因和修正方式。
