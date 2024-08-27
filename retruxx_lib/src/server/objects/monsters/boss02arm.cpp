#include "boss02arm.h"

namespace ai
{
	RT_CLASS_EXPORTS_BEGIN(Boss02Arm)
	RT_CLASS_EXPORTS_END;
	RT_CLASS_DEFINE(Boss02Arm);

	bool Boss02ArmPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	Boss02ArmPrototypeInfo::Boss02ArmPrototypeInfo()
	{
		throw std::logic_error("Not implemented");
	}

	void Boss02ArmPrototypeInfo::RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	void Boss02ArmPrototypeInfo::PostLoad()
	{
		throw std::logic_error("Not implemented");
	}

	Obj* Boss02ArmPrototypeInfo::CreateTargetObject() const
	{
		throw std::logic_error("Not implemented");
	}

	Boss02Arm::Boss02Arm(Boss02ArmPrototypeInfo const& prototype) : BossArm(prototype)
	{
		throw std::logic_error("Not implemented");
	}

	void Boss02Arm::Update(float, unsigned)
	{
		throw std::logic_error("Not implemented");
	}

	void Boss02Arm::BeginMovingContainerToBlock(int)
	{
		throw std::logic_error("Not implemented");
	}

	void Boss02Arm::StopAttack()
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Class* Boss02Arm::GetBaseClass()
	{
		return RT_CLASS_LOCAL(BossArm);
	}

	void Boss02Arm::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	void Boss02Arm::SetToDeadPose()
	{
		throw std::logic_error("Not implemented");
	}

	void Boss02Arm::BeginDie()
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Class* Boss02Arm::GetClass() const
	{
		throw std::logic_error("Not implemented");
	}

	void Boss02Arm::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
	{
		throw std::logic_error("Not implemented");
	}

	Boss02ArmPrototypeInfo const* Boss02Arm::GetPrototypeInfo() const
	{
		throw std::logic_error("Not implemented");
	}

	void Boss02Arm::BeginAttack(std::vector<int, std::allocator<int>> const&)
	{
		throw std::logic_error("Not implemented");
	}

	void Boss02Arm::SetRelativePositionsForContainer(CVector const&, CVector const&)
	{
		throw std::logic_error("Not implemented");
	}

	void Boss02Arm::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	void Boss02Arm::BeginMove()
	{
		throw std::logic_error("Not implemented");
	}

	void Boss02Arm::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
	{
		throw std::logic_error("Not implemented");
	}

	void Boss02Arm::_InternalCreateVisualPart()
	{
		throw std::logic_error("Not implemented");
	}

	Boss02Arm::~Boss02Arm()
	{
		throw std::logic_error("Not implemented");
	}

	void Boss02Arm::_SetCustomState(CustomState)
	{
		throw std::logic_error("Not implemented");
	}

	void Boss02Arm::_SetEffectsDisabled()
	{
		throw std::logic_error("Not implemented");
	}

	void Boss02Arm::_SetCustomNodeAction(int, bool)
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* Boss02Arm::CreateObject()
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* Boss02Arm::Clone()
	{
		throw std::logic_error("Not implemented");
	}
}
