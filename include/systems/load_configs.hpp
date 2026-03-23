#ifndef LOAD_CONFIGS_HPP
#define LOAD_CONFIGS_HPP

#include <nlohmann/json.hpp>

#include "config/game_config.hpp"

using json = nlohmann::json;

class ConfigManager
{
public:
    static ConfigManager& instance();

    Config configs;

    bool load_all_configs(const std::string& path);

private:
    ConfigManager() {}
};

#define G_Config ConfigManager::instance().configs

#endif