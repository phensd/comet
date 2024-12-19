
#include <optional>
#include <vector>
#include <string>
#include "logger.h"

namespace music_player::filesystem{

    std::vector<std::string> find_song_entries (std::vector<std::string> paths,music_player::logger& logger);

    bool validate_filetype(const std::string& path);

    std::string get_home_dir();
    std::optional<std::vector<std::string>> load_user_path_entries(std::string input_file_path,logger& logger);
    std::string get_data_directory();
    void write_user_path_entries(std::string output_file_path,std::vector<std::string>& entries_to_write);


    bool saved_json_exists(std::string json_path);

}