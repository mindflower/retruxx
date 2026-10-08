#include "core/kernel.h"
#include "core/timer.h"
#include "scene/nodes/sgnode.h"
#include "scene/servers/dataserver.h"
#include "server/objects/physicbodies/physicbody.h"

#include <stdexcept>
#include <tracelinemanager.h>

#include "server/objects/physicbodies/geoms/ray.h"

namespace m3d
{
    TraceLineManager::~TraceLineManager()
    {
        // RVA 0x863F20
        delete m_exceptionIds;
        m_exceptionIds = nullptr;
    }

    bool TraceLineManager::TraceLine(CVector const& v1, CVector const& v2)
    {
        // RVA 0x864060 - traces at most once every m_Dt ms and otherwise returns the last verdict.
        static dContact contact;

        // The raw current time: the shipped code does not notch the timer here.
        if (M3D_KERNEL->GetTimer()._GetCurTime() - m_LastTimeUpdated > m_Dt)
        {
            if (m_presentNode)
            {
                // The first ancestor that belongs to a game object is excluded from the trace.
                ai::PhysicBody* body = nullptr;
                for (auto* node = static_cast<SgNode*>(m_presentNode->GetParent()); node;
                     node = static_cast<SgNode*>(node->GetParent()))
                {
                    body = nullptr;
                    node->GetProperty(PROP_NODE_PHYSICBODY, &body);
                    if (body && body->GetOwner())
                    {
                        delete m_exceptionIds;
                        m_exceptionIds = nullptr;
                        m_exceptionIds = new ai::ObjIdExceptionalTraceLineCallback(
                            std::vector<int>(1, body->GetOwner()->GetId()));
                        break;
                    }
                }
                m_presentNode = nullptr;
            }

            m_LastTimeUpdated = M3D_KERNEL->GetTimer()._GetCurTime();

            CVector const dir(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z);
            dGeomSetPosition(m_traceLineRay->GetGeomId(), v1.x, v1.y, v1.z);
            m_traceLineRay->SetDirection(dir);
            m_traceLineRay->SetLength(sqrt(dir.z * dir.z + dir.x * dir.x + dir.y * dir.y));
            m_LastVerdict = ai::TraceLine(*m_traceLineRay, contact, false, false, false, true, m_exceptionIds, false, false);
        }
        return m_LastVerdict;
    }

    void TraceLineManager::SetTransparentBody(m3d::SgNode* n)
    {
        this->m_presentNode = n;
    }

    TraceLineManager::TraceLineManager(unsigned Dt)
    {
        this->m_LastTimeUpdated = 0;
        this->m_Dt = Dt;
        this->m_LastVerdict = 0;
        this->m_exceptionIds = 0;
        this->m_presentNode = 0;
    }

    void TraceLineManager::InitTraceLineRay(bool create)
    {
        if (create)
        {
            if (!m_traceLineRay)
                m_traceLineRay = ai::Ray::CreateObject(0, 0.0, 0);
        }
        else
        {
            delete m_traceLineRay;
            m_traceLineRay = nullptr;
        }
    }
}
