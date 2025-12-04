cmake_minimum_required(VERSION 3.13)

add_subdirectory(nodes)
add_subdirectory(servers)
add_subdirectory(shadows)

target_sources(retruxx_lib PRIVATE
    scenegraph.cpp
    scenelights.cpp
    sceneshadows.cpp
    traceline.cpp)
