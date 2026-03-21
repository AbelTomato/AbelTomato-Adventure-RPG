#ifndef SKILL_DEFINITIONS_HPP
#define SKILL_DEFINITIONS_HPP

#include "base_skill.hpp"

class NormalAttack : public BaseSkill
{
private:
    DamageType attack_attribute_type = DamageType::Physical;
    double damage_multifier = 1.0;
    int fixed_bonus = 0;
    SpecialEffects effects = {};

public:
    CombatIntent use(Creature &caster) override;
};

#endif