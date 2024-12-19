#pragma once
#include <string>
#include <vector>

namespace music_player::util {
    std::string lower (std::string input);
    std::vector<std::string> split_string(std::string input);
}