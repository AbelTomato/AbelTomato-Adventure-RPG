#ifndef LOAD_DATAS_HPP
#define LOAD_DATAS_HPP

#include <exception>
#include <iostream>
#include <map>
#include <nlohmann/json.hpp>

#include "data/buff_data.hpp"
#include "data/effect_data.hpp"
#include "data/item_data.hpp"
#include "data/job_data.hpp"
#include "data/race_data.hpp"
#include "data/skill_data.hpp"
#include "definitions/error_type.hpp"

using json = nlohmann::json;

template <typename T>
class DataRepository
{
public:
    void load(const json& j_array)  // 数据加载
    {
        if (!j_array.is_array())  // 检查类型
            throw ErrorCode::DataTypeMismatch;

        // 创建temp，不直接对成员操作
        std::map<int, T> temp_map;
        std::map<std::string, int> temp_name_to_id;

        for (const auto& item : j_array)
        {
            try
            {
                T data = item.get<T>();
                int id = data.id;
                std::string name = data.name;

                if (temp_map.contains(id)) throw ErrorCode::DuplicateId;

                temp_map[id] = std::move(data);
                temp_name_to_id[name] = id;
            }
            catch (const nlohmann::json::exception&)
            {
                throw ErrorCode::JsonParseError;
            }
        }

        data_map = std::move(temp_map);
        name_to_id = std::move(temp_name_to_id);
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
    std::map<int, T> data_map;              // 从id到data的映射
    std::map<std::string, int> name_to_id;  // 从name到id的映射
};

class DataManager
{
public:
    static DataManager& instance();

    void init(const std::string& base_path);

    DataRepository<RaceData> races;        // 种族
    DataRepository<SkillData> skills;      // 技能
    DataRepository<EffectData> effects;    // 效果
    DataRepository<BuffData> buffs;        // buff
    DataRepository<JobData> jobs;          // 职业
    DataRepository<ItemData> weapons;      // 武器
    DataRepository<ItemData> armors;       // 防具
    DataRepository<ItemData> accessories;  // 饰品
    DataRepository<ItemData> comsumables;  // 消耗品
    DataRepository<ItemData> materials;    // 材料
    DataRepository<ItemData> quests;       // 任务物品

    bool load_all_data();

    void load_all_races_data(const std::string& path);

    void load_all_skills_data(const std::string& path);

    void load_all_effects_data(const std::string& path);

    void load_all_buffs_data(const std::string& path);

    void load_all_jobs_data(const std::string& path);

    void load_all_weapons_data(const std::string& path);

    void load_all_armors_data(const std::string& path);

    void load_all_accessories_data(const std::string& path);

    void load_all_comsumables_data(const std::string& path);

    void load_all_materials_data(const std::string& path);

    void load_all_quests_data(const std::string& path);

private:
    std::string root_path;

    DataManager() {}
};

#endif