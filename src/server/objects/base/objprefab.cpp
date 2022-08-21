#include "objprefab.h"

#include <stdexcept>

namespace ai
{
	RT_CLASS_EXPORTS_BEGIN(ObjPrefab)
	RT_CLASS_EXPORTS_END;
	RT_CLASS_DEFINE(ObjPrefab);

	ObjPrefabPrototypeInfo::ObjInfo::ObjInfo()
	{
		throw std::logic_error("Not implemented");
	}

	void ObjPrefabPrototypeInfo::ObjInfo::PostLoad()
	{
		throw std::logic_error("Not implemented");
	}

	ObjPrefabPrototypeInfo::ObjPrefabPrototypeInfo()
	{
		throw std::logic_error("Not implemented");
	}

	void ObjPrefabPrototypeInfo::PostLoad()
	{
		throw std::logic_error("Not implemented");
	}

	ai::Obj* ObjPrefabPrototypeInfo::CreateTargetObject() const
	{
		throw std::logic_error("Not implemented");
	}

	bool ObjPrefabPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	ai::ObjPrefabPrototypeInfo const* ObjPrefab::GetPrototypeInfo() const
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Class* ObjPrefab::GetClass() const
	{
		throw std::logic_error("Not implemented");
	}

	bool ObjPrefab::RemoveChild(ai::Obj*)
	{
		throw std::logic_error("Not implemented");
	}

	void ObjPrefab::SetPosition(CVector const&)
	{
		throw std::logic_error("Not implemented");
	}

	ObjPrefab::ObjPrefab(ai::ObjPrefabPrototypeInfo const& prototype) : SimplePhysicObj(prototype)
	{
		throw std::logic_error("Not implemented");
	}

	void ObjPrefab::Remove()
	{
		throw std::logic_error("Not implemented");
	}

	void ObjPrefab::CreateChildren()
	{
		throw std::logic_error("Not implemented");
	}

	bool ObjPrefab::CanChildBeAdded(m3d::Class*) const
	{
		throw std::logic_error("Not implemented");
	}

	void ObjPrefab::SetPositionSelf(CVector const&)
	{
		throw std::logic_error("Not implemented");
	}

	void ObjPrefab::SetRotation(Quaternion const&)
	{
		throw std::logic_error("Not implemented");
	}

	void ObjPrefab::AddChild(ai::Obj*)
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Class* ObjPrefab::GetBaseClass()
	{
		return RT_CLASS_LOCAL(SimplePhysicObj);
	}

	void ObjPrefab::_InternalPostLoad()
	{
		throw std::logic_error("Not implemented");
	}

	ObjPrefab::~ObjPrefab()
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* ObjPrefab::CreateObject()
	{
		throw std::logic_error("Not implemented");
	}

	void ObjPrefab::AddTeam()
	{
		throw std::logic_error("Not implemented");
	}

	void ObjPrefab::AddVehicleChild(Vehicle*)
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* ObjPrefab::Clone()
	{
		throw std::logic_error("Not implemented");
	}
}
