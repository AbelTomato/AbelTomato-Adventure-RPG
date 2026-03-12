#include "creature.hpp"
#include <iostream>
#include <random>

Creature ::Creature(std::string n, int h, int ap, double chr) : name(n), hp(h), attack_power(ap), critical_hit_rate(chr) {}

void Creature::attack(Creature &target)
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

void Creature::take_damage(int damage, Creature *source)
{
    hp -= damage;
}

bool Creature::is_alive()
{
    return hp > 0;
}

bool Creature::critical_hit()
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

void Creature::death_reaction(Creature *death_source)
{
    std::cout << name << "死了！死因：" << death_source->get_name() << "\n";
}