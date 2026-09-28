cmake_minimum_required(VERSION 3.13)

add_executable(retruxx WIN32 main.cpp resource.rc)

target_link_libraries(retruxx retruxx_lib winmm strmiids)

# The shipped executable reserves 50,000,000 bytes of stack (its PE header says 0x2FAF080); ODE's
# collision code allocas per-geom data and an n*n bit matrix, which overflows the default 1 MB.
target_link_options(retruxx PRIVATE /STACK:50000000)
target_include_directories(retruxx PUBLIC ${RETRUXX_INCLUDE_PATH})