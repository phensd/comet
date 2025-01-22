#include "include/player.h"
#include "include/player_response_state.h"
#include "include/song_manager.h"
#include "include/util.h"
#include <algorithm>
#include <cctype>
#include <unistd.h>
#include "include/miniaudio_error_map.h"
void comet::player::start_song(std::string song_title,unsigned long long timestamp){
     //if a song is playing unload it
    if(!current_song_id.empty()){
        ma_sound_stop(&current_song);
        ma_sound_uninit(&current_song);
    } 


    
    ma_result result { ma_sound_init_from_file(&engine,
    smanager.id_to_song_map[(song_title).c_str()].full_path.c_str(),
                            MA_SOUND_FLAG_STREAM | MA_SOUND_FLAG_NO_SPATIALIZATION | MA_SOUND_FLAG_NO_PITCH ,
                            NULL,
                            NULL,
                            &current_song)};

    if(result == MA_SUCCESS){
        ma_sound_start(&current_song);
        ma_sound_seek_to_pcm_frame(&current_song,timestamp);
        current_song_id = song_title;
        //Make sure the song we started playing is selected in the player
        visually_select(song_title);
    }else{
        lgr.push_error_message(miniaudio_error_string.at(result));
        current_song_id = "";
    }

}



bool comet::player::try_song_loadback(){
    auto loadback {fsysmanager.get_song_loadback()};
    auto song_ids {smanager.get_all_song_ids()};
    if(loadback.empty()) return false;
    for(size_t i {0}; i < song_ids.size(); i++){
        if(smanager.id_to_song_map[song_ids.at(i)].full_path == loadback.path){
            start_song(*(i + song_ids.begin()),loadback.pcm_timestamp);
            //dont start playing the song instantly as the program opens
            handle_pause_button();
            return true;
        }
    }
    return false;
}


void comet::player::restart(){
    ma_sound_stop(&current_song); //this may be redundant
    ma_sound_seek_to_pcm_frame(&current_song,0);
    ma_sound_start(&current_song);

    lgr.log("Restarted song");
}

void comet::player::increase_volume(float value){
    volume += value;

    if(volume > max_volume) volume = max_volume;
    if(volume < 0.0f) volume = min_volume;
}

void comet::player::set_volume(double value){
    volume = value;

    if(volume > max_volume) volume = max_volume;
    if(volume < 0.0f) volume = min_volume;
}



void comet::player::seek_percentage(float interval, bool forward) {

    //early return if there is nothing to seek
    if(!song_playing()) return;

    ma_sound_stop(&current_song);

    ma_uint64 song_length {};
    ma_sound_get_length_in_pcm_frames(&current_song, &song_length);

    auto increase {song_length*interval};

    //if going back would set us to the beginning then just restart
    //so the next song doesn't start playing.
    if(ma_sound_get_time_in_pcm_frames(&current_song) - increase <= 0 && !forward) {
        restart();
        return;
    }
    //opposite, if going forward puts us at the end, just start the next song.
    if(ma_sound_get_time_in_pcm_frames(&current_song) + increase > song_length){
        play_next(true);
        return;
    }

    forward ? 
    ma_sound_seek_to_pcm_frame(&current_song,ma_sound_get_time_in_pcm_frames(&current_song) + (increase)):
    ma_sound_seek_to_pcm_frame(&current_song,ma_sound_get_time_in_pcm_frames(&current_song) - (increase));
    
    ma_sound_start(&current_song);


}


void comet::player::play_next(bool forward){

    std::vector<std::string>* song_ids;
    if(current_response_state == player_response_state::PLAY_NEXT || current_response_state == player_response_state::LOOP) song_ids = &smanager.public_song_ids;
    if(current_response_state == player_response_state::SHUFFLE) song_ids = &smanager.shuffled_song_ids;

    //if there are no songs to search through then return early and do nothing
    if(song_ids->size() < 1) return;

    auto itr {std::find(song_ids->begin(),song_ids->end(),current_song_id)};
    bool entry_found {itr != song_ids->end()};

    //if going forward..
    if(forward){
        //check if going one song forward would bring us to the end, if not then start the next song
        if(entry_found && (itr - song_ids->begin()) + 1 < song_ids->size()){
            start_song(*(itr + 1));
        }else{
            //if it does bring us to the end, loop back around to the beginning
            start_song(*song_ids->begin());
        }
    }else{
        //ditto but backwards, if going backwards would not bring us to the beginning of the list, then go backwards 
        if(entry_found && ( (itr - song_ids->begin()) - 1 ) >= 0){
            start_song(*(itr - 1));
        }else{
            //if it does, then loop back around to the end
            start_song(*(song_ids->end()-1));
        }

    }
}

void comet::player::on_song_end(){
    if(current_response_state == player_response_state::LOOP) restart();
    if(current_response_state == player_response_state::SHUFFLE || current_response_state == player_response_state::PLAY_NEXT) {
        play_next();
    }
}

void comet::player::set_play_button_text(){
    if(current_selection_is_not_playing() || !song_playing()){
        play_button_text = "➤";
    }else {
        play_button_text = "⏸";
    }

}

bool comet::player::handle_pause_button(){
    //no songs, return early
    if(smanager.public_song_ids.size() < 1 ) return true;
    //if there is nothing to pause, early return
    if(current_song_id.empty()) return true;
    //trying to unpause when a song is over will just restart it, early return
    if(song_over()) { restart(); return true; };
    if(song_playing()){
        pause_or_stop_song();
    }else{
        start_loaded_song();
    }
    return true;
}

bool comet::player::current_selection_is_not_playing(){
    return current_song_id != *(smanager.public_song_ids.begin() + selected);
}

bool comet::player::handle_play_button(){
    //no songs, return early
    if(smanager.public_song_ids.size() < 1 ) return true;
    //no song playing, play the currently highlighted song
    if(current_song_id.empty() || current_selection_is_not_playing()) {
        start_song( *(smanager.public_song_ids.begin() + selected));
        return true;
    } 

    return handle_pause_button();
}

//various things to update in real time
void comet::player::active_refresh(std::string_view current_song_display,std::vector<std::string>& tab_values){
    if(!current_song_display.empty()) {
        set_play_button_text();

        if(song_over()){
            on_song_end();
        }
        //update volume of song
        ma_sound_set_volume(&current_song,get_volume());
    };
    
    //display number of songs found in the tab
    tab_values[0] = (std::string) "Songs" + "(" + std::to_string(smanager.public_song_ids.size()) + ")";
}


void comet::player::refresh_entries(bool rescan,bool silent_log){
    smanager.map_song_ids(&current_song_id,rescan,silent_log);
    smanager.public_song_ids = smanager.get_filtered_entries(current_search);
    smanager.shuffled_song_ids = smanager.get_shuffled_selection();

}

void comet::player::apply_search_filter(){
    smanager.public_song_ids = smanager.get_filtered_entries(current_search);
    smanager.shuffled_song_ids = smanager.get_shuffled_selection();

    //if the user is searching make sure the current song id stays selected to make it easier to track
    if(current_search.size() >= 1) visually_select(current_song_id);
}

void comet::player::visually_select(std::string id){
    auto found {std::find(smanager.public_song_ids.begin(),smanager.public_song_ids.end(),id)};
    if(found != smanager.public_song_ids.end()) {
        selected = found - smanager.public_song_ids.begin();
    }else{
        //default to the top of the list if nothing was found
        selected = 0;
    }
}

void comet::player::clear_search(){
    //save what song the user had as a selection before we clear the search
    //if the current search resulted in an empty list, set the saved selection to the currently playing song
    std::string saved_search_selection {smanager.public_song_ids.size() > 0 ? smanager.public_song_ids[selected] : current_song_id};

    //clear the search input, then refresh the display
    current_search = "";
    smanager.public_song_ids = smanager.get_filtered_entries(current_search);
    //make sure the shuffled list is refreshed 
    smanager.shuffled_song_ids = smanager.get_shuffled_selection();

    //Make sure that when the search is cleared, we select the at-the-time selected song in the new context,
    //by finding the name of the song that was selected when the search was up, in the new list of songs.
    visually_select(saved_search_selection);

}

//takes in users search, splits it by spaces if they exist, then tries to match each part
//of the split with the song title
//if there are no spaces, it just matches the entire string with the title
//used to filter songs with the search bar
bool comet::player::match_search_string (std::string input, std::string to_match){
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


std::string comet::player::get_state_message(){

    //if nothing is selected say nothing
    if(current_song_id.empty()){
        return "";
    }

    std::string to_return{};

    //on the flip side, if something is selected to be played, but nothing is playing through mAudio, say the player is paused
    if(!current_song_id.empty() && !song_playing()){
        to_return = "Paused - ";
    }else {
        //otherwise, say the player is playing a song
        to_return = "Playing - ";
    }

    //add the time 
    to_return += util::create_timestamp_string(get_current_timestamp_seconds(), get_current_song_length_seconds());

    //add the current state (looping, shuffling) to the text
    to_return += " " + map_player_state_to_string[current_response_state];
    
    return to_return;
}

void comet::player::toggle_player_state(player_response_state desired_state){
    if(current_response_state == desired_state){
        current_response_state = player_response_state::PLAY_NEXT; //toggle back to default state
    }else{
        current_response_state = desired_state;
    }
}


comet::player::player(logger& logger, filesystem_manager& fsysmanager,class song_manager& smanager) : fsysmanager(fsysmanager),  smanager(smanager) , lgr(logger){
    ma_engine_init(NULL,&engine);
    logger.log(try_song_loadback() ? "Loaded previously playing song from JSON successfully - " + fsysmanager.get_song_loadback().path : "");
    this->lgr = logger;
}

comet::player::~player(){
    ma_engine_uninit(&engine);
    //save the current song so it can be loaded on boot
    if(!current_song_id.empty()){
        ma_uint64 timestamp {ma_sound_get_time_in_pcm_frames(&current_song)};
        fsysmanager.set_song_loadback({smanager.id_to_song_map[current_song_id].full_path,timestamp});
    };
    
}
