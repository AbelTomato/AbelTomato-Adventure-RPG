#ifndef LOAD_DATAS_HPP
#define LOAD_DATAS_HPP

#include <exception>
#include <iostream>
#include <map>
#include <nlohmann/json.hpp>

#include "datas/buff_data.hpp"
#include "datas/effect_data.hpp"
#include "datas/job_data.hpp"
#include "datas/race_data.hpp"
#include "datas/skill_data.hpp"

using json = nlohmann::json;

template <typename T>
class DataRepository
{
public:
    bool load(const json& j_array)
    {
        if (!j_array.is_array()) return false;

        data_map.clear();
        name_to_id.clear();

        for (const auto& item : j_array)
        {
            try
            {
                T data = item.get<T>();
                int id = data.id;
                std::string name = data.name;

                data_map[id] = std::move(data);
                name_to_id[name] = id;
            }
            catch (const std::exception& e)
            {
                std::cerr << "加载数据时出现错误：" << e.what() << std::endl;
                return false;
            }
        }

        return true;
    }

    const T* get_by_id(const int id) const
    {
        auto it = data_map.find(id);
        if (it != data_map.end()) return &(it->second);
        return nullptr;
    }

    const T* get_by_name(const std::string& name) const
    {
        auto it = name_to_id.find(name);
        if (it != name_to_id.end()) return get_by_id(it->second);
        return nullptr;
    }

private:
    std::map<int, T> data_map;
    std::map<std::string, int> name_to_id;
};

class DataManager
{
public:
    static DataManager& instance();

    DataRepository<RaceData> races;
    DataRepository<SkillData> skills;
    DataRepository<EffectData> effects;
    DataRepository<BuffData> buffs;
    DataRepository<JobData> jobs;

    bool load_all_races_data(const std::string& path);

    bool load_all_skills_data(const std::string& path);

    bool load_all_effects_data(const std::string& path);

    bool load_all_buffs_data(const std::string& path);

    bool load_all_jobs_data(const std::string& path);

private:
    DataManager() {}
};

#endif