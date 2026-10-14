#pragma once
#include "sphere.h"
#include "core/clazz.h"

#include "server/obstacle.h"
#include "retruxx/common.h"

namespace ai
{
    class SphereForIntersection : public ai::Sphere
    {
        friend class IntersectionManager;

    public:
        static int const SIGNATURE;
        int m_signature;

        enum SpherePurpose
        {
            LOOKING = 0,
            INTERSECTING = 1,
        };

    public:
        virtual ~SphereForIntersection() override;
        static ai::SphereForIntersection* __fastcall CreateObject(
            float radius,
            ai::SphereForIntersection::SpherePurpose purpose,
            ai::Obstacle* owner);
        ai::SphereForIntersection::SpherePurpose GetPurpose() const;
        void SetTargetClasses(
            retruxx::set<m3d::Class*, retruxx::less<m3d::Class*>, retruxx::allocator<m3d::Class*>> const&
                targetClasses);
        ai::Obstacle* GetOwner() const;

    private:
        SphereForIntersection(
            dxGeom* const geomId,
            ai::SphereForIntersection::SpherePurpose purpose,
            ai::Obstacle* owner);
        ai::SphereForIntersection::SpherePurpose m_purpose;
        retruxx::set<m3d::Class*, retruxx::less<m3d::Class*>, retruxx::allocator<m3d::Class*>> m_targetClasses;
        ai::SphereForIntersection* m_prev;
        ai::SphereForIntersection* m_next;
        ai::Obstacle* m_owner;
    };
}  // namespace ai
