#pragma once
#include "filesystem.h"
#include <unordered_map>
namespace comet{

    class song_manager{

        struct song {
            std::string file_name{};
            std::string full_path{};


            //todo: get tag library to add metadatas here
            std::string name_pretty{};
        };



        //"all song entries" contains every song entry found
        //no matter what, while
        //"filtered entries" is used to categorize search results
        //the public facing one (public_song_entries)
        //switches between pointing to these two
        std::vector<std::string> all_song_ids {};
        std::vector<std::string> filtered_song_ids {};

        filesystem_manager& fsysmanager;
        logger& lgr;



        public:
            const std::vector<std::string>& get_all_song_ids() {return all_song_ids;};
            void map_song_ids();

            std::vector<std::string>& get_filtered_entries(std::string current_player_search);
            std::unordered_map<std::string, song> id_to_song_map{};
            std::vector<std::string> public_song_ids{};
            song_manager(filesystem_manager& fsysmanager,struct logger& logger);



    };
}