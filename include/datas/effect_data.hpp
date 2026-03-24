#ifndef EFFECT_DATA_HPP
#define EFFECT_DATA_HPP

#include <nlohmann/json.hpp>

#include "base_data.hpp"
#include "definitions/effect_type.hpp"

struct EffectData : public BaseData
{
    EffectType effect_type;
    double value_scaler;     // 转换系数
    double duration_scaler;  // 时间系数
    int target_sub_type_id; /*代表目标子类型ID，由上下文中的effect_type决定目标，
                            例如当effect_type=Attribute_Change时，
                            target_sub_type_id就决定当前effect是扣血还是加蓝，
                            为Apply_Buff时，就决定是应用buff的种类*/

    int calc_value(double final_power) const;
    int calc_duration(double final_power) const;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(EffectData, id, name, description, effect_type, value_scaler,
                                   duration_scaler, target_sub_type_id);
};

#endif