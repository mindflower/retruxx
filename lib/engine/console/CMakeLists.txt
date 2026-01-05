cmake_minimum_required(VERSION 3.13)

target_sources(retruxx_lib PRIVATE
    cmdline.cpp
    cmdparams.cpp
    console.cpp
    console_commands.cpp
    console_render.cpp
    console_serialization.cpp
    cvar.cpp)
