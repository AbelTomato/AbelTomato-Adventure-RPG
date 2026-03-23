#ifndef LOAD_DATAS_HPP
#define LOAD_DATAS_HPP

#include <map>
#include <nlohmann/json.hpp>

#include "datas/race_data.hpp"
#include "datas/skill_data.hpp"

using json = nlohmann::json;

class DataManager
{
public:
    static DataManager& instance();

    std::map<int, RaceData> races_id;
    std::map<std::string, RaceData> races_name;
    std::map<int, SkillData> skills_id;
    std::map<std::string, SkillData> skills_name;

    bool load_all_races_datas(const std::string& path);

    bool load_all_skills_datas(const std::string& path);

    std::optional<RaceData> get_race_by_id(const int id);

    std::optional<RaceData> get_race_by_name(const std::string& name);

    std::optional<SkillData> get_skill_by_id(const int id);

    std::optional<SkillData> get_skill_by_name(const std::string& name);

    const RaceData* get_race_ptr_by_id(const int id);

    const RaceData* get_race_ptr_by_name(const std::string& name);

    const SkillData* get_skill_ptr_by_id(const int id);

    const SkillData* get_skill_ptr_by_name(const std::string& name);

private:
    DataManager() {}
};

#endif