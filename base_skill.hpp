#ifndef BASE_SKILL_HPP
#define BASE_SKILL_HPP

#include "definitions.hpp"
#include "creature.hpp"

class BaseSkill
{
public:
    virtual bool can_use(const Creature &caster) const = 0;

    virtual CombatIntent execute(Creature &caster);

    virtual ~BaseSkill() = default;
};

#endif