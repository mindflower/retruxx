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
        virtual  ~RoadNode() override /* 0x00 */;
        virtual m3d::Object* Clone() override /* 0x04 */;
        static m3d::Object* CreateObject();
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x34 */;
        static m3d::Class m_classRoadNode;

        /* 0x0034 */ CVector m_origin;
        /* 0x0040 */ int m_roadSetHandle;
        /* 0x0044 */ CStr m_roadSetName;
        /* 0x0050 */ int m_type;
        /* 0x0054 */ unsigned int m_modelNum;
        /* 0x0058 */ unsigned int m_skinNumber;
        /* 0x005c */ bool m_asCliff;
        /* 0x005d */ char Padding_267[3];
        /* 0x0060 */ float m_minX;
        /* 0x0064 */ float m_maxX;
        /* 0x0068 */ float m_maxWorldY;
        /* 0x006c */ CVector minb;
        /* 0x0078 */ CVector maxb;
        /* 0x0084 */ CStr m_linkedNames[4];
        /* 0x00b4 */ m3d::RoadNode* m_linkedNodes[4];
        /* 0x00c4 */ m3d::RoadNode* m_friend;
        /* 0x00c8 */ m3d::RoadManager* m_owner;
        /* 0x00cc */ CVector* m_cachedVertices;
        /* 0x00d0 */ std::vector<unsigned int, std::allocator<unsigned int> > m_cellsCovered;
        /* 0x00e0 */ m3d::rend::VbPoolField m_VbPoolField;
        /* 0x00f4 */ m3d::rend::IbPoolField m_IbPoolField;
        /* 0x0104 */ m3d::GeomObjectRoad* m_geomObject;
        /* 0x0108 */ bool m_bRoadDrawn;
        /* 0x0109 */ bool m_bInverted;
        /* 0x010a */ char Padding_268[2];
        /* 0x010c */ CVector m_boundCenter;
        /* 0x0118 */ float m_boundRadius;
        /* 0x011c */ int m_frameVisible;
        void SetOwner(m3d::RoadManager* owner);
        virtual int ReadFromXmlNode(m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* node) override /* 0x08 */;
        virtual int WriteToXmlNode(m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* node) override /* 0x10 */;
        virtual int ReadFromXmlNodeAfterAdd(m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* node) override /* 0x0c */;
        int GetType() const;
        int GetRoadSetHandle() const;
        int GetModelNum() const;
        CVector GetLinkPoint(float t, float xComponent);
        CVector GetPoint1();
        CVector GetPoint2();
        CVector GetPoint3();
        CVector GetPoint4();
        int GetSoilType();
    }; /* size: 0x0120 */
}
