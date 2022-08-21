#include "dummyobject.h"

#include <stdexcept>

RT_CLASS_EXPORT_METHOD_DEFINE(DummyObject, SetModelName)
{
	throw std::logic_error("Not implemented");
}

namespace ai
{
	RT_CLASS_EXPORTS_BEGIN(DummyObject)
		RT_CLASS_EXPORT(DummyObject, m3d::METHOD, SetModelName, "", "", "")
	RT_CLASS_EXPORTS_END;
	RT_CLASS_DEFINE(DummyObject);

	DummyObjectPrototypeInfo::DummyObjectPrototypeInfo()
	{
		throw std::logic_error("Not implemented");
	}

	bool DummyObjectPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	Obj* DummyObjectPrototypeInfo::CreateTargetObject() const
	{
		throw std::logic_error("Not implemented");
	}

	void DummyObject::Registration()
	{
		throw std::logic_error("Not implemented");
	}

	eGObjPropertySaveStatus DummyObject::GetPropertySaveStatus(int) const
	{
		throw std::logic_error("Not implemented");
	}

	int DummyObject::GetPropertyId(char const*) const
	{
		throw std::logic_error("Not implemented");
	}

	void DummyObject::SetModelName(char const*)
	{
		throw std::logic_error("Not implemented");
	}

	bool DummyObject::SetPropertyById(int, m3d::AIParam const&)
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Class* DummyObject::GetBaseClass()
	{
		return RT_CLASS_LOCAL(SimplePhysicObj);
	}

	m3d::Class* DummyObject::GetClass() const
	{
		throw std::logic_error("Not implemented");
	}

	CStr DummyObject::GetPropertyName(int) const
	{
		throw std::logic_error("Not implemented");
	}

	DummyObjectPrototypeInfo const* DummyObject::GetPrototypeInfo() const
	{
		throw std::logic_error("Not implemented");
	}

	void DummyObject::SetModelNameUnsafe(CStr const&)
	{
		throw std::logic_error("Not implemented");
	}

	void DummyObject::SetSgNodeAndCollision(m3d::SgNode*, CollisionInfo const*)
	{
		throw std::logic_error("Not implemented");
	}

	void DummyObject::GetPropertiesIDs(std::set<int, std::less<int>, std::allocator<int>>&) const
	{
		throw std::logic_error("Not implemented");
	}

	void DummyObject::GetPropertiesNames(std::set<CStr, std::less<CStr>, std::allocator<CStr>>&) const
	{
		throw std::logic_error("Not implemented");
	}

	DummyObject::DummyObject(DummyObjectPrototypeInfo const& prototypeInfo) : SimplePhysicObj(prototypeInfo)
	{
		throw std::logic_error("Not implemented");
	}

	bool DummyObject::CanChildBeAdded(m3d::Class*) const
	{
		throw std::logic_error("Not implemented");
	}

	bool DummyObject::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
	{
		throw std::logic_error("Not implemented");
	}

	bool DummyObject::_GetPropertyInternal(int, m3d::AIParam&) const
	{
		throw std::logic_error("Not implemented");
	}

	void DummyObject::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
	{
		throw std::logic_error("Not implemented");
	}

	DummyObject::~DummyObject()
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* DummyObject::Clone()
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* DummyObject::CreateObject()
	{
		throw std::logic_error("Not implemented");
	}
}
