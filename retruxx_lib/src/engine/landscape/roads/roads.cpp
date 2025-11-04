#include "m3dapp.h"
#include "road.h"

namespace m3d
{
    RT_CLASS_EXPORTS_BEGIN(RoadNode)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(RoadNode);

    int RoadNode::WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Class* RoadNode::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Object);
    }

    int RoadNode::ReadFromXmlNodeAfterAdd(cmn::XmlFile*, cmn::XmlNode*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    RoadNode::~RoadNode()
    {
        if (m_VbPoolField.Vb.IsValid())
        {
            M3D_RENDERER->ReleaseVb(m_VbPoolField.Vb);
        }
        if (m_IbPoolField.Ib.IsValid())
        {
            M3D_RENDERER->ReleaseIb(m_IbPoolField.Ib);
        }
    }

    int RoadNode::GetSoilType()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int RoadNode::ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RoadNode::SetOwner(RoadManager*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector RoadNode::GetLinkPoint(float, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector RoadNode::GetPoint1()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector RoadNode::GetPoint2()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Object* RoadNode::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector RoadNode::GetPoint3()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector RoadNode::GetPoint4()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Class* RoadNode::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Object* RoadNode::CreateObject()
    {
        return new RoadNode;
    }

    RoadNode::RoadNode(RoadNode const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    RoadNode::RoadNode()
    {
        this->minb.x = 0.0;
        this->minb.y = 0.0;
        this->minb.z = 0.0;
        this->maxb.x = 0.0;
        this->maxb.y = 0.0;
        this->maxb.z = 0.0;
        this->m_frameVisible = -1;
        this->m_linkedNodes[0] = 0;
        this->m_linkedNodes[1] = 0;
        this->m_linkedNodes[2] = 0;
        this->m_linkedNodes[3] = 0;
        this->m_bRoadDrawn = 0;
        this->m_geomObject = 0;
        this->m_type = 0;
        this->m_modelNum = 0;
        this->m_skinNumber = 0;
        this->m_friend = 0;
        this->m_asCliff = 0;
        this->m_boundCenter = CVector(0.0, 0.0, 0.0);
        this->m_boundRadius = 0.0;
        this->m_cachedVertices = 0;
    }
}
