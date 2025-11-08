#include "bossarm.h"

namespace ai
{
	RT_CLASS_EXPORTS_BEGIN(BossArm)
	RT_CLASS_EXPORTS_END;
	RT_CLASS_DEFINE(BossArm);

	void BossArmPrototypeInfo::AttackActionInfo::LoadFromXML(m3d::cmn::XmlNode const*)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	BossArmPrototypeInfo::AttackActionInfo::AttackActionInfo()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void BossArmPrototypeInfo::RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	BossArmPrototypeInfo::BossArmPrototypeInfo()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	Obj* BossArmPrototypeInfo::CreateTargetObject() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	bool BossArmPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	m3d::Class* BossArm::GetClass() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	BossArmPrototypeInfo const* BossArm::GetPrototypeInfo() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	BossArm::BossArm(BossArmPrototypeInfo const& prototype) :
        VehiclePart(prototype),
        m_turningSpeed(prototype.m_turningSpeed)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	int BossArm::GetNumExploadedLoads() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void BossArm::Update(float, unsigned)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void BossArm::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void BossArm::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	bool BossArm::CanChildBeAdded(m3d::Class*) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	int BossArm::OnEvent(Event const&)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void BossArm::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	m3d::Class* BossArm::GetBaseClass()
	{
		return RT_CLASS_LOCAL(VehiclePart);
	}

	void BossArm::_PlaceLoadOnLoadpoint(float)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void BossArm::_SetAttackState(BossArm::AttackState)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	BossArm::AttackState BossArm::_GetAttackState() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void BossArm::_SetLoadPrototypeIds(retruxx::vector<int, retruxx::allocator<int>> const&)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	BossArm::~BossArm()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void BossArm::_TurnToDir(CVector const&, float)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	CVector const& BossArm::_GetCurLoadVelocity() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	m3d::Object* BossArm::CreateObject()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	m3d::Object* BossArm::Clone()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void BossArm::_OnObjectDie(Event const&)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}
}
