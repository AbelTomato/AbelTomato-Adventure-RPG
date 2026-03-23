#include "systems/load_datas.hpp"

#include <fstream>
#include <iostream>
#include <optional>

#include "datas/race_data.hpp"
#include "datas/skill_data.hpp"

DataManager& DataManager::instance()
{
    static DataManager inst;
    return inst;
}

bool DataManager::load_all_races_datas(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "无法打开生物初始属性文件：" << path << std::endl;
        return false;
    }

    try
    {
        json j;
        file >> j;

        auto race_list = j.get<std::vector<RaceData>>();

        for (auto& r : race_list)
        {
            int id = r.get_id();
            std::string name = r.get_name();

            races_id.emplace(id, r);
            races_name.emplace(name, std::move(r));
        }
    }
    catch (json::parse_error& e)
    {
        std::cerr << "JSON 解析失败：" << e.what() << std::endl;
        return false;
    }

    std::cout << "生物初始属性加载成功！" << std::endl;
    return true;
}

bool DataManager::load_all_skills_datas(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "无法打开技能数据文件：" << path << std::endl;
        return false;
    }

    try
    {
        json j;
        file >> j;

        auto skill_list = j.get<std::vector<SkillData>>();

        for (auto& s : skill_list)
        {
            int id = s.get_id();
            std::string name = s.get_name();

            skills_id.emplace(id, s);
            skills_name.emplace(name, std::move(s));
        }
    }
    catch (json::parse_error& e)
    {
        std::cerr << "JSON 解析失败：" << e.what() << std::endl;
        return false;
    }

    std::cout << "技能数据加载成功！" << std::endl;
    return true;
}

std::optional<RaceData> DataManager::get_race_by_id(const int id)
{
    auto it = races_id.find(id);
    if (it != races_id.end()) return it->second;

    return std::nullopt;
}

std::optional<RaceData> DataManager::get_race_by_name(const std::string& name)
{
    auto it = races_name.find(name);
    if (it != races_name.end()) return it->second;

    return std::nullopt;
}

std::optional<SkillData> DataManager::get_skill_by_id(const int id)
{
    auto it = skills_id.find(id);
    if (it != skills_id.end()) return it->second;

    return std::nullopt;
}

std::optional<SkillData> DataManager::get_skill_by_name(const std::string& name)
{
    auto it = skills_name.find(name);
    if (it != skills_name.end()) return it->second;

    return std::nullopt;
}

const RaceData* DataManager::get_race_ptr_by_id(const int id)
{
    auto it = races_id.find(id);
    if (it != races_id.end()) return &(it->second);

    return nullptr;
}

const RaceData* DataManager::get_race_ptr_by_name(const std::string& name)
{
    auto it = races_name.find(name);
    if (it != races_name.end()) return &(it->second);

    return nullptr;
}

const SkillData* DataManager::get_skill_ptr_by_id(const int id)
{
    auto it = skills_id.find(id);
    if (it != skills_id.end()) return &(it->second);

    return nullptr;
}

const SkillData* DataManager::get_skill_ptr_by_name(const std::string& name)
{
    auto it = skills_name.find(name);
    if (it != skills_name.end()) return &(it->second);

    return nullptr;
}