#include "bossarm.h"

namespace ai
{
	RT_CLASS_EXPORTS_BEGIN(BossArm)
	RT_CLASS_EXPORTS_END;
	RT_CLASS_DEFINE(BossArm);

	void BossArmPrototypeInfo::AttackActionInfo::LoadFromXML(m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	BossArmPrototypeInfo::AttackActionInfo::AttackActionInfo()
	{
		throw std::logic_error("Not implemented");
	}

	void BossArmPrototypeInfo::RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	BossArmPrototypeInfo::BossArmPrototypeInfo()
	{
		throw std::logic_error("Not implemented");
	}

	Obj* BossArmPrototypeInfo::CreateTargetObject() const
	{
		throw std::logic_error("Not implemented");
	}

	bool BossArmPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Class* BossArm::GetClass() const
	{
		throw std::logic_error("Not implemented");
	}

	BossArmPrototypeInfo const* BossArm::GetPrototypeInfo() const
	{
		throw std::logic_error("Not implemented");
	}

	BossArm::BossArm(BossArmPrototypeInfo const& prototype) : VehiclePart(prototype)
	{
		throw std::logic_error("Not implemented");
	}

	int BossArm::GetNumExploadedLoads() const
	{
		throw std::logic_error("Not implemented");
	}

	void BossArm::Update(float, unsigned)
	{
		throw std::logic_error("Not implemented");
	}

	void BossArm::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	void BossArm::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	bool BossArm::CanChildBeAdded(m3d::Class*) const
	{
		throw std::logic_error("Not implemented");
	}

	int BossArm::OnEvent(Event const&)
	{
		throw std::logic_error("Not implemented");
	}

	void BossArm::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Class* BossArm::GetBaseClass()
	{
		return RT_CLASS_LOCAL(VehiclePart);
	}

	void BossArm::_PlaceLoadOnLoadpoint(float)
	{
		throw std::logic_error("Not implemented");
	}

	void BossArm::_SetAttackState(BossArm::AttackState)
	{
		throw std::logic_error("Not implemented");
	}

	BossArm::AttackState BossArm::_GetAttackState() const
	{
		throw std::logic_error("Not implemented");
	}

	void BossArm::_SetLoadPrototypeIds(std::vector<int, std::allocator<int>> const&)
	{
		throw std::logic_error("Not implemented");
	}

	BossArm::~BossArm()
	{
		throw std::logic_error("Not implemented");
	}

	void BossArm::_TurnToDir(CVector const&, float)
	{
		throw std::logic_error("Not implemented");
	}

	CVector const& BossArm::_GetCurLoadVelocity() const
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* BossArm::CreateObject()
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* BossArm::Clone()
	{
		throw std::logic_error("Not implemented");
	}

	void BossArm::_OnObjectDie(Event const&)
	{
		throw std::logic_error("Not implemented");
	}
}
