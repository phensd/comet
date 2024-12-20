#pragma once
#include "../vendor/miniaudio.h"
#include <vector>
#include "logger.h"
#include <map>

namespace  music_player {

    struct player {

        enum class player_response_state {
            LOOP,
            SHUFFLE,
            DO_NOTHING_WHEN_SONG_IS_OVER
        };

        player_response_state current_response_state {player::player_response_state::LOOP};


        ma_engine engine;
        ma_sound current_song;


        std::string current_song_title{};
        std::string current_search {};
        int selected {};


        std::vector<std::string> user_paths_entries {};

        std::vector<std::string> public_song_entries{};
       


        float current_song_stamp_milliseconds(ma_sound& song);
        void start_song(std::string file_path,ma_sound& song,logger& logger);
        void load_song_into_current(std::string file_path,logger& logger);

        void increase_volume(float value);
        float get_current_song_length_seconds() {return current_song_length_seconds;}

        void restart(ma_sound& song,logger& logger);

        void seek_percentage(ma_sound& song, float interval, bool forward);


        void active_refresh(std::string_view current_song_display,logger& logger,std::vector<std::string>& tab_values);
        void refresh_entries(logger& logger);

        std::string get_state_message();



        //volume gauge needs this
        float get_max_volume(){return max_volume;}
        float get_volume() {return volume;}

        void apply_search_filter();

        void clear_search();

        void toggle_player_state(player_response_state desired_state);

        player(logger& logger);
        ~player();

        private:
            float volume {0.5f};
            float current_song_length_seconds {};

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



            std::map<player_response_state,std::string> map_player_state_to_string {
                {player_response_state::LOOP, "[Loop]"},  
                {player_response_state::SHUFFLE, "[Shuffle]"},  
                {player_response_state::DO_NOTHING_WHEN_SONG_IS_OVER, ""},  
            };





    };

        
}
