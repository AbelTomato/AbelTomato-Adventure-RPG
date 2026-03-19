#ifndef CREATURE_HPP
#define CREATURE_HPP

#include <string>
#include "definitions.hpp"

class Creature
{
protected:
    Attributes attrs;
    DerivedStats stats;
    int level;
    int current_hp, current_mp, current_sp;
    int free_attribute_points;
    std::string name;

public:
    Creature(std::string n, int lvl, int fap, const Attributes &a);

    virtual ~Creature() = default;

    virtual void refresh_stats();

    void set_attributes(const Attributes &new_attrs);

    virtual void attack(Creature &target);

    virtual void take_damage(int damage, Creature *source);

    bool is_alive();

    virtual void death_reaction(Creature *death_source);

    bool critical_hit();

    const std::string &get_name() const;

    const int &get_hp() const;
};

#endif