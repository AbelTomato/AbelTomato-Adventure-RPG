#include "core\global.hpp"

#include <ctime>

#include "datas/race_data.hpp"
#include "datas/skill_data.hpp"
#include "systems/load_datas.hpp"

namespace Global
{
std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));

int get_random_int(int min, int max)
{
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}

double get_random_double()
{
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(rng);
}
}  // namespace Global

const RaceData* get_race_by_id(const int id)
{
    return DataManager::instance().get_race_ptr_by_id(id);
}

const RaceData* get_race_by_name(const std::string& name)
{
    return DataManager::instance().get_race_ptr_by_name(name);
}

const SkillData* get_skill_by_id(const int id)
{
    return DataManager::instance().get_skill_ptr_by_id(id);
}

const SkillData* get_skill_by_name(const std::string& name)
{
    return DataManager::instance().get_skill_ptr_by_name(name);
}