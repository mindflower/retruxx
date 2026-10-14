#pragma once
#include <vector>
#include <core/clazz.h>
#include <math/vector.h>
#include <renderer/i_renderer.h>

namespace m3d
{
    class GeomObjectRoad;
    class RoadManager;

    class RoadNode : public m3d::Object
    {
    protected:
        RoadNode();
        RoadNode(const m3d::RoadNode& r);

    public:
        virtual  ~RoadNode() override;
        virtual m3d::Object* Clone() override;
        static m3d::Object* CreateObject();
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classRoadNode;

        CVector m_origin;
        int m_roadSetHandle;
        CStr m_roadSetName;
        int m_type;
        unsigned int m_modelNum;
        unsigned int m_skinNumber;
        bool m_asCliff;
        float m_minX;
        float m_maxX;
        float m_maxWorldY;
        CVector minb;
        CVector maxb;
        CStr m_linkedNames[4];
        m3d::RoadNode* m_linkedNodes[4];
        m3d::RoadNode* m_friend;
        m3d::RoadManager* m_owner;
        CVector* m_cachedVertices;
        std::vector<unsigned int, std::allocator<unsigned int> > m_cellsCovered;
        m3d::rend::VbPoolField m_VbPoolField;
        m3d::rend::IbPoolField m_IbPoolField;
        m3d::GeomObjectRoad* m_geomObject;
        bool m_bRoadDrawn;
        bool m_bInverted;
        CVector m_boundCenter;
        float m_boundRadius;
        int m_frameVisible;
        void SetOwner(m3d::RoadManager* owner);
        virtual int ReadFromXmlNode(m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* node) override;
        virtual int WriteToXmlNode(m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* node) override;
        virtual int ReadFromXmlNodeAfterAdd(m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* node) override;
        int GetType() const;
        int GetRoadSetHandle() const;
        int GetModelNum() const;
        CVector GetLinkPoint(float t, float xComponent);
        CVector GetPoint1();
        CVector GetPoint2();
        CVector GetPoint3();
        CVector GetPoint4();
        int GetSoilType();
    };
}
