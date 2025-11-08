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
        RETRUXX_NOT_IMPLEMENTED;
    }

    JointedObjPrototypeInfo::JointedObjPrototypeInfo()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool JointedObjPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ai::Obj* JointedObjPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void JointedObj::CalcSplineNeighbours()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void JointedObj::Init(CStr const&, CVector const&, Quaternion const&, float, m3d::SgNode*, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    JointedObj::JointedObj(JointedObjPrototypeInfo const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void JointedObj::AddExternalJoint(int, CVector, CStr)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void JointedObj::SetDisableTimer(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void JointedObj::SetDeadTimer(int, bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void JointedObj::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void JointedObj::Remove()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void JointedObj::RelinkSceneGraphNode()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* JointedObj::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void JointedObj::SetAsRope(bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void JointedObj::RenderDebugInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    JointedObjPrototypeInfo const* JointedObj::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void JointedObj::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void JointedObj::InitImpulses(CVector, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void JointedObj::Disable(bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void JointedObj::PostCollide()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void JointedObj::Update(float, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* JointedObj::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    void JointedObj::ReattachExternalJoint(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void JointedObj::_InternalPostLoad()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    JointedObj::~JointedObj()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void JointedObj::CheckDisablePhysics()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool JointedObj::IsParent(int, int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void JointedObj::PutToNewSpace(dxSpace*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void JointedObj::RecalcBoundBox()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* JointedObj::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* JointedObj::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}
