cmake_minimum_required(VERSION 3.13)

add_executable(retruxx_tests)
add_subdirectory(utils)
add_subdirectory(math)

target_link_libraries(retruxx_tests retruxx_lib gtest gtest_main)
target_include_directories(retruxx_tests PUBLIC
    ${RETRUXX_INCLUDE_PATH}
    "${PROJECT_SOURCE_DIR}/deps/gtest/googletest/include"
    "${PROJECT_SOURCE_DIR}/tests")

add_test(NAME retruxx_tests COMMAND retruxx_tests)