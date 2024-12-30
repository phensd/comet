#pragma once
#include <filesystem>
#include <optional>
#include <vector>
#include <string>
#include "logger.h"
#include <nlohmann/json.hpp>


namespace comet{
    class filesystem_manager{




        bool validate_filetype(const std::string& path);

        std::string get_home_dir();
        void write_json_file(std::string output_file_path);


        std::optional<nlohmann::json> json_file;

        //used to avoid rescanning and write to json file aswell.
        std::vector<std::filesystem::path> song_entry_cache{};

        std::optional<nlohmann::json> load_json_file(std::string input_file_path,logger& logger);





        public:


            std::vector<std::string> user_paths_entries{};
            std::string get_data_directory();
            bool saved_json_exists(std::string json_path);

            std::vector<std::filesystem::path> find_song_entries (comet::logger& logger);
            std::vector<std::string> get_default_path_entries();

            filesystem_manager(logger& logger);
            ~filesystem_manager(){write_json_file("comet.json");}
        





    };
}