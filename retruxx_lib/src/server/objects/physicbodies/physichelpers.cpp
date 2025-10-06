#include "physichelpers.h"

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
}

