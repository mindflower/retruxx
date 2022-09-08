#include <stdexcept>
#include <scene/nodes/sgnodesprite.h>

namespace m3d
{
    RT_CLASS_EXPORTS_BEGIN(SgSpriteNode)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(SgSpriteNode);

    Object* SgSpriteNode::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    Class* SgSpriteNode::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SgNode);
    }

    Class* SgSpriteNode::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    int SgSpriteNode::GetPropertiesList(std::set<unsigned>&) const
    {
        throw std::logic_error("Not implemented");
    }

    int SgSpriteNode::Render(SgNodeRenderFlags, void*, int, int)
    {
        throw std::logic_error("Not implemented");
    }

    DataServer* SgSpriteNode::GetServer() const
    {
        throw std::logic_error("Not implemented");
    }

    Object* SgSpriteNode::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    int SgSpriteNode::ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*)
    {
        throw std::logic_error("Not implemented");
    }

    int SgSpriteNode::SetProperty(unsigned, void*)
    {
        throw std::logic_error("Not implemented");
    }

    int SgSpriteNode::WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*)
    {
        throw std::logic_error("Not implemented");
    }

    int SgSpriteNode::GetProperty(unsigned, void*) const
    {
        throw std::logic_error("Not implemented");
    }

    SgSpriteNode::~SgSpriteNode()
    {
        throw std::logic_error("Not implemented");
    }

    void SgSpriteNode::UpdateOwnBoundingBox()
    {
        throw std::logic_error("Not implemented");
    }

    SgSpriteNode::SgSpriteNode()
    {
        throw std::logic_error("Not implemented");
    }

    SgSpriteNode::SgSpriteNode(SgSpriteNode const&)
    {
        throw std::logic_error("Not implemented");
    }
}
