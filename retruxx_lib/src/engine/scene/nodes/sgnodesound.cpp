#include <stdexcept>
#include <scene/nodes/sgnodesound.h>

namespace m3d
{
    RT_CLASS_EXPORTS_BEGIN(SgSoundSourceNode)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(SgSoundSourceNode);

    Object* SgSoundSourceNode::CreateObject()
    {
        throw retruxx::logic_error("Not implemented");
    }

    Class* SgSoundSourceNode::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SgNode);
    }

    void SgSoundSourceNode::CanBeFree()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgSoundSourceNode::ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    DataServer* SgSoundSourceNode::GetServer() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgSoundSourceNode::GetPropertiesList(retruxx::set<unsigned>&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    Class* SgSoundSourceNode::GetClass() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgSoundSourceNode::WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    float SgSoundSourceNode::IntersectRay(CVector const&, CVector const&, SgNode*&, Class*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgSoundSourceNode::Render(SgNodeRenderFlags, void*, int, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    Object* SgSoundSourceNode::Clone()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgSoundSourceNode::GetProperty(unsigned, void*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgSoundSourceNode::SetProperty(unsigned, void*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SgSoundSourceNode::Restart()
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool SgSoundSourceNode::IsFree() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    SgSoundSourceNode::~SgSoundSourceNode()
    {
        throw retruxx::logic_error("Not implemented");
    }

    SgSoundSourceNode::SgSoundSourceNode()
    {
        throw retruxx::logic_error("Not implemented");
    }

    SgSoundSourceNode::SgSoundSourceNode(SgSoundSourceNode const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SgSoundSourceNode::UpdateOwnBoundingBox()
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool SgSoundSourceNode::_OnSoundStopped()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgSoundSourceNode::_InternalRender()
    {
        throw retruxx::logic_error("Not implemented");
    }
}
