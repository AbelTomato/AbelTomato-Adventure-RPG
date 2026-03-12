#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "creature.hpp"

#include <string>

class Player : public Creature
{
protected:
    int exp;
    int level;

public:
    Player(std::string n, int h, int ap, int e, int l, double chr);

    void attack(Creature &target) override;

    void take_damage(int damage, Creature *source) override;

    void gain_exp(int amount);

    bool is_exp_full();

    void level_up();

    void death_reaction(Creature *death_source) override;
};

#endif