#ifndef GAME_CONFIG_HPP
#define GAME_CONFIG_HPP

#include <definitions/core_attributes.hpp>
#include <map>
#include <nlohmann/json.hpp>
#include <stdexcept>

struct Config
{
    std::map<std::string, std::map<std::string, double>> settings;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Config, settings);

    // category为类型名，key为键名
    double get(const std::string& category, const std::string& key) const
    {
        try
        {
            return settings.at(category).at(key);
        }
        catch (const std::out_of_range& e)
        {
            throw std::runtime_error("Config Error: Cannot find key '" + key + "' in category '" +
                                     category);
        }
    }
};

#endif