#include "boss02.h"

RT_CLASS_EXPORT_METHOD_DEFINE(Boss02, NextState)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Boss02, SetJustMovedContainerToBlock)
{
    RETRUXX_NOT_IMPLEMENTED;
}

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Boss02)
    RT_CLASS_EXPORT(Boss02, m3d::METHOD, NextState, "", "", "")
    RT_CLASS_EXPORT(Boss02, m3d::METHOD, SetJustMovedContainerToBlock, "", "", "")
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Boss02);

    void Boss02PrototypeInfo::StateInfo::SaveToXML(m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss02PrototypeInfo::StateInfo::PostLoad()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Boss02PrototypeInfo::StateInfo::StateInfo(StateInfo const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss02PrototypeInfo::StateInfo::LoadFromXML(m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Boss02PrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = ComplexPhysicObjPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            // TODO: implement Boss02PrototypeInfo::LoadFromXML
        }
        return result;
    }

    void Boss02PrototypeInfo::PostLoad()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Boss02PrototypeInfo::Boss02PrototypeInfo()
    {
        m_speed = 1.0f;
        m_containerPrototypeId = -1;
        m_relPosForContainerPickUp = ZeroVector;
        m_relRotForContainerPickUp = IdentityQuaternion;
        m_relPosForContainerPutDown = ZeroVector;
    }

    void Boss02PrototypeInfo::RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Obj* Boss02PrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss02::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss02::CreateChildren()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss02::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Boss02::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Boss02::Boss02(Boss02PrototypeInfo const& prototype) : ComplexPhysicObj(prototype)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss02::NextState()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Boss02::CanChildBeAdded(m3d::Class*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Boss02::OnEvent(Event const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss02::SetJustMovedContainerToBlock()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss02::Update(float, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Boss02::GetBaseClass()
    {
        return RT_CLASS_LOCAL(ComplexPhysicObj);
    }

    Boss02PrototypeInfo const* Boss02::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Boss02::RemoveChild(Obj*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss02::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Boss02::GetHealth() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss02::AddChild(Obj*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss02::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss02::SetPositionSelf(CVector const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Boss02::GetMaxHealth() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss02::Die()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss02::_InternalPostLoad()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss02::_InternalCreateVisualPart()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Boss02::~Boss02()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss02::_SetMoveState(MoveState)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss02::_PlaceContainerToInitialPosition()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    VehiclePart* Boss02::_GetPlatform()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss02::_OnBossCriticalLoadsExploded(Event const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Boss02::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss02::_SetMovePosition(CVector const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Boss02::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Boss02Arm* Boss02::_GetArm()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Boss02Arm const* Boss02::_GetArm() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss02::_SetToDeadPose()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss02::_OnBossArmActionFinished(Event const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai
