#ifndef RACE_DATA_HPP
#define RACE_DATA_HPP

#include <string>

#include "config/game_config.hpp"

class RaceData
{
private:
    int id;
    std::string race_name;
    Attributes base_attr;
    int init_hp, init_mp, init_sp;
    int init_evasion_value, init_physical_hit_value, init_magical_hit_value;
    double init_physical_crit_rate, init_magical_crit_rate, init_true_crit_rate;
    double init_physical_damage_reduction, init_magical_damage_reduction,
        init_true_damage_reduction;
    double init_ignore_defense_rate;

public:
    int get_id()
    {
        return id;
    }

    const std::string get_name()
    {
        return race_name;
    }

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(RaceData, id, race_name, base_attr, init_hp, init_mp, init_sp,
                                   init_evasion_value, init_physical_hit_value,
                                   init_magical_hit_value, init_physical_crit_rate,
                                   init_magical_crit_rate, init_true_crit_rate,
                                   init_physical_damage_reduction, init_magical_damage_reduction,
                                   init_true_damage_reduction, init_ignore_defense_rate);
};

#endif