#include "include/util.h"

std::string music_player::util::lower (std::string input) {
    for(int i {0}; i < input.size(); ++i) {
        input[i] = std::tolower(input[i]);
    }
    return input;
};

std::vector<std::string> music_player::util::split_string(std::string input){

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