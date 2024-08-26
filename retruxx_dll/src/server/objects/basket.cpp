#include "basket.h"

#include <stdexcept>

namespace ai
{
	RT_CLASS_EXPORTS_BEGIN(Basket)
	RT_CLASS_EXPORTS_END;
	RT_CLASS_DEFINE(Basket);

	bool BasketPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	PointBase<int> const& BasketPrototypeInfo::GetRepositorySize() const
	{
		throw std::logic_error("Not implemented");
	}

	std::map<CStr, PointBase<int>, std::less<CStr>, std::allocator<std::pair<CStr const, PointBase<int>>>> const&
	BasketPrototypeInfo::GetSlotPositions() const
	{
		throw std::logic_error("Not implemented");
	}

	int BasketPrototypeInfo::GetRepositoryCapacity() const
	{
		throw std::logic_error("Not implemented");
	}

	BoundsBase<int> BasketPrototypeInfo::GetSlotBounds(CStr const&, bool) const
	{
		throw std::logic_error("Not implemented");
	}

	BasketPrototypeInfo::BasketPrototypeInfo()
	{
		throw std::logic_error("Not implemented");
	}

	Obj* BasketPrototypeInfo::CreateTargetObject() const
	{
		throw std::logic_error("Not implemented");
	}

	void BasketPrototypeInfo::_InternalCopyFrom(PrototypeInfo const&)
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Class* Basket::GetClass() const
	{
		throw std::logic_error("Not implemented");
	}

	Basket::Basket(BasketPrototypeInfo const& prototype) : VehiclePart(prototype)
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Class* Basket::GetBaseClass()
	{
		return RT_CLASS_LOCAL(VehiclePart);
	}

	BasketPrototypeInfo const* Basket::GetPrototypeInfo() const
	{
		throw std::logic_error("Not implemented");
	}

	Basket::~Basket()
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* Basket::CreateObject()
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* Basket::Clone()
	{
		throw std::logic_error("Not implemented");
	}
}
