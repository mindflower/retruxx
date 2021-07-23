#pragma once
#include <core/stringm3d.h>
#include <math/point2d.h>

namespace m3d
{
    namespace cmn
    {
        class XmlFile;
        class XmlNode;
    }
}

namespace ai
{
    class Resource
    {
    public:
        Resource(Resource const *);
        PointBase<int> GetGeomSize() const ;
        int GetParentId() const ;
        CStr GetName() const ;
        int GetId() const ;
        bool bIsKindOf(int) const ;
        void LoadFromXML(m3d::cmn::XmlFile *, m3d::cmn::XmlNode const *);

    private:
        int m_id;
        int m_parentId;
        CStr m_name;
        PointBase<int> m_geomSize;
    };
}
