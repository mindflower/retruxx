#include "bossmetalarm.h"

RT_CLASS_EXPORT_METHOD_DEFINE(BossMetalArm, Die)
{
	throw std::logic_error("Not implemented");
}

namespace ai
{
	RT_CLASS_EXPORTS_BEGIN(BossMetalArm)
		RT_CLASS_EXPORT(BossMetalArm, m3d::METHOD, Die, "", "", "")
	RT_CLASS_EXPORTS_END;
	RT_CLASS_DEFINE(BossMetalArm);

	BossMetalArmPrototypeInfo::AttackActionInfo::AttackActionInfo()
	{
		throw std::logic_error("Not implemented");
	}

	void BossMetalArmPrototypeInfo::AttackActionInfo::LoadFromXML(m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	BossMetalArmPrototypeInfo::BossMetalArmPrototypeInfo()
	{
		throw std::logic_error("Not implemented");
	}

	void BossMetalArmPrototypeInfo::RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	bool BossMetalArmPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	void BossMetalArmPrototypeInfo::PostLoad()
	{
		throw std::logic_error("Not implemented");
	}

	Obj* BossMetalArmPrototypeInfo::CreateTargetObject() const
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Class* BossMetalArm::GetBaseClass()
	{
		return RT_CLASS_LOCAL(SimplePhysicObj);
	}

	BossMetalArm::BossMetalArm(BossMetalArmPrototypeInfo const& prototype) :
        SimplePhysicObj(prototype),
        m_turningSpeed(prototype.m_turningSpeed)
	{
		throw std::logic_error("Not implemented");
	}

	void BossMetalArm::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	bool BossMetalArm::CanChildBeAdded(m3d::Class*) const
	{
		throw std::logic_error("Not implemented");
	}

	BossMetalArmPrototypeInfo const* BossMetalArm::GetPrototypeInfo() const
	{
		throw std::logic_error("Not implemented");
	}

	float BossMetalArm::GetMaxHealth() const
	{
		throw std::logic_error("Not implemented");
	}

	float BossMetalArm::GetHealth() const
	{
		throw std::logic_error("Not implemented");
	}

	int BossMetalArm::OnEvent(Event const&)
	{
		throw std::logic_error("Not implemented");
	}

	void BossMetalArm::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Class* BossMetalArm::GetClass() const
	{
		throw std::logic_error("Not implemented");
	}

	void BossMetalArm::Update(float, unsigned)
	{
		throw std::logic_error("Not implemented");
	}

	void BossMetalArm::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	void BossMetalArm::Die()
	{
		throw std::logic_error("Not implemented");
	}

	void BossMetalArm::SetPositionSelf(CVector const&)
	{
		throw std::logic_error("Not implemented");
	}

	BossMetalArm::~BossMetalArm()
	{
		throw std::logic_error("Not implemented");
	}

	void BossMetalArm::_TurnToDir(CVector const&, float)
	{
		throw std::logic_error("Not implemented");
	}

	int BossMetalArm::_GetCurAnimationFrame() const
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* BossMetalArm::CreateObject()
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* BossMetalArm::Clone()
	{
		throw std::logic_error("Not implemented");
	}

	void BossMetalArm::_SetAttackState(AttackState)
	{
		throw std::logic_error("Not implemented");
	}

	void BossMetalArm::_PlaceLoadOnLoadpoint()
	{
		throw std::logic_error("Not implemented");
	}

	void BossMetalArm::_OnObjectDie(Event const&)
	{
		throw std::logic_error("Not implemented");
	}
}
