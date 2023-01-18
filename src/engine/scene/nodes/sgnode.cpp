#include <stdexcept>
#include <scene/nodes/sgnode.h>
#include <math/obb.h>

#include "config.h"
#include "core/kernel.h"
#include "core/console/cvar.h"

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
        //TODO: implement SgNode::UpdateXForm
        return 1;
        //throw std::logic_error("Not implemented");
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
        memset(&this->m_ownXForm, 0, sizeof(this->m_ownXForm));
        this->m_ownXForm._44 = 1.0;
        this->m_ownXForm._33 = 1.0;
        this->m_ownXForm._22 = 1.0;
        this->m_ownXForm._11 = 1.0;
        memset(&this->m_currentXForm, 0, sizeof(this->m_currentXForm));
        this->m_currentXForm._44 = 1.0;
        this->m_currentXForm._33 = 1.0;
        this->m_currentXForm._22 = 1.0;
        this->m_currentXForm._11 = 1.0;
        *&this->m_origin.y = 0i64;
        this->m_origin.x = 0.0;
        this->m_scaling.z = 1.0;
        this->m_scaling.y = 1.0;
        this->m_scaling.x = 1.0;
        this->m_rotation.x = 0.0;
        this->m_rotation.y = 0.0;
        this->m_rotation.z = 0.0;
        this->m_rotation.w = 1.0;
        this->m_boundingRadius = 0.0;
        this->m_isXFormDirty = 0;
        this->m_isOwnBoundingBoxDirty = 1;
        this->m_isChildDirty = 0;
        this->m_ttl = 0;
        this->m_srvId = -1;
        this->m_removeImmediateAfterParent = 1;
        this->m_initedWithRitual = RITUAL_NONE;
        this->m_nextThinkTime = 1;
        this->m_prevThinkTime = 1;
        *&this->m_originWorldAbsForSphere.y = 0i64;
        this->m_originWorldAbsForSphere.x = 0.0;
        this->m_isContoured = 0;
        this->m_transparencyType = TT_NONE;
        this->m_frameTransparent = -1;
        this->m_isRemoveIfFree = 0;
        this->m_isInRemoveIfFree = 0;
        this->m_contourColor = m3d::g_Kernel->GetEngineCfg().m_g_contourColor.GetC();
        this->m_contourWidth = m3d::g_Kernel->GetEngineCfg().m_g_contourWidth.GetF();
        this->m_frameVisible = 0;
        this->m_frameVisible2 = 0;
        this->m_predictIdx = 0;
        this->m_isRootNode = 0;
        this->m_forGraph = 0;
        this->m_isWaitingForRender = 0;
        this->m_initedWithRitual = RITUAL_NONE;
        this->m_onScreenSize = 0.0;
        this->m_properties[0] = 0;
        this->m_properties[1] = 0;
        this->m_properties[2] = 0;
        this->m_props[8] = 1;
        this->m_props[2] = 0;
        this->m_props[4] = 0;
        this->m_props[6] = 0;
        this->m_props[5] = 0;
        this->m_props[7] = 0;
        this->m_props[9] = 0;
        this->m_props[1] = 1000;
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
