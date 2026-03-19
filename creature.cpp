#include "creature.hpp"
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
    using namespace Config::Attr;
    using namespace Config::Growth;
    using namespace Config::Battle;

    int old_max_hp = stats.max_hp;
    double base_hp = (attrs.strength * ATTR_HP_PER_STRENGTH) +
                     (attrs.physique * ATTR_HP_PER_PHYSIQUE) +
                     (attrs.willpower * ATTR_HP_PER_WILLPOWER);
    base_hp += (attrs.life_growth * level * GROWTH_HP_PER_LIFE_GROWTH_PER_LEVEL);
    stats.max_hp = static_cast<int>(base_hp * (1 + stats.hp_percent_bonus) + stats.hp_flat_bonus);

    if (old_max_hp > 0)
    {
        current_hp = (current_hp / old_max_hp) * stats.max_hp;
    }
    if (current_hp > stats.max_hp)
    {
        current_hp = stats.max_hp;
    }

    int old_max_mp = stats.max_mp;
    double base_mp = (attrs.bewitchment * ATTR_MP_PER_BEWITCHMENT) +
                     (attrs.esthesia * ATTR_MP_PER_ESTHESIA) +
                     (attrs.willpower * ATTR_MP_PER_WILLPOWER);
    base_mp += (attrs.magic_growth * level * GROWTH_MP_PER_MAGIC_GROWTH_PER_LEVEL);
    stats.max_mp = static_cast<int>(base_mp * (1 + stats.hp_percent_bonus) + stats.mp_flat_bonus);

    if (old_max_mp > 0)
    {
        current_mp = (current_mp / old_max_mp) * stats.max_mp;
    }
    if (current_mp > stats.max_mp)
    {
        current_mp = stats.max_mp;
    }
}

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