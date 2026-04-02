#pragma once

namespace Attr
{
constexpr int MaxCount = 1000;
constexpr int BaseCount = 100;
constexpr int DerivedCount = 300;
constexpr int ModCount = 500;
constexpr int FinalCount = 700;

enum class ID : int
{
    // --- 基础一级属性 (1-10) ---
    BaseStrength = 1,
    BasePhysique = 2,
    BaseDexterity = 3,
    BaseEsthesia = 4,
    BaseBewitchment = 5,
    BaseWillpower = 6,
    BaseLifeGrowth = 7,
    BaseMagicGrowth = 8,
    BaseSpeed = 9,
    BaseLuck = 10,

    // --- 二级属性基准 (101-120) ---
    DerivedPhysAtk = 101,
    DerivedMagAtk = 102,
    DerivedBlockRate = 103,
    DerivedEvasion = 104,
    DerivedPhysHit = 105,
    DerivedMagHit = 106,
    DerivedPhysCritRate = 107,
    DerivedMagCritRate = 108,
    DerivedPhysDef = 109,
    DerivedMagDef = 110,
    DerivedPhysDamageRed = 111,
    DerivedMagDamageRed = 112,
    DerivedTrueDamageRed = 113,
    DerivedIgnorePhysDef = 114,
    DerivedIgnoreMagDef = 115,
    DerivedPhysCritDmg = 117,
    DerivedMagCritDmg = 118,
    DerivedPhysDamageInc = 119,
    DerivedMagDamageInc = 120,

    // --- 属性加成 Mod (301-331) ---
    ModStrength = 301,
    ModPhysique = 302,
    ModDexterity = 303,
    ModEsthesia = 304,
    ModBewitchment = 305,
    ModWillpower = 306,
    ModLifeGrowth = 307,
    ModMagicGrowth = 308,
    ModSpeed = 309,
    ModLuck = 310,
    ModMaxHP = 311,
    ModMaxMP = 312,
    ModPhysAtk = 313,
    ModMagAtk = 314,
    ModEvasion = 315,
    ModPhysHit = 316,
    ModMagHit = 317,
    ModPhysDef = 318,
    ModMagDef = 319,
    ModPhysCritDmg = 320,
    ModMagCritDmg = 321,
    ModPhysCritRate = 322,
    ModMagCritRate = 323,
    ModBlockRate = 324,
    ModPhysDamageRed = 325,
    ModMagDamageRed = 326,
    ModTrueDamageRed = 327,
    ModIgnorePhysDef = 328,
    ModIgnoreMagDef = 329,
    ModPhysDamageInc = 330,
    ModMagDamageInc = 331,

    // --- 最终属性 (501-529) ---
    FinalStrength = 501,
    FinalPhysique = 502,
    FinalDexterity = 503,
    FinalEsthesia = 504,
    FinalBewitchment = 505,
    FinalWillpower = 506,
    FinalLifeGrowth = 507,
    FinalMagicGrowth = 508,
    FinalSpeed = 509,
    FinalLuck = 510,
    FinalPhysAtk = 511,
    FinalMagAtk = 512,
    FinalBlockRate = 513,
    FinalEvasion = 514,
    FinalPhysHit = 515,
    FinalMagHit = 516,
    FinalPhysCritRate = 517,
    FinalMagCritRate = 518,
    FinalPhysDef = 519,
    FinalMagDef = 520,
    FinalPhysDamageRed = 521,
    FinalMagDamageRed = 522,
    FinalTrueDamageRed = 523,
    FinalIgnorePhysDef = 524,
    FinalIgnoreMagDef = 525,
    FinalPhysCritDmg = 526,
    FinalMagCritDmg = 527,
    FinalPhysDamageInc = 528,
    FinalMagDamageInc = 529,

    // --- 实时状态 (701-705) ---
    CurMaxHP = 701,
    CurMaxMP = 702,
    CurMaxSP = 703,
    CurLevel = 704,
    CurExp = 705
};
}  // namespace Attr
