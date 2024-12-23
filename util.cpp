#include "include/util.h"
#include <format>
#include <sstream>
#include <iomanip>

std::string comet::util::create_timestamp_string(double current_timestamp_seconds, float current_song_length_seconds){

    std::string to_return;

    //using string stream for std::fill and std::width
    std::stringstream seconds_lhs{};
    seconds_lhs << std::setfill('0') << std::setw(2) << (int) current_timestamp_seconds % 60;
    to_return += std::format("{}:{}",std::to_string((current_timestamp_seconds / 60))[0],seconds_lhs.str());

    to_return += "/";
    
    std::stringstream seconds_rhs{};
    seconds_rhs << std::setfill('0') << std::setw(2) << (int) current_song_length_seconds % 60;
    to_return += std::format("{}:{}",std::to_string((current_song_length_seconds / 60))[0],seconds_rhs.str());


    return to_return;
}


std::string comet::util::lower (std::string input) {
    for(int i {0}; i < input.size(); ++i) {
        input[i] = std::tolower(input[i]);
    }
    return input;
};

std::vector<std::string> comet::util::split_string(std::string input){

    std::vector<std::string> split_strings{};
    
    if(input.find_first_of(" ",0) != 0){
        split_strings.push_back(input.substr(0,input.find_first_of(" ",0)));
    }
    
    for(size_t i{0}; i < input.size(); ++i){
        if(std::isspace(input[i])){
            //if there is another space ahead or we are at the end then continue
            if(std::isspace(input[i+1]) || i+1 == input.size()) continue;

            
            auto size {input.find_first_of(" ",i+1) - (i+1)};
            auto output {input.substr(i+1,size)};
        
            split_strings.push_back(output);    
            
        }
    }
    return split_strings;
}