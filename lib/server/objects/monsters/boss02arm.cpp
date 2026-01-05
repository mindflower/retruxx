#include "boss02arm.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Boss02Arm)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Boss02Arm);

    bool Boss02ArmPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = BossArmPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeIntAttrib(m_frameToPickUpContainerForBlock, xmlNode, "FrameToPickUpContainerForBlock");
            m3d::SafeIntAttrib(m_frameToReleaseContainerForBlock, xmlNode, "FrameToReleaseContainerForBlock");
            m3d::SafeIntAttrib(m_frameToPickUpContainerForDie, xmlNode, "FrameToPickUpContainerForDie");
            m3d::SafeIntAttrib(m_frameToReleaseContainerForDie, xmlNode, "FrameToReleaseContainerForDie");

            CStr strActionForBlock;
            m3d::SafeStrAttrib(strActionForBlock, xmlNode, "ActionForBlock");
            m_actionForBlock = m3d::GetActionByName(strActionForBlock.c_str());

            CStr strActionForDie;
            m3d::SafeStrAttrib(strActionForDie, xmlNode, "ActionForDie");
            m_actionForDie = m3d::GetActionByName(strActionForDie.c_str());

            m3d::SafeStrAttrib(m_blockingContainerPrototypeName, xmlNode, "ContainerPrototype");
        }
        return result;
    }

    Boss02ArmPrototypeInfo::Boss02ArmPrototypeInfo()
    {
        m_frameToPickUpContainerForBlock = 0;
        m_frameToReleaseContainerForBlock = 0;
        m_frameToPickUpContainerForDie = 0;
        m_frameToReleaseContainerForDie = 0;
        m_actionForBlock = AT_STAND1;
        m_actionForDie = AT_STAND1;
        m_blockingContainerPrototypeId = -1;
    }

    void Boss02ArmPrototypeInfo::RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        // TODO: implement ::RefreshFromXml
    }

    void Boss02ArmPrototypeInfo::PostLoad()
    {
        // TODO: implement ::PostLoad
    }

    Obj* Boss02ArmPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Boss02Arm::Boss02Arm(Boss02ArmPrototypeInfo const& prototype) : BossArm(prototype)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss02Arm::Update(float, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss02Arm::BeginMovingContainerToBlock(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss02Arm::StopAttack()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Boss02Arm::GetBaseClass()
    {
        return RT_CLASS_LOCAL(BossArm);
    }

    void Boss02Arm::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss02Arm::SetToDeadPose()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss02Arm::BeginDie()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Boss02Arm::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss02Arm::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Boss02ArmPrototypeInfo const* Boss02Arm::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss02Arm::BeginAttack(std::vector<int, std::allocator<int>> const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss02Arm::SetRelativePositionsForContainer(CVector const&, CVector const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss02Arm::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss02Arm::BeginMove()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss02Arm::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss02Arm::_InternalCreateVisualPart()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Boss02Arm::~Boss02Arm()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss02Arm::_SetCustomState(CustomState)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss02Arm::_SetEffectsDisabled()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss02Arm::_SetCustomNodeAction(int, bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Boss02Arm::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Boss02Arm::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai
