#ifndef CREATURE_HPP
#define CREATURE_HPP

#include <string>
#include "definitions.hpp"

class Creature
{
private:
    bool is_stats_dirty = true;

    void refresh_stats();

protected:
    Attributes attrs;
    DerivedStats stats;
    BonusStats bonus;
    RaceData races;
    int level;
    int current_hp, current_mp, current_sp;
    int free_attribute_points;
    std::string name;

public:
    Creature(std::string n, int lvl, int fap, const Attributes &a);

    virtual ~Creature() = default;

    const DerivedStats &get_stats()
    {
        if (is_stats_dirty)
            refresh_stats();
        return stats;
    }

    void reset_and_collect_bonuses();

    void calculate_derived_values();

    void sync_current_status(const int old_max_hp, const int old_max_mp, const int old_max_sp);

    virtual AttackResult attack(Creature &target, CombatIntent &intent);

    virtual void take_damage(int damage, Creature *source);

    bool is_alive();

    virtual void death_reaction(Creature *death_source);

    const std::string &get_name() const;

    const int &get_hp() const;

    const int &get_mp() const;

    const int &get_sp() const;

    const int &get_level() const;
};

#endif