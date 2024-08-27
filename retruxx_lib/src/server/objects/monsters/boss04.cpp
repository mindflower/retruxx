#include "boss04.h"

#include <stdexcept>

RT_CLASS_EXPORT_METHOD_DEFINE(Boss04, NextState)
{
	throw std::logic_error("Not implemented");
}

namespace ai
{
	RT_CLASS_EXPORTS_BEGIN(Boss04)
		RT_CLASS_EXPORT(Boss04, m3d::METHOD, NextState, "", "", "")
	RT_CLASS_EXPORTS_END;
	RT_CLASS_DEFINE(Boss04);

	void Boss04PrototypeInfo::PostLoad()
	{
		throw std::logic_error("Not implemented");
	}

	Boss04PrototypeInfo::Boss04PrototypeInfo()
	{
		throw std::logic_error("Not implemented");
	}

	bool Boss04PrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	Obj* Boss04PrototypeInfo::CreateTargetObject() const
	{
		throw std::logic_error("Not implemented");
	}

	void Boss04PrototypeInfo::RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	eGObjPropertySaveStatus Boss04::GetPropertySaveStatus(int) const
	{
		throw std::logic_error("Not implemented");
	}

	void Boss04::NextState()
	{
		throw std::logic_error("Not implemented");
	}

	Boss04::Boss04(Boss04PrototypeInfo const& prototype) : ComplexPhysicObj(prototype)
	{
		throw std::logic_error("Not implemented");
	}

	CStr Boss04::GetPropertyName(int) const
	{
		throw std::logic_error("Not implemented");
	}

	bool Boss04::CanChildBeAdded(m3d::Class*) const
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Class* Boss04::GetClass() const
	{
		throw std::logic_error("Not implemented");
	}

	void Boss04::Registration()
	{
	}

	void Boss04::GetPropertiesIDs(std::set<int, std::less<int>, std::allocator<int>>&) const
	{
		throw std::logic_error("Not implemented");
	}

	void Boss04::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
	{
		throw std::logic_error("Not implemented");
	}

	void Boss04::CreateChildren()
	{
		throw std::logic_error("Not implemented");
	}

	void Boss04::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
	{
		throw std::logic_error("Not implemented");
	}

	void Boss04::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	float Boss04::GetMaxHealth() const
	{
		throw std::logic_error("Not implemented");
	}

	int Boss04::GetPropertyId(char const*) const
	{
		throw std::logic_error("Not implemented");
	}

	Boss04PrototypeInfo const* Boss04::GetPrototypeInfo() const
	{
		throw std::logic_error("Not implemented");
	}

	bool Boss04::RemoveChild(Obj*)
	{
		throw std::logic_error("Not implemented");
	}

	void Boss04::SetRotation(Quaternion const&)
	{
		throw std::logic_error("Not implemented");
	}

	bool Boss04::SetPropertyById(int, m3d::AIParam const&)
	{
		throw std::logic_error("Not implemented");
	}

	void Boss04::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	float Boss04::GetHealth() const
	{
		throw std::logic_error("Not implemented");
	}

	void Boss04::AddChild(Obj*)
	{
		throw std::logic_error("Not implemented");
	}

	void Boss04::Update(float, unsigned)
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Class* Boss04::GetBaseClass()
	{
		return RT_CLASS_LOCAL(ComplexPhysicObj);
	}

	int Boss04::OnEvent(Event const&)
	{
		throw std::logic_error("Not implemented");
	}

	void Boss04::SetPosition(CVector const&)
	{
		throw std::logic_error("Not implemented");
	}

	void Boss04::GetPropertiesNames(std::set<CStr, std::less<CStr>, std::allocator<CStr>>&) const
	{
		throw std::logic_error("Not implemented");
	}

	void Boss04::_InternalPostLoad()
	{
		throw std::logic_error("Not implemented");
	}

	Boss04::~Boss04()
	{
		throw std::logic_error("Not implemented");
	}

	bool Boss04::_GetPropertyInternal(int, m3d::AIParam&) const
	{
		throw std::logic_error("Not implemented");
	}

	void Boss04::_InternalCreateVisualPart()
	{
		throw std::logic_error("Not implemented");
	}

	bool Boss04::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
	{
		throw std::logic_error("Not implemented");
	}

	void Boss04::_OnBoss04StationDestroyed(Event const&)
	{
		throw std::logic_error("Not implemented");
	}

	void Boss04::_SpawnDrone()
	{
		throw std::logic_error("Not implemented");
	}

	void Boss04::_SetState(Boss04State)
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* Boss04::Clone()
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* Boss04::CreateObject()
	{
		throw std::logic_error("Not implemented");
	}

	void Boss04::_CreateStations(bool)
	{
		throw std::logic_error("Not implemented");
	}
}
