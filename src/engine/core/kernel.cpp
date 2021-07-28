#include <atomic>
#include <core/kernel.h>

namespace m3d
{
    Kernel* g_Kernel = nullptr;

    int Kernel::GetUniqueId()
    {
        static std::atomic_int g_uniqueId = 0;
        if (g_uniqueId < 0)
        {
            g_uniqueId = 0;
        }
        return g_uniqueId++;
    }
}
