cmake_minimum_required(VERSION 3.13)

add_subdirectory(lib)

target_sources(retruxx_lib PRIVATE
    lapi.c
    lcode.c
    ldebug.c
    ldo.c
    ldump.c
    lfunc.c
    lgc.c
    llex.c
    lmem.c
    lobject.c
    lopcodes.c
    lparser.c
    lstate.c
    lstring.c
    ltable.c
    ltm.c
    lundump.c
    lvm.c
    lzio.c)
