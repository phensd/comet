#include "include/player.h"
#include "include/util.h"
#include <algorithm>
#include <cctype>
#include <iterator>
#include "include/filesystem.h"

void music_player::player::load_song_into_current(std::string file_path,logger& logger){
    ma_sound_init_from_file(&this->engine, file_path.c_str(), MA_SOUND_FLAG_STREAM,NULL,NULL,&this->current_song);
    ma_sound_get_length_in_seconds(&this->current_song,&current_song_length_seconds);

    std::string message {"Loaded song " + file_path + " --- Length (seconds) " + std::to_string(current_song_length_seconds)};
    logger.log(message);
}


void music_player::player::start_song(std::string file_path, ma_sound& song,logger&logger){
    load_song_into_current(file_path,logger);
    ma_sound_start(&song);
}

void music_player::player::restart(ma_sound& song,logger& logger){
    ma_sound_stop(&song);
    ma_sound_seek_to_pcm_frame(&song,0);
    ma_sound_start(&song);

    logger.log("Restarted song");
}

void music_player::player::increase_volume(float value){
    volume += value;

    if(volume > max_volume) volume = max_volume;
    if(volume < 0.0f) volume = min_volume;
}




void music_player::player::seek_percentage(ma_sound& song, float interval, bool forward) {

    ma_sound_stop(&song);

    ma_uint64 length {};
    ma_sound_get_length_in_pcm_frames(&song, &length);

    forward ? 
    ma_sound_seek_to_pcm_frame(&song,ma_sound_get_time_in_pcm_frames(&song) + (length*interval)):
    ma_sound_seek_to_pcm_frame(&song,ma_sound_get_time_in_pcm_frames(&song) - (length*interval));
    
    ma_sound_start(&song);


}

//various things to update in real time
void music_player::player::active_refresh(std::string_view current_song_display,logger& logger,std::vector<std::string>& tab_values){
    
    if(!current_song_display.empty()) {
        //loop songs by default for now
        if(ma_sound_at_end(&current_song)){
            restart(current_song,logger);
        }
        //update volume of song
        ma_sound_set_volume(&current_song,get_volume());
    };


    //display number of songs found in the tab
    tab_values[0] = (std::string) "Songs" + "(" + std::to_string(public_song_entries.size()) + ")";

}

music_player::player::~player(){
    ma_engine_uninit(&engine);
}

void music_player::player::refresh_entries(logger& logger){
    all_song_entries = filesystem::find_song_entries(user_paths_entries,logger);
    public_song_entries = get_filtered_entries();

}

void music_player::player::apply_search_filter(){
    public_song_entries = get_filtered_entries();
}

void music_player::player::clear_search(){

    //save what song the user had as a selection before we clear the search
    //if the current search resulted in an empty list, set the saved selection to the first element of the all-songs list
    std::string saved_search_selection {public_song_entries.size() > 0 ? public_song_entries[selected] : all_song_entries[0]};

    //clear the search input, then refresh the display
    current_search = "";
    public_song_entries = get_filtered_entries();

    //After we clear the search, find the song that was previously selected in the new refreshed list and
    //set the current selection to it
    auto itr {std::find(public_song_entries.begin(),public_song_entries.end(),saved_search_selection)};
    if(itr != public_song_entries.end()){
        selected = (itr - public_song_entries.begin());
    }
}

bool music_player::player::match_search_string (std::string input, std::string to_match){
    //if there is spaces in the search, match each segment of the search (split by spaces) individually
    if(input.find(" ") != std::string::npos){
        auto splits {util::split_string(input)};
        for(auto & split : splits){
            if (util::lower(to_match).find(util::lower(split)) == std::string::npos){
                return false;
            }
        }
    }else {
        //otherwise match the entire string to the input
        return (util::lower(to_match).find(util::lower(input)) != std::string::npos);
    }
    return true;
}


std::vector<std::string>& music_player::player::get_filtered_entries(){
    if(current_search.length() > 0){

        filtered_entries.clear();

        std::copy_if(all_song_entries.begin(),all_song_entries.end(),std::back_inserter(filtered_entries),
        [this] (std::string& entry){
            return match_search_string(current_search,entry);
        });

        return filtered_entries;
    }
    //if no search filter is in place, return all the songs
    return all_song_entries;
}


std::vector<std::string> music_player::player::get_default_path_entries(){

    std::vector<std::string> defaults{};

    //push a default /home/user/music path to the list
    defaults.emplace_back((std::string) getenv("HOME") + "/Music");

    //the default empty path slots
    for(int i{0}; i < 9; ++i){
        defaults.emplace_back("");
    }

    return defaults;
}

music_player::player::player(logger& logger){

    ma_result result;
    ma_engine_init(NULL,&engine);
    

    if(filesystem::saved_json_exists("comet.json")){
        //if the json fails to parse and returns nothing then use the default entries
        user_paths_entries = filesystem::load_user_path_entries("comet.json",logger).value_or(get_default_path_entries());
    }else {
        //if the json file doesnt exist then use the default entries too
        user_paths_entries = get_default_path_entries();
    }
    


    //fill out the song entries
    all_song_entries = filesystem::find_song_entries(user_paths_entries,logger);


    //set up the song entries
    public_song_entries = get_filtered_entries();

}
