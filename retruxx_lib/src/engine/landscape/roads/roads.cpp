#include "road.h"

namespace m3d
{
    RT_CLASS_EXPORTS_BEGIN(RoadNode)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(RoadNode);

    int RoadNode::WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*)
    {
        throw std::logic_error("Not implemented");
    }

    Class* RoadNode::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Object);
    }

    int RoadNode::ReadFromXmlNodeAfterAdd(cmn::XmlFile*, cmn::XmlNode*)
    {
        throw std::logic_error("Not implemented");
    }

    RoadNode::~RoadNode()
    {
        throw std::logic_error("Not implemented");
    }

    int RoadNode::GetSoilType()
    {
        throw std::logic_error("Not implemented");
    }

    int RoadNode::ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*)
    {
        throw std::logic_error("Not implemented");
    }

    void RoadNode::SetOwner(RoadManager*)
    {
        throw std::logic_error("Not implemented");
    }

    CVector RoadNode::GetLinkPoint(float, float)
    {
        throw std::logic_error("Not implemented");
    }

    CVector RoadNode::GetPoint1()
    {
        throw std::logic_error("Not implemented");
    }

    CVector RoadNode::GetPoint2()
    {
        throw std::logic_error("Not implemented");
    }

    Object* RoadNode::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    CVector RoadNode::GetPoint3()
    {
        throw std::logic_error("Not implemented");
    }

    CVector RoadNode::GetPoint4()
    {
        throw std::logic_error("Not implemented");
    }

    Class* RoadNode::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    Object* RoadNode::CreateObject()
    {
        return new RoadNode;
    }

    RoadNode::RoadNode(RoadNode const&)
    {
        throw std::logic_error("Not implemented");
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
