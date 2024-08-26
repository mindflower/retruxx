#pragma once
#include <vector>
#include <core/clazz.h>
#include <math/vector.h>
#include <renderer/i_renderer.h>

namespace m3d
{
    class GeomObjectRoad;
    class RoadManager;

    class RoadNode :  public Object
    {
    public:
        virtual int WriteToXmlNode(cmn::XmlFile *,cmn::XmlNode *);
        static Class * GetBaseClass();
        virtual int ReadFromXmlNodeAfterAdd(cmn::XmlFile *,cmn::XmlNode *);
        virtual ~RoadNode();
        int GetSoilType();
        virtual int ReadFromXmlNode(cmn::XmlFile *,cmn::XmlNode *);
        void SetOwner(RoadManager *);
        CVector GetLinkPoint(float,float);
        CVector GetPoint1();
        CVector GetPoint2();
        virtual Object * Clone();
        CVector GetPoint3();
        CVector GetPoint4();
        virtual Class * GetClass() const ;
        static Object * CreateObject();
    protected:
        RoadNode(RoadNode const &);
        RoadNode();

    public:
        RT_CLASS_DECLARE(RoadNode);

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
