#include <stdexcept>
#include <tracelinemanager.h>

namespace m3d
{
    TraceLineManager::~TraceLineManager()
    {
        throw std::logic_error("Not implemented");
    }

    bool TraceLineManager::TraceLine(CVector const&, CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    void TraceLineManager::SetTransparentBody(m3d::SgNode*)
    {
        throw std::logic_error("Not implemented");
    }

    TraceLineManager::TraceLineManager(unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    void TraceLineManager::InitTraceLineRay(bool)
    {
        throw std::logic_error("Not implemented");
    }
}
