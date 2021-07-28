#include <config.h>
#include <core/console/console.h>

namespace m3d
{
    int EngineConfig::Load(CStr const& fname)
    {
        return m_console->Load(fname);
    }
}
