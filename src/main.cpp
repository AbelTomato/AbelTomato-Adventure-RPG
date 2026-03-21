#include <windows.h>

#include <iostream>
#include <limits>
#include <string>

#include "core/global.hpp"
#include "entities/monster.hpp"
#include "entities/player.hpp"
#include "systems/skill_manager.hpp"

// TODO:设计一个UI交互类

/*
battle_result start_battle(Creature &object1, Creature &object2) // TODO:完善战斗逻辑
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
*/

int main() {
    SetConsoleOutputCP(65001);

    SkillManager manager;

    // 这里的路径是相对于你执行编译后 game.exe 运行的位置
    if (manager.load_skills("../data/skills.json")) {
        std::cout << "--- 技能书加载成功 ---" << std::endl;

        // 测试读取 ID 为 2 的火球术
        SkillData fire = manager.get_skill(2);
        if (fire.id != 0) {
            std::cout << "读取到技能：" << fire.skill_name << std::endl;
            std::cout << "描述：" << fire.description << std::endl;
            if (!fire.damages.empty()) {
                std::cout << "首段伤害倍率：" << fire.damages[0].multiplier << std::endl;
            }
        } else {
            std::cout << "未找到该技能，请检查 JSON 里的 ID！" << std::endl;
        }
    } else {
        std::cout << "技能书加载失败，请检查路径或 JSON 格式！" << std::endl;
    }

    std::cin.get();

    return 0;
}