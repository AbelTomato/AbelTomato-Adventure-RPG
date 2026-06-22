#include "core/game_core.hpp"

#include <iostream>
#include <sstream>
#include <string>

int main()
{
    std::ostringstream buffer;
    buffer << std::cin.rdbuf();
    const std::string input_json = buffer.str();
    const std::string output_json = handle_json(input_json);

    std::cout << output_json << std::endl;

    return 0;
}