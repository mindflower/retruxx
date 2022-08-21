#include "boss03.h"

#include <stdexcept>

RT_CLASS_EXPORT_METHOD_DEFINE(Boss03, StartPlacingDrone)
{
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Boss03, StartMoving)
{
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Boss03, NoticePlayer)
{
	throw std::logic_error("Not implemented");
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
		throw std::logic_error("Not implemented");
	}

	Boss03PrototypeInfo::Boss03PrototypeInfo()
	{
		throw std::logic_error("Not implemented");
	}

	void Boss03PrototypeInfo::PostLoad()
	{
		throw std::logic_error("Not implemented");
	}

	void Boss03PrototypeInfo::RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	Obj* Boss03PrototypeInfo::CreateTargetObject() const
	{
		throw std::logic_error("Not implemented");
	}

	void Boss03::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
	{
		throw std::logic_error("Not implemented");
	}

	void Boss03::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
	{
		throw std::logic_error("Not implemented");
	}

	void Boss03::CreateChildren()
	{
		throw std::logic_error("Not implemented");
	}

	void Boss03::Registration()
	{
		throw std::logic_error("Not implemented");
	}

	Boss03::Boss03(Boss03PrototypeInfo const& prototype) : AnimatedComplexPhysicObj(prototype)
	{
		throw std::logic_error("Not implemented");
	}

	void Boss03::InflictDamage(DamageInfo const&)
	{
		throw std::logic_error("Not implemented");
	}

	void Boss03::GetPropertiesIDs(std::set<int, std::less<int>, std::allocator<int>>&) const
	{
		throw std::logic_error("Not implemented");
	}

	bool Boss03::StartPlacingDrone()
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Class* Boss03::GetClass() const
	{
		throw std::logic_error("Not implemented");
	}

	CStr Boss03::GetPropertyName(int) const
	{
		throw std::logic_error("Not implemented");
	}

	bool Boss03::CanChildBeAdded(m3d::Class*) const
	{
		throw std::logic_error("Not implemented");
	}

	void Boss03::StartMoving(CVector const&, CVector const&)
	{
		throw std::logic_error("Not implemented");
	}

	eGObjPropertySaveStatus Boss03::GetPropertySaveStatus(int) const
	{
		throw std::logic_error("Not implemented");
	}

	void Boss03::GetPropertiesNames(std::set<CStr, std::less<CStr>, std::allocator<CStr>>&) const
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Class* Boss03::GetBaseClass()
	{
		return RT_CLASS_LOCAL(AnimatedComplexPhysicObj);
	}

	void Boss03::Update(float, unsigned)
	{
		throw std::logic_error("Not implemented");
	}

	bool Boss03::ApplyModifier(Modifier const&)
	{
		throw std::logic_error("Not implemented");
	}

	void Boss03::NoticePlayer()
	{
		throw std::logic_error("Not implemented");
	}

	bool Boss03::SetPropertyById(int, m3d::AIParam const&)
	{
		throw std::logic_error("Not implemented");
	}

	void Boss03::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	float Boss03::GetHealth() const
	{
		throw std::logic_error("Not implemented");
	}

	void Boss03::AddChild(Obj*)
	{
		throw std::logic_error("Not implemented");
	}

	Boss03PrototypeInfo const* Boss03::GetPrototypeInfo() const
	{
		throw std::logic_error("Not implemented");
	}

	bool Boss03::RemoveChild(Obj*)
	{
		throw std::logic_error("Not implemented");
	}

	int Boss03::GetPropertyId(char const*) const
	{
		throw std::logic_error("Not implemented");
	}

	void Boss03::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	float Boss03::GetMaxHealth() const
	{
		throw std::logic_error("Not implemented");
	}

	void Boss03::_InternalPostLoad()
	{
		throw std::logic_error("Not implemented");
	}

	bool Boss03::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
	{
		throw std::logic_error("Not implemented");
	}

	void Boss03::_Construct(bool)
	{
		throw std::logic_error("Not implemented");
	}

	bool Boss03::_GetPropertyInternal(int, m3d::AIParam&) const
	{
		throw std::logic_error("Not implemented");
	}

	void Boss03::_InternalCreateVisualPart()
	{
		throw std::logic_error("Not implemented");
	}

	Boss03::~Boss03()
	{
		throw std::logic_error("Not implemented");
	}

	void Boss03::_SetAllPartsNotDamageable()
	{
		throw std::logic_error("Not implemented");
	}

	void Boss03::_UpdateLiveFightingWithWings(float)
	{
		throw std::logic_error("Not implemented");
	}

	void Boss03::_SetDroneSpawningStatus(DroneSpawningStatus)
	{
		throw std::logic_error("Not implemented");
	}

	Boss03Part* Boss03::_GetBossPartByName(CStr const&)
	{
		throw std::logic_error("Not implemented");
	}

	bool Boss03::_OnHealthValueBeforeApplyModifier(Modifier const&, float&)
	{
		throw std::logic_error("Not implemented");
	}

	bool Boss03::_ProceedSettingVerticalAngle(float, float)
	{
		throw std::logic_error("Not implemented");
	}

	void Boss03::_SpawnDroneNow()
	{
		throw std::logic_error("Not implemented");
	}

	void Boss03::_UpdatePathTracking(float)
	{
		throw std::logic_error("Not implemented");
	}

	Boss03Part* Boss03::_GetMainPart()
	{
		throw std::logic_error("Not implemented");
	}

	Boss03Part* Boss03::_GetDroneSpawner()
	{
		throw std::logic_error("Not implemented");
	}

	void Boss03::_SetLiveStatus(LiveStatus)
	{
		throw std::logic_error("Not implemented");
	}

	void Boss03::_RecalcFlyPath()
	{
		throw std::logic_error("Not implemented");
	}

	void Boss03::_UpdateLookAtPlayer(float)
	{
		throw std::logic_error("Not implemented");
	}

	void Boss03::_UpdateLivePlacingDrones(float)
	{
		throw std::logic_error("Not implemented");
	}

	void Boss03::_UpdateSwinging(float)
	{
		throw std::logic_error("Not implemented");
	}

	bool Boss03::_ProceedSettingHorizontalDirection(float, CVector const&)
	{
		throw std::logic_error("Not implemented");
	}

	CStr const& Boss03::_DroneSpawningStatusToStr(DroneSpawningStatus)
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* Boss03::CreateObject()
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* Boss03::Clone()
	{
		throw std::logic_error("Not implemented");
	}

	CStr const& Boss03::_LiveStatusToStr(LiveStatus)
	{
		throw std::logic_error("Not implemented");
	}

	void Boss03::_SetPathTrackingStatus(PathTrackingStatus)
	{
		throw std::logic_error("Not implemented");
	}

	float Boss03::CalcKeyPartsMaxDurability() const
	{
		throw std::logic_error("Not implemented");
	}
}
