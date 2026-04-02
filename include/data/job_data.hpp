#pragma once

#include "base_data.hpp"
#include "definitions/core_attributes.hpp"
#include "nlohmann/json.hpp"

struct JobData : public BaseData
{
    Attributes base_attr;
    std::vector<int> job_skills;  // 职业技能

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(JobData, id, name, description, base_attr, job_skills);
};
