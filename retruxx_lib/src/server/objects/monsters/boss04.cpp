#include "boss04.h"

#include <stdexcept>

RT_CLASS_EXPORT_METHOD_DEFINE(Boss04, NextState)
{
	throw retruxx::logic_error("Not implemented");
}

namespace ai
{
	RT_CLASS_EXPORTS_BEGIN(Boss04)
		RT_CLASS_EXPORT(Boss04, m3d::METHOD, NextState, "", "", "")
	RT_CLASS_EXPORTS_END;
	RT_CLASS_DEFINE(Boss04);

	void Boss04PrototypeInfo::PostLoad()
	{
		throw retruxx::logic_error("Not implemented");
	}

	Boss04PrototypeInfo::Boss04PrototypeInfo()
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool Boss04PrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw retruxx::logic_error("Not implemented");
	}

	Obj* Boss04PrototypeInfo::CreateTargetObject() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss04PrototypeInfo::RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw retruxx::logic_error("Not implemented");
	}

	eGObjPropertySaveStatus Boss04::GetPropertySaveStatus(int) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss04::NextState()
	{
		throw retruxx::logic_error("Not implemented");
	}

	Boss04::Boss04(Boss04PrototypeInfo const& prototype) : ComplexPhysicObj(prototype)
	{
		throw retruxx::logic_error("Not implemented");
	}

	CStr Boss04::GetPropertyName(int) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool Boss04::CanChildBeAdded(m3d::Class*) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	m3d::Class* Boss04::GetClass() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss04::Registration()
	{
	}

	void Boss04::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss04::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss04::CreateChildren()
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss04::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss04::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw retruxx::logic_error("Not implemented");
	}

	float Boss04::GetMaxHealth() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	int Boss04::GetPropertyId(char const*) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	Boss04PrototypeInfo const* Boss04::GetPrototypeInfo() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool Boss04::RemoveChild(Obj*)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss04::SetRotation(Quaternion const&)
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool Boss04::SetPropertyById(int, m3d::AIParam const&)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss04::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw retruxx::logic_error("Not implemented");
	}

	float Boss04::GetHealth() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss04::AddChild(Obj*)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss04::Update(float, unsigned)
	{
		throw retruxx::logic_error("Not implemented");
	}

	m3d::Class* Boss04::GetBaseClass()
	{
		return RT_CLASS_LOCAL(ComplexPhysicObj);
	}

	int Boss04::OnEvent(Event const&)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss04::SetPosition(CVector const&)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss04::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss04::_InternalPostLoad()
	{
		throw retruxx::logic_error("Not implemented");
	}

	Boss04::~Boss04()
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool Boss04::_GetPropertyInternal(int, m3d::AIParam&) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss04::_InternalCreateVisualPart()
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool Boss04::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss04::_OnBoss04StationDestroyed(Event const&)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss04::_SpawnDrone()
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss04::_SetState(Boss04State)
	{
		throw retruxx::logic_error("Not implemented");
	}

	m3d::Object* Boss04::Clone()
	{
		throw retruxx::logic_error("Not implemented");
	}

	m3d::Object* Boss04::CreateObject()
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss04::_CreateStations(bool)
	{
		throw retruxx::logic_error("Not implemented");
	}
}
