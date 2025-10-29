#include "bossmetalarmload.h"

#include <stdexcept>

namespace ai
{
	RT_CLASS_EXPORTS_BEGIN(BossMetalArmLoad)
	RT_CLASS_EXPORTS_END;
	RT_CLASS_DEFINE(BossMetalArmLoad);

	BossMetalArmLoadPrototypeInfo::BossMetalArmLoadPrototypeInfo()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	bool BossMetalArmLoadPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	Obj* BossMetalArmLoadPrototypeInfo::CreateTargetObject() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void BossMetalArmLoadPrototypeInfo::PostLoad()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	bool BossMetalArmLoad::ApplyModifier(Modifier const&)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	m3d::Class* BossMetalArmLoad::GetBaseClass()
	{
		return RT_CLASS_LOCAL(DummyObject);
	}

	void BossMetalArmLoad::SetCollisionMode(CollisionMode)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	BossMetalArmLoad::BossMetalArmLoad(BossMetalArmLoadPrototypeInfo const& prototypeInfo) :
        DummyObject(prototypeInfo),
        m_health(prototypeInfo.m_maxHealth, 0.0, prototypeInfo.m_maxHealth)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	BossMetalArmLoadPrototypeInfo const* BossMetalArmLoad::GetPrototypeInfo() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	int BossMetalArmLoad::CollideBossMetalArmLoadWithObject(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void BossMetalArmLoad::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	m3d::Class* BossMetalArmLoad::GetClass() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void BossMetalArmLoad::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void BossMetalArmLoad::Explode()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	BossMetalArmLoad::~BossMetalArmLoad()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	m3d::Object* BossMetalArmLoad::CreateObject()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void BossMetalArmLoad::_OnAfterHealthValueChange(float)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	m3d::Object* BossMetalArmLoad::Clone()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void BossMetalArmLoad::_CreateBlastWave()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}
}
