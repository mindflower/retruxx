#include <scene/nodes/sgnodeanimatedmodel.h>

namespace m3d
{
    RT_CLASS_EXPORTS_BEGIN(SgAnimatedModelNode)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(SgAnimatedModelNode);

    Class* SgAnimatedModelNode::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SgNode);
    }

    Object* SgAnimatedModelNode::CreateObject()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgAnimatedModelNode::UpdateXForm(bool, bool)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgAnimatedModelNode::ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgAnimatedModelNode::ReadFromXmlNodeAfterAdd(cmn::XmlFile*, cmn::XmlNode*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Object* SgAnimatedModelNode::Clone()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgAnimatedModelNode::Think(int, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgAnimatedModelNode::WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgAnimatedModelNode::Render(SgNodeRenderFlags, void*, int, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgAnimatedModelNode::SetProperty(unsigned, void*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    DataServer* SgAnimatedModelNode::GetServer() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgAnimatedModelNode::GetPropertiesList(retruxx::set<unsigned>&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgAnimatedModelNode::GetProperty(unsigned, void*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    Class* SgAnimatedModelNode::GetClass() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    SgAnimatedModelNode::~SgAnimatedModelNode()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SgAnimatedModelNode::UpdateOwnBoundingBox()
    {
        throw retruxx::logic_error("Not implemented");
    }

    SgAnimatedModelNode::SgAnimatedModelNode()
    {
        throw retruxx::logic_error("Not implemented");
    }

    SgAnimatedModelNode::SgAnimatedModelNode(SgAnimatedModelNode const&)
    {
        throw retruxx::logic_error("Not implemented");
    }
}
