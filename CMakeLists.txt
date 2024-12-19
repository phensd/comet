cmake_minimum_required (VERSION 3.11)


include(FetchContent)


set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG main 
)
FetchContent_MakeAvailable(ftxui)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)


project(music_player CXX)
find_package(ftxui 5 REQUIRED)


add_executable(music_player filesystem.cpp main.cpp player.cpp logger.cpp util.cpp)



target_link_libraries(music_player
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component 
  PRIVATE nlohmann_json::nlohmann_json
)

