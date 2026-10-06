cmake_minimum_required(VERSION 3.13)

# The DirectInput 8 input driver. The engine loads it at run time as input_di8.dll and talks to it
# through the m3d::input::IInput interface (lib/include/iface.h), so this is a separate shared
# library and not part of retruxx_lib.
#
# The layout follows the original input_di8 project: a single input_di8.cpp.

# The DirectX SDK (June 2010) supplies dinput.h, dinput8.lib and dxguid.lib. CMake reads DXSDK_DIR
# from the environment.
if(DEFINED ENV{DXSDK_DIR})
    set(INPUT_DI8_DXSDK_DIR "$ENV{DXSDK_DIR}")
elseif(EXISTS "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)")
    set(INPUT_DI8_DXSDK_DIR "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)")
else()
    message(FATAL_ERROR "input_di8: DirectX SDK not found. Set DXSDK_DIR.")
endif()
string(REGEX REPLACE "[\\/]+$" "" INPUT_DI8_DXSDK_DIR "${INPUT_DI8_DXSDK_DIR}")
if(NOT EXISTS "${INPUT_DI8_DXSDK_DIR}/Include/dinput.h")
    message(FATAL_ERROR "input_di8: ${INPUT_DI8_DXSDK_DIR}/Include/dinput.h not found.")
endif()

add_library(input_di8 SHARED
    main.cpp
    input_di8.cpp

    # The engine value types the driver shares with the executable. They are compiled into the
    # DLL rather than linked from retruxx_lib so the DLL stays independent of the engine.
    ${PROJECT_SOURCE_DIR}/lib/engine/core/stringm3d.cpp
    ${PROJECT_SOURCE_DIR}/lib/engine/core/timer.cpp
    ${PROJECT_SOURCE_DIR}/lib/engine/console/cvar.cpp
    ${PROJECT_SOURCE_DIR}/lib/engine/math/matrix.cpp
    ${PROJECT_SOURCE_DIR}/lib/engine/math/plane.cpp
    ${PROJECT_SOURCE_DIR}/lib/engine/math/quaternion.cpp
    ${PROJECT_SOURCE_DIR}/lib/engine/math/vector.cpp
    ${PROJECT_SOURCE_DIR}/lib/engine/math/vector2.cpp
    ${PROJECT_SOURCE_DIR}/lib/engine/math/vector4.cpp)

set_target_properties(input_di8 PROPERTIES OUTPUT_NAME "input_di8" PREFIX "")

target_include_directories(input_di8 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${RETRUXX_INCLUDE_PATH}
    "${PROJECT_SOURCE_DIR}/lib/include")
# After the Windows SDK, so the SDK's own headers win where both declare something.
target_include_directories(input_di8 SYSTEM PRIVATE "${INPUT_DI8_DXSDK_DIR}/Include")

target_compile_definitions(input_di8 PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX _CRT_SECURE_NO_WARNINGS)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    target_link_directories(input_di8 PRIVATE "${INPUT_DI8_DXSDK_DIR}/Lib/x64")
else()
    target_link_directories(input_di8 PRIVATE "${INPUT_DI8_DXSDK_DIR}/Lib/x86")
endif()
target_link_libraries(input_di8 PRIVATE dinput8 dxguid user32 winmm)
