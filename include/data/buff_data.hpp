#ifndef BUFF_DATA_HPP
#define BUFF_DATA_HPP

#include "base_data.hpp"
#include "definitions/buff_type.hpp"

struct BuffData : public BaseData
{
    TriggerType trigger_type;          // 触发时机类型
    int max_stack;                     // 最大叠加层数
    bool dispellable;                  // 是否可被驱散
    std::string stance;                // 立场（增益或是减益）
    std::vector<int> tick_effect_ids;  // 触发效果列表

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(BuffData, id, name, description, trigger_type, max_stack,
                                   dispellable, tick_effect_ids);
};

#endif