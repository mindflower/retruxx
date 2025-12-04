cmake_minimum_required(VERSION 3.13)

target_sources(retruxx_lib PRIVATE
    breakableobjectcolliders.cpp
    bulletcolliders.cpp
    colliderkrnl.cpp
    defaultcollider.cpp
    physicunitcolliders.cpp
    vehiclecolliders.cpp
    vehiclesplintercolliders.cpp
    wheelasphaltcollider.cpp)
