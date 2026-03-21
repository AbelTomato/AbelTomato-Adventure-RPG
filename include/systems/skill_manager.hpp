#ifndef SKILL_MANAGER_HPP
#define SKILL_MANAGER_HPP

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "core/definitions.hpp"
#include "nlohmann/json.hpp"


using json = nlohmann::json;

class SkillManager {
public:
    bool load_skills(const std::string& file_path);

    SkillData get_skill(int id) const;

private:
    std::map<int, SkillData> skill_pool;

    SkillData parse_skill(const json& j);
};

#endif