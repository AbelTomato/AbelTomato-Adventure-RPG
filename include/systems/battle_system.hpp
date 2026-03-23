#ifndef BATTLE_SYSTEM_HPP
#define BATTLE_SYSTEM_HPP

#include "config/game_constants.hpp"
#include "core/creature.hpp"
#include "core/global.hpp"


class BattleSystem
{
private:
    static bool check_hit(Creature& attacker, Creature& target, CombatIntent& intent);

    static bool check_crit(Creature& attacker, CombatIntent& intent);

    static int calc_attack_value(Creature& attacker, CombatIntent& intent, bool is_crit);

    static bool check_ignore_defense(Creature& attacker, CombatIntent& intent);

    static bool check_block(Creature& target, CombatIntent& intent);

    static int calc_reduction(Creature& target, CombatIntent& intent, int init_damage_value,
                              bool is_blocked, bool is_ignore_defense);

public:
    static AttackResult execute_attack(Creature& attacker, Creature& target, CombatIntent& intent);
};

#endif