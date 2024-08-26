#include "bar.h"

#include <stdexcept>

namespace ai
{
	RT_CLASS_EXPORTS_BEGIN(Bar)
	RT_CLASS_EXPORTS_END;
	RT_CLASS_DEFINE(Bar);

	Obj* BarPrototypeInfo::CreateTargetObject() const
	{
		throw std::logic_error("Not implemented");
	}

	BarPrototypeInfo::BarPrototypeInfo()
	{
		throw std::logic_error("Not implemented");
	}

	bool BarPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	void BarPrototypeInfo::_InternalCopyFrom(PrototypeInfo const&)
	{
		throw std::logic_error("Not implemented");
	}

	bool Bar::bWithBarman() const
	{
		throw std::logic_error("Not implemented");
	}

	bool Bar::CanChildBeAdded(m3d::Class*) const
	{
		throw std::logic_error("Not implemented");
	}

	Bar::Bar(BarPrototypeInfo const& prototypeInfo) : Building(prototypeInfo)
	{
		throw std::logic_error("Not implemented");
	}

	bool Bar::RemoveChild(Obj*)
	{
		throw std::logic_error("Not implemented");
	}

	void Bar::Update(float, unsigned)
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Class* Bar::GetClass() const
	{
		throw std::logic_error("Not implemented");
	}

	BarPrototypeInfo const* Bar::GetPrototypeInfo() const
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Class* Bar::GetBaseClass()
	{
		return RT_CLASS_LOCAL(Building);
	}

	void Bar::CreateChildren()
	{
		throw std::logic_error("Not implemented");
	}

	void Bar::AddChild(Obj*)
	{
		throw std::logic_error("Not implemented");
	}

	Bar::~Bar()
	{
		throw std::logic_error("Not implemented");
	}

	void Bar::CreateBarman()
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* Bar::CreateObject()
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* Bar::Clone()
	{
		throw std::logic_error("Not implemented");
	}
}
