#pragma once
#include "sphere.h"
#include "core/clazz.h"

#include "server/obstacle.h"
#include "thirdparty/containers.h"


namespace ai
{
    class SphereForIntersection : public ai::Sphere
    {
    public:
        static const int SIGNATURE;
        /* 0x0018 */ int m_signature;

        enum SpherePurpose
        {
            LOOKING = 0,
            INTERSECTING = 1,
        };

    public:
        virtual  ~SphereForIntersection() override /* 0x00 */;
        static ai::SphereForIntersection* __fastcall CreateObject(float radius, ai::SphereForIntersection::SpherePurpose purpose, ai::Obstacle* owner);
        ai::SphereForIntersection::SpherePurpose GetPurpose() const;
        void SetTargetClasses(const retruxx::set<m3d::Class*, retruxx::less<m3d::Class*>, retruxx::allocator<m3d::Class*> >& targetClasses);
        ai::Obstacle* GetOwner() const;

    private:
        SphereForIntersection(dxGeom* const geomId, ai::SphereForIntersection::SpherePurpose purpose, ai::Obstacle* owner);
        /* 0x001c */ ai::SphereForIntersection::SpherePurpose m_purpose;
        retruxx::set<m3d::Class*, retruxx::less<m3d::Class*>, retruxx::allocator<m3d::Class*> > m_targetClasses;
        /* 0x002c */ ai::SphereForIntersection* m_prev;
        /* 0x0030 */ ai::SphereForIntersection* m_next;
        /* 0x0034 */ ai::Obstacle* m_owner;
    }; /* size: 0x0038 */
}
