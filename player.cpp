#include "include/player.h"
#include "include/song_manager.h"
#include "include/util.h"
#include <algorithm>
#include <cctype>
#include <random>

void comet::player::start_song(std::vector<std::string>::iterator song_title_itr, logger& logger){
     //if a song is playing unload it
    if(!current_song_title.empty()){
        ma_sound_stop(&current_song);
        ma_sound_uninit(&current_song);
    } 

    current_song_title = *song_title_itr;

    ma_sound_init_from_file(&engine, smanager.id_to_song_map[(*song_title_itr).c_str()].full_path.c_str(), MA_SOUND_FLAG_STREAM | MA_SOUND_FLAG_NO_SPATIALIZATION | MA_SOUND_FLAG_NO_PITCH ,NULL,NULL,&current_song);
    ma_sound_start(&current_song);

    //Make sure the song we started playing is selected in the player
    visually_select(song_title_itr);
}


void comet::player::restart(logger& logger){
    ma_sound_stop(&current_song); //this may be redundant
    ma_sound_seek_to_pcm_frame(&current_song,0);
    ma_sound_start(&current_song);

    logger.log("Restarted song");
}

void comet::player::increase_volume(float value){
    volume += value;

    if(volume > max_volume) volume = max_volume;
    if(volume < 0.0f) volume = min_volume;
}




void comet::player::seek_percentage(float interval, bool forward) {

    ma_sound_stop(&current_song);

    ma_uint64 length {};
    ma_sound_get_length_in_pcm_frames(&current_song, &length);

    forward ? 
    ma_sound_seek_to_pcm_frame(&current_song,ma_sound_get_time_in_pcm_frames(&current_song) + (length*interval)):
    ma_sound_seek_to_pcm_frame(&current_song,ma_sound_get_time_in_pcm_frames(&current_song) - (length*interval));
    
    ma_sound_start(&current_song);


}

std::vector<std::string>::iterator comet::player::get_and_select_random_song(){
    static std::random_device dev;
    static std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0,smanager.public_song_ids.size()); 

    auto num {dist(rng)};
    selected = num;
    return smanager.public_song_ids.begin() + num;

}


void comet::player::play_next(logger& logger, bool forward){

    auto itr {std::find(smanager.public_song_ids.begin(),smanager.public_song_ids.end(),current_song_title)};
    bool entry_found {itr != smanager.public_song_ids.end()};

    //if going forward..
    if(forward){
        //check if going one song forward would bring us to the end, if not then start the next song
        if(entry_found && (itr - smanager.public_song_ids.begin()) + 1 < smanager.public_song_ids.size()){
            start_song(itr + 1,logger);
        }else{
            //if it does bring us to the end, loop back around to the beginning
            start_song(smanager.public_song_ids.begin(),logger);
        }
    }else{
        //ditto but backwards, if going backwards would not bring us to the beginning of the list, then go backwards 
        if(entry_found && ( (itr - smanager.public_song_ids.begin()) - 1 ) >= 0){
            start_song(itr - 1,logger);
        }else{
            //if it does, then loop back around to the end
            start_song(smanager.public_song_ids.end()-1,logger);
        }

    }
}

void comet::player::on_song_end(logger& logger){
    if(current_response_state == player_response_state::LOOP) restart(logger);
    if(current_response_state == player_response_state::SHUFFLE) start_song(get_and_select_random_song(),logger);
    if(current_response_state == player_response_state::PLAY_NEXT) play_next(logger);
}

//various things to update in real time
void comet::player::active_refresh(std::string_view current_song_display,logger& logger,std::vector<std::string>& tab_values){
    if(!current_song_display.empty()) {
        if(song_over()){
            on_song_end(logger);
        }
        //update volume of song
        ma_sound_set_volume(&current_song,get_volume());
    };


    //display number of songs found in the tab
    tab_values[0] = (std::string) "Songs" + "(" + std::to_string(smanager.public_song_ids.size()) + ")";
}


void comet::player::refresh_entries(logger& logger){
    smanager.map_song_ids();
    smanager.public_song_ids = smanager.get_filtered_entries(current_search);

}

void comet::player::apply_search_filter(){
    smanager.public_song_ids = smanager.get_filtered_entries(current_search);
}

void comet::player::clear_search(){
    //save what song the user had as a selection before we clear the search
    //if the current search resulted in an empty list, set the saved selection to the first element of the all-songs list
    std::string saved_search_selection {smanager.public_song_ids.size() > 0 ? smanager.public_song_ids[selected] : smanager.get_all_song_ids()[0]};

    //clear the search input, then refresh the display
    current_search = "";
    smanager.public_song_ids = smanager.get_filtered_entries(current_search);


    //Make sure that when the search is cleared, we select the at-the-time selected song in the new context,
    //by finding the name of the song that was selected when the search was up, in the new list of songs.
    visually_select(std::find(smanager.public_song_ids.begin(),smanager.public_song_ids.end(),saved_search_selection));

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
    if(current_song_title.empty()){
        return "";
    }

    std::string to_return{};

    //on the flip side, if something is selected to be played, but nothing is playing through mAudio, say the player is paused
    if(!current_song_title.empty() && !song_playing()){
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


comet::player::player(logger& logger,filesystem_manager& fsysmanager,class song_manager& smanager) : fsysmanager(fsysmanager),  smanager(smanager){


    ma_result result;
    ma_engine_init(NULL,&engine);


}
