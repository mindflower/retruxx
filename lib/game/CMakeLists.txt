cmake_minimum_required(VERSION 3.13)

add_subdirectory(impulses)
add_subdirectory(music)
add_subdirectory(uimanager)
add_subdirectory(uimisc)
add_subdirectory(uiwindows)

target_sources(retruxx_lib PRIVATE
    game.cpp
    game_initdone.cpp
    globalscriptfuncs.cpp
    main.cpp
    profile.cpp
    view32.cpp
    view32_cinematic.cpp
    view32_console.cpp
    view32_loadsave.cpp
    view32_video.cpp
    vivisectionblock.cpp)
