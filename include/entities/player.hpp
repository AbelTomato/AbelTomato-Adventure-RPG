#ifndef PLAYER_HPP
#define PLAYER_HPP
#include <string>

#include "core/creature.hpp"


class Player : public Creature {
protected:
    int exp;
    int level;
    int exp_to_next_level;

public:
    Player(std::string n, int h, int ap, int e, int l, double chr);

    void attack(Creature& target);

    void take_damage(int damage, Creature* source);

    void gain_exp(int amount);

    bool is_exp_full();

    void level_up();

    void death_reaction(Creature* death_source) override;

    int get_exp() const {
        return exp;
    }

    int get_exp_to_next_level() const {
        return exp_to_next_level;
    }
};

#endif