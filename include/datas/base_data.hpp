#ifndef BASE_DATA_HPP
#define BASE_DATA_HPP

#include <string>

class BaseData
{
public:
    int id;
    std::string name;

    ~BaseData() = default;
};

#endif