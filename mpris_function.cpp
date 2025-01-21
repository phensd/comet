#include "include/mpris_function.h"

void comet::mpris_function::mpris_function_play(player *player){
    //"If CanPlay is false, attempting to call this method should have no effect."    
    //todo

    //"If there is no track to play, this has no effect."
    if(player->get_song_manager().public_song_ids.size() < 1 ) return;

    //Starts or resumes playback.
    if(player->current_song_id.empty() || player->current_selection_is_not_playing()) {
        auto index {player->get_song_manager().public_song_ids.begin() + player->selected };
        player->start_song(*index);
        return;
    } 

    //"If already playing, this has no effect.""
    if(player->song_playing()){
        return;
    }else{
    //"If paused, playback resumes from the current position."
        player->start_loaded_song();
    }

    return;

}

void comet::mpris_function::mpris_function_play_or_pause(player *player){
    //this happens to fit.
    player->handle_pause_button();
}

void comet::mpris_function::mpris_function_play_next(player *player){
    player->play_next(true);
}

void comet::mpris_function::mpris_function_previous(player *player){
    player->play_next(false);
}