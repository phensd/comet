#include "include/filesystem.h"
#include "nlohmann/detail/conversions/to_chars.hpp"
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <nlohmann/detail/exceptions.hpp>
#include <set>

std::string comet::filesystem_manager::get_home_dir(){
    return (std::string) getenv("HOME") + "/";
}

std::string comet::filesystem_manager::get_data_directory(){

    auto full_json_path {get_home_dir() + ".local/share/comet/"};

    //this does nothing if the path already exists
    std::filesystem::create_directories(full_json_path);

    return(full_json_path);

}


bool comet::filesystem_manager::saved_json_exists(std::string json_path){
    return std::filesystem::exists(get_data_directory() + json_path);
}

void comet::filesystem_manager::write_json_file(std::string input_file_path){

    //remove invalid paths from being saved
    for(auto& entry : user_paths_entries){if (!std::filesystem::is_directory(entry)) entry = "";}

    std::ofstream output_file{get_data_directory() + input_file_path};
    nlohmann::json json_output;
    json_output["paths"] = user_paths_entries;
    json_output["cached_entries"] = song_entry_cache;
    output_file << std::setw(4) << json_output;

}

std::optional<nlohmann::json> comet::filesystem_manager::load_json_file(std::string input_file_path,logger& logger){

    std::ifstream input_file {get_data_directory() + input_file_path};
    nlohmann::json json_input;
    try {
        input_file >> json_input;
        return json_input;
    }catch(nlohmann::detail::parse_error){
        logger.log("JSON file failed to load, parse error");
        //if the syntax is wrong we will return nothing
        return std::nullopt;
    }
}


bool comet::filesystem_manager::validate_filetype(const std::string& path){

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

std::vector<std::filesystem::path> comet::filesystem_manager::find_song_entries(comet::logger& logger,bool rescan){

    //hopefully copying a std::filesystem::path does not cause disk i/o?
    //according to cppreference it only *syntactically* represents a file system path
    //so it shouldnt...?
    if(!rescan && song_entry_cache.size() > 0 ) return song_entry_cache;

    std::set<std::filesystem::path> to_return {};

    std::cerr << "If it takes a while to open, directories are probably being scanned slowly...";


    for(auto const& path : user_paths_entries){
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

    song_entry_cache = {to_return.begin(),to_return.end()};
    return {to_return.begin(),to_return.end()};
}

std::vector<std::string> comet::filesystem_manager::get_default_path_entries(){

    std::vector<std::string> defaults{};

    //push a default /home/user/music path to the list
    defaults.emplace_back((std::string) getenv("HOME") + "/Music");

    //the default empty path slots
    for(int i{0}; i < 9; ++i){
        defaults.emplace_back("");
    }

    return defaults;
}


comet::filesystem_manager::filesystem_manager(logger& logger){
    if(saved_json_exists("comet.json")) json_file = load_json_file("comet.json",logger);

    if(json_file.has_value()){
        //if the json fails to parse and returns nothing then use the default entries
        user_paths_entries = json_file.value()["paths"];
        song_entry_cache = json_file.value()["cached_entries"];
    }else {
        //if the json file doesnt exist then use the default entries too
        user_paths_entries = get_default_path_entries();
    }
}