#include "include/song_manager.h"
#include "include/filesystem.h"
#include <algorithm>
#include <unordered_set>
#include "include/player.h"
void comet::song_manager::map_song_ids(){
    all_song_ids.clear();

    std::unordered_set<std::string> filter{};

    for(std::filesystem::path& path : fsysmanager.find_song_entries(lgr)){
        song new_song{};

        new_song.file_name = path.filename();
        new_song.full_path = path.c_str();
        //todo new_song.name_pretty = 


        std::string id;

        if(song_title_display_option_selected == 0)id = path.filename();
        //metadata form, todo if(song_title_display_option_selected == 1)id = path.filename();
        if(song_title_display_option_selected == 2)id = path.c_str();

        
        //if there is a duplicate ID then add (1) to it so when play_next searches for a song a duplicate wont trip it up
        if(!filter.insert(id).second){
            id += " (1)";
            filter.insert(id);
        }
        id_to_song_map[id] = new_song;
    }

    //todo: maybe the user should be able to choose how these are sorted

    //make sure the songs list is sorted by file name
    all_song_ids = {filter.begin(),filter.end()};
    auto sortby = [&] (std::string& lhs,std::string& rhs){
        song& data_lhs {id_to_song_map[lhs]};
        song& data_rhs {id_to_song_map[rhs]};
        return data_lhs.full_path > data_rhs.full_path;

    };
    std::sort(all_song_ids.begin(),all_song_ids.end(),sortby);
    //reverse it so its not backwards for whatever reason
    std::reverse(all_song_ids.begin(),all_song_ids.end());

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
            return comet::player::match_search_string(current_player_search,entry_data.full_path);
        });
        return filtered_song_ids;
    }
    //if no search filter is in place, return all the songs
    return all_song_ids;
}


comet::song_manager::song_manager(comet::filesystem_manager& fsysmanager,comet::logger& logger) : fsysmanager(fsysmanager), lgr(logger){
    map_song_ids();
    public_song_ids = all_song_ids;
}