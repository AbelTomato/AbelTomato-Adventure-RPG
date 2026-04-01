#ifndef BASE_DATA_HPP
#define BASE_DATA_HPP

#include <string>

struct BaseData
{
    int id;
    std::string name;
    std::string description;

    virtual ~BaseData() = default;
};

#endif