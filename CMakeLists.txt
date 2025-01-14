cmake_minimum_required (VERSION 3.11)


include(FetchContent)


set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG main 
  EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(ftxui)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz EXCLUDE_FROM_ALL)
FetchContent_MakeAvailable(json)


FetchContent_Declare(TagLib URL https://github.com/taglib/taglib/releases/download/v2.0.2/taglib-2.0.2.tar.gz EXCLUDE_FROM_ALL)
FetchContent_MakeAvailable(TagLib)


project(comet CXX C)

find_package(PkgConfig REQUIRED)
find_package(ftxui 5 REQUIRED)
find_package(ZLIB)
find_package(TagLib 2.0.0 REQUIRED)
pkg_check_modules(GLIB glib-2.0 gio-2.0 REQUIRED)

#Generates the gdbus boilerplate files when running cmake.
execute_process(COMMAND bash -c "gdbus-codegen --generate-c-code mpris --c-namespace comet --output-directory ../dbus --interface-prefix org.mpris. ../dbus/service.xml")

#Ditto, but is placed into the makefile and executes when running make.
add_custom_target(generate_dbus_boilerplate ALL COMMAND bash -c "gdbus-codegen --generate-c-code mpris --c-namespace comet --output-directory ../dbus --interface-prefix org.mpris. ../dbus/service.xml")

add_executable(comet 
              filesystem.cpp 
              main.cpp 
              player.cpp
              logger.cpp
              util.cpp
              input.cpp
              song_manager.cpp
              ../dbus/mpris.c
              mpris_handler.cpp
              )






target_include_directories(comet PUBLIC ${GLIB_INCLUDE_DIRS})


target_link_libraries(comet
  PUBLIC ftxui::screen
  PUBLIC ftxui::dom
  PUBLIC ftxui::component 
  PUBLIC nlohmann_json::nlohmann_json
  PUBLIC TagLib::tag 
  PUBLIC ${GLIB_LDFLAGS}
)




set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Wfatal-errors")
set(CMAKE_CXX_FLAGS_DEBUG "-ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -s")

set_property(TARGET comet PROPERTY CXX_STANDARD 20)
install(TARGETS comet RUNTIME DESTINATION bin)