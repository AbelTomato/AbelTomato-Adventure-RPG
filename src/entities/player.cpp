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

void Player::death_reaction(Creature *death_source)
{
    std::cout << "你死了，屈辱地死在了" << death_source->get_name() << "的手下\n";
}
