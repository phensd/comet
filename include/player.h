#pragma once
#include "../vendor/miniaudio.h"
#include <cstdio>
#include <vector>
#include "filesystem.h"
#include "logger.h"
#include "player_response_state.h"
#include "song_manager.h"
#include <map>

namespace  comet {
    class player {
        private:
            filesystem_manager& fsysmanager;
            song_manager& smanager;



            float volume {0.5f};

            float max_volume {1.0f};
            float min_volume {0.015f};

            void visually_select(std::vector<std::string>::iterator itr);



            ma_engine engine;
            ma_sound current_song;


            player_response_state current_response_state {player_response_state::PLAY_NEXT};

            std::map<player_response_state,std::string> map_player_state_to_string {
                {player_response_state::LOOP, "[Loop]"},  
                {player_response_state::SHUFFLE, "[Shuffle]"},  
                {player_response_state::PLAY_NEXT, ""},  
            };

            void set_play_button_text();

       
        public:
            std::string current_song_id{};
            std::string current_search {};
            int selected {};
            void play_next(logger& logger, bool forward=true);

            static bool match_search_string (std::string input, std::string to_match);


            std::string play_button_text {"➤"};





            void start_song(const std::vector<std::string>::iterator song_title_itr,logger& logger);

            void increase_volume(float value);

            float get_current_song_length_seconds() {float length; ma_sound_get_length_in_seconds(&current_song, &length); return length;}
            double get_current_timestamp_seconds() { return ma_sound_get_time_in_milliseconds(&current_song)/1000.0f; };


            void restart(logger& logger);

            void seek_percentage(float interval, bool forward);


            void active_refresh(std::string_view current_song_display,logger& logger,std::vector<std::string>& tab_values);
            void refresh_entries(logger& logger,bool rescan=false);

            void on_song_end(logger& logger);


            std::string get_state_message();

            //volume gauge needs this
            float get_max_volume(){return max_volume;}
            float get_volume() {return volume;}

            void apply_search_filter();

            void clear_search();

            void toggle_player_state(player_response_state desired_state);
            
            bool handle_play_button(logger& logger);
            //spacebar is our pause button
            bool handle_pause_button(logger& logger);

            bool current_selection_is_not_playing();

            //one liners will be defined here instead of in cpp file
            bool song_over() {return ma_sound_at_end(&current_song);}
            void pause_or_stop_song() {{ma_sound_stop(&current_song);}}
            void start_loaded_song() {ma_sound_start(&current_song);}
            bool song_playing() {{return !current_song_id.empty() ? ma_sound_is_playing(&current_song) : false;}}

            player(logger& logger,filesystem_manager& fsysmanager,class song_manager& song_manager);


            const player_response_state get_current_response_state() {return current_response_state;};

            //these should not exist since this class is basically a singleton
            player(const player& loan) = delete;
            player& operator=(const player& other) = delete;
            ~player(){ ma_engine_uninit(&engine);}




        
    };
}