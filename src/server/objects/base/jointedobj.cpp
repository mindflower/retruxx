#include "jointedobj.h"
#include "compositeobj.h"
#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(JointedObj)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(JointedObj);

    ExternalJointInfo::ExternalJointInfo()
    {
        throw std::logic_error("Not implemented");
    }

    JointedObjPrototypeInfo::JointedObjPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    bool JointedObjPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    ai::Obj* JointedObjPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    void JointedObj::CalcSplineNeighbours()
    {
        throw std::logic_error("Not implemented");
    }

    void JointedObj::Init(CStr const&, CVector const&, Quaternion const&, float, m3d::SgNode*, float)
    {
        throw std::logic_error("Not implemented");
    }

    JointedObj::JointedObj(JointedObjPrototypeInfo const&)
    {
        throw std::logic_error("Not implemented");
    }

    void JointedObj::AddExternalJoint(int, CVector, CStr)
    {
        throw std::logic_error("Not implemented");
    }

    void JointedObj::SetDisableTimer(int)
    {
        throw std::logic_error("Not implemented");
    }

    void JointedObj::SetDeadTimer(int, bool)
    {
        throw std::logic_error("Not implemented");
    }

    void JointedObj::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    void JointedObj::Remove()
    {
        throw std::logic_error("Not implemented");
    }

    void JointedObj::RelinkSceneGraphNode()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* JointedObj::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    void JointedObj::SetAsRope(bool)
    {
        throw std::logic_error("Not implemented");
    }

    void JointedObj::RenderDebugInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    JointedObjPrototypeInfo const* JointedObj::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    void JointedObj::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    void JointedObj::InitImpulses(CVector, float)
    {
        throw std::logic_error("Not implemented");
    }

    void JointedObj::Disable(bool)
    {
        throw std::logic_error("Not implemented");
    }

    void JointedObj::PostCollide()
    {
        throw std::logic_error("Not implemented");
    }

    void JointedObj::Update(float, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* JointedObj::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    void JointedObj::ReattachExternalJoint(int)
    {
        throw std::logic_error("Not implemented");
    }

    void JointedObj::_InternalPostLoad()
    {
        throw std::logic_error("Not implemented");
    }

    JointedObj::~JointedObj()
    {
        throw std::logic_error("Not implemented");
    }

    void JointedObj::CheckDisablePhysics()
    {
        throw std::logic_error("Not implemented");
    }

    bool JointedObj::IsParent(int, int)
    {
        throw std::logic_error("Not implemented");
    }

    void JointedObj::PutToNewSpace(dxSpace*)
    {
        throw std::logic_error("Not implemented");
    }

    void JointedObj::RecalcBoundBox()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* JointedObj::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* JointedObj::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }
}
