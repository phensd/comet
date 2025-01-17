#include "include/logger.h"
#include <filesystem>
#include <iostream>

bool comet::logger::log(const std::string& message,bool output_to_cerr){

    //just remove the file if its bigger than 10mb
    if(std::filesystem::exists(output_file_path) && std::filesystem::file_size(output_file_path) > 10000000) std::filesystem::remove(output_file_path);

    // std::ofstream output_file {output_file_path,std::ofstream::app};

    //this will make the window briefly flash to the terminal 
    //this is desirable (imo) when the song paths are being scanned, 
    //then the user can see what is going on if it is taking a while
    //otherwise not.
    //this is only used for that instance
    auto time_t = std::time(nullptr);
    if(output_to_cerr) std::cerr << "[comet " << std::put_time(std::localtime(&time_t),"%B %d %H:%M:%S") << "] "<< message << '\n';


    // output_file << message << '\n';

    // output_file.close();

    return true;
}

void comet::logger::pop_error_messages(){
    if(error_message_count() < 1) return;

    error_timer_current -= 0.1f;
    if(error_timer_current <= 0){
        error_queue.pop();
        error_timer_current = error_timer_max;
    }
}

//for displaying errors to the user on the bottom of the screen.
std::string comet::logger::error_message_topmost(){
    return error_message_count()  > 0 ? error_queue.front() : "";
};

size_t comet::logger::error_message_count(){
    return error_queue.size();
}

void comet::logger::push_error_message(std::string error_message){
    error_queue.push("ERROR: " + error_message);
}
comet::logger::logger(){
}