#pragma once
#include "kernel.h"
#include <new>

//TODO: custom new and delete

//void* __cdecl operator new(std::size_t count)
//{
//    return M3D_KERNEL->g_mar.AllocMem(count, nullptr, 0);
//}
//
//void* __cdecl operator new(std::size_t count, std::nothrow_t const&) noexcept
//{
//    try
//    {
//        return M3D_KERNEL->g_mar.AllocMem(count, nullptr, 0);
//    }
//    catch(...)
//    {
//        return nullptr;
//    }
//}
//
//void* __cdecl operator new[](std::size_t sz)
//{
//    return M3D_KERNEL->g_mar.AllocMem(sz, nullptr, 0);
//}
//
//void __cdecl operator delete(void* p)
//{
//    if (p)
//    {
//        M3D_KERNEL->g_mar.FreeMem(p, nullptr, 0);
//    }
//}
//
//void __cdecl operator delete[](void* p)
//{
//    if (p)
//    {
//        M3D_KERNEL->g_mar.FreeMem(p, nullptr, 0);
//    }
//}

