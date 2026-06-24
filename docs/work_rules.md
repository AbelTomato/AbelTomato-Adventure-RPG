1. Godot 调用 FastAPI Mock API。
2. C++ handle_json() 与 abel_core_cli。
3. FastAPI 从 mock 切换到真实 C++ CLI。
4. 端到端真实 C++ 联调。
5. 提供 handle_json(input_json) 统一入口。
6. 提供 abel_core_cli stdin/stdout。
7. 读取 examples/requests/attack.json。
8. 输出符合 docs/api_contract.md 的 JSON。
9. 第一版只支持 action.type = attack。
10. 开发日志遵循如下格式
每完成一个小阶段，在对应成员日志顶部追加：
```md

## YYYY-MM-DD HH:mm - 阶段名称

### 本阶段目标

### 学习目标

### 你先尝试的内容

### 修改文件

### 验证方式

### 验证结果

### 遇到的问题

### 纠错与解释

### 学到的东西

### 仍然不理解的问题

### 当前状态

### 下一步建议

### agent协助：
本阶段目标
学习目标
修改文件
实现内容
验证方式
验证结果
文档回填
学习收获
下一步建议
```

11. 