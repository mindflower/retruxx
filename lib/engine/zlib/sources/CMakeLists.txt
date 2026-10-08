cmake_minimum_required(VERSION 3.13)

# zlib 1.1.4, as statically linked into the shipped game: only the files uncompress() needs.
set(ZLIB_SOURCES
    adler32.c
    infblock.c
    infcodes.c
    inffast.c
    inflate.c
    inftrees.c
    infutil.c
    uncompr.c
    zutil.c)

target_sources(retruxx_lib PRIVATE ${ZLIB_SOURCES})

# The sources include "zlib.h" unqualified. Like ODE, zlib is built with the library's /Gr, although the shipped
# copy is __cdecl; nothing outside retruxx calls it, so the convention is not observable.
set_source_files_properties(${ZLIB_SOURCES} TARGET_DIRECTORY retruxx_lib PROPERTIES
    INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/lib/include/zlib")
