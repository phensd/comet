#pragma once
#include <string>
#include <vector>

namespace comet::util {
    std::string lower (std::string input);
    std::vector<std::string> split_string(std::string input);

    std::string create_timestamp_string(double current_timestamp_seconds, float current_song_length_seconds);
}