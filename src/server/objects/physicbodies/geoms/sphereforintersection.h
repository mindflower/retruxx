#pragma once
#include "sphere.h"
#include <set>

namespace m3d {
    struct Class;
}

namespace ai
{
    class Obstacle;

    class SphereForIntersection :  public Sphere
    {
    public:
        enum SpherePurpose
        {
            LOOKING = 0x0,
            INTERSECTING = 0x1,
        };

    public:
        void SetTargetClasses(std::set<m3d::Class *> const &);
        static SphereForIntersection * __fastcall CreateObject(float,SpherePurpose,Obstacle *);
        virtual ~SphereForIntersection();
        Obstacle * GetOwner() const ;

    private:
        SphereForIntersection(dxGeom * const,enum SpherePurpose,Obstacle *);

    private:
        int m_signature;
        SpherePurpose m_purpose;
        std::set<m3d::Class *> m_targetClasses;
        SphereForIntersection *m_prev;
        SphereForIntersection *m_next;
        Obstacle *m_owner;
    };
}
