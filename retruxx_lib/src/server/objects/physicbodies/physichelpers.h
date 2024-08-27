#pragma once
#include "geoms/geom.h"
#include <vector>
#include <core/ref_count_helper.h>
#include <core/ref_ptr.h>
#include <math/quaternion.h>
#include <math/vector.h>

namespace ai
{
    class CollisionInfo
    {
    public:
        CollisionInfo();
        CollisionInfo(CollisionInfo const &);
        void Init();

    private:
        GeomType m_geomType;
        CVector m_relTranslation;
        Quaternion m_relRotation;
        CVector m_size;
        float m_radius;
        ref_ptr<ref_count_helper<std::vector<CVector>>> m_trimeshVertices;
        unsigned int m_numTrimeshVertices;
        ref_ptr<ref_count_helper<std::vector<int>>> m_trimeshIndices;
        unsigned int m_numTrimeshIndices;
    };
}
