#pragma once

#include <definitions/core_attributes.hpp>
#include <exception>
#include <map>
#include <nlohmann/json.hpp>
#include <stdexcept>

struct Config
{
    std::map<std::string, std::map<std::string, double>> settings;  // 从配置参数名到值的映射

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
            throw std::runtime_error("[Config Error]: Key '" + key + "' not found in category '" +
                                     category + "'.");
        }
        catch (const std::exception& e)
        {
            throw std::runtime_error(std::string("[Internal Error]: '") + e.what() + "'.");
        }
        catch (...)
        {
            throw std::runtime_error("[Unknown Error]: Catched in Config access!");
        }
    }
};
