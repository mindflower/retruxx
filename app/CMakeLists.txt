cmake_minimum_required(VERSION 3.13)

add_executable(retruxx WIN32 main.cpp)

target_link_libraries(retruxx retruxx_lib winmm strmiids)
target_include_directories(retruxx PUBLIC ${RETRUXX_INCLUDE_PATH})