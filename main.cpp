#include <ftxui/component/component_options.hpp>
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
    comet::filesystem_manager fsysmanager {logger};
    comet::player engine {logger,fsysmanager};

    

    auto screen = ScreenInteractive::Fullscreen();

    

    std::vector<std::string> tab_values{
      "Songs", //this is adjusted in real time in player.cpp/active_refresh to add a (X) where X is the number of songs
      "Options",
    }; int tab_selected {0};
    auto tab_menu = Toggle(&tab_values, &tab_selected);


    //the main song selector
    auto song_selector {Menu(&engine.public_song_entries,&engine.selected) | frame};


    std::vector<std::string> song_title_display_options{
        "File name",
        "Tagged name",
        "Full path",
    };int song_title_display_option_selected {0};
    auto song_title_display_toggle = Toggle(&song_title_display_options, &song_title_display_option_selected);


    //temporarily binding the function like this until i refactor later
    auto button_func = [&logger,&engine] () {engine.refresh_entries(logger);};
    auto refresh_entries_button {Button("[Refresh Entries]",button_func,ButtonOption::Ascii())};


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
        auto input_box {create_input_box(fsysmanager.user_paths_entries[i],"Enter a path...")};

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
        user_paths
        
    },&tab_selected);

    //container holding the whole gui
    //except for the gauges
    auto main_window = Container::Horizontal({
        tabs,
        tab_menu,
        search_bar,
        song_title_display_toggle,
        refresh_entries_button
    });


    comet::register_main_inputs(tab_selected, song_selector, search_bar, main_window, engine, logger);

    //the main renderer/order of components
    auto component = Renderer(main_window, [&] {
        if(tab_selected == 0){
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
        }

        if(tab_selected == 1){
            return vbox({
                vbox({
                    hbox
                    (
                        tab_menu->Render(),
                        separator(),
                        search_bar->Render()

                    ),
                    
                    vbox(
                        window(text("Paths for song searches"),user_paths->Render() | frame),
                        hbox (
                            text("* Display songs by their : "),
                            song_title_display_toggle->Render() | flex,
                            refresh_entries_button->Render()
                        )

                        
                    ) | flex,
                    
                })
            });
        }
        //this is here for now to get the compiler to shut up about no return in non void function
        return vbox({});
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


    return 0;
}
