#pragma once
#include "geoms/geom.h"
#include <vector>
#include <core/ref_count_helper.h>
#include <core/ref_ptr.h>
#include <math/quaternion.h>
#include <math/vector.h>

#include "skelmodel.h"
#include "geoms/ray.h"

class Segment;
#include "scene/nodes/sgnode.h"
#include "server/dynamicscene.h"

namespace ai
{
    struct CollisionInfo
    {
        /* 0x0000 */ ai::GeomType m_geomType;
        CVector m_relTranslation;
        Quaternion m_relRotation;
        CVector m_size;
        /* 0x002c */ float m_radius;
        ref_ptr<ref_count_helper<retruxx::vector<CVector, retruxx::allocator<CVector> > > > m_trimeshVertices;
        /* 0x0034 */ unsigned int m_numTrimeshVertices;
        ref_ptr<ref_count_helper<retruxx::vector<int, retruxx::allocator<int> > > > m_trimeshIndices;
        /* 0x003c */ unsigned int m_numTrimeshIndices;
        CollisionInfo(const ai::CollisionInfo& info);
        CollisionInfo();
        void Init();
    }; /* size: 0x0040 */

    int RoughSign(float);
    bool GetCollisionInfoByModelName(CStr const&, retruxx::vector<ai::CollisionInfo>&, bool);
    bool GetCollisionInfoByServerHandle(int, retruxx::vector<CollisionInfo>&, bool);
    void CommonGeomMovedCallback(dxGeom*);
    void SetUniversalJointParams(dxJoint* joint);
    void SetNodeElapsedAnimationTimeInMs(m3d::SgNode*, int);
    int GetNodeCurAnimationFrame(m3d::SgNode const*);
    void SetNodeCurAnimationFrame(m3d::SgNode*, int);
    CVector ProjectVectorOntoPlane(CVector const&, CVector const&);
    int TraceLine(ai::Ray const&, retruxx::vector<ai::Geom*> const&, dContact*);
    int TraceSegment(Segment const& segment, retruxx::vector<ai::Geom*> const& Geoms, dContact* closestContact);
    bool TraceLine(ai::Ray const&, dContact&, bool, bool, bool, bool, ai::TraceLineCallback*, bool, bool);
    bool CollideGeom(ai::Geom const& testGeom, bool dontCollideWithLittle, bool dontCollideWithPlayer,
                     bool dontCollideWithWater, bool dontCollideWithShells);
    bool GetValidPosition(CVector const&, float, unsigned char, CVector&, bool, bool, std::set<m3d::Class* > const&);
    m3d::AnimInfo* GetNodeAnimInfo(m3d::SgNode const*);
    int GetNodeElapsedAnimationTimeInMs(m3d::SgNode const* node);
    bool GetSmoothAcceleratedValue(float elapsedTime, float destination, float position, float velocity, float acceleration, float maxVelocity, float& newPosition, float& newVelocity);
    CVector GetRandomDeviatedVector(CVector const&, float);

    template <class T>
    void SetDirectionToObject(T& obj, CVector const& direction)
    {
        // RVA 0x5CFB00 (ai::Geom), 0x5FB340 (ai::PhysicObj) - turns the object to face along
        // direction (taken to be normalized), with no roll: the product of a turn about the up
        // axis (0, hy, 0, hw) and a pitch about x (ex, 0, 0, ew).
        // The Geom instantiation writes the same quaternion straight to ODE in (w, x, y, z) order,
        // which is what Geom::SetRotation does.
        double const halfYaw = atan2(direction.x, direction.z) * 0.5f;
        float const hy = static_cast<float>(sin(halfYaw));
        float const hw = static_cast<float>(cos(halfYaw));
        double const halfPitch = -asin(direction.y) * 0.5f;
        float const ex = static_cast<float>(sin(halfPitch));
        float const ew = static_cast<float>(cos(halfPitch));

        Quaternion rotation;
        rotation.x = hw * ex + ew * 0.0f + hy * 0.0f;
        rotation.y = ew * hy + ex * 0.0f + hw * 0.0f;
        rotation.z = hw * 0.0f + ew * 0.0f - hy * ex;
        rotation.w = ew * hw - ex * 0.0f - hy * 0.0f;
        obj.SetRotation(rotation);
    }
}
