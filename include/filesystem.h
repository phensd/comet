#pragma once
#include <filesystem>
#include <optional>
#include <vector>
#include <string>
#include "logger.h"
#include "song.h"
#include "saved_song_loadback.h"
#include <unordered_map>
namespace comet{
    class filesystem_manager{

        bool validate_filetype(const std::string& path,logger& logger);

        std::string get_home_dir();
        void write_json_file(std::string output_file_path);


        std::optional<nlohmann::json> json_file;

        //used to avoid rescanning and write to json file aswell.
        std::vector<std::filesystem::path> song_entry_path_cache{};
        std::unordered_map<std::filesystem::path,song> processed_entries_cache{};

        std::optional<nlohmann::json> load_json_file(std::string input_file_path,logger& logger);

        void validate(logger& logger);

        saved_song_loadback song_on_load {};



        public:
            std::string saved_song_display_selection {};

            void set_song_loadback(saved_song_loadback loadback){
                if(!loadback.empty()){
                    this->song_on_load = loadback;
                }
            }

            saved_song_loadback get_song_loadback(){return song_on_load;};

            std::vector<std::string> user_paths_entries{};
            std::string get_data_directory();
            bool saved_json_exists(std::string json_path);

            std::vector<std::filesystem::path> find_song_entries (comet::logger& logger,bool rescan=false);
            std::vector<std::string> get_default_path_entries();


            const std::unordered_map<std::filesystem::path,song>& get_processed_entries_cache() {return processed_entries_cache;};
            bool register_processed_entry(const std::filesystem::path& path, const song& song) {return processed_entries_cache.insert({path,song}).second;};


            filesystem_manager(logger& logger);
            ~filesystem_manager(){write_json_file("comet.json");}

    };
}