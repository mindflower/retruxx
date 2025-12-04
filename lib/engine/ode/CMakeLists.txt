cmake_minimum_required(VERSION 3.13)

add_subdirectory(sources)

target_sources(retruxx_lib PRIVATE
    odememory.cpp)
