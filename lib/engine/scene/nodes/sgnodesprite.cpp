#include <stdexcept>
#include <scene/nodes/sgnodesprite.h>

namespace m3d
{
    RT_CLASS_EXPORTS_BEGIN(SgSpriteNode)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(SgSpriteNode);

    Object* SgSpriteNode::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Class* SgSpriteNode::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SgNode);
    }

    Class* SgSpriteNode::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int SgSpriteNode::GetPropertiesList(retruxx::set<unsigned>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int SgSpriteNode::Render(SgNodeRenderFlags, void*, int, int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    DataServer* SgSpriteNode::GetServer() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Object* SgSpriteNode::Clone()
    {
        return new SgSpriteNode(*this);
    }

    int SgSpriteNode::ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int SgSpriteNode::SetProperty(unsigned, void*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int SgSpriteNode::WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int SgSpriteNode::GetProperty(unsigned, void*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    SgSpriteNode::~SgSpriteNode()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void SgSpriteNode::UpdateOwnBoundingBox()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    SgSpriteNode::SgSpriteNode()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    SgSpriteNode::SgSpriteNode(SgSpriteNode const& node) : SgNode(node)
    {
        for (int i = 0; i < 6; ++i)
        {
            m_props[i] = node.m_props[i];
        }
    }
}
