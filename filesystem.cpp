#include "include/filesystem.h"
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <nlohmann/detail/exceptions.hpp>
#include <nlohmann/json.hpp>
#include <set>

std::string music_player::filesystem::get_home_dir(){
    return (std::string) getenv("HOME") + "/";
}

std::string music_player::filesystem::get_data_directory(){

    auto full_json_path {get_home_dir() + ".local/share/comet/"};

    //this does nothing if the path already exists
    std::filesystem::create_directories(full_json_path);

    return(full_json_path);

}

bool music_player::filesystem::saved_json_exists(std::string json_path){
    return std::filesystem::exists(get_data_directory() + json_path);
}

void music_player::filesystem::write_user_path_entries(std::string input_file_path,std::vector<std::string>& entries_to_write){

    //remove invalid paths from being saved
    for(auto& entry : entries_to_write){if (!std::filesystem::is_directory(entry)) entry = "";}

    std::ofstream output_file{get_data_directory() + input_file_path};
    nlohmann::json json_output;
    json_output["paths"] = entries_to_write;
    output_file << std::setw(4) << json_output;

}

std::optional<std::vector<std::string>> music_player::filesystem::load_user_path_entries(std::string input_file_path,logger& logger){

    std::ifstream input_file {get_data_directory() + input_file_path};
    nlohmann::json json_input;
    try {
        input_file >> json_input;
        return json_input["paths"];
    }catch(nlohmann::detail::parse_error){
        logger.log("JSON file failed to load, parse error");
        //if the syntax is wrong we will return nothing
        return std::nullopt;
    }
}


bool music_player::filesystem::validate_filetype(const std::string& path){

    std::ifstream input {path,std::fstream::binary};
                
    unsigned char buffer {};
    std::stringstream numbers {};

    int i{0};
    while(input >> buffer && i < 5){
        numbers << std::setw(2) << std::setfill('0') << std::hex << static_cast<unsigned>(buffer) << ' ';
        ++i;
    }

    const static std::vector<std::string> magic_numbers  { 
        "ff fb", //mp3 untagged
        "49 44 33", //mp3 tagged
        "52 49 46 46", //wav
        "57 41 56 45" //wav secondary? wikipedia lists this but idk
    };


    for(const auto& entry : magic_numbers) {
        if(numbers.str().find(entry) != std::string::npos){
            return true;
        }
    }

    return false;
}

std::vector<std::string> music_player::filesystem::find_song_entries(std::vector<std::string> paths,music_player::logger& logger){

    std::set<std::string> to_return {};

    std::cerr << "If it takes a while to open, directories are probably being scanned slowly...";


    for(auto const& path : paths){
        //verify that the path actually exists
        if(!std::filesystem::is_directory(path)){
            logger.log("Invalid path entered (may be a default path set by program, not a big deal, ignoring) - " + path,true);
        }else{
            logger.log("Valid user entry path - " + path,true);

            //create the iterator, go recursively
            auto itr { std::filesystem::recursive_directory_iterator(path) };

            //wrapped in a try block, incase we encounter an error
            try {
                for(auto const& entry : itr ){

                    //more than 4 takes way too long, so dont go deeper
                    if(itr.depth() > 4){ itr.disable_recursion_pending();};

                    //validate that we can actually use the filetype before we add it
                    if(validate_filetype(entry.path())){
                        logger.log("Found entry in " + path + " " +  (std::string) entry.path(),true);
                        to_return.insert(entry.path());
                    }else {
                        logger.log("Invalid audio format (cannot be read by miniaudio) - " + (std::string) entry.path(),true);
                    }
                
                };
            }catch(std::filesystem::filesystem_error){
                logger.log("Filesystem error, probably no permission. Aborting",true);
                continue;
            }
        }
    }

    logger.log("No. entries found total " +  std::to_string(to_return.size()),true);

    return {to_return.begin(),to_return.end()};
}