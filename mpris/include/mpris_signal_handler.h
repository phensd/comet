#pragma once
#include <functional>
#include <mutex>
#include <queue>
#include "../../include/player.h"

namespace comet {
    class mpris_signal_handler{


        public:
            enum class signal {
                PLAY,
                PLAY_NEXT,
                PREVIOUS,
                PLAY_PAUSE
            };
            
            //the main public function to be called to get signals and their functions.
            std::optional<std::function<void (player* player)>> pulse();
            void push_signal(signal signal);


        private:
            std::queue<signal> signals;

            //This class's methods will be accessed from multiple threads so we have a mutex just in case.
            std::mutex signals_mutex;


            std::function<void (player* player)> get_function_from_signal(signal signal);

    };
}