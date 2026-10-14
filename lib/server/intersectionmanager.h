#pragma once
#include "obstacle.h"
#include "core/clazz.h"
#include "core/ref_ptr.h"
#include "objects/physicbodies/geoms/sphere.h"
#include "retruxx/common.h"

namespace ai
{
    class IntersectionManager
    {
    private:
        IntersectionManager();

    public:
        static void GetIntersectedObjects(
            retruxx::set<
                ref_ptr<ai::Obstacle>,
                retruxx::less<ref_ptr<ai::Obstacle>>,
                retruxx::allocator<ref_ptr<ai::Obstacle>>>& objIds,
            ai::Sphere const* pLookSphere,
            retruxx::set<m3d::Class*, retruxx::less<m3d::Class*>, retruxx::allocator<m3d::Class*>> const& targetClasses,
            bool bCheckBoxes,
            bool bCheckPlayerPassmap);
        static void GetIntersectedObjectsByKindOf(
            retruxx::set<
                ref_ptr<ai::Obstacle>,
                retruxx::less<ref_ptr<ai::Obstacle>>,
                retruxx::allocator<ref_ptr<ai::Obstacle>>>& objIds,
            ai::Sphere const* pLookSphere,
            retruxx::set<m3d::Class*, retruxx::less<m3d::Class*>, retruxx::allocator<m3d::Class*>> const& targetClasses,
            bool bCheckBoxes);
        static bool IsSphereValid(
            ai::Sphere const* pIntersectionSphere,
            retruxx::set<m3d::Class*, retruxx::less<m3d::Class*>, retruxx::allocator<m3d::Class*>> const& targetClasses,
            bool bCheckPlayerPassmap);
        static bool SpheresIntersect(CVector const& center1, float radius1, CVector const center2, float radius2);
        static void Registration();
        static void ClearCounters();
        static void AddLookingSphere(ai::SphereForIntersection* lookingSphere);
        static void RemoveLookingSphere(ai::SphereForIntersection* lookingSphere);

    private:
        static void _GetIntersectedObjectsCustom(
            retruxx::set<
                ref_ptr<ai::Obstacle>,
                retruxx::less<ref_ptr<ai::Obstacle>>,
                retruxx::allocator<ref_ptr<ai::Obstacle>>>& objIds,
            ai::Sphere const* pLookSphere,
            retruxx::set<m3d::Class*, retruxx::less<m3d::Class*>, retruxx::allocator<m3d::Class*>> const& targetClasses,
            void (*)(void*, dxGeom*, dxGeom*),
            bool bCheckBoxes,
            bool bCheckPlayerPassmap);
        static inline ai::SphereForIntersection* m_firstLookingSphere = nullptr;
    };
}  // namespace ai
