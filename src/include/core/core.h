#pragma once
#include "kernel.h"
#include <new>

//TODO: custom new and delete

//void* operator new(size_t count)
//{
//    return m3d::g_Kernel->g_mar.AllocMem(count, nullptr, 0);
//}
//
//void* operator new(size_t count, std::nothrow_t const&) noexcept
//{
//    try
//    {
//        return m3d::g_Kernel->g_mar.AllocMem(count, nullptr, 0);
//    }
//    catch(...)
//    {
//        return nullptr;
//    }
//}
//
//void* operator new[](size_t sz)
//{
//    return m3d::g_Kernel->g_mar.AllocMem(sz, nullptr, 0);
//}
//
//void operator delete(void* p)
//{
//    if (p)
//    {
//        m3d::g_Kernel->g_mar.FreeMem(p, nullptr, 0);
//    }
//}
//
//void operator delete[](void* p)
//{
//    if (p)
//    {
//        m3d::g_Kernel->g_mar.FreeMem(p, nullptr, 0);
//    }
//}

