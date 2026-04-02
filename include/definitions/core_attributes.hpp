#pragma once

#include <nlohmann/json.hpp>

struct Attributes
{                     // 基本属性
    int strength;     // 力量
    int physique;     // 体质
    int dexterity;    // 灵巧
    int esthesia;     // 感知
    int bewitchment;  // 魔力
    int willpower;    // 意志

    int life_growth;   // 生命成长
    int magic_growth;  // 法力成长
    int speed;         // 速度
    int luck;          // 幸运

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Attributes, strength, physique, dexterity, esthesia, bewitchment,
                                   willpower, life_growth, magic_growth, speed, luck);
};

using FinalAttributes = Attributes;

struct Mod
{
    double pct = 0.0;
    int flat = 0;

    void clear()
    {
        pct = 0.0;
        flat = 0;
    }

    Mod& operator+=(const Mod& other)
    {
        pct += other.pct;
        flat += other.flat;

        return *this;
    }

    Mod operator+(const Mod& other)
    {
        return {this->pct + other.pct, this->flat + other.flat};
    }
};

#endif