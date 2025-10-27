#include "basket.h"

#include <stdexcept>

namespace ai
{
	RT_CLASS_EXPORTS_BEGIN(Basket)
	RT_CLASS_EXPORTS_END;
	RT_CLASS_DEFINE(Basket);

	bool BasketPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
	{
		auto result = ai::VehiclePartPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
		if (result)
		{
			ref_ptr repositoryDescriptionNode = xmlFile->CreateNode();
			xmlNode->GetFirstChild(repositoryDescriptionNode, "RepositoryDescription");

			if (!repositoryDescriptionNode->IsEmpty() && repositoryDescriptionNode->IsOfType(m3d::cmn::XML_NODE_ELEMENT))
			{
				ref_ptr slotNode = xmlFile->CreateNode();
				for (repositoryDescriptionNode->GetFirstChild(slotNode, "Slot"); !slotNode->IsEmpty(); slotNode->GetNextSibling(slotNode, "Slot"))
				{
					CStr slotName;
					m3d::SafeStrAttrib(slotName, slotNode, "Name");

                    CVector2 pos;
					m3d::SafeVector2Attrib(pos, slotNode, "Pos");
					PointBase<int> point(pos.x, pos.y);

					m_slots.emplace(std::move(slotName), std::move(point));
				}

                CVector2 size;
				m3d::SafeVector2Attrib(size, repositoryDescriptionNode, "RepositorySize");
				this->m_repositorySize.x = size.x;
				this->m_repositorySize.y = size.y;
			}
		}
		return result;
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
		this->m_repositorySize.x = 10;
		this->m_repositorySize.y = 10;
	}

	Obj* BasketPrototypeInfo::CreateTargetObject() const
	{
        return new Basket(*this);
	}

	void BasketPrototypeInfo::_InternalCopyFrom(PrototypeInfo const&)
	{
		throw retruxx::logic_error("Not implemented");
	}

	m3d::Class* Basket::GetClass() const
	{
		return RT_CLASS_LOCAL(Basket);
	}

	Basket::Basket(BasketPrototypeInfo const& prototype) : VehiclePart(prototype)
	{
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
