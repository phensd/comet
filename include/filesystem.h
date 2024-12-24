#pragma once
#include <filesystem>
#include <optional>
#include <vector>
#include <string>
#include "logger.h"

namespace comet{
    class filesystem_manager{




        bool validate_filetype(const std::string& path);

        std::string get_home_dir();
        void write_user_path_entries(std::string output_file_path,std::vector<std::string>& entries_to_write);


        std::vector<std::filesystem::path> found_song_paths{};





        public:


            std::vector<std::string> user_paths_entries{};
            std::string get_data_directory();
            bool saved_json_exists(std::string json_path);
            std::optional<std::vector<std::string>> load_user_path_entries(std::string input_file_path,logger& logger);

            std::vector<std::filesystem::path> find_song_entries (comet::logger& logger);
            std::vector<std::string> get_default_path_entries();

            filesystem_manager(logger& logger);
            ~filesystem_manager(){write_user_path_entries("comet.json", user_paths_entries);}
        





    };
}