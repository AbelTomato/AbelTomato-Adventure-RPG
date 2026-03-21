#ifndef GAME_CONSTANTS_HPP
#define GAME_CONSTANTS_HPP

// TODO:挪到json里

namespace Config // TODO:增加暴击率等上限参数
{
    namespace Attr
    { // 属性换算
        static constexpr int ATTR_HP_PER_STRENGTH = 8;
        static constexpr int ATTR_HP_PER_PHYSIQUE = 15;
        static constexpr int ATTR_HP_PER_WILLPOWER = 3;

        static constexpr int ATTR_MP_PER_ESTHESIA = 3;
        static constexpr int ATTR_MP_PER_BEWITCHMENT = 12;
        static constexpr int ATTR_MP_PER_WILLPOWER = 7;

        static constexpr double ATTR_SP_PER_STRENGTH = 0.3;
        static constexpr double ATTR_SP_PER_PHYSIQUE = 0.5;
        static constexpr double ATTR_SP_PER_WILLPOWER = 0.2;

        static constexpr int ATTR_EVASION_PER_DEXTERITY = 2;
        static constexpr int ATTR_EVASION_PER_ESTHESIA = 1;
        static constexpr int ATTR_EVASION_PER_SPEED = 3;
        static constexpr int ATTR_EVASION_PER_LUCK = 5;

        static constexpr int ATTR_PHYSICAL_HIT_PER_DEXTERITY = 1;
        static constexpr int ATTR_PHYSICAL_HIT_PER_ESTHESIA = 2;
        static constexpr int ATTR_PHYSICAL_HIT_PER_SPEED = 2;
        static constexpr int ATTR_PHYSICAL_HIT_PER_LUCK = 3;

        static constexpr int ATTR_MAGICAL_HIT_PER_ESTHESIA = 1;
        static constexpr int ATTR_MAGICAL_HIT_PER_BEWITCHMENT = 2;
        static constexpr int ATTR_MAGICAL_HIT_PER_LUCK = 2;

        static constexpr double ATTR_SPEED_PER_DEXTERITY = 0.2;

        static constexpr double ATTR_DEFENSE_PER_PHYSIQUE = 3;

        static constexpr double ATTR_IGNORE_DEFENSE_RATE_PER_ESTHESIA = 0.0007;
        static constexpr double ATTR_IGNORE_DEFENSE_RATE_PER_LUCK = 0.001;

        static constexpr double ATTR_PHYSICAL_CRIT_RATE_PER_ESTHESIA = 0.0002;
        static constexpr double ATTR_PHYSICAL_CRIT_RATE_PER_LUCK = 0.0005;

        static constexpr double ATTR_MAGICAL_CRIT_RATE_PER_ESTHESIA = 0.0001;
        static constexpr double ATTR_MAGICAL_CRIT_RATE_PER_LUCK = 0.0003;
    }

    namespace Growth
    { // 成长与级别
        static constexpr int GROWTH_HP_PER_LIFE_GROWTH_PER_LEVEL = 5;
        static constexpr int GROWTH_MP_PER_MAGIC_GROWTH_PER_LEVEL = 4;

        static constexpr int GROWTH_FREE_ATTRIBUTE_PER_LEVEL = 10;
    }

    namespace Battle
    { // 战斗机制
        static constexpr int BATTLE_BASE_PHYSICAL_ATTACK_POWER_PER_STRENGTH = 5;
        static constexpr int BATTLE_BASE_PHYSICAL_ATTACK_POWER_PER_PHYSIQUE = 1;

        static constexpr int BATTLE_BASE_MAGICAL_ATTACK_POWER_PER_BEWITCHMENT = 6;
        static constexpr int BATTLE_BASE_MAGICAL_ATTACK_POWER_PER_WILLPOWER = 1;

        static constexpr int BATTLE_BASE_TRUE_ATTACK_POWER_PER_STRENGTH = 1;
        static constexpr int BATTLE_BASE_TRUE_ATTACK_POWER_PER_BEWITCHMENT = 1;

        static constexpr double BATTLE_HIT_ATTR_BASELINE = 0.8;
        static constexpr double BATTLE_HIT_GRADE_DIFFERENCE_WEIGHT = 0.05;
        static constexpr double BATTLE_HIT_BASELINE_HIT = 0.85;
        static constexpr double BATTLE_HIT_UPPER_BOUNDARY = 0.98;
        static constexpr double BATTLE_HIT_BOTTOM_BOUNDARY = 0.05;

        static constexpr double BATTLE_BLOCK_DAMAGE_REDUCTION_RATIO = 0.7;
    }

    namespace Initial
    { // 初始状态

    }
}

#endif