#ifndef SKILL_DATA_HPP
#define SKILL_DATA_HPP

#include <definitions/combat_type.hpp>
#include <nlohmann/json.hpp>
#include <vector>

#include "base_data.hpp"
#include "definitions/base_type.hpp"
#include "definitions/skill_type.hpp"

struct SkillData : public BaseData  // 技能仅用来触发效果，由效果决定强度
{
    struct EffectTrigger
    {
        int effect_id;                                // 对应触发的效果id
        double chance;                                // 触发几率
        EffectSettlementType effect_settlement_type;  // 触发强度结算方式
        double magnification;                         // 效果倍率
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(EffectTrigger, effect_id, chance, effect_settlement_type,
                                       magnification);
    };

    /*例如技能为治疗术，触发效果为基础治疗，id为2001,触发几率为100%，
    强度结算方式为自身魔法攻击，效果倍率为150%*/

    struct Costs
    {
        StatusType status_type;
        CostMethod cost_method;
        double value;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Costs, status_type, cost_method, value);
    };

    /*衡量释放代价，例如释放火球术，status_type=MP,cost_method=FLAT(固定值)
    value=10，则意思就是，固定消耗10点法力*/

    struct Cast
    {
        int min_range;                    // 最小施法距离
        int max_range;                    // 最大施法距离
        ShapeType shape_type;             // 形状类型
        bool is_relative_to_caster;       // 是否与施法者位置关联
        bool is_target_selected;          // 是否必须选中目标
        std::vector<double> cast_params;  // 范围参数
        TargetFilter target_filter;       // 目标筛选器

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Cast, min_range, max_range, shape_type,
                                       is_relative_to_caster, is_target_selected, cast_params,
                                       target_filter);
    };

    std::string display_effect;  // 对于技能在数据方面的描述

    std::vector<Costs> cost;
    int cooldown;            // 冷却时间
    double hit_correction;   // 命中修正
    double crit_correction;  // 暴击修正
    Cast cast;

    std::vector<EffectTrigger> effect_triggers;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(SkillData, id, name, description, display_effect, cost, cooldown,
                                   hit_correction, crit_correction, cast, effect_triggers);
};

#endif