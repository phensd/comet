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
#include "include/input.h"

using namespace ftxui;

int main() {

    

    comet::logger logger{};
    comet::player engine {logger};
    

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


    comet::register_main_inputs(tab_selected, song_selector, search_bar, main_window, engine, logger);

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

    comet::filesystem::write_user_path_entries("comet.json", engine.user_paths_entries);

    return 0;
}
