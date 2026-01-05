#include "compositeobj.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(CompositeObj)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(CompositeObj);

    bool CompositeObjPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        return PrototypeInfo::LoadFromXML(xmlFile, xmlNode);
    }

    CompositeObjPrototypeInfo::CompositeObjPrototypeInfo() = default;

    ai::Obj* CompositeObjPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CompositeObj::CompositeObj(CompositeObjPrototypeInfo const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CompositeObj::Update(float, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CompositeObj::Disable(bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector CompositeObj::GetPosition()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* CompositeObj::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CompositeObj::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CompositeObj::RelinkSceneGraphNode()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CompositeObj::PostCollide()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Quaternion CompositeObj::GetRotation()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CompositeObj::SetDeadTimer(int, bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CompositeObj::Init(CStr const&, CVector const&, Quaternion const&, float, m3d::SgNode*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* CompositeObj::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    void CompositeObj::RenderDebugInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CompositeObj::Remove()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CompositeObjPrototypeInfo const* CompositeObj::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CompositeObj::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CompositeObj::InitImpulses(CVector, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CompositeObj::~CompositeObj()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CompositeObj::CreateBreakableJoints()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CompositeObj::_InternalPostLoad()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CompositeObj::RecalcBoundBox()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CompositeObj::PutToGlobalSpace()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CompositeObj::PutToTempSpace()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CompositeObj::CheckDisablePhysics()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* CompositeObj::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* CompositeObj::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai
