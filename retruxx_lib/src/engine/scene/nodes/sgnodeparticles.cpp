#include <stdexcept>
#include <scene/nodes/sgnodeparticles.h>

namespace m3d
{
    RT_CLASS_EXPORTS_BEGIN(SgParticlesNode)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(SgParticlesNode);

    RT_CLASS_EXPORTS_BEGIN(SgParticlesOpaqueNode)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(SgParticlesOpaqueNode);

    Object* SgParticlesNode::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    Class* SgParticlesNode::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SgNode);
    }

    int SgParticlesNode::WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*)
    {
        throw std::logic_error("Not implemented");
    }

    int SgParticlesNode::Think(int, int)
    {
        throw std::logic_error("Not implemented");
    }

    Object* SgParticlesNode::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    int SgParticlesNode::ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*)
    {
        throw std::logic_error("Not implemented");
    }

    Class* SgParticlesNode::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    bool SgParticlesNode::IsFree() const
    {
        throw std::logic_error("Not implemented");
    }

    int SgParticlesNode::GetProperty(unsigned, void*) const
    {
        throw std::logic_error("Not implemented");
    }

    int SgParticlesNode::GetPropertiesList(std::set<unsigned, std::less<unsigned>>&) const
    {
        throw std::logic_error("Not implemented");
    }

    int SgParticlesNode::SetProperty(unsigned, void*)
    {
        throw std::logic_error("Not implemented");
    }

    void SgParticlesNode::Restart()
    {
        throw std::logic_error("Not implemented");
    }

    DataServer* SgParticlesNode::GetServer() const
    {
        throw std::logic_error("Not implemented");
    }

    int SgParticlesNode::Render(SgNodeRenderFlags, void*, int, int)
    {
        throw std::logic_error("Not implemented");
    }

    void SgParticlesNode::CanBeFree()
    {
        throw std::logic_error("Not implemented");
    }

    SgParticlesNode::~SgParticlesNode()
    {
        throw std::logic_error("Not implemented");
    }

    SgParticlesNode::SgParticlesNode(SgParticlesNode const&)
    {
        throw std::logic_error("Not implemented");
    }

    SgParticlesNode::SgParticlesNode()
    {
        throw std::logic_error("Not implemented");
    }

    void SgParticlesNode::UpdateOwnBoundingBox()
    {
        throw std::logic_error("Not implemented");
    }

    Class* SgParticlesOpaqueNode::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SgParticlesNode);
    }

    Object* SgParticlesOpaqueNode::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    SgParticlesOpaqueNode::~SgParticlesOpaqueNode()
    {
        throw std::logic_error("Not implemented");
    }

    Class* SgParticlesOpaqueNode::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    Object* SgParticlesOpaqueNode::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    SgParticlesOpaqueNode::SgParticlesOpaqueNode(SgParticlesOpaqueNode const&)
    {
        throw std::logic_error("Not implemented");
    }

    SgParticlesOpaqueNode::SgParticlesOpaqueNode()
    {
        throw std::logic_error("Not implemented");
    }
}
