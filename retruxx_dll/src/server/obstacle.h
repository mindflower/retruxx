#pragma once

class Obb;
class Aabb;
class CVector;
class Quaternion;

namespace m3d
{
    class Object;
    class SgNode;
}

namespace ai
{
    class PhysicObj;
    class Box;
    class SphereForIntersection;

    class Obstacle
    {
    public:
        void UnlinkFromOwner();
        ~Obstacle();
        Quaternion GetRotation() const ;
        int DecRef();
        Obstacle(m3d::SgNode *);
        Obstacle(Obb const &);
        Obstacle(PhysicObj const *);
        Aabb GetAabb() const ;
        m3d::Object * GetOwner() const ;
        void Disable();
        CVector GetPosition() const ;
        int IncRef();
        void Enable();
        CVector GetLinearVelocity() const ;
        Box const * GetBox() const ;
        SphereForIntersection const * GetSphere() const ;
        void RenderDebugInfo() const ;
        bool bIsEnabled() const ;
        float GetIntersectionRadius() const ;
        PhysicObj * GetOwnerPhysicObj() const ;

    private:
        void _Init();

    private:
        int m_refCount;
        bool m_bIsEnabled;
        SphereForIntersection *m_intersectionSphere;
        Box *m_intersectionBox;
        int m_ownerPhysicObjId;
        m3d::SgNode *m_ownerSgNode;
    };
}
