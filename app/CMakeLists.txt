cmake_minimum_required(VERSION 3.13)

add_executable(retruxx WIN32 main.cpp resource.rc)

target_link_libraries(retruxx retruxx_lib)

# The shipped executable reserves 50,000,000 bytes of stack in its PE header (0x2FAF080), which every
# thread of the process then inherits. main.cpp instead runs the game on a thread created with that
# stack, so the PE keeps the default 1 MB for all the others.
target_link_options(retruxx PRIVATE /STACK:1048576)
target_include_directories(retruxx PUBLIC ${RETRUXX_INCLUDE_PATH})