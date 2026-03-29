#include "systems/battle_system.hpp"

bool BattleSystem::check_hit(Creature& attacker, Creature& target, CombatIntent& intent)
{
    using namespace Config::Battle;
    using namespace Global;

    int hit_value = (intent.skill.damage_attribute == DamageType::Physical)
                        ? attacker.get_stats().physical_hit_value
                        : attacker.get_stats().magical_hit_value;
    int evasion_value = target.get_stats().evasion_value;
    int level_diff = attacker.get_level() - target.get_level();

    double attr_factor =
        static_cast<double>(hit_value) / (hit_value + evasion_value * BATTLE_HIT_ATTR_BASELINE + 1);

    double final_rate =
        attr_factor * BATTLE_HIT_BASELINE_HIT + (level_diff * BATTLE_HIT_GRADE_DIFFERENCE_WEIGHT);

    if (final_rate > BATTLE_HIT_UPPER_BOUNDARY) final_rate = BATTLE_HIT_UPPER_BOUNDARY;
    if (final_rate < BATTLE_HIT_BOTTOM_BOUNDARY) final_rate = BATTLE_HIT_BOTTOM_BOUNDARY;

    return get_random_double() < final_rate;
}

bool BattleSystem::check_crit(Creature& attacker, CombatIntent& intent)
{
    using namespace Global;

    double crit_rate;

    if (intent.skill.damage_attribute == DamageType::Physical)
        crit_rate = attacker.get_stats().physical_crit_rate;
    else if (intent.skill.damage_attribute == DamageType::Magical)
        crit_rate = attacker.get_stats().magical_crit_rate;
    else
        crit_rate = attacker.get_stats().true_crit_rate;

    return get_random_double() < crit_rate;
}

int BattleSystem::calc_attack_value(Creature& attacker, CombatIntent& intent, bool is_crit)
{
    int base_attack_value;
    double damage_increase, crit_damage;
    DerivedStats attacker_stats = attacker.get_stats();

    if (intent.skill.damage_attribute == DamageType::Physical)
    {
        base_attack_value = attacker_stats.base_physical_attack_power;
        damage_increase = attacker_stats.physical_damage_increase;
        crit_damage = attacker_stats.physical_crit_damage;
    }
    else if (intent.skill.damage_attribute == DamageType::Magical)
    {
        base_attack_value = attacker_stats.base_magical_attack_power;
        damage_increase = attacker_stats.magical_damage_increase;
        crit_damage = attacker_stats.magical_crit_damage;
    }
    else
    {
        base_attack_value = attacker_stats.base_true_attack_power;
        damage_increase = 0;
        crit_damage = attacker_stats.true_crit_damage;
    }

    int attack_value = (base_attack_value * (1.0 + damage_increase)) *
                       ((is_crit) ? (2.0 * (1.0 + crit_damage)) : 1);

    return attack_value;
}

bool BattleSystem::check_ignore_defense(Creature& attacker, CombatIntent& intent)
{
    using namespace Global;

    return get_random_double() < attacker.get_stats().ignore_defense_rate;
}

bool BattleSystem::check_block(Creature& target, CombatIntent& intent)
{
    using namespace Global;

    return get_random_double() < target.get_stats().block_rate;
}

int BattleSystem::calc_reduction(Creature& target, CombatIntent& intent, int init_damage_value,
                                 bool is_blocked, bool is_ignore_defense)
{
    using namespace Config::Battle;

    DerivedStats target_stats = target.get_stats();

    double cur_damage = static_cast<double>(init_damage_value);
    int defense = target_stats.defense;
    double true_damage_reduction = target_stats.true_damage_reduction;

    if (!is_ignore_defense)
    {
        cur_damage -= defense;
        if (cur_damage < 0) cur_damage = 0;
    }

    if (intent.skill.damage_attribute == DamageType::Physical)
        cur_damage *= (1.0 - target_stats.physical_damage_reduction);
    else if (intent.skill.damage_attribute == DamageType::Magical)
        cur_damage *= (1.0 - target_stats.magical_damage_reduction);

    cur_damage *= (1.0 - true_damage_reduction);

    if (is_blocked) cur_damage *= (1.0 - BATTLE_BLOCK_DAMAGE_REDUCTION_RATIO);

    int final_damage = static_cast<int>(cur_damage);

    return (final_damage < 1) ? 1 : final_damage;
}

AttackResult BattleSystem::execute_attack(Creature& attacker, Creature& target,
                                          CombatIntent& intent)
{
    AttackResult atk_res;

    if (!check_hit(attacker, target, intent))
    {
        atk_res.type = HitResult::Miss;
        atk_res.damage = 0;
        return atk_res;
    }

    bool is_crit = check_crit(attacker, intent);

    int init_damage_value = calc_attack_value(attacker, intent, is_crit);

    bool is_ignore_defense = check_ignore_defense(attacker, intent);

    bool is_blocked = check_block(target, intent);

    int damage_value_after_reduction =
        calc_reduction(target, intent, init_damage_value, is_blocked, is_ignore_defense);

    HitResult hit_res;

    if (is_blocked)
        hit_res = HitResult::Block;
    else if (is_crit)
        hit_res = HitResult::Critical;
    else
        hit_res = HitResult::Hit;

    atk_res = {hit_res, damage_value_after_reduction};

    return atk_res;
}