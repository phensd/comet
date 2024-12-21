#pragma once
#include <ftxui/component/component_base.hpp>
#include "logger.h"
#include "player.h"

namespace music_player{
void register_main_inputs(int& tab_selected, 
                    ftxui::Component& song_selector,
                    ftxui::Component& search_bar,
                    ftxui::Component& main_window,
                    player& engine,
                    logger& logger);
}