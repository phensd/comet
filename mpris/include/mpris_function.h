#pragma once
#include "../../include/player.h"


namespace comet::mpris_function{
    void mpris_function_play(player* player);
    void mpris_function_play_or_pause(player* player);
    void mpris_function_play_next(player* player);
    void mpris_function_previous(player* player);
}