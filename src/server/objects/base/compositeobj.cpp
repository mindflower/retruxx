#include "compositeobj.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(CompositeObj)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(CompositeObj);

    bool CompositeObjPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    CompositeObjPrototypeInfo::CompositeObjPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    ai::Obj* CompositeObjPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    CompositeObj::CompositeObj(CompositeObjPrototypeInfo const&)
    {
        throw std::logic_error("Not implemented");
    }

    void CompositeObj::Update(float, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    void CompositeObj::Disable(bool)
    {
        throw std::logic_error("Not implemented");
    }

    CVector CompositeObj::GetPosition()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* CompositeObj::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    void CompositeObj::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    void CompositeObj::RelinkSceneGraphNode()
    {
        throw std::logic_error("Not implemented");
    }

    void CompositeObj::PostCollide()
    {
        throw std::logic_error("Not implemented");
    }

    Quaternion CompositeObj::GetRotation()
    {
        throw std::logic_error("Not implemented");
    }

    void CompositeObj::SetDeadTimer(int, bool)
    {
        throw std::logic_error("Not implemented");
    }

    void CompositeObj::Init(CStr const&, CVector const&, Quaternion const&, float, m3d::SgNode*)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* CompositeObj::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    void CompositeObj::RenderDebugInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    void CompositeObj::Remove()
    {
        throw std::logic_error("Not implemented");
    }

    CompositeObjPrototypeInfo const* CompositeObj::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    void CompositeObj::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    void CompositeObj::InitImpulses(CVector, float)
    {
        throw std::logic_error("Not implemented");
    }

    CompositeObj::~CompositeObj()
    {
        throw std::logic_error("Not implemented");
    }

    void CompositeObj::CreateBreakableJoints()
    {
        throw std::logic_error("Not implemented");
    }

    void CompositeObj::_InternalPostLoad()
    {
        throw std::logic_error("Not implemented");
    }

    void CompositeObj::RecalcBoundBox()
    {
        throw std::logic_error("Not implemented");
    }

    void CompositeObj::PutToGlobalSpace()
    {
        throw std::logic_error("Not implemented");
    }

    void CompositeObj::PutToTempSpace()
    {
        throw std::logic_error("Not implemented");
    }

    void CompositeObj::CheckDisablePhysics()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* CompositeObj::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* CompositeObj::Clone()
    {
        throw std::logic_error("Not implemented");
    }
}
