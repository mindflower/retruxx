#include <stdexcept>
#include <scene/nodes/sgnodesound.h>

namespace m3d
{
    RT_CLASS_EXPORTS_BEGIN(SgSoundSourceNode)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(SgSoundSourceNode);

    Object* SgSoundSourceNode::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    Class* SgSoundSourceNode::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    void SgSoundSourceNode::CanBeFree()
    {
        throw std::logic_error("Not implemented");
    }

    int SgSoundSourceNode::ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*)
    {
        throw std::logic_error("Not implemented");
    }

    DataServer* SgSoundSourceNode::GetServer() const
    {
        throw std::logic_error("Not implemented");
    }

    int SgSoundSourceNode::GetPropertiesList(std::set<unsigned>&) const
    {
        throw std::logic_error("Not implemented");
    }

    Class* SgSoundSourceNode::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    int SgSoundSourceNode::WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*)
    {
        throw std::logic_error("Not implemented");
    }

    float SgSoundSourceNode::IntersectRay(CVector const&, CVector const&, SgNode*&, Class*)
    {
        throw std::logic_error("Not implemented");
    }

    int SgSoundSourceNode::Render(SgNodeRenderFlags, void*, int, int)
    {
        throw std::logic_error("Not implemented");
    }

    Object* SgSoundSourceNode::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    int SgSoundSourceNode::GetProperty(unsigned, void*) const
    {
        throw std::logic_error("Not implemented");
    }

    int SgSoundSourceNode::SetProperty(unsigned, void*)
    {
        throw std::logic_error("Not implemented");
    }

    void SgSoundSourceNode::Restart()
    {
        throw std::logic_error("Not implemented");
    }

    bool SgSoundSourceNode::IsFree() const
    {
        throw std::logic_error("Not implemented");
    }

    SgSoundSourceNode::~SgSoundSourceNode()
    {
        throw std::logic_error("Not implemented");
    }

    SgSoundSourceNode::SgSoundSourceNode()
    {
        throw std::logic_error("Not implemented");
    }

    SgSoundSourceNode::SgSoundSourceNode(SgSoundSourceNode const&)
    {
        throw std::logic_error("Not implemented");
    }

    void SgSoundSourceNode::UpdateOwnBoundingBox()
    {
        throw std::logic_error("Not implemented");
    }

    bool SgSoundSourceNode::_OnSoundStopped()
    {
        throw std::logic_error("Not implemented");
    }

    int SgSoundSourceNode::_InternalRender()
    {
        throw std::logic_error("Not implemented");
    }
}
