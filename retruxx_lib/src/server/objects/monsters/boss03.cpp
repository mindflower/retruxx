#include "boss03.h"

#include <stdexcept>

RT_CLASS_EXPORT_METHOD_DEFINE(Boss03, StartPlacingDrone)
{
	throw retruxx::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Boss03, StartMoving)
{
	throw retruxx::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Boss03, NoticePlayer)
{
	throw retruxx::logic_error("Not implemented");
}

namespace ai
{
	RT_CLASS_EXPORTS_BEGIN(Boss03)
		RT_CLASS_EXPORT(Boss03, m3d::METHOD, StartPlacingDrone, "", "", "")
		RT_CLASS_EXPORT(Boss03, m3d::METHOD, StartMoving, "", "", "")
		RT_CLASS_EXPORT(Boss03, m3d::METHOD, NoticePlayer, "", "", "")
	RT_CLASS_EXPORTS_END;
	RT_CLASS_DEFINE(Boss03);

	bool Boss03PrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw retruxx::logic_error("Not implemented");
	}

	Boss03PrototypeInfo::Boss03PrototypeInfo()
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss03PrototypeInfo::PostLoad()
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss03PrototypeInfo::RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw retruxx::logic_error("Not implemented");
	}

	Obj* Boss03PrototypeInfo::CreateTargetObject() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss03::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss03::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss03::CreateChildren()
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss03::Registration()
	{
	}

	Boss03::Boss03(Boss03PrototypeInfo const& prototype) :
        AnimatedComplexPhysicObj(prototype),
        m_health(prototype.m_maxHealth, 0.0, prototype.m_maxHealth)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss03::InflictDamage(DamageInfo const&)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss03::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool Boss03::StartPlacingDrone()
	{
		throw retruxx::logic_error("Not implemented");
	}

	m3d::Class* Boss03::GetClass() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	CStr Boss03::GetPropertyName(int) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool Boss03::CanChildBeAdded(m3d::Class*) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss03::StartMoving(CVector const&, CVector const&)
	{
		throw retruxx::logic_error("Not implemented");
	}

	eGObjPropertySaveStatus Boss03::GetPropertySaveStatus(int) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss03::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	m3d::Class* Boss03::GetBaseClass()
	{
		return RT_CLASS_LOCAL(AnimatedComplexPhysicObj);
	}

	void Boss03::Update(float, unsigned)
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool Boss03::ApplyModifier(Modifier const&)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss03::NoticePlayer()
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool Boss03::SetPropertyById(int, m3d::AIParam const&)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss03::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw retruxx::logic_error("Not implemented");
	}

	float Boss03::GetHealth() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss03::AddChild(Obj*)
	{
		throw retruxx::logic_error("Not implemented");
	}

	Boss03PrototypeInfo const* Boss03::GetPrototypeInfo() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool Boss03::RemoveChild(Obj*)
	{
		throw retruxx::logic_error("Not implemented");
	}

	int Boss03::GetPropertyId(char const*) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss03::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw retruxx::logic_error("Not implemented");
	}

	float Boss03::GetMaxHealth() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss03::_InternalPostLoad()
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool Boss03::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss03::_Construct(bool)
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool Boss03::_GetPropertyInternal(int, m3d::AIParam&) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss03::_InternalCreateVisualPart()
	{
		throw retruxx::logic_error("Not implemented");
	}

	Boss03::~Boss03()
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss03::_SetAllPartsNotDamageable()
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss03::_UpdateLiveFightingWithWings(float)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss03::_SetDroneSpawningStatus(DroneSpawningStatus)
	{
		throw retruxx::logic_error("Not implemented");
	}

	Boss03Part* Boss03::_GetBossPartByName(CStr const&)
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool Boss03::_OnHealthValueBeforeApplyModifier(Modifier const&, float&)
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool Boss03::_ProceedSettingVerticalAngle(float, float)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss03::_SpawnDroneNow()
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss03::_UpdatePathTracking(float)
	{
		throw retruxx::logic_error("Not implemented");
	}

	Boss03Part* Boss03::_GetMainPart()
	{
		throw retruxx::logic_error("Not implemented");
	}

	Boss03Part* Boss03::_GetDroneSpawner()
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss03::_SetLiveStatus(LiveStatus)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss03::_RecalcFlyPath()
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss03::_UpdateLookAtPlayer(float)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss03::_UpdateLivePlacingDrones(float)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss03::_UpdateSwinging(float)
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool Boss03::_ProceedSettingHorizontalDirection(float, CVector const&)
	{
		throw retruxx::logic_error("Not implemented");
	}

	CStr const& Boss03::_DroneSpawningStatusToStr(DroneSpawningStatus)
	{
		throw retruxx::logic_error("Not implemented");
	}

	m3d::Object* Boss03::CreateObject()
	{
		throw retruxx::logic_error("Not implemented");
	}

	m3d::Object* Boss03::Clone()
	{
		throw retruxx::logic_error("Not implemented");
	}

	CStr const& Boss03::_LiveStatusToStr(LiveStatus)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss03::_SetPathTrackingStatus(PathTrackingStatus)
	{
		throw retruxx::logic_error("Not implemented");
	}

	float Boss03::CalcKeyPartsMaxDurability() const
	{
		throw retruxx::logic_error("Not implemented");
	}
}
