#ifndef EFFECT_DATA_HPP
#define EFFECT_DATA_HPP

#include <nlohmann/json.hpp>

#include "base_data.hpp"
#include "definitions/effect_type.hpp"

class EffectData : public BaseData
{
private:
    std::string description;
    EffectType effect_type;

    double value_scaler;     // 转换系数
    double duration_scaler;  // 时间系数

    int effect_type_id;  // 关联的具体效果id

public:
    int calc_value(double final_power);  // TODO: 完善计算逻辑

    int calc_duration(double final_power);

    EffectType get_type() const
    {
        return effect_type;
    }

    int get_related_id() const
    {
        return effect_type_id;
    }

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(EffectData, id, name, effect_type, value_scaler, duration_scaler,
                                   effect_type_id);
};

#endif