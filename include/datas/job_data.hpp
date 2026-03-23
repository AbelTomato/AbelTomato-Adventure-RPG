#ifndef JOB_DATA_HPP
#define JOB_DATA_HPP

#include "base_data.hpp"
#include "definitions/core_attributes.hpp"
#include "nlohmann/json.hpp"

class JobData : public BaseData
{
private:
    Attributes base_attr;
    int init_hp, init_mp, init_sp;  // 初始生命、法力、精力
    std::vector<int> job_skills;    // 职业技能

public:
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(JobData, id, name, base_attr, init_hp, init_mp, init_sp,
                                   job_skills);
};

#endif