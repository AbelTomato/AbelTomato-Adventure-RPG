#include "systems/load_configs.hpp"

#include <exception>
#include <fstream>
#include <iostream>

#include "config/game_config.hpp"

ConfigManager& ConfigManager::instance()
{
    static ConfigManager inst;
    return inst;
}

bool ConfigManager::load_all_configs(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "无法打开配置文件：" << path << std::endl;
        return false;
    }

    try
    {
        json j;
        file >> j;
        configs = j.get<Config>();
    }
    catch (const nlohmann::json::exception& e)
    {
        std::cerr << "[Config Error] 加载失败: " << e.what() << std::endl;
        return false;
    }
    catch (const std::exception& e)
    {
        std::cerr << "[Unknown Error] 发生了意料之外的崩溃: " << e.what() << std::endl;
        return false;
    }

    std::cout << "全局配置加载成功！" << std::endl;
    return true;
}

bool ConfigManager::sync_cache(const Config& conf)
{
    try
    {
        // 3.2.1.1. BaseMaxHP 相关参数
        cache.HPPerStrength = conf.get("attr", "HPPerStrength");
        cache.HPPerPhysique = conf.get("attr", "HPPerPhysique");
        cache.HPPerWillpower = conf.get("attr", "HPPerWillpower");

        // 3.2.1.2. BaseMaxMP 相关参数
        cache.MPPerEsthesia = conf.get("attr", "MPPerEsthesia");
        cache.MPPerBewitchment = conf.get("attr", "MPPerBewitchment");
        cache.MPPerWillpower = conf.get("attr", "MPPerWillpower");

        // 3.2.1.3. BaseMaxSP 相关参数
        cache.SPPerStrength = conf.get("attr", "SPPerStrength");
        cache.SPPerPhysique = conf.get("attr", "SPPerPhysique");
        cache.SPPerWillpower = conf.get("attr", "SPPerWillpower");

        // 3.2.1.4. Evasion 相关参数
        cache.EvasionPerDexterity = conf.get("attr", "EvasionPerDexterity");
        cache.EvasionPerEsthesia = conf.get("attr", "EvasionPerEsthesia");
        cache.EvasionPerSpeed = conf.get("attr", "EvasionPerSpeed");
        cache.EvasionPerLuck = conf.get("attr", "EvasionPerLuck");

        // 3.2.1.5 & 3.2.1.6. Hit 相关参数
        cache.PhysicalHitPerDexterity = conf.get("attr", "PhysicalHitPerDexterity");
        cache.PhysicalHitPerEsthesia = conf.get("attr", "PhysicalHitPerEsthesia");
        cache.PhysicalHitPerSpeed = conf.get("attr", "PhysicalHitPerSpeed");
        cache.PhysicalHitPerLuck = conf.get("attr", "PhysicalHitPerLuck");
        cache.MagicalHitPerEsthesia = conf.get("attr", "MagicalHitPerEsthesia");
        cache.MagicalHitPerBewitchment = conf.get("attr", "MagicalHitPerBewitchment");
        cache.MagicalHitPerLuck = conf.get("attr", "MagicalHitPerLuck");

        // 3.2.1.7 & 3.2.1.8. Defense 相关参数
        cache.PhysicalDefensePerPhysique = conf.get("attr", "PhysicalDefensePerPhysique");
        cache.MagicalDefensePerPhysique = conf.get("attr", "MagicalDefensePerPhysique");
        cache.MagicalDefensePerBewitchment = conf.get("attr", "MagicalDefensePerBewitchment");

        // 3.2.1.9 & 3.2.1.10. Ignore Defense Rate 相关参数
        cache.IgnorePhysicalDefenseRatePerStrength =
            conf.get("attr", "IgnorePhysicalDefenseRatePerStrength");
        cache.IgnorePhysicalDefenseRatePerEsthesia =
            conf.get("attr", "IgnorePhysicalDefenseRatePerEsthesia");
        cache.IgnorePhysicalDefenseRatePerLuck =
            conf.get("attr", "IgnorePhysicalDefenseRatePerLuck");
        cache.IgnoreMagicalDefenseRatePerBewitchment =
            conf.get("attr", "IgnoreMagicalDefenseRatePerBewitchment");
        cache.IgnoreMagicalDefenseRatePerEsthesia =
            conf.get("attr", "IgnoreMagicalDefenseRatePerEsthesia");
        cache.IgnoreMagicalDefenseRatePerLuck = conf.get("attr", "IgnoreMagicalDefenseRatePerLuck");

        // 3.2.1.11 & 3.2.1.12. Crit Rate 相关参数
        cache.PhysicalCritRatePerEsthesia = conf.get("attr", "PhysicalCritRatePerEsthesia");
        cache.PhysicalCritRatePerLuck = conf.get("attr", "PhysicalCritRatePerLuck");
        cache.MagicalCritRatePerEsthesia = conf.get("attr", "MagicalCritRatePerEsthesia");
        cache.MagicalCritRatePerLuck = conf.get("attr", "MagicalCritRatePerLuck");

        // 3.2.1.13 & 3.2.1.14. Growth 相关参数
        cache.HPPerLifeGrowthPerLevel = conf.get("attr", "HPPerLifeGrowthPerLevel");
        cache.MPPerMagicGrowthPerLevel = conf.get("attr", "MPPerMagicGrowthPerLevel");

        // 3.2.1.15 & 3.2.1.16. Attack Power 相关参数
        cache.BasePhysicalAttackPowerPerStrength =
            conf.get("attr", "BasePhysicalAttackPowerPerStrength");
        cache.BasePhysicalAttackPowerPerPhysique =
            conf.get("attr", "BasePhysicalAttackPowerPerPhysique");
        cache.BaseMagicalAttackPowerPerBewitchment =
            conf.get("attr", "BaseMagicalAttackPowerPerBewitchment");
        cache.BaseMagicalAttackPowerPerWillpower =
            conf.get("attr", "BaseMagicalAttackPowerPerWillpower");
    }
    catch (const std::exception& e)
    {
        std::cerr << "[Unknown Error]: 参数加载失败" << e.what() << std::endl;
        return false;
    }

    std::cout << "缓存加载成功！" << std::endl;
    return true;
}