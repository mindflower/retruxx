#include "physichelpers.h"

#include "physicbody.h"

namespace ai
{
    CollisionInfo::CollisionInfo(const ai::CollisionInfo& info)
    {
        this->m_geomType = info.m_geomType;
        this->m_relTranslation = info.m_relTranslation;
        this->m_relRotation = info.m_relRotation;
        this->m_size = info.m_size;
        this->m_radius = info.m_radius;
        this->m_trimeshVertices = info.m_trimeshVertices;
        this->m_trimeshIndices = info.m_trimeshIndices;
    }

    CollisionInfo::CollisionInfo()
    {
        Init();
    }

    void CollisionInfo::Init()
    {
        this->m_geomType = GEOM_TYPE_NONE;
        this->m_relTranslation.x = 0.0;
        this->m_relTranslation.y = 0.0;
        this->m_relTranslation.z = 0.0;
        this->m_relRotation.x = 0.0;
        this->m_relRotation.y = 0.0;
        this->m_relRotation.z = 0.0;
        this->m_relRotation.w = 1.0;
        this->m_size.x = 0.0;
        this->m_size.y = 0.0;
        this->m_size.z = 0.0;
        this->m_radius = 0.0;

        m_trimeshVertices = {};
        m_trimeshIndices = {};
    }

    int ai::RoughSign(float value)
    {
        if (value > 0.000001)
            return 1;
        if (value >= -0.000001)
            return 0;
        return -1;
    }

    bool GetCollisionInfoByServerHandle(int, retruxx::vector<CollisionInfo>&, bool)
    {
        // TODO: impement GetCollisionInfoByServerHandle
        return false;
        throw std::logic_error("Not implemented");
    }

    void CommonGeomMovedCallback(dxGeom* geomId)
    {
        auto Data = (m3d::Object*)dGeomGetData(geomId);
        auto v2 = (ai::PhysicBody*)Data;
        if (Data)
        {
            PhysicObj* Owner = nullptr;
            if (Data->IsKindOf(&ai::PhysicBody::m_classPhysicBody))
            {
                v2->TransferPhysicParamsToSceneGraphNode();
                Owner = v2->GetOwner();
            }
            else
            {
                if (!v2->IsKindOf(&ai::PhysicObj::m_classPhysicObj))
                    return;
                Owner = (ai::PhysicObj*)v2;
            }
            if (Owner)
            {
                if ((Owner->GetFlags() & 2) == 0)
                    Owner->RelinkGeomsToCollisionCells();
            }
        }
    }

    void SetNodeElapsedAnimationTimeInMs(m3d::SgNode*, int)
    {
        throw std::logic_error("Not implemented");
    }

    CVector ProjectVectorOntoPlane(CVector const& normal, CVector const& v)
    {
        auto v3 = 0.0;
        CVector result;
        auto v6 = normal.y * v.y + normal.x * v.x + v.z * normal.z;
        if (fabs(v6) >= 0.001)
        {
            v3 = v6;
        }
        float v7 = normal.x * v3;
        float v8 = v.z - (v3 * normal.z);
        float v9 = v.y - (normal.y * v3);
        result.x = v.x - v7;
        result.y = v9;
        result.z = v8;
        return result;
    }
}

