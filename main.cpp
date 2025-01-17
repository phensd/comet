#define MINIAUDIO_IMPLEMENTATION
#include "include/mpris_handler.h"
#include "include/song_manager.h"
#include <ftxui/component/component_options.hpp>
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
    comet::song_manager song_manager{fsysmanager,logger};
    comet::player engine {logger,fsysmanager,song_manager};
    comet::mpris_handler mpris_handler {&engine};
    
    auto screen = ScreenInteractive::Fullscreen();


    std::vector<std::string> tab_values{
      "Songs", //this is adjusted in real time in player.cpp/active_refresh to add a (X) where X is the number of songs
      "Options",
    }; int tab_selected {0};
    auto tab_menu = Toggle(&tab_values, &tab_selected);



    //the main song selector
    auto song_selector {Menu(&song_manager.public_song_ids,&engine.selected) | frame};
    // auto song_selector {Collapsible("All songs",song_selector_in) | frame};


    //create the menu option manually so we can modify the on_change callback
    MenuOption song_title_display_options {MenuOption::Toggle()};
    
    //function for when the display option selection is changed
    song_title_display_options.on_change = [&] (void){
        //refresh entries, do not rescan directories
        engine.refresh_entries(false,true);
        //save the current option in the filesystem manager so it can be saved to the JSON
        fsysmanager.saved_song_display_selection = *(song_manager.song_title_display_options.begin() + song_manager.song_title_display_option_selected);
    };

    //"Maybe" decorator forces them to only be interactable on specific tabs of the player
    auto song_title_display_toggle = Menu(&song_manager.song_title_display_options, &song_manager.song_title_display_option_selected,song_title_display_options) | Maybe([&] {return tab_selected == 1;});

    auto refresh_entries_button {Button("[Scan Directories]",[&engine]{engine.refresh_entries(true);},ButtonOption::Ascii()) | Maybe([&] {return tab_selected == 1;})};

    //this is just the ascii button option but with the transform set to not add any label upon being focused
    ButtonOption ascii_no_transform = ButtonOption::Ascii();
    ascii_no_transform.transform = [](const EntryState& s) {return text(" " + s.label + " ");};
    //https://arthursonzogni.com/FTXUI/doc/component__options_8cpp_source.html (line 165)
    auto play_button {Button(&engine.play_button_text,[&engine](){engine.handle_play_button();},ascii_no_transform) | Maybe( [&] {return tab_selected == 0;})};
    auto next_song_forward_button {Button("⏭",[&engine](){engine.play_next();},ascii_no_transform) | Maybe([&] {return tab_selected == 0;})};
    auto next_song_backward_button {Button("⏮",[&engine](){engine.play_next(false);},ascii_no_transform) | Maybe([&] {return tab_selected == 0;})};


    auto volume_up_button {Button("+",[&engine](){engine.increase_volume(0.20);},ButtonOption::Ascii()) | Maybe([&] {return tab_selected == 0;})};
    auto volume_down_button {Button("-",[&engine](){engine.increase_volume(-0.20);},ButtonOption::Ascii()) | Maybe([&] {return tab_selected == 0;})};


    auto seek_forward_button {Button("↻",[&engine](){engine.seek_percentage(0.04,true);},ascii_no_transform) | Maybe([&] {return tab_selected == 0;})};
    auto seek_backward_button {Button("↺",[&engine](){engine.seek_percentage(0.04,false);},ascii_no_transform) | Maybe([&] {return tab_selected == 0;})};



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
                engine.refresh_entries(true);
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
        refresh_entries_button,
        play_button,
        next_song_forward_button,
        next_song_backward_button,
        volume_up_button,
        volume_down_button,
        seek_forward_button,
        seek_backward_button,
    });


    comet::register_main_inputs(tab_selected, song_selector, search_bar, main_window, engine, song_manager, logger);

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
                        vbox(
                            volume_up_button->Render(),
                            border(gaugeUp(engine.get_volume()) | color(Color(182,193,253))) | size(HEIGHT,EQUAL,12),
                            volume_down_button->Render()
                        )
                        
                    ) | flex,
                    
                    hbox(
                        vbox(
                            text(engine.get_state_message()),
                            hbox(
                                text(engine.current_song_id) | flex,
                                next_song_backward_button->Render(),
                                seek_backward_button->Render(),
                                play_button->Render(),
                                seek_forward_button->Render(),
                                next_song_forward_button->Render()
                            ),
                            border(gauge( (!engine.current_song_id.empty() ? engine.get_current_timestamp_seconds() / engine.get_current_song_length_seconds() : 0))  | color(Color(182,193,253))) | size(HEIGHT,EQUAL,3),
                            text(logger.error_message_topmost())

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
        screen.Post( [&] { engine.active_refresh(engine.current_song_id,tab_values);});
        screen.Post(Event::Custom);
    }
  });
    

        
    std::thread run_mpris_handler([&] { mpris_handler.run();});

    screen.Loop(component);
    refresh_ui_continue = false;

    mpris_handler.stop();
    run_mpris_handler.join();

    refresh_ui.join();


    return 0;
}
