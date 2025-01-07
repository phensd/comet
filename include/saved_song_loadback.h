#pragma once
#include <string>
#include <nlohmann/json.hpp>
#include "nlohmann/detail/macro_scope.hpp"
namespace comet{
    struct saved_song_loadback{
        std::string path {""};
        //miniaudio aliases ull to ma_uint64, but i do not want to include its header here.
        unsigned long long pcm_timestamp {0};


        bool empty(){return path.empty() && pcm_timestamp <= 0;};


        NLOHMANN_DEFINE_TYPE_INTRUSIVE(saved_song_loadback,path,pcm_timestamp);

    };
}