#ifndef CREATURE_HPP
#define CREATURE_HPP

#include <string>

class Creature
{
protected:
    int hp, attack_power;
    double critical_hit_rate;
    std::string name;

public:
    Creature(std::string n, int h, int ap, double chr);

    virtual ~Creature() = default;

    virtual void attack(Creature &target);

    virtual void take_damage(int damage, Creature *source);

    bool is_alive();

    virtual void death_reaction(Creature *death_source);

    bool critical_hit();

    const std::string &get_name() const;

    const int &get_hp() const;
};

#endif