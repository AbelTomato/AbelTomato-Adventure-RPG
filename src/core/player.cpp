#include "player.hpp"
#include <iostream>

Player::Player(std::string n, int h, int ap, int e, int l, double chr) : Creature(n, h, ap, chr), exp(e), level(l) {}

void Player::gain_exp(int amount) // TODO:完善经验获取逻辑
{
    int cur_exp = get_exp();
    int cur_to_next_level_exp = get_exp_to_next_level();
    int diff_exp = cur_to_next_level_exp - cur_exp;

    while (amount >= diff_exp)
    {
        amount -= diff_exp;
        level_up();
        diff_exp = get_exp_to_next_level();
    }

    cur_exp += amount;
}

void Player::level_up() // TODO:完善升级逻辑
{
    std::cout << "金光泻落，你升级了！\n";
    level++;
    std::cout << "当前等级Lv." << level << "\n";
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

void Player::death_reaction(Creature *death_source)
{
    std::cout << "你死了，屈辱地死在了" << death_source->get_name() << "的手下\n";
}
