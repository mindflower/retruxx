#include "intersectionmanager.h"
#include <m3dapp.h>
#include <stdexcept>
#include <math/vector.h>

namespace ai
{
    namespace
    {
        m3d::DbgCounter* cntIntersectionCalls = nullptr;
        m3d::DbgCounter* cntObjectsChecked = nullptr;
        m3d::DbgCounter* cntIntersectingObjectsChecked = nullptr;
        m3d::DbgCounter* cntObjectsSatisfied = nullptr;
    }

    bool IntersectionManager::SpheresIntersect(CVector const&, float, CVector, float)
    {
        throw std::logic_error("Not implemented");
    }

    bool IntersectionManager::IsSphereValid(Sphere const*, std::set<m3d::Class*> const&, bool)
    {
        throw std::logic_error("Not implemented");
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

    void IntersectionManager::AddLookingSphere(SphereForIntersection*)
    {
        throw std::logic_error("Not implemented");
    }

    void IntersectionManager::ClearCounters()
    {
        throw std::logic_error("Not implemented");
    }

    void IntersectionManager::RemoveLookingSphere(SphereForIntersection*)
    {
        throw std::logic_error("Not implemented");
    }
}
