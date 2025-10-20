#pragma once
#include "geoms/geom.h"
#include <vector>
#include <core/ref_count_helper.h>
#include <core/ref_ptr.h>
#include <math/quaternion.h>
#include <math/vector.h>

#include "skelmodel.h"
#include "geoms/ray.h"
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
    void SetNodeElapsedAnimationTimeInMs(m3d::SgNode*, int);
    CVector ProjectVectorOntoPlane(CVector const&, CVector const&);
    int TraceLine(ai::Ray const&, retruxx::vector<ai::Geom*> const&, dContact*);
    bool TraceLine(ai::Ray const&, dContact&, bool, bool, bool, bool, ai::TraceLineCallback*, bool, bool);
    bool GetValidPosition(CVector const&, float, unsigned char, CVector&, bool, bool, std::set<m3d::Class* > const&);
    m3d::AnimInfo* GetNodeAnimInfo(m3d::SgNode const*);
}
