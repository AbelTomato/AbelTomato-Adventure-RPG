#pragma once

#include <nlohmann/json.hpp>

#include "config/game_config.hpp"
#include "config_cache.hpp"

using json = nlohmann::json;

class ConfigManager
{
public:
    static ConfigManager& instance();

    Config configs;
    CalcCoefficients cache;

    bool load_all_configs(const std::string& path);

    bool sync_cache(const Config& conf);

private:
    ConfigManager() {}
};

#define G_Config ConfigManager::instance().configs
#define G_Cache ConfigManager::instance().cache
