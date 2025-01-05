#include "include/song_manager.h"
#include "include/filesystem.h"
#include <algorithm>
#include <random>
#include <taglib/fileref.h>
#include <unordered_set>
#include "include/player.h"

std::string comet::song_manager::create_name_pretty(const std::filesystem::path& path){
    TagLib::FileRef tags {path.c_str()};
    auto tag {tags.tag()};

    //if there are no tags default to the filename as a safe default
    if(tag->isEmpty() || (tag->artist().isEmpty() || tag->album().isEmpty() || tag->title().isEmpty())){
        lgr.log((std::string)"Registered file tag name: \"" + path.filename().c_str() + "\"" + " (No metadata or appropriate metadata was found, fell back to file name.)",true);
        return path.filename();
    }

    std::stringstream id;

    id << tag->artist() << ": " << tag->album() << " - " << tag->title();



    lgr.log((std::string)"Registered file tag name: \"" + id.str() + "\"",true);
    return id.str();

}

std::string comet::song_manager::create_id(const std::filesystem::path& path,int selected_option,const std::string& name_pretty){
    if(song_title_display_option_selected == 1)return path.filename();
    if(song_title_display_option_selected == 2)return path.c_str();
    //name_pretty is passed here to avoid generating it twice
    //it is part of the song structure so it can be used in searches by the user
    if(song_title_display_option_selected == 0)return name_pretty;
    return "this id should never show!!!";
}

void comet::song_manager::map_song_ids(std::string* const current_song_id, bool rescan, bool silent_log){

    all_song_ids.clear();

    //this is used to stop duplicate IDs from going through
    //instead, a "(1)" is added to the ID when it fails to insert.
    std::unordered_set<std::string> filter{};

    //if a song is already playing (or paused but set in the player)
    //make sure the song id updates in the display when we remap them here
    if(!rescan && current_song_id != nullptr && !current_song_id->empty()){
        const song& song_data {id_to_song_map[*current_song_id]};
        *current_song_id = create_id(song_data.full_path,song_title_display_option_selected,song_data.name_pretty);
    }

    for(std::filesystem::path& path : fsysmanager.find_song_entries(lgr,rescan)){

        song new_song{};

        //if we have processed the song already, just use the one we processed
        if(fsysmanager.get_processed_entries_cache().contains(path)){
            lgr.log((std::string) "Entry already processed, taking metadata from cache - " + path.c_str(),!silent_log);
            new_song = fsysmanager.get_processed_entries_cache().at(path);
        }else{
            lgr.log((std::string) "Processing entry at - " + path.c_str(),!silent_log);
            new_song.file_name = path.filename();
            new_song.full_path = path.c_str();
            //use of cache here is mostly to avoid reading the file for the metadata every time when it is not needed.
            new_song.name_pretty = create_name_pretty(path);
        }

        auto id {create_id(path,song_title_display_option_selected,new_song.name_pretty)};

        
        //if there is a duplicate ID then add (1) to it so when play_next searches for a song a duplicate wont trip it up
        if(!filter.insert(id).second){
            id += " (1)";
            filter.insert(id);
        }
        id_to_song_map[id] = new_song;
        fsysmanager.register_processed_entry(path,new_song);
    }

    //todo: maybe the user should be able to choose how these are sorted

    //make sure the songs list is sorted by file name
    all_song_ids = {filter.begin(),filter.end()};
    auto sortby = [&] (std::string& lhs,std::string& rhs){
        song& data_lhs {id_to_song_map[lhs]};
        song& data_rhs {id_to_song_map[rhs]};
        return data_lhs.full_path < data_rhs.full_path;

    };
    std::sort(all_song_ids.begin(),all_song_ids.end(),sortby);

}
std::vector<std::string>& comet::song_manager::get_filtered_entries(std::string current_player_search){
    //if there is a search, filter the results according to that search
    if(current_player_search.length() > 0){
        filtered_song_ids.clear();
        std::copy_if(all_song_ids.begin(),all_song_ids.end(),std::back_inserter(filtered_song_ids),
        [this, current_player_search](std::string& entry){
            song& entry_data {id_to_song_map[entry]};
            //match the search with the full path of the song entry, instead of the chosen display id
            //when i add the option to display the song entries by their metadata, i should
            //have it search the metadata if available
            //also search for the mp3 tagged name.
            //not sure if itll make much of a difference but may be nice to have.
            return comet::player::match_search_string(current_player_search,entry_data.full_path)
            || comet::player::match_search_string(current_player_search,entry_data.name_pretty);
        });
        return filtered_song_ids;
    }
    //if no search filter is in place, return all the songs
    return all_song_ids;
}


std::vector<std::string> comet::song_manager::get_shuffled_selection(){

    static std::random_device dev;
    static std::mt19937 rng(dev());

    std::vector<std::string> shuffled {public_song_ids.begin(),public_song_ids.end()};
    std::shuffle(shuffled.begin(),shuffled.end(),rng);


    return shuffled;
}


comet::song_manager::song_manager(comet::filesystem_manager& fsysmanager,comet::logger& logger) : fsysmanager(fsysmanager), lgr(logger){

    if(!fsysmanager.saved_song_display_selection.empty()){
        auto found {std::find(song_title_display_options.begin(),song_title_display_options.end(),fsysmanager.saved_song_display_selection)};
        if(found != song_title_display_options.end()){
            song_title_display_option_selected = (found - song_title_display_options.begin());
        }
    }
    map_song_ids();
    public_song_ids = all_song_ids;
    shuffled_song_ids = get_shuffled_selection();
}