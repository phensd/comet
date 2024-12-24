#include "include/song_manager.h"
#include "include/filesystem.h"
#include <algorithm>
#include "include/player.h"
void comet::song_manager::map_song_ids(){
    all_song_ids.clear();
    for(std::filesystem::path& path : fsysmanager.find_song_entries(lgr)){
        song new_song{};

        new_song.file_name = path.filename();
        new_song.full_path = path.c_str();
        //todo new_song.name_pretty = 


        //todo: take into account how the user wants the songs displayed, for now just use filename
        std::string id {path.filename()};
        id_to_song_map[id] = new_song;

        all_song_ids.push_back(id);
    }

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