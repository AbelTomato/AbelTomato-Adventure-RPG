#pragma once

#include <string>

struct BaseData  // data基类
{
    int id;
    std::string name;
    std::string description;

    virtual ~BaseData() = default;
};
