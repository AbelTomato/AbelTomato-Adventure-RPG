#include "skill_definitions.hpp"

CombatIntent NormalAttack::use(Creature &caster)
{
    return {attack_attribute_type,
            damage_multifier,
            fixed_bonus,
            effects};
}