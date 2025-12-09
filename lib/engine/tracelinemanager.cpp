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
        delete m_exceptionIds;
    }

    bool TraceLineManager::TraceLine(CVector const& v1, CVector const& v2)
    {
        // TODO: generated code TraceLineManager::TraceLine
        // Check if we need to update the trace (throttle updates based on time)
        float currentTime = M3D_KERNEL->GetTimer().GetCurTime();
        if (currentTime - m_LastTimeUpdated > m_Dt)
        {
            // Handle exception IDs for the current node
            if (m_presentNode)
            {
                m3d::SgNode* parentNode = static_cast<m3d::SgNode*>(m_presentNode->GetParent());
                if (parentNode)
                {
                    // Traverse up the parent hierarchy to find a physics body
                    m3d::SgNode* currentNode = parentNode;
                    ai::PhysicBody* foundBody = nullptr;

                    while (currentNode)
                    {
                        // Query for physics body component
                        ai::PhysicBody* body = nullptr;
                        currentNode->GetProperty(PROP_NODE_PHYSICBODY, &body);  // Some interface ID

                        if (body && body->GetOwner())
                        {
                            foundBody = body;
                            break;
                        }

                        currentNode = static_cast<m3d::SgNode*>(currentNode->GetParent());
                    }

                    // If we found a physics body, set up exception IDs
                    if (foundBody)
                    {
                        // Clean up previous exception callback
                        if (m_exceptionIds)
                        {
                            delete m_exceptionIds;
                            m_exceptionIds = nullptr;
                        }

                        // Get the object ID from the physics body owner
                        int objId = foundBody->GetOwner()->GetId();

                        // Create exception list with this object ID
                        std::vector<int> exceptionIds;
                        exceptionIds.push_back(objId);

                        // Create callback for exceptional trace line handling
                        m_exceptionIds = new ai::ObjIdExceptionalTraceLineCallback(exceptionIds);
                    }
                }

                m_presentNode = nullptr;  // Reset present node after processing
            }

            // Update the trace line
            m_LastTimeUpdated = currentTime;

            // Calculate direction vector
            CVector dir;
            dir.x = v2.x - v1.x;
            dir.y = v2.y - v1.y;
            dir.z = v2.z - v1.z;

            // Set ray position and direction
            dGeomSetPosition(m_traceLineRay->GetGeomId(), v1.x, v1.y, v1.z);
            m_traceLineRay->SetDirection(dir);

            // Calculate and set ray length
            float length = sqrt(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
            m_traceLineRay->SetLength(length);

            static dContact contact_0;
            // Perform the actual trace line
            m_LastVerdict = ai::TraceLine(
                *m_traceLineRay,
                contact_0,      // Contact result
                0,               // Some flags
                0,               // Additional parameters
                0,               // More parameters
                1,               // Probably a boolean flag
                m_exceptionIds,  // Exception handling
                0,               // Unknown
                0                // Unknown
            );
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
