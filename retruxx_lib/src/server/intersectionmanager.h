#pragma once
#include "obstacle.h"
#include "core/clazz.h"
#include "core/ref_ptr.h"
#include "objects/physicbodies/geoms/sphere.h"
#include "thirdparty/containers.h"

namespace ai
{
    class IntersectionManager
    {
    private:
        IntersectionManager();

    public:
        static void __fastcall GetIntersectedObjects(retruxx::set<ref_ptr<ai::Obstacle>, retruxx::less<ref_ptr<ai::Obstacle> >, retruxx::allocator<ref_ptr<ai::Obstacle> > >& objIds, const ai::Sphere* pLookSphere, const retruxx::set<m3d::Class*, retruxx::less<m3d::Class*>, retruxx::allocator<m3d::Class*> >& targetClasses, bool bCheckBoxes, bool bCheckPlayerPassmap);
        static void __fastcall GetIntersectedObjectsByKindOf(retruxx::set<ref_ptr<ai::Obstacle>, retruxx::less<ref_ptr<ai::Obstacle> >, retruxx::allocator<ref_ptr<ai::Obstacle> > >& objIds, const ai::Sphere* pLookSphere, const retruxx::set<m3d::Class*, retruxx::less<m3d::Class*>, retruxx::allocator<m3d::Class*> >& targetClasses, bool bCheckBoxes);
        static bool __fastcall IsSphereValid(const ai::Sphere* pIntersectionSphere, const retruxx::set<m3d::Class*, retruxx::less<m3d::Class*>, retruxx::allocator<m3d::Class*> >& targetClasses, bool bCheckPlayerPassmap);
        static bool __fastcall SpheresIntersect(const CVector& center1, float radius1, const CVector center2, float radius2);
        static void __fastcall Registration();
        static void __fastcall ClearCounters();
        static void __fastcall AddLookingSphere(ai::SphereForIntersection* lookingSphere);
        static void __fastcall RemoveLookingSphere(ai::SphereForIntersection* lookingSphere);

    private:
        static void __fastcall _GetIntersectedObjectsCustom(retruxx::set<ref_ptr<ai::Obstacle>, retruxx::less<ref_ptr<ai::Obstacle> >, retruxx::allocator<ref_ptr<ai::Obstacle> > >& objIds, const ai::Sphere* pLookSphere, const retruxx::set<m3d::Class*, retruxx::less<m3d::Class*>, retruxx::allocator<m3d::Class*> >& targetClasses, void (*nearCallback)(retruxx::set<ref_ptr<ai::Obstacle>, retruxx::less<ref_ptr<ai::Obstacle> >, retruxx::allocator<ref_ptr<ai::Obstacle> > >& objIds, const ai::Sphere* pLookSphere, const retruxx::set<m3d::Class*, retruxx::less<m3d::Class*>, retruxx::allocator<m3d::Class*> >& targetClasses, void*, dxGeom*, dxGeom*), bool bCheckBoxes, bool bCheckPlayerPassmap);
        static inline ai::SphereForIntersection* m_firstLookingSphere = nullptr;
    }; /* size: 0x0001 */
}
