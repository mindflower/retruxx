#pragma once
#include <map>
#include <vector>
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
        friend class ResourceManager;
    public:
        Resource(Resource const *);
        PointBase<int> GetGeomSize() const ;
        int GetParentId() const ;
        CStr GetName() const ;
        int GetId() const ;
        bool bIsKindOf(int) const ;
        void LoadFromXML(m3d::cmn::XmlFile *, m3d::cmn::XmlNode const *);

    private:
        int m_id = -1;
        int m_parentId = -1;
        CStr m_name;
        PointBase<int> m_geomSize {-1, -1};
    };

    class ResourceManager
    {
    public:
        CStr GetResourceName(int) const;
        void Init();
        void GetResourceDescendants(int, std::vector<int>&) const;
        bool ResourceHasChildren(int) const;
        int GetResourceId(CStr const&) const;
        ResourceManager();
        CStr GetResourceNameByVehiclePartName(CStr const&) const;
        Resource* GetResource(int) const;
        bool bResourceIsKindOf(int, int);
        ~ResourceManager();

    private:
        void _ReadResourceFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*, Resource const*);
        void _LoadFromXmlFile(char const*);
        void _LoadVehiclePartTypeToResourceXmlFile(char const*);

    private:
        std::map<CStr, ai::Resource*> m_resourceMap;
        std::vector<ai::Resource*> m_resourceVector;
        std::map<CStr, CStr> m_vehiclePart2Resource;
    };
}
