#ifndef BUFF_DATA_HPP
#define BUFF_DATA_HPP

#include <string>

#include "base_data.hpp"
#include "definitions/buff_type.hpp"

class BuffData : public BaseData
{
private:
    std::string description;

    TriggerType trigger_type;  // 触发时机
    int max_stack;             // 最大叠加层数
    bool dispellable;          // 是否可被驱散

    std::vector<int> tick_effect_ids;  // buff触发时执行的effect列表
public:
    TriggerType get_trigger_type() const
    {
        return trigger_type;
    }

    const std::vector<int>& get_effects() const
    {
        return tick_effect_ids;
    }

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(BuffData, id, name, description, trigger_type, max_stack,
                                   dispellable, tick_effect_ids);
};

#endif