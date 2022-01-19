#pragma once
#include <set>

namespace m3d
{
    struct Class;
}

class CVector;

namespace ai
{
    class SphereForIntersection;
    class Sphere;

    class IntersectionManager
    {
    public:
        static bool SpheresIntersect(CVector const&, float, CVector, float);
        static bool IsSphereValid(Sphere const*, std::set<m3d::Class*> const&, bool);
        static void Registration();
        static void AddLookingSphere(SphereForIntersection*);
        static void ClearCounters();
        static void RemoveLookingSphere(SphereForIntersection*);

    private:
        //GetIntersectedObjectsByKindOf(std::set<ref_ptr<Obstacle>>&, Sphere const*, std::set<m3d::Class*> const&, bool);
        //GetIntersectedObjects(std::set<ref_ptr<Obstacle>>&, Sphere const*, std::set<m3d::Class*> const&, bool, bool);
        //_GetIntersectedObjectsCustom(std::set<ref_ptr<Obstacle>>&, Sphere const*, std::set<m3d::Class*> const&, void (*)(void*, dxGeom*, dxGeom*), bool, bool);
    };
}
