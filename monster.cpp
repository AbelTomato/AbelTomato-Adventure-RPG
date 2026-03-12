#include "monster.hpp"
#include <iostream>

Monster::Monster(std::string n, int h, int ap, double crh) : Creature(n, h, ap, crh) {}

void Monster::attack(Creature &target)
{
    std::cout << "咕叽！\n";

    if (Creature::critical_hit())
    {
        std::cout << "咕咕!\n";
        target.take_damage(attack_power * 2, this);
    }
    else
        target.take_damage(attack_power, this);
}

void Monster::take_damage(int damage, Creature *source)
{
    std::cout << "叽叽!\n";
    std::cout << name << "受到了" << damage << "点伤害！\n";

    hp -= damage;
}

void Monster::death_reaction(Creature *source)
{
    std::cout << "咕叽咕叽...\n";
}