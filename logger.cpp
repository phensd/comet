#include "include/logger.h"
#include <filesystem>
#include <fstream>
#include <iostream>

bool comet::logger::log(const std::string& message,bool output_to_cerr){

    //just remove the file if its bigger than 10mb
    if(std::filesystem::exists(output_file_path) && std::filesystem::file_size(output_file_path) > 10000000) std::filesystem::remove(output_file_path);

    std::ofstream output_file {output_file_path,std::ofstream::app};

    //this will make the window briefly flash to the terminal 
    //this is desirable (imo) when the song paths are being scanned, 
    //then the user can see what is going on if it is taking a while
    //otherwise not.
    //this is only used for that instance
    if(output_to_cerr) std::cerr << message << '\n';


    output_file << message << '\n';

    output_file.close();

    return true;
}

comet::logger::logger(){
}