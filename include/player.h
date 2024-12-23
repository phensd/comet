#pragma once
#include "../vendor/miniaudio.h"
#include <vector>
#include "logger.h"
#include "player_response_state.h"
#include <map>

namespace  comet {
    class player {
        private:
            float volume {0.5f};

            float max_volume {1.0f};
            float min_volume {0.015f};

            //"all song entries" contains every song entry found
            //no matter what, while
            //"filtered entries" is used to categorize search results
            //the public facing one (public_song_entries)
            //switches between pointing to these two
            std::vector<std::string> all_song_entries {};
            std::vector<std::string> filtered_entries {};

            std::vector<std::string>& get_filtered_entries();
            bool match_search_string (std::string input, std::string to_match);

            std::vector<std::string> get_default_path_entries();

            std::vector<std::string>::iterator get_and_select_random_song();


            //since this is a one liner now we can put it in the header by my own standards
            void visually_select(std::vector<std::string>::iterator itr) {selected = itr - public_song_entries.begin();}



            ma_engine engine;
            ma_sound current_song;


            player_response_state current_response_state {player_response_state::PLAY_NEXT};

            std::map<player_response_state,std::string> map_player_state_to_string {
                {player_response_state::LOOP, "[Loop]"},  
                {player_response_state::SHUFFLE, "[Shuffle]"},  
                {player_response_state::PLAY_NEXT, ""},  
            };

       
        public:
            std::string current_song_title{};
            std::string current_search {};
            int selected {};
            void play_next(logger& logger, bool forward=true);


            std::vector<std::string> user_paths_entries {};

            std::vector<std::string> public_song_entries{};


            void start_song(std::vector<std::string>::iterator song_title_itr,logger& logger);

            void increase_volume(float value);

            float get_current_song_length_seconds() {float length; ma_sound_get_length_in_seconds(&current_song, &length); return length;}
            double get_current_timestamp_seconds() { return ma_sound_get_time_in_milliseconds(&current_song)/1000.0f; };


            void restart(logger& logger);

            void seek_percentage(float interval, bool forward);


            void active_refresh(std::string_view current_song_display,logger& logger,std::vector<std::string>& tab_values);
            void refresh_entries(logger& logger);

            void on_song_end(logger& logger);


            std::string get_state_message();

            //volume gauge needs this
            float get_max_volume(){return max_volume;}
            float get_volume() {return volume;}

            void apply_search_filter();

            void clear_search();

            void toggle_player_state(player_response_state desired_state);


            //one liners will be defined here instead of in cpp file
            bool song_over() {return ma_sound_at_end(&current_song);}
            void pause_or_stop_song() {{ma_sound_stop(&current_song);}}
            void start_loaded_song() {ma_sound_start(&current_song);}
            bool song_playing() {{return ma_sound_is_playing(&current_song);}}

            player(logger& logger);

            //these should not exist since this class is basically a singleton
            player(const player& loan) = delete;
            player& operator=(const player& other) = delete;
            ~player(){ ma_engine_uninit(&engine);}




        
    };
}