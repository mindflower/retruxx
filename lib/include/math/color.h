#pragma once

#include <cstdint>


inline uint32_t AddrToColor(const void* ptr) {
    uintptr_t addr = reinterpret_cast<uintptr_t>(ptr);
    #ifdef _WIN64
        addr ^= addr >> 32;
    #endif
    return static_cast<uint32_t>(addr) | 0xFF000000u;
}