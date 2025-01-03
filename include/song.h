#pragma once
#include "nlohmann/detail/macro_scope.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace comet {
    struct song {
        std::string file_name{};
        std::string full_path{};

        //string with meta data tags, otherwise the file name
        std::string name_pretty{};

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(song,file_name,full_path,name_pretty);
    };
}