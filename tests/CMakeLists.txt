cmake_minimum_required(VERSION 3.13)

set(GTEST_PATH "${PROJECT_SOURCE_DIR}/deps/gtest/googletest/include")

add_executable(retruxx_tests test.cpp)
target_link_libraries(retruxx_tests retruxx_lib gtest gtest_main)
target_include_directories(retruxx_tests PUBLIC ${RETRUXX_INCLUDE_PATH} ${GTEST_PATH})

add_test(NAME retruxx_tests COMMAND retruxx_tests)