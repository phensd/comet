#include "include/filesystem.h"
#include "include/saved_song_loadback.h"
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
    json_output["cache_scanned_paths"] = song_entry_path_cache;
    json_output["song_display_option"] = saved_song_display_selection.empty() ? "Tagged name" : saved_song_display_selection;
    json_output["processed_song_entries"] = processed_entries_cache;
    json_output["saved_song_loadback"] = song_on_load;
    output_file << std::setw(4) << json_output;

}

std::optional<nlohmann::json> comet::filesystem_manager::load_json_file(std::string input_file_path,logger& logger){

    std::ifstream input_file {get_data_directory() + input_file_path};
    nlohmann::json json_input;
    try {
        input_file >> json_input;
        return json_input;
    }catch(nlohmann::detail::parse_error){
        //if the syntax is wrong we will return nothing
        return std::nullopt;
    }
}


bool comet::filesystem_manager::validate_filetype(const std::string& path,logger& logger){

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
        "57 41 56 45", //wav secondary? wikipedia lists this but idk
        "66 4c 61 43", //flac
        "66 4C 61 43" //flac
    };


    for(const auto& entry : magic_numbers) {
        if(numbers.str().find(entry) != std::string::npos){
            return true;
        }
    }

    logger.log("Invalid magic number - " + numbers.str() + " from file at - " + path.c_str(),true);
    return false;
}

std::vector<std::filesystem::path> comet::filesystem_manager::find_song_entries(comet::logger& logger,bool rescan){

    //hopefully copying a std::filesystem::path does not cause disk i/o?
    //according to cppreference it only *syntactically* represents a file system path
    //so it shouldnt...?
    if(!rescan && song_entry_path_cache.size() > 0 ) return song_entry_path_cache;

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

                    //don't process directories as files of course
                    if(entry.is_directory()) continue;
                    
                    //validate that we can actually use the filetype before we add it
                    if(validate_filetype(entry.path(),logger)){
                        logger.log("Found entry in " + path + " " +  (std::string) entry.path(),true);
                        to_return.insert(entry.path());
                    }else {
                        logger.log("Invalid audio format (cannot be read by miniaudio) - " + (std::string) entry.path(),true);
                        logger.push_error_message("Not a valid audio format - " + (std::string) entry.path().filename());
                    }
                
                };
            }catch(std::filesystem::filesystem_error){
                logger.log("Filesystem error, probably no permission. Aborting",true);
                logger.push_error_message("Filesystem error, probably no permission. - " + path);
                continue;
            }
        }
    }

    logger.log("No. entries found total " +  std::to_string(to_return.size()),true);

    song_entry_path_cache = {to_return.begin(),to_return.end()};
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


void comet::filesystem_manager::validate(logger& logger){
    //if the user modified the json in any way, or files were removed, make sure any invalid song entry paths are removed.
    // logger.log("Validating cached song paths...",true);

    // auto cached_entries_deleted {std::erase_if(song_entry_path_cache,
    //     [&] (std::filesystem::path& path)
    //     //incase the user modified the json file on their own, make sure things are correct
    //     {return !std::filesystem::is_regular_file(path) || !validate_filetype(path,logger);})};
    // logger.log("Validation finished, " + std::to_string(cached_entries_deleted) + " entries removed due to being invalid.",true);

    //make sure any user paths that may have been deleted will be erased 
    for(auto& entry : user_paths_entries){if (!std::filesystem::is_directory(entry)) entry = "";}

}

comet::filesystem_manager::filesystem_manager(logger& logger){
    if(saved_json_exists("comet.json")) json_file = load_json_file("comet.json",logger);

    if(json_file.has_value()){
        logger.log("JSON file found and loaded.",true);
        //2nd arguments are default values, in case the value does not exist in the JSON value.
        user_paths_entries = json_file.value().value("paths",get_default_path_entries());
        song_entry_path_cache = json_file.value().value("cache_scanned_paths",std::vector<std::filesystem::path>{});
        //"Tagged name" will be the default
        saved_song_display_selection = json_file.value().value("song_display_option","Tagged name");

        //these entries are not especially designed to be believed 
        //that is, if the path key does not match up with a scanned/cached file path, then a new entry is created with the scanned/cached path instantly
        processed_entries_cache = json_file.value().value("processed_song_entries",std::unordered_map<std::filesystem::path,song>{});

        //Load the song that was playing when the player was previously open
        song_on_load = json_file.value().value("saved_song_loadback",saved_song_loadback{});

        //make sure the cached entries and user path entries in the JSON file are still syntactically valid
        validate(logger);

    }else {
        logger.log("JSON file failed to load, most likely a parse error. Using default values.",true);
        //if the json fails to parse and returns nothing then use the default entries
        //every other value obtained from the json file is fine when it is default initialized
        user_paths_entries = get_default_path_entries();
    }
}