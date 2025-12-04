cmake_minimum_required(VERSION 3.13)

add_subdirectory(ai)
add_subdirectory(colliders)
add_subdirectory(components)
add_subdirectory(formations)
add_subdirectory(objects)
add_subdirectory(roles)
add_subdirectory(statistic)

target_sources(retruxx_lib PRIVATE
    affix.cpp
    cinematicpath.cpp
    dynamicquestmanager.cpp
    dynamicscene.cpp
    event.cpp
    externalpaths.cpp
    geomrepository.cpp
    geomrepositoryitem.cpp
    infocone.cpp
    intersectionmanager.cpp
    izvratrepository.cpp
    map.cpp
    modifier.cpp
    obstacle.cpp
    passagedata.cpp
    path.cpp
    playerpassmap.cpp
    processmanager.cpp
    quest.cpp
    queststate.cpp
    relationship.cpp
    resourcemanager.cpp
    server.cpp
    static2dpath.cpp
    utils.cpp
    weaponfirer.cpp)
