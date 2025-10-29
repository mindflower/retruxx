#include "bossmetalarm.h"

RT_CLASS_EXPORT_METHOD_DEFINE(BossMetalArm, Die)
{
	RETRUXX_NOT_IMPLEMENTED;
}

namespace ai
{
	RT_CLASS_EXPORTS_BEGIN(BossMetalArm)
		RT_CLASS_EXPORT(BossMetalArm, m3d::METHOD, Die, "", "", "")
	RT_CLASS_EXPORTS_END;
	RT_CLASS_DEFINE(BossMetalArm);

	BossMetalArmPrototypeInfo::AttackActionInfo::AttackActionInfo()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void BossMetalArmPrototypeInfo::AttackActionInfo::LoadFromXML(m3d::cmn::XmlNode const*)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	BossMetalArmPrototypeInfo::BossMetalArmPrototypeInfo()
	{
        this->m_turningSpeed = 0.5;
        this->m_lpIdForLoad = -1;
	}

	void BossMetalArmPrototypeInfo::RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	bool BossMetalArmPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
	{
        RETRUXX_NOT_IMPLEMENTED;
	}

	void BossMetalArmPrototypeInfo::PostLoad()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	Obj* BossMetalArmPrototypeInfo::CreateTargetObject() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	m3d::Class* BossMetalArm::GetBaseClass()
	{
		return RT_CLASS_LOCAL(SimplePhysicObj);
	}

	BossMetalArm::BossMetalArm(BossMetalArmPrototypeInfo const& prototype) :
        SimplePhysicObj(prototype),
        m_turningSpeed(prototype.m_turningSpeed)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void BossMetalArm::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	bool BossMetalArm::CanChildBeAdded(m3d::Class*) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	BossMetalArmPrototypeInfo const* BossMetalArm::GetPrototypeInfo() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	float BossMetalArm::GetMaxHealth() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	float BossMetalArm::GetHealth() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	int BossMetalArm::OnEvent(Event const&)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void BossMetalArm::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	m3d::Class* BossMetalArm::GetClass() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void BossMetalArm::Update(float, unsigned)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void BossMetalArm::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void BossMetalArm::Die()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void BossMetalArm::SetPositionSelf(CVector const&)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	BossMetalArm::~BossMetalArm()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void BossMetalArm::_TurnToDir(CVector const&, float)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	int BossMetalArm::_GetCurAnimationFrame() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	m3d::Object* BossMetalArm::CreateObject()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	m3d::Object* BossMetalArm::Clone()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void BossMetalArm::_SetAttackState(AttackState)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void BossMetalArm::_PlaceLoadOnLoadpoint()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void BossMetalArm::_OnObjectDie(Event const&)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}
}
