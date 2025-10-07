#include "intersectionmanager.h"
#include <m3dapp.h>
#include <stdexcept>
#include <math/vector.h>

#include "objects/physicbodies/geoms/sphereforintersection.h"

namespace ai
{
    namespace
    {
        m3d::DbgCounter* cntIntersectionCalls = nullptr;
        m3d::DbgCounter* cntObjectsChecked = nullptr;
        m3d::DbgCounter* cntIntersectingObjectsChecked = nullptr;
        m3d::DbgCounter* cntObjectsSatisfied = nullptr;
    }

    bool IntersectionManager::SpheresIntersect(const CVector&, float, const CVector, float)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool IntersectionManager::IsSphereValid(Sphere const*, retruxx::set<m3d::Class*> const&, bool)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void IntersectionManager::Registration()
    {
        auto idx = m3d::Application::g_pApp->GetDbgCounterStack().AddCounter("GetIntersectedObjects() calls");
        if (idx < m3d::Application::g_pApp->GetDbgCounterStack().GetNumCounters())
        {
            cntIntersectionCalls = m3d::Application::g_pApp->GetDbgCounterStack().GetCounter(idx);
        }
        cntIntersectionCalls->SetI(0);

        idx = m3d::Application::g_pApp->GetDbgCounterStack().AddCounter("objects checked for intersection");
        if (idx < m3d::Application::g_pApp->GetDbgCounterStack().GetNumCounters())
        {
            cntObjectsChecked = m3d::Application::g_pApp->GetDbgCounterStack().GetCounter(idx);
        }
        cntObjectsChecked->SetI(0);

        idx = m3d::Application::g_pApp->GetDbgCounterStack().AddCounter("intersecting objects of them");
        if (idx < m3d::Application::g_pApp->GetDbgCounterStack().GetNumCounters())
        {
            cntIntersectingObjectsChecked = m3d::Application::g_pApp->GetDbgCounterStack().GetCounter(idx);
        }
        cntIntersectingObjectsChecked->SetI(0);

        idx = m3d::Application::g_pApp->GetDbgCounterStack().AddCounter("objects satisfied intersection");
        if (idx < m3d::Application::g_pApp->GetDbgCounterStack().GetNumCounters())
        {
            cntObjectsSatisfied = m3d::Application::g_pApp->GetDbgCounterStack().GetCounter(idx);
        }
        cntObjectsSatisfied->SetI(0);
    }

    void IntersectionManager::AddLookingSphere(SphereForIntersection* lookingSphere)
    {
        lookingSphere->m_next = ai::IntersectionManager::m_firstLookingSphere;
        lookingSphere->m_prev = 0;
        if (ai::IntersectionManager::m_firstLookingSphere)
            ai::IntersectionManager::m_firstLookingSphere->m_prev = lookingSphere;
        ai::IntersectionManager::m_firstLookingSphere = lookingSphere;
    }

    void IntersectionManager::ClearCounters()
    {
        cntIntersectionCalls->SetI(0);
        cntObjectsChecked->SetI(0);
        cntIntersectingObjectsChecked->SetI(0);
        cntObjectsSatisfied->SetI(0);
    }

    void IntersectionManager::RemoveLookingSphere(SphereForIntersection*)
    {
        throw retruxx::logic_error("Not implemented");
    }
}
