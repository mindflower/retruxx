cmake_minimum_required(VERSION 3.13)

# Platform check
if(NOT WIN32)
    message(FATAL_ERROR "This project is Windows-only (Win32)")
endif()

project(retruxx LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(MSVC)
    add_compile_options(/MP)
endif()

set(BUILD_GMOCK OFF CACHE BOOL "Build only googletest")
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

enable_testing()

set(RETRUXX_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/lib")

add_subdirectory(app)
add_subdirectory(lib)
add_subdirectory(dxrender9)
add_subdirectory(input_di8)
add_subdirectory(sound)
add_subdirectory(tests)
add_subdirectory(deps/gtest)

add_dependencies(retruxx retruxx_lib dxrender9 input_di8 sound)
add_dependencies(retruxx_tests retruxx_lib gtest gtest_main)