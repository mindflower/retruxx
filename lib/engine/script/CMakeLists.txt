cmake_minimum_required(VERSION 3.13)

add_subdirectory(lua)

target_sources(retruxx_lib PRIVATE
    funcarg.cpp
    funcstack.cpp
    luaaiparam.cpp
    luaquaternion.cpp
    luavector.cpp
    scriptcontext.cpp
    scriptlet.cpp
    scriptserver.cpp)
