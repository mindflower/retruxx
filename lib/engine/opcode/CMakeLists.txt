cmake_minimum_required(VERSION 3.13)

add_subdirectory(ice)

target_sources(retruxx_lib PRIVATE
    OPC_AABBCollider.cpp
    OPC_BoxPruning.cpp
    OPC_Common.cpp
    OPC_HybridModel.cpp
    OPC_Picking.cpp
    OPC_SweepAndPrune.cpp
    Opcode.cpp
    StdAfx.cpp
    opc_aabbtree.cpp
    opc_basemodel.cpp
    opc_collider.cpp
    opc_lsscollider.cpp
    opc_meshinterface.cpp
    opc_model.cpp
    opc_obbcollider.cpp
    opc_optimizedtree.cpp
    opc_planescollider.cpp
    opc_raycollider.cpp
    opc_spherecollider.cpp
    opc_treebuilders.cpp
    opc_treecollider.cpp
    opc_volumecollider.cpp)
