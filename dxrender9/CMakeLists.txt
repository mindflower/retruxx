cmake_minimum_required(VERSION 3.13)

# The Direct3D 9 renderer driver. The engine loads it at run time as dxrender9.dll and talks to it
# through the m3d::rend::IRenderer interface (lib/include/renderer), so this is a separate shared
# library and not part of retruxx_lib.
#
# The layout follows the original dxrender9 project: device.cpp/.h, matrices.cpp, lights.cpp, clipplanes.cpp,
# screenshot.cpp, query.cpp, mesh.cpp, fsrt.cpp, rendertarget.cpp, drawcalls.cpp, texture_man*.cpp,
# vb_man*.cpp, ib_man*.cpp and shaders/.

# The DirectX SDK (June 2010) supplies d3dx9. CMake reads DXSDK_DIR from the environment.
if(DEFINED ENV{DXSDK_DIR})
    set(DXRENDER9_DXSDK_DIR "$ENV{DXSDK_DIR}")
elseif(EXISTS "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)")
    set(DXRENDER9_DXSDK_DIR "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)")
else()
    message(FATAL_ERROR "dxrender9: DirectX SDK not found. Set DXSDK_DIR.")
endif()
string(REGEX REPLACE "[\\/]+$" "" DXRENDER9_DXSDK_DIR "${DXRENDER9_DXSDK_DIR}")
if(NOT EXISTS "${DXRENDER9_DXSDK_DIR}/Include/d3dx9.h")
    message(FATAL_ERROR "dxrender9: ${DXRENDER9_DXSDK_DIR}/Include/d3dx9.h not found.")
endif()

add_library(dxrender9 SHARED
    main.cpp
    i_renderer_impl.cpp
    engine_helpers.cpp
    device.cpp
    matrices.cpp
    lights.cpp
    clipplanes.cpp
    screenshot.cpp
    query.cpp
    mesh.cpp
    fsrt.cpp
    rendertarget.cpp
    drawcalls.cpp
    texture_man.cpp
    texture_man_stats.cpp
    vb_man.cpp
    vb_man_stats.cpp
    ib_man.cpp
    ib_man_stats.cpp
    shaders/shaders.cpp
    shaders/shader_include.cpp
    shaders/effects/effect.cpp
    shaders/effects/effect_statemanager.cpp
    shaders/effects/effect_strings.cpp
    shaders/hlsl/hlsl_shader.cpp
    shaders/assembly/asm_shader.cpp

    # The engine value types the renderer shares with the executable. They are compiled into the
    # DLL rather than linked from retruxx_lib so the DLL stays independent of the engine.
    ${PROJECT_SOURCE_DIR}/lib/engine/core/stringm3d.cpp
    ${PROJECT_SOURCE_DIR}/lib/engine/core/timer.cpp
    ${PROJECT_SOURCE_DIR}/lib/engine/console/cvar.cpp
    ${PROJECT_SOURCE_DIR}/lib/engine/math/matrix.cpp
    ${PROJECT_SOURCE_DIR}/lib/engine/math/plane.cpp
    ${PROJECT_SOURCE_DIR}/lib/engine/math/quaternion.cpp
    ${PROJECT_SOURCE_DIR}/lib/engine/math/vector.cpp
    ${PROJECT_SOURCE_DIR}/lib/engine/math/vector2.cpp
    ${PROJECT_SOURCE_DIR}/lib/engine/math/vector4.cpp
    ${PROJECT_SOURCE_DIR}/lib/include/renderer/i_renderer.cpp
    ${PROJECT_SOURCE_DIR}/lib/include/renderer/i_renderer_colors.cpp)

set_target_properties(dxrender9 PROPERTIES OUTPUT_NAME "dxrender9" PREFIX "")

target_include_directories(dxrender9 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${RETRUXX_INCLUDE_PATH}
    "${PROJECT_SOURCE_DIR}/lib/include")
# After the Windows SDK, so the SDK's own headers win where both declare something.
target_include_directories(dxrender9 SYSTEM PRIVATE "${DXRENDER9_DXSDK_DIR}/Include")

target_compile_definitions(dxrender9 PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX _CRT_SECURE_NO_WARNINGS)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    target_link_directories(dxrender9 PRIVATE "${DXRENDER9_DXSDK_DIR}/Lib/x64")
else()
    target_link_directories(dxrender9 PRIVATE "${DXRENDER9_DXSDK_DIR}/Lib/x86")
endif()
target_link_libraries(dxrender9 PRIVATE d3d9 d3dx9 dxguid winmm)
