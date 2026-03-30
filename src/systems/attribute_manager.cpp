#include "systems/attribute_manager.hpp"

#include "definitions/attribute_id.hpp"
#include "systems/load_configs.hpp"
#include "systems/stat_calculator.hpp"

void AttributeManager::recalculate()
{
    // 首先计算一级属性的最终值，因为二级属性的计算依赖于此
    calculate_primary_finals();

    // 调用计算类的计算二级属性函数
    StatCalculator::compute_derived_bases(_data, G_Cache);

    // 最后把二级属性的Base加上它们的Mod，得到最终属性
    apply_secondary_mods();

    // 清洗脏标记
    _is_dirty = false;
}

void AttributeManager::calculate_primary_finals()
{
    // 遍历一级属性范围
    for (int i = 1; i <= 10; i++)
    {
        int base_idx = i;
        int mod_idx =
            Attr::DerivedCount + i;  // 注意这里在索引设置时不可变动位置，否则会导致计算错误
        int final_idx = Attr::ModCount + i;
        _data[final_idx] = StatCalculator::compute_additive_multiplicative(
            _data[base_idx], _mods[mod_idx].flat, _mods[mod_idx].pct);
    }
}

void AttributeManager::apply_secondary_mods()
{
    using namespace Attr;

    auto calc_final = [&](ID d_id, ID m_id, ID f_id)
    {
        _data[static_cast<int>(f_id)] = StatCalculator::compute_additive_multiplicative(
            _data[static_cast<int>(d_id)], _mods[static_cast<int>(m_id)].flat,
            _mods[static_cast<int>(m_id)].pct);
    };

    calc_final(ID::CurMaxHP, ID::ModMaxHP, ID::CurMaxHP);
    calc_final(ID::CurMaxMP, ID::ModMaxMP, ID::CurMaxMP);
    calc_final(ID::DerivedPhysAtk, ID::ModPhysAtk, ID::FinalPhysAtk);
    calc_final(ID::DerivedMagAtk, ID::ModMagAtk, ID::FinalMagAtk);
    calc_final(ID::DerivedEvasion, ID::ModEvasion, ID::FinalEvasion);
    calc_final(ID::DerivedPhysHit, ID::ModPhysHit, ID::FinalPhysHit);
    calc_final(ID::DerivedMagHit, ID::ModMagHit, ID::FinalMagHit);
    calc_final(ID::DerivedPhysDef, ID::ModPhysDef, ID::FinalPhysDef);
    calc_final(ID::DerivedMagDef, ID::ModMagDef, ID::FinalMagDef);

    // 对于“率”类属性进行特殊处理，改为直接加法
    auto apply_rate = [&](ID d_id, ID m_id, ID f_id)
    {
        _data[static_cast<int>(f_id)] =
            _data[static_cast<int>(d_id)] + _data[static_cast<int>(m_id)];
    };

    apply_rate(ID::DerivedPhysCritDmg, ID::ModPhysCritDmg, ID::FinalPhysCritDmg);
    apply_rate(ID::DerivedMagCritDmg, ID::ModMagCritDmg, ID::FinalMagCritDmg);
    apply_rate(ID::DerivedPhysCritRate, ID::ModPhysCritRate, ID::FinalPhysCritRate);
    apply_rate(ID::DerivedMagCritRate, ID::ModMagCritRate, ID::FinalMagCritRate);
    apply_rate(ID::DerivedBlockRate, ID::ModBlockRate, ID::FinalBlockRate);
    apply_rate(ID::DerivedPhysDamageRed, ID::ModPhysDamageRed, ID::FinalPhysDamageRed);
    apply_rate(ID::DerivedMagDamageRed, ID::ModMagDamageRed, ID::FinalMagDamageRed);
    apply_rate(ID::DerivedTrueDamageRed, ID::ModTrueDamageRed, ID::FinalTrueDamageRed);
    apply_rate(ID::DerivedIgnorePhysDef, ID::ModIgnorePhysDef, ID::FinalIgnorePhysDef);
    apply_rate(ID::DerivedIgnoreMagDef, ID::ModIgnoreMagDef, ID::FinalIgnoreMagDef);
    apply_rate(ID::DerivedPhysDamageInc, ID::ModPhysDamageInc, ID::FinalPhysDamageInc);
    apply_rate(ID::DerivedMagDamageInc, ID::ModMagDamageInc, ID::FinalMagDamageInc);
}

double AttributeManager::get(Attr::ID id)
{
    if (_is_dirty)  // 如果脏标记为真则需要先更新
    {
        recalculate();
    }
    return _data[static_cast<int>(id)];
}

void AttributeManager::set_base(Attr::ID id, double value)
{
    _data[static_cast<int>(id)] = value;
    _is_dirty = true;
}

void AttributeManager::update_mod(Attr::ID id, Mod& delta)
{
    int idx = static_cast<int>(id);
    _mods[idx] += delta;
    _is_dirty = true;
}

void AttributeManager::update_rate_mod(Attr::ID id, double delta)
{
    _data[static_cast<int>(id)] += delta;
    _is_dirty = true;
}
