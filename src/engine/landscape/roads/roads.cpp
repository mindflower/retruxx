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
        throw std::logic_error("Not implemented");
    }

    RoadNode::RoadNode(RoadNode const&)
    {
        throw std::logic_error("Not implemented");
    }

    RoadNode::RoadNode()
    {
        throw std::logic_error("Not implemented");
    }
}
