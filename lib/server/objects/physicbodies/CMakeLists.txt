cmake_minimum_required(VERSION 3.13)

add_subdirectory(geoms)

target_sources(retruxx_lib PRIVATE
    boxybody.cpp
    compoundvehiclepart.cpp
    genericbody.cpp
    physicbody.cpp
    physichelpers.cpp
    raybody.cpp
    simplephysicbody.cpp
    sphericbody.cpp
    vehiclepart.cpp)
