#pragma once

// 属性转换系数（基础属性 → 衍生属性）
// 对应 spec 中 3.2.1 节的各个公式参数
struct CalcCoefficients
{
    // BaseMaxHP (3.2.1.1)
    double HPPerStrength;
    double HPPerPhysique;
    double HPPerWillpower;

    // BaseMaxMP (3.2.1.2)
    double MPPerEsthesia;
    double MPPerBewitchment;
    double MPPerWillpower;

    // BaseMaxSP (3.2.1.3)
    double SPPerStrength;
    double SPPerPhysique;
    double SPPerWillpower;

    // Evasion (3.2.1.4)
    double EvasionPerDexterity;
    double EvasionPerEsthesia;
    double EvasionPerSpeed;
    double EvasionPerLuck;

    // PhysicalHit (3.2.1.5)
    double PhysicalHitPerDexterity;
    double PhysicalHitPerEsthesia;
    double PhysicalHitPerSpeed;
    double PhysicalHitPerLuck;

    // MagicalHit (3.2.1.6)
    double MagicalHitPerEsthesia;
    double MagicalHitPerBewitchment;
    double MagicalHitPerLuck;

    // PhysicalDefense (3.2.1.7)
    double PhysicalDefensePerPhysique;

    // MagicalDefense (3.2.1.8)
    double MagicalDefensePerPhysique;
    double MagicalDefensePerBewitchment;

    // IgnorePhysicalDefenseRate (3.2.1.9)
    double IgnorePhysicalDefenseRatePerStrength;
    double IgnorePhysicalDefenseRatePerEsthesia;
    double IgnorePhysicalDefenseRatePerLuck;

    // IgnoreMagicalDefenseRate (3.2.1.10)
    double IgnoreMagicalDefenseRatePerBewitchment;
    double IgnoreMagicalDefenseRatePerEsthesia;
    double IgnoreMagicalDefenseRatePerLuck;

    // PhysicalCritRate (3.2.1.11)
    double PhysicalCritRatePerEsthesia;
    double PhysicalCritRatePerLuck;

    // MagicalCritRate (3.2.1.12)
    double MagicalCritRatePerEsthesia;
    double MagicalCritRatePerLuck;

    // GrowthHPPerLevel (3.2.1.13)
    double HPPerLifeGrowthPerLevel;

    // GrowthMPPerLevel (3.2.1.14)
    double MPPerMagicGrowthPerLevel;

    // BasePhysicalAttackPower (3.2.1.15)
    double BasePhysicalAttackPowerPerStrength;
    double BasePhysicalAttackPowerPerPhysique;

    // BaseMagicalAttackPower (3.2.1.16)
    double BaseMagicalAttackPowerPerBewitchment;
    double BaseMagicalAttackPowerPerWillpower;
};
