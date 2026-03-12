#include <iostream>
#include <string>
#include <limits>
#include <windows.h>
#include "player.hpp"
#include "monster.hpp"
#include "global.hpp"

battle_result start_battle(Creature &object1, Creature &object2)
{
    std::cout << "\n===遭遇战：" << object2.get_name() << "出现了！===\n";

    while (object1.is_alive() && object2.is_alive())
    {
        std::cout << "\n[" << object1.get_name() << "HP:" << object1.get_hp() << "] vs "
                  << "[" << object2.get_name() << "HP:" << object2.get_hp() << "]\n";

        std::cout << "1.攻击 2.逃跑\n请选择：";
        int choice;
        std::cin >> choice;

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        if (choice == 1)
            object1.attack(object2);
        else
            std::cout << "你试图逃跑，但腿软了没跑掉！\n";

        if (!object2.is_alive())
        {
            object2.death_reaction(&object1);
            std::cout << object2.get_name() << "倒下了！战斗胜利！\n";

            std::cout << "\n[按回车键结束战斗...]";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();

            return battle_result::VICTORY;
        }

        object2.attack(object1);

        if (!object1.is_alive())
        {
            object1.death_reaction(&object2);
            std::cout << "你失败了\n";

            std::cout << "\n[按回车键结束战斗...]";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();

            return battle_result::DEFEAT;
        }

        std::cout << "\n--- 按回车键进入下一回合 ---";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();

        system("cls");
    }
}

int main()
{
    SetConsoleOutputCP(65001);

    std::cout << "--- 欢迎加入番茄勇者Abel的旅程 ---" << std::endl;

    Player hero("Abel", 120, 15, 0, 1, 0.2);
    Monster slime("史莱姆", 100, 10, 0.15);

    start_battle(hero, slime);

    return 0;
}