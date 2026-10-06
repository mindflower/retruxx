cmake_minimum_required(VERSION 3.13)

# The sound driver. The engine loads it at run time as sound.dll and talks to it through the
# snd::ISound interface (lib/include/iface.h), so this is a separate shared library and not part of
# retruxx_lib.
#
# The layout follows the original sound project: CM3DSoundManager.cpp, SoundGroup.cpp, the headers
# sounditem.h and soundgroup.h, and the FMOD layer under fmod/.

# The FMOD Core SDK (FMOD Studio API, "api/core"). CMake reads FMOD_SDK_DIR from the environment
# and falls back to the default install path. Its CMakeLists is not used: it describes the x64
# libraries only.
if(DEFINED ENV{FMOD_SDK_DIR})
    set(SOUND_FMOD_DIR "$ENV{FMOD_SDK_DIR}")
elseif(EXISTS "C:/Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/core")
    set(SOUND_FMOD_DIR "C:/Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/core")
else()
    message(FATAL_ERROR "sound: FMOD Core SDK not found. Set FMOD_SDK_DIR to its api/core folder.")
endif()
string(REGEX REPLACE "[\\/]+$" "" SOUND_FMOD_DIR "${SOUND_FMOD_DIR}")
if(NOT EXISTS "${SOUND_FMOD_DIR}/inc/fmod.h")
    message(FATAL_ERROR "sound: ${SOUND_FMOD_DIR}/inc/fmod.h not found.")
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(SOUND_FMOD_LIB_DIR "${SOUND_FMOD_DIR}/lib/x64")
else()
    set(SOUND_FMOD_LIB_DIR "${SOUND_FMOD_DIR}/lib/x86")
endif()

add_library(fmod_core SHARED IMPORTED)
set_target_properties(fmod_core PROPERTIES
    IMPORTED_LOCATION "${SOUND_FMOD_LIB_DIR}/fmod.dll"
    IMPORTED_IMPLIB "${SOUND_FMOD_LIB_DIR}/fmod_vc.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${SOUND_FMOD_DIR}/inc")

add_library(sound SHARED
    main.cpp
    engine_helpers.cpp
    CM3DSoundManager.cpp
    SoundGroup.cpp
    fmod/fsound_compat.cpp

    # The engine value types the driver shares with the executable. They are compiled into the
    # DLL rather than linked from retruxx_lib so the DLL stays independent of the engine.
    ${PROJECT_SOURCE_DIR}/lib/engine/core/stringm3d.cpp
    ${PROJECT_SOURCE_DIR}/lib/engine/core/threadsync.cpp
    ${PROJECT_SOURCE_DIR}/lib/engine/console/cvar.cpp
    ${PROJECT_SOURCE_DIR}/lib/engine/math/matrix.cpp
    ${PROJECT_SOURCE_DIR}/lib/engine/math/plane.cpp
    ${PROJECT_SOURCE_DIR}/lib/engine/math/quaternion.cpp
    ${PROJECT_SOURCE_DIR}/lib/engine/math/vector.cpp
    ${PROJECT_SOURCE_DIR}/lib/engine/math/vector2.cpp
    ${PROJECT_SOURCE_DIR}/lib/engine/math/vector4.cpp)

set_target_properties(sound PROPERTIES OUTPUT_NAME "sound" PREFIX "")

target_include_directories(sound PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${RETRUXX_INCLUDE_PATH}
    "${PROJECT_SOURCE_DIR}/lib/include")

target_compile_definitions(sound PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX _CRT_SECURE_NO_WARNINGS)

target_link_libraries(sound PRIVATE fmod_core)

# fmod.dll has to sit next to the executable.
add_custom_command(TARGET sound POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:fmod_core>" "$<TARGET_FILE_DIR:sound>")
