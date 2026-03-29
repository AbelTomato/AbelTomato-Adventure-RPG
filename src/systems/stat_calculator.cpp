#include "systems/stat_calculator.hpp"

double StatCalculator::compute_additive_multiplicative(double base, double flat, double pct)
{
    return (base + flat) * (1.0 + pct);
}

void StatCalculator::compute_derived_bases(std::array<double, Attr::MaxCount>& data,
                                           const CalcCoefficients& coeffs)
{
    using namespace Attr;

    // TODO: 完成计算逻辑

    double str = data[static_cast<int>(ID::FinalStrength)];
    double vit = data[static_cast<int>(ID::FinalPhysique)];
    double dex = data[static_cast<int>(ID::FinalDexterity)];
    double est = data[static_cast<int>(ID::FinalEsthesia)];
    double bew = data[static_cast<int>(ID::FinalBewitchment)];
    double wil = data[static_cast<int>(ID::FinalWillpower)];
    double spd = data[static_cast<int>(ID::FinalSpeed)];
    double luk = data[static_cast<int>(ID::FinalLuck)];
    double lgr = data[static_cast<int>(ID::FinalLifeGrowth)];   // 生命成长
    double mgr = data[static_cast<int>(ID::FinalMagicGrowth)];  // 法力成长
    double lvl = data[static_cast<int>(ID::CurLevel)];          // 当前等级

    // 基础最大生命（无等级成长）
    double base_max_hp =
        str * coeffs.HPPerStrength + vit * coeffs.HPPerPhysique + wil * coeffs.HPPerWillpower;
    // 等级成长生命 = 生命成长 * 每级生命系数
    double growth_hp_per_level = lgr * coeffs.HPPerLifeGrowthPerLevel;
    // 当前最大生命 = 基础 + 成长 * (等级-1)
    data[static_cast<int>(ID::CurMaxHP)] = base_max_hp + growth_hp_per_level * (lvl - 1.0);

    // 基础最大法力
    double base_max_mp =
        est * coeffs.MPPerEsthesia + bew * coeffs.MPPerBewitchment + wil * coeffs.MPPerWillpower;
    double growth_mp_per_level = mgr * coeffs.MPPerMagicGrowthPerLevel;
    data[static_cast<int>(ID::CurMaxMP)] = base_max_mp + growth_mp_per_level * (lvl - 1.0);

    // 基础最大精力（无等级成长）
    double base_max_sp =
        str * coeffs.SPPerStrength + vit * coeffs.SPPerPhysique + wil * coeffs.SPPerWillpower;
    data[static_cast<int>(ID::CurMaxSP)] = base_max_sp;

    // ========== 派生属性基准 ==========
    // 物理攻击力
    data[static_cast<int>(ID::DerivedPhysAtk)] = str * coeffs.BasePhysicalAttackPowerPerStrength +
                                                 vit * coeffs.BasePhysicalAttackPowerPerPhysique;

    // 魔法攻击力
    data[static_cast<int>(ID::DerivedMagAtk)] = bew * coeffs.BaseMagicalAttackPowerPerBewitchment +
                                                wil * coeffs.BaseMagicalAttackPowerPerWillpower;

    // 闪避值
    data[static_cast<int>(ID::DerivedEvasion)] =
        dex * coeffs.EvasionPerDexterity + est * coeffs.EvasionPerEsthesia +
        spd * coeffs.EvasionPerSpeed + luk * coeffs.EvasionPerLuck;

    // 物理命中值
    data[static_cast<int>(ID::DerivedPhysHit)] =
        dex * coeffs.PhysicalHitPerDexterity + est * coeffs.PhysicalHitPerEsthesia +
        spd * coeffs.PhysicalHitPerSpeed + luk * coeffs.PhysicalHitPerLuck;

    // 魔法命中值
    data[static_cast<int>(ID::DerivedMagHit)] = est * coeffs.MagicalHitPerEsthesia +
                                                bew * coeffs.MagicalHitPerBewitchment +
                                                luk * coeffs.MagicalHitPerLuck;

    // 物理防御力
    data[static_cast<int>(ID::DerivedPhysDef)] = vit * coeffs.PhysicalDefensePerPhysique;

    // 魔法防御力
    data[static_cast<int>(ID::DerivedMagDef)] =
        vit * coeffs.MagicalDefensePerPhysique + bew * coeffs.MagicalDefensePerBewitchment;

    // 无视物理防御几率
    data[static_cast<int>(ID::DerivedIgnorePhysDef)] =
        str * coeffs.IgnorePhysicalDefenseRatePerStrength +
        est * coeffs.IgnorePhysicalDefenseRatePerEsthesia +
        luk * coeffs.IgnorePhysicalDefenseRatePerLuck;

    // 无视魔法防御几率
    data[static_cast<int>(ID::DerivedIgnoreMagDef)] =
        bew * coeffs.IgnoreMagicalDefenseRatePerBewitchment +
        est * coeffs.IgnoreMagicalDefenseRatePerEsthesia +
        luk * coeffs.IgnoreMagicalDefenseRatePerLuck;

    // 物理暴击率
    data[static_cast<int>(ID::DerivedPhysCritRate)] =
        est * coeffs.PhysicalCritRatePerEsthesia + luk * coeffs.PhysicalCritRatePerLuck;

    // 魔法暴击率
    data[static_cast<int>(ID::DerivedMagCritRate)] =
        est * coeffs.MagicalCritRatePerEsthesia + luk * coeffs.MagicalCritRatePerLuck;
}
