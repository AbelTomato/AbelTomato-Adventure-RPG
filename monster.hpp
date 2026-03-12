#ifndef MONSTER_HPP
#define MONSTER_HPP

#include "creature.hpp"
#include <string>

class Monster : public Creature
{
public:
    Monster(std::string n, int h, int ap, double crh);

    void attack(Creature &target) override;

    void take_damage(int damage, Creature *source) override;

    void death_reaction(Creature *death_source) override;
};

#endif