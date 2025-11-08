#include "boss02arm.h"

namespace ai
{
	RT_CLASS_EXPORTS_BEGIN(Boss02Arm)
	RT_CLASS_EXPORTS_END;
	RT_CLASS_DEFINE(Boss02Arm);

	bool Boss02ArmPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	Boss02ArmPrototypeInfo::Boss02ArmPrototypeInfo()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Boss02ArmPrototypeInfo::RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Boss02ArmPrototypeInfo::PostLoad()
	{
		RETRUXX_NOT_IMPLEMENTED;
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
}
