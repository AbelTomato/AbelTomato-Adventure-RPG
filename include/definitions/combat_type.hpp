#ifndef COMBAT_TYPE_HPP
#define COMBAT_TYPE_HPP

#include <nlohmann/json.hpp>

class Creature;

enum class battle_result
{
    VICTORY,
    DEFEAT,
    ESCAPED
};

enum class HitResult  // 攻击结果类型
{
    Miss,      // 闪避
    Hit,       // 命中
    Critical,  // 暴击
    Block      // 格挡
};

#endif