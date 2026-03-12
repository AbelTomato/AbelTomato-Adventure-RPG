#include "player.hpp"
#include <iostream>

Player::Player(std::string n, int h, int ap, int e, int l, double chr) : Creature(n, h, ap, chr), exp(e), level(l) {}

void Player::gain_exp(int amount)
{
    exp += amount;

    if (exp >= 100)
    {
        exp %= 100;
        level_up();
    }
}

void Player::attack(Creature &target)
{
    std::cout << name << "挥舞拳头，发起了攻击！\n";

    if (Creature::critical_hit())
    {
        std::cout << "我拳也未尝不利！\n";
        target.take_damage(attack_power * 2, this);
    }
    else
        target.take_damage(attack_power, this);
}

void Player::take_damage(int damage, Creature *source)
{
    std::cout << "呃啊！\n";
    std::cout << name << "受到了" << damage << "点伤害！\n";

    hp -= damage;
}

void Player::level_up()
{
    std::cout << "金光泻落，你升级了！\n";
    level++;
    std::cout << "当前等级Lv." << level << "\n";
}

void Player::death_reaction(Creature *death_source)
{
    std::cout << "你死了，屈辱地死在了" << death_source->get_name() << "的手下\n";
}
