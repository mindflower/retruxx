#include <stdexcept>
#include <tracelinemanager.h>

#include "server/objects/physicbodies/geoms/ray.h"

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

    void TraceLineManager::InitTraceLineRay(bool create)
    {
        if (create)
        {
            if (!m_traceLineRay)
                m_traceLineRay = ai::Ray::CreateObject(0, 0.0, 0);
        }
        else if (m_traceLineRay)
        {
            delete m_traceLineRay;
        }
    }
}
