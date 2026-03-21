#include "systems/skill_manager.hpp"

bool SkillManager::load_skills(const std::string& file_path) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "错误：无法打开技能文件 " << file_path << std::endl;
        return false;
    }

    json j_array;

    try {
        file >> j_array;
    } catch (json::parse_error& e) {
        std::cerr << "JSON解析错误: " << e.what() << std::endl;
        return false;
    }

    for (const auto& item : j_array) {
        SkillData skill = parse_skill(item);
        skill_pool[skill.id] = skill;
    }

    std::cout << "成功加载了 " << skill_pool.size() << " 个技能。" << std::endl;

    return true;
}

SkillData SkillManager::parse_skill(const json& j) {
    SkillData s;

    s.id = j.value("id", 0);  // value可以提供默认值
    s.skill_name = j.value("name", "未命名技能");
    s.description = j.value("description", "");
    s.cost_hp = j.value("cost_hp", 0);
    s.cost_mp = j.value("cost_mp", 0);
    s.cost_sp = j.value("cost_sp", 0);
    s.cooldown = j.value("cooldown", 0);

    s.target_type = static_cast<TargetType>(j.value("target_type", 0));

    if (j.contains("damages") && j["damages"].is_array()) {
        for (const auto& d : j["damages"]) {
            SkillData::DamageInfo d_info;
            d_info.type = static_cast<DamageType>(d.value("type", 0));
            d_info.multiplier = d.value("multiplier", 1.0);
            d_info.fixed_bonus = d.value("fixed_bonus", 0);
            s.damages.push_back(d_info);
        }
    }

    if (j.contains("effect_triggers") && j["effect_triggers"].is_array()) {
        for (const auto& e : j["effect_triggers"]) {
            SkillData::EffectTrigger e_info;
            e_info.effect_id = e.value("effect_id", 0);
            e_info.chance = e.value("chance", 0.0);
            e_info.power = e.value("power", 0);
            s.effect_triggers.push_back(e_info);
        }
    }

    return s;
}

SkillData SkillManager::get_skill(int id) const {
    auto it = skill_pool.find(id);  // 使用it迭代器访问skill_pool中是否存在对应id，存在则返回值
    if (it != skill_pool.end()) return it->second;
    return SkillData();
}