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
        throw retruxx::logic_error("Not implemented");
    }

    Class* SgParticlesNode::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SgNode);
    }

    int SgParticlesNode::WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgParticlesNode::Think(int, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    Object* SgParticlesNode::Clone()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgParticlesNode::ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    Class* SgParticlesNode::GetClass() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool SgParticlesNode::IsFree() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgParticlesNode::GetProperty(unsigned, void*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgParticlesNode::GetPropertiesList(retruxx::set<unsigned, retruxx::less<unsigned>>&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgParticlesNode::SetProperty(unsigned, void*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SgParticlesNode::Restart()
    {
        throw retruxx::logic_error("Not implemented");
    }

    DataServer* SgParticlesNode::GetServer() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgParticlesNode::Render(SgNodeRenderFlags, void*, int, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SgParticlesNode::CanBeFree()
    {
        throw retruxx::logic_error("Not implemented");
    }

    SgParticlesNode::~SgParticlesNode()
    {
        throw retruxx::logic_error("Not implemented");
    }

    SgParticlesNode::SgParticlesNode(SgParticlesNode const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    SgParticlesNode::SgParticlesNode()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SgParticlesNode::UpdateOwnBoundingBox()
    {
        throw retruxx::logic_error("Not implemented");
    }

    Class* SgParticlesOpaqueNode::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SgParticlesNode);
    }

    Object* SgParticlesOpaqueNode::CreateObject()
    {
        throw retruxx::logic_error("Not implemented");
    }

    SgParticlesOpaqueNode::~SgParticlesOpaqueNode()
    {
        throw retruxx::logic_error("Not implemented");
    }

    Class* SgParticlesOpaqueNode::GetClass() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    Object* SgParticlesOpaqueNode::Clone()
    {
        throw retruxx::logic_error("Not implemented");
    }

    SgParticlesOpaqueNode::SgParticlesOpaqueNode(SgParticlesOpaqueNode const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    SgParticlesOpaqueNode::SgParticlesOpaqueNode()
    {
        throw retruxx::logic_error("Not implemented");
    }
}
