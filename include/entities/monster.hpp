#ifndef MONSTER_HPP
#define MONSTER_HPP

#include <string>

#include "core/creature.hpp"

class Monster : public Creature {
private:
    int exp_reward;

public:
    Monster(std::string n, int h, int ap, double crh, int er);

    void attack(Creature& target);

    void take_damage(int damage, Creature* source) override;

    void death_reaction(Creature* death_source) override;
};

#endif