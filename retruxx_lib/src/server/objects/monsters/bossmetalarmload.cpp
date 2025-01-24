#include "bossmetalarmload.h"

#include <stdexcept>

namespace ai
{
	RT_CLASS_EXPORTS_BEGIN(BossMetalArmLoad)
	RT_CLASS_EXPORTS_END;
	RT_CLASS_DEFINE(BossMetalArmLoad);

	BossMetalArmLoadPrototypeInfo::BossMetalArmLoadPrototypeInfo()
	{
		throw std::logic_error("Not implemented");
	}

	bool BossMetalArmLoadPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	Obj* BossMetalArmLoadPrototypeInfo::CreateTargetObject() const
	{
		throw std::logic_error("Not implemented");
	}

	void BossMetalArmLoadPrototypeInfo::PostLoad()
	{
		throw std::logic_error("Not implemented");
	}

	bool BossMetalArmLoad::ApplyModifier(Modifier const&)
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Class* BossMetalArmLoad::GetBaseClass()
	{
		return RT_CLASS_LOCAL(DummyObject);
	}

	void BossMetalArmLoad::SetCollisionMode(CollisionMode)
	{
		throw std::logic_error("Not implemented");
	}

	BossMetalArmLoad::BossMetalArmLoad(BossMetalArmLoadPrototypeInfo const& prototypeInfo) :
        DummyObject(prototypeInfo),
        m_health(prototypeInfo.m_maxHealth, 0.0, prototypeInfo.m_maxHealth)
	{
		throw std::logic_error("Not implemented");
	}

	BossMetalArmLoadPrototypeInfo const* BossMetalArmLoad::GetPrototypeInfo() const
	{
		throw std::logic_error("Not implemented");
	}

	int BossMetalArmLoad::CollideBossMetalArmLoadWithObject(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
	{
		throw std::logic_error("Not implemented");
	}

	void BossMetalArmLoad::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Class* BossMetalArmLoad::GetClass() const
	{
		throw std::logic_error("Not implemented");
	}

	void BossMetalArmLoad::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
	{
		throw std::logic_error("Not implemented");
	}

	void BossMetalArmLoad::Explode()
	{
		throw std::logic_error("Not implemented");
	}

	BossMetalArmLoad::~BossMetalArmLoad()
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* BossMetalArmLoad::CreateObject()
	{
		throw std::logic_error("Not implemented");
	}

	void BossMetalArmLoad::_OnAfterHealthValueChange(float)
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* BossMetalArmLoad::Clone()
	{
		throw std::logic_error("Not implemented");
	}

	void BossMetalArmLoad::_CreateBlastWave()
	{
		throw std::logic_error("Not implemented");
	}
}
