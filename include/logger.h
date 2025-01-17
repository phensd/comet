#pragma once
#include <queue>
#include <string>

namespace comet {
    class logger {

        std::string output_file_path{};

        std::queue<std::string> error_queue;
        float error_timer_max {(7.5f)};
        float error_timer_current {error_timer_max};




        public:
            bool log(const std::string& message,bool output_to_cerr=false);
            logger();

            //for displaying errors to the user on the bottom of the screen.
            std::string error_message_topmost();
            void push_error_message(std::string error_message);
            size_t error_message_count();
            void pop_error_messages();
    };
}