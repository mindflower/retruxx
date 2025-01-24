#include "basket.h"

#include <stdexcept>

namespace ai
{
	RT_CLASS_EXPORTS_BEGIN(Basket)
	RT_CLASS_EXPORTS_END;
	RT_CLASS_DEFINE(Basket);

	bool BasketPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw retruxx::logic_error("Not implemented");
	}

	PointBase<int> const& BasketPrototypeInfo::GetRepositorySize() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	retruxx::map<CStr, PointBase<int>, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, PointBase<int>>>> const&
	BasketPrototypeInfo::GetSlotPositions() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	int BasketPrototypeInfo::GetRepositoryCapacity() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	BoundsBase<int> BasketPrototypeInfo::GetSlotBounds(CStr const&, bool) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	BasketPrototypeInfo::BasketPrototypeInfo()
	{
		throw retruxx::logic_error("Not implemented");
	}

	Obj* BasketPrototypeInfo::CreateTargetObject() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void BasketPrototypeInfo::_InternalCopyFrom(PrototypeInfo const&)
	{
		throw retruxx::logic_error("Not implemented");
	}

	m3d::Class* Basket::GetClass() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	Basket::Basket(BasketPrototypeInfo const& prototype) : VehiclePart(prototype)
	{
		throw retruxx::logic_error("Not implemented");
	}

	m3d::Class* Basket::GetBaseClass()
	{
		return RT_CLASS_LOCAL(VehiclePart);
	}

	BasketPrototypeInfo const* Basket::GetPrototypeInfo() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	Basket::~Basket()
	{
		throw retruxx::logic_error("Not implemented");
	}

	m3d::Object* Basket::CreateObject()
	{
		throw retruxx::logic_error("Not implemented");
	}

	m3d::Object* Basket::Clone()
	{
		throw retruxx::logic_error("Not implemented");
	}
}
