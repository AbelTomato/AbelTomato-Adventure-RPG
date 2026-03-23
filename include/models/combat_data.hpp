#ifndef COMBAT_DATA_HPP
#define COMBAT_DATA_HPP

class Creature;
#include <definitions/combat_type.hpp>

struct AttackResult
{
    HitResult type;
    int damage;
};

struct CombatIntent
{
    int skill_id;  // 只需存一个技能id，以进行技能索引
    Creature* attacker = nullptr;
    Creature* target = nullptr;
};

#endif