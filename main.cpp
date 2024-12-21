#define MINIAUDIO_IMPLEMENTATION
#include <cstdlib>
#include <ftxui/component/event.hpp>
#include <ftxui/component/mouse.hpp>
#include <ftxui/screen/color.hpp>
#include "include/player.h"
#include <string>  
#include "ftxui/component/component.hpp" 
#include "ftxui/component/component_base.hpp"      
#include "ftxui/component/screen_interactive.hpp"  
#include "ftxui/dom/elements.hpp"  
#include "include/logger.h"
#include <cctype>
#include "include/filesystem.h"

using namespace ftxui;

int main() {

    

    music_player::logger logger{};
    music_player::player engine {logger};
    

    auto screen = ScreenInteractive::Fullscreen();


    std::vector<std::string> tab_values{
      "Songs", //this is adjusted in real time in player.cpp/active_refresh to add a (X) where X is the number of songs
      "Paths",
    };

    int tab_selected {0};
    //menu that contains the tabs
    auto tab_menu = Toggle(&tab_values, &tab_selected);
    //the main song selector
    auto song_selector {Menu(&engine.public_song_entries,&engine.selected) | frame};



    auto create_input_box = [&engine] (std::string& content,std::string text) {
        auto input {Input(&content,text)};
        //make sure all input boxes refuse the enter key 
        //so user cant make multiple lines
        input |= CatchEvent([&](Event event) {
            return event == Event::Return;
        });

        return input;
    };


    auto user_paths = Container::Vertical({});
    for(int i {0}; i < 10; ++i){
        auto input_box {create_input_box(engine.user_paths_entries[i],"Enter a path...")};

        input_box |= CatchEvent([&](Event event) {
            if(event == Event::Return){
                engine.refresh_entries(logger);
                //if there is a search currently 
                //then the user might not see the affects of the list refreshing
                engine.clear_search();
                return true;
            }
            return false;
        });
        user_paths->Add(input_box);
    }
    //if I add the frame before I add the elements, for some reason, the loop above does not work.
    user_paths |= frame;



    //a search bar for filtering songs
    auto search_bar {create_input_box(engine.current_search,"Search...")};
    search_bar |=  CatchEvent([&] (Event event){

        if(search_bar->Focused()){
            engine.apply_search_filter();
        }

        if(search_bar->Focused() && event == Event::Delete){
            //clear the search
            engine.clear_search();
            //focus the song selector
            song_selector->TakeFocus();
            return true;
        }
        return false;
    });


    //container holding each tab
    auto tabs = Container::Tab({
        song_selector,
        user_paths,
        
    },&tab_selected);

    //container holding the whole gui
    //except for the gauges
    auto main_window = Container::Horizontal({
        tabs,
        tab_menu,
        search_bar
    });


    //main key inputs 
    main_window |= CatchEvent([&] (Event event){


        //Focusing search bar with "?"
        if(event == Event::Character('?')){
            search_bar->TakeFocus();
            return true;
        }


        //if pressing escape while searching, focus the song selector
        if(event == Event::Escape){
            if(search_bar->Focused()){
                song_selector->TakeFocus();
            }
            return true;
        }



        //only do these for the song selector page
        //and dont take them if search bar is being used
        if(tab_selected != 0 || search_bar->Active()) return false;


        //hold ctrl while using uparrows for faster navigation
        //note: it does not go out of index for some reason
        //so this is safe without any checks
        if(event == Event::ArrowUpCtrl){
            engine.selected -=2;
            return true;
        }

        if(event == Event::ArrowDownCtrl){
            engine.selected +=2;
            return true;
        }

        //even if the search bar isn't focused, clear it when pressing delete
        if(event == Event::Delete){
            engine.clear_search();
            return true;
        }


        //starting song
        if(event == Event::Return){
            //if there are no songs to play do nothing!!!!!
            if(engine.public_song_entries.size() < 1 ) return true;
            engine.start_song( engine.public_song_entries.begin() + engine.selected,logger);
            return true;
        }

        //restarting song
        if(event == Event::r){
            if(!engine.current_song_title.empty()) engine.restart(logger);
            return true;
        }

        //next song
        if(event == Event::Character('.')){
            engine.play_next(logger,true);
            return true;
        }
        //previous song
        if(event == Event::Character(',')){
            engine.play_next(logger,false);
            return true;
        }

        //toggling states
        //loop
        if(event == Event::l){
            engine.toggle_player_state(music_player::player_response_state::LOOP);
            return true;
        }
        //shuffle
        if(event == Event::s){
            engine.toggle_player_state(music_player::player_response_state::SHUFFLE);
            return true;
        }

        //pausing and unpausing (this is spacebar)
        if(event == Event::Character(' ')){
            //no song playing, do nothing
            if(engine.current_song_title.empty()) return true;
            //trying to unpause when a song is over will just restart it
            if(engine.song_over()) { engine.restart(logger); return true; };


            if(engine.song_playing()){
                engine.pause_or_stop_song();
            }else{
                engine.start_loaded_song();
            }
            return true;
        }

        //Refresh songs list
        if(event == Event::F4){
            engine.current_search = "";
            engine.refresh_entries(logger);
            logger.log("Refreshed songs list");
        }

        //dont let user seek if there is no data to seek
        if(!engine.current_song_title.empty()){
            //seek right
            if(event == Event::ArrowRight){
                engine.seek_percentage(0.025f,true);
                return true;
            }

            if(event == Event::ArrowRightCtrl){
                engine.seek_percentage(0.075f,true);
                return true;
            }

            //seek left
            if(event == Event::ArrowLeft){
                engine.seek_percentage(0.025f,false);
                return true;
            }

            if(event == Event::ArrowLeftCtrl){
                engine.seek_percentage(0.075f,false);
                return true;
            }
        }

    
        //volume controls
        if(event == Event::Character("=")){
            engine.increase_volume(0.07);
            return true;
        }
        if(event == Event::Character("-")){
            engine.increase_volume(-0.07);
            return true;
        }
        if(event == Event::Character("+")){
            engine.increase_volume(0.15);
            return true;
        }
        if(event == Event::Character("_")){
            engine.increase_volume(-0.15);
            return true;
        }

        return false;
    });

    //the main renderer/order of components
    auto component = Renderer(main_window, [&] {
        return vbox({
            vbox({
                hbox
                (
                    tab_menu->Render(),
                    separator(),
                    search_bar->Render()

                ),
                
                hbox(
                    border(tabs->Render() | flex),
                    border(gaugeUp(engine.get_volume()) | color(Color(182,193,253))) | size(HEIGHT,EQUAL,12)
                    
                ) | flex,
                
                hbox(
                    vbox(
                    
                        text(engine.get_state_message()),
                        text(engine.current_song_title),
                        border(gauge( (!engine.current_song_title.empty() ? engine.get_current_timestamp_seconds() / engine.get_current_song_length_seconds() : 0))  | color(Color(182,193,253) ))

                    ) | flex,
                    //make progress bar align (sorta) with end of the panels
                    separatorEmpty() | size(WIDTH,EQUAL,3)
                )
            })
        });
    });



    //makes the ui refresh constantly
    //https://arthursonzogni.github.io/FTXUI/examples_2component_2homescreen_8cpp-example.html#a72
    std::atomic<bool> refresh_ui_continue = true;
    std::thread refresh_ui([&] {
    while (refresh_ui_continue) {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(0.1s);

        //various things that need to refresh frequently 
        screen.Post( [&] { engine.active_refresh(engine.current_song_title, logger, tab_values);});
        screen.Post(Event::Custom);




    }
  });

    screen.Loop(component);
    refresh_ui_continue = false;
    refresh_ui.join();

    music_player::filesystem::write_user_path_entries("comet.json", engine.user_paths_entries);

    return 0;
}
