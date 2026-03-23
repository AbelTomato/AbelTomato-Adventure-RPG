#include "systems/load_datas.hpp"

#include <fstream>
#include <iostream>

DataManager& DataManager::instance()
{
    static DataManager inst;
    return inst;
}

template <typename T>
bool load_file(DataRepository<T>& repo, const std::string& path, const std::string& data_type_name)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "无法打开" << data_type_name << "文件：" << path << std::endl;
        return false;
    }

    try
    {
        json j;
        file >> j;
        if (!repo.load(j))
        {
            return false;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << data_type_name << "解析失败：" << e.what() << std::endl;
        return false;
    }

    std::cout << data_type_name << "加载成功！" << std::endl;
    return true;
}

bool DataManager::load_all_races_data(const std::string& path)
{
    return load_file(races, path, "种族数据");
}

bool DataManager::load_all_skills_data(const std::string& path)
{
    return load_file(skills, path, "技能数据");
}

bool DataManager::load_all_effects_data(const std::string& path)
{
    return load_file(effects, path, "效果数据");
}

bool DataManager::load_all_buffs_data(const std::string& path)
{
    return load_file(buffs, path, "Buff数据");
}

bool DataManager::load_all_jobs_data(const std::string& path)
{
    return load_file(jobs, path, "职业数据");
}