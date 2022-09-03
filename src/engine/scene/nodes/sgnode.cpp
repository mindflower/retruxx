#include <stdexcept>
#include <scene/nodes/sgnode.h>
#include <math/obb.h>

RT_CLASS_EXPORT_METHOD_DEFINE(SgNode, GetOrigin)
{
    throw std::logic_error("Not implemented");
}

namespace m3d
{
    RT_CLASS_EXPORTS_BEGIN(SgNode)
        RT_CLASS_EXPORT(SgNode, m3d::METHOD, GetOrigin, "", "", "")
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(SgNode);

    Object* SgNode::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    Class* SgNode::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Object);
    }

    Obb SgNode::GetObb() const
    {
        throw std::logic_error("Not implemented");
    }

    CVector const& SgNode::GetOrigin() const
    {
        throw std::logic_error("Not implemented");
    }

    CMatrix const& SgNode::GetCurrentMatrix() const
    {
        throw std::logic_error("Not implemented");
    }

    int SgNode::GetPrevThinkTime() const
    {
        throw std::logic_error("Not implemented");
    }

    int SgNode::SetScale(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    void SgNode::SetTransparencyType(TransparencyType)
    {
        throw std::logic_error("Not implemented");
    }

    int SgNode::GetServerItemProperty(unsigned, void*) const
    {
        throw std::logic_error("Not implemented");
    }

    bool SgNode::IsXFormUpdateNeeded() const
    {
        throw std::logic_error("Not implemented");
    }

    int SgNode::WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*)
    {
        throw std::logic_error("Not implemented");
    }

    CVector const& SgNode::GetOriginWorldAbs() const
    {
        throw std::logic_error("Not implemented");
    }

    void SgNode::SetPrevThinkTime(int)
    {
        throw std::logic_error("Not implemented");
    }

    CVector const& SgNode::GetScale() const
    {
        throw std::logic_error("Not implemented");
    }

    Class* SgNode::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    int SgNode::SetServerItemProperty(unsigned, void*) const
    {
        throw std::logic_error("Not implemented");
    }

    int SgNode::Think(int, int)
    {
        throw std::logic_error("Not implemented");
    }

    int SgNode::GetTtl() const
    {
        throw std::logic_error("Not implemented");
    }

    void SgNode::CanBeFree()
    {
        throw std::logic_error("Not implemented");
    }

    CVector const& SgNode::GetOriginWorldAbsForSphere() const
    {
        throw std::logic_error("Not implemented");
    }

    int SgNode::RemoveChild(Object*)
    {
        throw std::logic_error("Not implemented");
    }

    int SgNode::GetNextThinkTime() const
    {
        throw std::logic_error("Not implemented");
    }

    int SgNode::Render(SgNodeRenderFlags, void*, int, int)
    {
        throw std::logic_error("Not implemented");
    }

    Quaternion const& SgNode::GetRotation() const
    {
        throw std::logic_error("Not implemented");
    }

    int SgNode::GetProperty(unsigned, void*) const
    {
        throw std::logic_error("Not implemented");
    }

    SceneGraph* SgNode::GetGraph()
    {
        throw std::logic_error("Not implemented");
    }

    int SgNode::GetServerHandle() const
    {
        throw std::logic_error("Not implemented");
    }

    bool SgNode::IsFree() const
    {
        throw std::logic_error("Not implemented");
    }

    DataServer* SgNode::GetServer() const
    {
        throw std::logic_error("Not implemented");
    }

    Aabb SgNode::GetAabb() const
    {
        throw std::logic_error("Not implemented");
    }

    int SgNode::AddChild(Object*)
    {
        throw std::logic_error("Not implemented");
    }

    Object* SgNode::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    int SgNode::ReadFromXmlNodeAfterAdd(cmn::XmlFile*, cmn::XmlNode*)
    {
        throw std::logic_error("Not implemented");
    }

    TransparencyParams& SgNode::GetTransparencyParams()
    {
        throw std::logic_error("Not implemented");
    }

    void SgNode::SetBoundingBoxDirty()
    {
        throw std::logic_error("Not implemented");
    }

    void SgNode::RemoveImmediateAfterParent(bool)
    {
        throw std::logic_error("Not implemented");
    }

    unsigned SgNode::GetContourColor()
    {
        throw std::logic_error("Not implemented");
    }

    Quaternion const& SgNode::GetRotationWorldAbs() const
    {
        throw std::logic_error("Not implemented");
    }

    int SgNode::SetRotation(Quaternion const&)
    {
        throw std::logic_error("Not implemented");
    }

    int SgNode::SetProperty(unsigned, void*)
    {
        throw std::logic_error("Not implemented");
    }

    float SgNode::GetBoundingRadius() const
    {
        throw std::logic_error("Not implemented");
    }

    void SgNode::GetVisCellBounds(PointBase<int>&, PointBase<int>&) const
    {
        throw std::logic_error("Not implemented");
    }

    float SgNode::GetContourWidth()
    {
        throw std::logic_error("Not implemented");
    }

    int SgNode::SetOriginAbs(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    int SgNode::UpdateXForm(bool, bool)
    {
        throw std::logic_error("Not implemented");
    }

    bool SgNode::VisCellBoundsChanged() const
    {
        throw std::logic_error("Not implemented");
    }

    int SgNode::ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*)
    {
        throw std::logic_error("Not implemented");
    }

    int SgNode::GetPropertiesList(std::set<unsigned, std::less<unsigned>, std::allocator<unsigned>>&) const
    {
        throw std::logic_error("Not implemented");
    }

    Aabb SgNode::GetOwnAabb() const
    {
        throw std::logic_error("Not implemented");
    }

    float SgNode::IntersectRay(CVector const&, CVector const&, SgNode*&, Class*)
    {
        throw std::logic_error("Not implemented");
    }

    void SgNode::Restart()
    {
        throw std::logic_error("Not implemented");
    }

    CMatrix SgNode::MatrixFromFlags(SgNodeRenderFlags, void*) const
    {
        throw std::logic_error("Not implemented");
    }

    void SgNode::UpdateOwnBoundingBox()
    {
        throw std::logic_error("Not implemented");
    }

    void SgNode::RitualInConstructor(Ritual)
    {
        throw std::logic_error("Not implemented");
    }

    SgNode::~SgNode()
    {
        throw std::logic_error("Not implemented");
    }

    SgNode::SgNode(SgNode const&)
    {
        throw std::logic_error("Not implemented");
    }

    SgNode::SgNode()
    {
        throw std::logic_error("Not implemented");
    }

    void SgNode::RitualInDestructor()
    {
        throw std::logic_error("Not implemented");
    }

    void SgNode::InternalInit()
    {
        throw std::logic_error("Not implemented");
    }
}
