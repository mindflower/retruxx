#pragma once
#include <core/clazz.h>

namespace m3d
{
    class RoadNode :  public Object
    {
    public:
        virtual int WriteToXmlNode(cmn::XmlFile *,cmn::XmlNode *);
        static Class * __fastcall GetBaseClass();
        virtual int ReadFromXmlNodeAfterAdd(cmn::XmlFile *,cmn::XmlNode *);
        virtual ~RoadNode();
        int GetSoilType();
        virtual int ReadFromXmlNode(cmn::XmlFile *,cmn::XmlNode *);
        void SetOwner(RoadManager *);
        struct CVector GetLinkPoint(float,float);
        struct CVector GetPoint1();
        struct CVector GetPoint2();
        virtual Object * Clone();
        struct CVector GetPoint3();
        struct CVector GetPoint4();
        virtual Class * GetClass() const ;
        static Object * __fastcall CreateObject();
    protected:
        RoadNode(class RoadNode const &);
        RoadNode();
    private:
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
        RoadNode *m_linkedNodes[4];
        RoadNode *m_friend;
        RoadManager *m_owner;
        CVector *m_cachedVertices;
        std::vector<unsigned int> m_cellsCovered;
        rend::VbPoolField m_VbPoolField;
        rend::IbPoolField m_IbPoolField;
        GeomObjectRoad *m_geomObject;
        bool m_bRoadDrawn;
        bool m_bInverted;
        CVector m_boundCenter;
        float m_boundRadius;
        int m_frameVisible;
    };
}
