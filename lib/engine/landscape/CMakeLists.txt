cmake_minimum_required(VERSION 3.13)

add_subdirectory(roads)

target_sources(retruxx_lib PRIVATE
    geomobject.cpp
    landscape.cpp
    landscapecollision.cpp
    landscapegrass.cpp
    landscaperender.cpp
    landscapewater.cpp)
