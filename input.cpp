#include "include/input.h"
#include "include/player.h"
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/event.hpp>
#include "ftxui/component/component.hpp" 
#include "include/song_manager.h"



void comet::register_main_inputs(int& tab_selected, 
                                  ftxui::Component& song_selector,
                                  ftxui::Component& search_bar,
                                  ftxui::Component& main_window,
                                  comet::player& engine,
                                  comet::song_manager& song_manager,
                                  logger& logger){
    //main key inputs 
    main_window |= ftxui::CatchEvent([&] (ftxui::Event event){

        //Focusing search bar with "?"
        if(event == ftxui::Event::Character('?')){
            search_bar->TakeFocus();
            return true;
        }


        //if pressing escape while searching, focus the song selector
        if(event == ftxui::Event::Escape){
            if(search_bar->Focused()){
                song_selector->TakeFocus();
            }else {
                engine.visually_select(engine.current_song_id);
            }
            return true;
        }



        //only do these for the song selector page
        //and dont take them if search bar is being used
        if(tab_selected != 0 || search_bar->Active()) return false;


        //hold ctrl while using uparrows for faster navigation
        //note: it does not go out of index for some reason
        //so this is safe without any checks
        if(event == ftxui::Event::ArrowUpCtrl){
            engine.selected -=2;
            return true;
        }

        if(event == ftxui::Event::ArrowDownCtrl){
            engine.selected +=2;
            return true;
        }

        //even if the search bar isn't focused, clear it when pressing delete
        if(event == ftxui::Event::Delete){
            engine.clear_search();
            return true;
        }


        //starting song
        if(event == ftxui::Event::Return){
            engine.handle_play_button();
            return true;
        }

        //restarting song
        if(event == ftxui::Event::r){
            if(!engine.current_song_id.empty()) engine.restart();
            return true;
        }

        //next song
        if(event == ftxui::Event::Character('.')){
            engine.play_next(true);
            return true;
        }
        //previous song
        if(event == ftxui::Event::Character(',')){
            engine.play_next(false);
            return true;
        }

        //toggling states
        //loop
        if(event == ftxui::Event::l){
            engine.toggle_player_state(comet::player_response_state::LOOP);
            return true;
        }
        //shuffle
        if(event == ftxui::Event::s){
            engine.toggle_player_state(comet::player_response_state::SHUFFLE);
            return true;
        }

        //pausing and unpausing (this is spacebar)
        if(event == ftxui::Event::Character(' ')){
            return engine.handle_pause_button();
        }

        //Refresh songs list
        if(event == ftxui::Event::F4){
            engine.current_search = "";
            engine.refresh_entries();
            logger.log("Refreshed songs list");
        }

        //dont let user seek if there is no data to seek
        if(!engine.current_song_id.empty()){
            //seek right
            if(event == ftxui::Event::ArrowRight){
                engine.seek_percentage(0.025f,true);
                return true;
            }

            if(event == ftxui::Event::ArrowRightCtrl){
                engine.seek_percentage(0.075f,true);
                return true;
            }

            //seek left
            if(event == ftxui::Event::ArrowLeft){
                engine.seek_percentage(0.025f,false);
                return true;
            }

            if(event == ftxui::Event::ArrowLeftCtrl){
                engine.seek_percentage(0.075f,false);
                return true;
            }
        }

    
        //volume controls
        if(event == ftxui::Event::Character("=")){
            engine.increase_volume(0.07);
            return true;
        }
        if(event == ftxui::Event::Character("-")){
            engine.increase_volume(-0.07);
            return true;
        }
        if(event == ftxui::Event::Character("+")){
            engine.increase_volume(0.15);
            return true;
        }
        if(event == ftxui::Event::Character("_")){
            engine.increase_volume(-0.15);
            return true;
        }

        return false;
    });


}