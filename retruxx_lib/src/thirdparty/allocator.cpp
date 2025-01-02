#ifdef RETRUXX_DLL
#include "allocator.h"
#include "core/kernel.h"
#include "injecttools.h"

namespace retruxx
{
    void deallocate_impl(void* ptr, size_t)
    {	// deallocate object at _Ptr, ignore size
        M3D_KERNEL->g_mar.FreeMem(ptr, 0, 0);
    }

    void* allocate_impl(size_t count)
    {	// allocate array of _Count elements
        return M3D_KERNEL->g_mar.AllocMem(count, 0, 0);
    }
}

#endif //RETRUXX_DLL
