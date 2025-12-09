cmake_minimum_required(VERSION 3.13)

add_library(retruxx_lib)
target_include_directories(retruxx_lib PUBLIC
    ${RETRUXX_INCLUDE_PATH}
	"${PROJECT_SOURCE_DIR}/lib/include"
	"${PROJECT_SOURCE_DIR}/lib/include/script/lua"
    "${PROJECT_SOURCE_DIR}/lib/engine/opcode"
	"${PROJECT_SOURCE_DIR}/lib/engine/video")
	
if(MSVC)
    # Apply __fastcall to a specific target's compilation
    target_compile_options(retruxx_lib PRIVATE /Gr)
endif()

add_subdirectory(engine)
add_subdirectory(game)
add_subdirectory(include)
add_subdirectory(retruxx)
add_subdirectory(server)


