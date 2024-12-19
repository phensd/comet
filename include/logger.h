#pragma once
#include <string>

namespace music_player {
    struct logger {

        std::string output_file_path{};


        bool log(const std::string& message,bool output_to_cerr=false);



        logger();
    };
}