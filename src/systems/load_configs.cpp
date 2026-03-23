#include "systems/load_configs.hpp"

#include <fstream>
#include <iostream>

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
    catch (json::parse_error& e)
    {
        std::cerr << "JSON 解析失败：" << e.what() << std::endl;
        return false;
    }

    std::cout << "全局配置加载成功！" << std::endl;
    return true;
}