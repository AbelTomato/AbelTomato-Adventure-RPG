#pragma once

#include <array>
#include <cmath>

#include "definitions/attribute_id.hpp"
#include "definitions/core_attributes.hpp"

class AttributeManager
{
private:
    std::array<double, static_cast<int>(Attr::MaxCount)> _data{};
    std::array<Mod, static_cast<int>(Attr::MaxCount)> _mods{};

    bool _is_dirty = true;

    void recalculate();  // 重新计算属性

    void calculate_primary_finals();  // 计算一级属性的最终结果

    void apply_secondary_mods();  // 完成二级属性的加成计算

public:
    AttributeManager() = default;

    double get(Attr::ID id);  // 获取对应属性

    void set_base(Attr::ID id, double value);  // 导入数据时设置属性值

    void update_mod(Attr::ID id, Mod& delta);  // 更新Mod类型的二级属性加成项

    void update_rate_mod(Attr::ID id, double delta);  // 更新double类型的二级属性加成项
};
