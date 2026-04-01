#include "systems/data_manager.hpp"

#include <exception>
#include <fstream>
#include <iostream>

#include "definitions/error_type.hpp"

DataManager& DataManager::instance()
{
    static DataManager inst;
    return inst;
}

template <typename T>
void load_file(DataRepository<T>& repo, const std::string& path, const std::string& data_type_name)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        throw ErrorCode::FileNotFound;
    }

    json j;

    try
    {
        file >> j;
    }
    catch (const std::exception& e)
    {
        throw ErrorCode::JsonParseError;
    }

    repo.load(j);

    std::cout << "[Success]: " << data_type_name << " 加载成功 (" << path << ")" << std::endl;
}

void DataManager::init(const std::string& base_path)
{
    this->root_path = base_path;
}

bool DataManager::load_all_data()
{
    try
    {
        load_all_races_data(root_path + "/race.json");
        load_all_skills_data(root_path + "/skill.json");
        load_all_effects_data(root_path + "/effect.json");
        load_all_buffs_data(root_path + "/buff.json");
        load_all_jobs_data(root_path + "/job.json");
        load_all_weapons_data(root_path + "/weapon.json");
        load_all_armors_data(root_path + "/armor.json");
        load_all_accessories_data(root_path + "/accessory.json");
        load_all_comsumables_data(root_path + "/comsumable.json");
        load_all_materials_data(root_path + "/material.json");
        load_all_quests_data(root_path + "/quest.json");

        return true;
    }
    catch (const ErrorCode& e)
    {
        std::cerr << "[LoadingError]:" << to_string(e) << std::endl;
        return false;
    }
    catch (const std::exception& e)
    {
        std::cerr << "[UnknownError]:" << e.what() << std::endl;
        return false;
    }
}

void DataManager::load_all_races_data(const std::string& path)
{
    load_file(races, path, "种族数据");
}

void DataManager::load_all_skills_data(const std::string& path)
{
    load_file(skills, path, "技能数据");
}

void DataManager::load_all_effects_data(const std::string& path)
{
    load_file(effects, path, "效果数据");
}

void DataManager::load_all_buffs_data(const std::string& path)
{
    load_file(buffs, path, "Buff数据");
}

void DataManager::load_all_jobs_data(const std::string& path)
{
    load_file(jobs, path, "职业数据");
}

void DataManager::load_all_weapons_data(const std::string& path)
{
    load_file(weapons, path, "武器数据");
}

void DataManager::load_all_armors_data(const std::string& path)
{
    load_file(armors, path, "防具数据");
}

void DataManager::load_all_accessories_data(const std::string& path)
{
    load_file(accessories, path, "饰品数据");
}

void DataManager::load_all_comsumables_data(const std::string& path)
{
    load_file(comsumables, path, "消耗品数据");
}

void DataManager::load_all_materials_data(const std::string& path)
{
    load_file(materials, path, "材料数据");
}

void DataManager::load_all_quests_data(const std::string& path)
{
    load_file(quests, path, "任务物品数据");
}