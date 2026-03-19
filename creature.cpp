#include "creature.hpp"
#include "stat_calculator.hpp"
#include <iostream>
#include <random>

Creature::Creature(std::string n, int lvl, int fap, const Attributes &a)
    : name(n), level(lvl), free_attribute_points(fap), attrs(a)
{
    refresh_stats();

    current_hp = stats.max_hp;
    current_mp = stats.max_mp;
    current_sp = stats.max_sp;
}

void Creature::refresh_stats() // TODO: 完善更新逻辑，增加依赖树，添加脏标记处理
{
    int old_max_hp = stats.max_hp, old_max_mp = stats.max_mp, old_max_sp = stats.max_sp;

    reset_and_collect_bonuses(); // 重置并重新计算所有加成

    calculate_derived_values(); // 按照现有加成重新计算二级属性

    sync_current_status(old_max_hp, old_max_mp, old_max_sp); // 属性对齐

    is_stats_dirty = false;
}

void Creature::reset_and_collect_bonuses() // TODO:完善装备、buff、负重加成
{
    bonus.clear(); // 清空原有加成

    // TODO:枚举装备和buff列表等等
}

void Creature::calculate_derived_values()
{
    const FinalAttributes f_attrs = StatCalculator::make_final_attrs(attrs, bonus);

    stats.max_hp = StatCalculator::calc_max_hp(f_attrs, level, bonus, races.init_hp);
    stats.max_mp = StatCalculator::calc_max_mp(f_attrs, level, bonus, races.init_mp);
    stats.max_sp = StatCalculator::calc_max_sp(f_attrs, bonus);
    stats.base_physical_attack_power = StatCalculator::calc_base_physical_attack_power(f_attrs, bonus);
    stats.base_magical_attack_power = StatCalculator::calc_base_magical_attack_power(f_attrs, bonus);
    stats.block_rate = StatCalculator::calc_block_rate(bonus);
    stats.evasion_value = StatCalculator::calc_evasion_value(f_attrs, bonus, races.init_evasion_value);
    stats.hit_value = StatCalculator::calc_hit_value(f_attrs, bonus, races.init_hit_value);
    stats.physical_crit_rate = StatCalculator::calc_physical_crit_rate(f_attrs, bonus, races.init_physical_crit_rate);
    stats.magical_crit_rate = StatCalculator::calc_magical_crit_rate(f_attrs, bonus, races.init_magical_crit_rate);
    stats.defense = StatCalculator::calc_defense(f_attrs, bonus);
    stats.physical_damage_reduction = StatCalculator::calc_physical_damage_reduction(bonus, races.init_physical_damage_reduction);
    stats.magical_damage_reduction = StatCalculator::calc_magical_damage_reduction(bonus, races.init_magical_damage_reduction);
    stats.ignore_defense_rate = StatCalculator::calc_ignore_defense_rate(f_attrs, bonus, races.init_ignore_defense_rate);
}

void Creature::sync_current_status(const int old_max_hp, const int old_max_mp, const int old_max_sp)
{
    auto calc_cur_stats_res = [](const int old_max, const int cur, const int cur_max) -> int
    {
        return cur * cur_max / old_max;
    };

    current_hp = calc_cur_stats_res(old_max_hp, current_hp, stats.max_hp);
    current_mp = calc_cur_stats_res(old_max_mp, current_mp, stats.max_mp);
    current_sp = calc_cur_stats_res(old_max_sp, current_sp, stats.max_sp);
} // TODO:完成状态维护

void Creature::attack(Creature &target) // TODO:重写攻击逻辑
{
    std::cout << name << "发起了攻击！\n";

    if (critical_hit())
    {
        std::cout << name << "发现了对方弱点，触发了会心一击！\n";
        target.take_damage(attack_power * 2, this);
    }
    else
        target.take_damage(attack_power, this);
}

void Creature::take_damage(int damage, Creature *source) // TODO:重写受伤逻辑
{
    hp -= damage;
}

bool Creature::is_alive()
{
    return hp > 0;
}

bool Creature::critical_hit() // TODO:重写暴击逻辑
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::bernoulli_distribution d(critical_hit_rate);

    return d(gen);
}

const std::string &Creature::get_name() const
{
    return name;
}

const int &Creature::get_hp() const
{
    return hp;
}

void Creature::death_reaction(Creature *death_source) // TODO:重写死亡逻辑
{
    std::cout << name << "死了！死因：" << death_source->get_name() << "\n";
}