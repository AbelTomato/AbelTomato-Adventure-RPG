#ifndef EFFECT_DATA_HPP
#define EFFECT_DATA_HPP

#include <nlohmann/json.hpp>

#include "base_data.hpp"
#include "definitions/effect_type.hpp"

struct EffectData : public BaseData
{
    EffectType effect_type;
    int target_sub_type_id; /*代表目标子类型ID，由上下文中的effect_type决定目标，
                            例如当effect_type=Attribute_Change时，
                            target_sub_type_id就决定当前effect是扣血还是加蓝，
                            为Apply_Buff时，就决定是应用buff的种类*/

    double power_efficiency;  // 强度转化效率

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(EffectData, id, name, description, effect_type,
                                   target_sub_type_id, power_efficiency);
};

/*
当effect由skill触发时，根据skill中的EffectSettlementType和manificaition，
计算得到相应的power值，然后对于实际触发的effect，传入power值，
由power值决定最终数值，比如若effect_type=Attribute_Change时，
power值本身就是伤害或治疗数值（不一定，可能会根据公式微调），
若effect_type=Apply_Buff，则设计一个函数来计算power对应的持续回合数。
*/

#endif