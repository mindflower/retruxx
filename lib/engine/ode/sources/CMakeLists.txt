cmake_minimum_required(VERSION 3.13)

target_sources(retruxx_lib PRIVATE
    array.cpp
    collision_kernel.cpp
    collision_space.cpp
    collision_std.cpp
    collision_transform.cpp
    collision_trimesh.cpp
    collision_trimesh_box.cpp
    collision_trimesh_ccylinder.cpp
    collision_trimesh_ray.cpp
    collision_trimesh_sphere.cpp
    collision_trimesh_trimesh.cpp
    collision_util.cpp
    dcone.cpp
    dterrainy.cpp
    dterrainz.cpp
    error.cpp
    fastdot.c
    fastdot_cpp.cpp
    fastldlt.c
    fastlsolve.c
    fastltsolve.c
    joint.cpp
    lcp.cpp
    mass.cpp
    matrix_ode.cpp
    memory.cpp
    misc.cpp
    obstack.cpp
    ode.cpp
    odemath.cpp
    quickstep.cpp
    rotation.cpp
    step.cpp
    timer_ode.cpp
    util.cpp)
