#pragma once
#include "filesystem.h"
#include <unordered_map>
namespace comet{

    class song_manager{

        struct song {
            std::string file_name{};
            std::string full_path{};

            //string with meta data tags, otherwise the file name
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
        std::string create_id(const std::filesystem::path& path, int selected_option,const std::string& name_pretty);
        std::string create_name_pretty(const std::filesystem::path& path);


        public:
            const std::vector<std::string>& get_all_song_ids() {return all_song_ids;};
            void map_song_ids(bool rescan = false);

            std::vector<std::string>& get_filtered_entries(std::string current_player_search);
            std::unordered_map<std::string, song> id_to_song_map{};
            std::vector<std::string> public_song_ids{};
            song_manager(filesystem_manager& fsysmanager,struct logger& logger);


            //used to allow the user to choose how they want the songs to be identified in the song selector
            std::vector<std::string> song_title_display_options{
                "Tagged name",
                "File name",
                "Full path",
            };int song_title_display_option_selected {0};



    };
}