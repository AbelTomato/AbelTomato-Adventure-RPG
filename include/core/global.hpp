#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <random>

#include "datas/race_data.hpp"
#include "datas/skill_data.hpp"

namespace Global
{
extern std::mt19937 rng;

int get_random_int(int min, int max);

double get_random_double();
}  // namespace Global

const RaceData* get_race_by_id(const int id);

const RaceData* get_race_by_name(const std::string name);

const SkillData* get_skill_by_id(const int id);

const SkillData* get_skill_by_id(const std::string name);

#endif