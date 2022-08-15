#include "affix.h"

#include <stdexcept>

namespace ai
{
	bool Affix::ModificationInfo::ApplyToObj(Affix const*, Obj*) const
	{
		throw std::logic_error("Not implemented");
	}

	Affix::ModificationInfo::ModificationInfo(CStr const&)
	{
		throw std::logic_error("Not implemented");
	}

	bool Affix::ApplyToObj(Obj*) const
	{
		throw std::logic_error("Not implemented");
	}

	void Affix::PostLoad()
	{
		throw std::logic_error("Not implemented");
	}

	Affix::Affix(AffixGroup*)
	{
		throw std::logic_error("Not implemented");
	}

	bool Affix::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	AffixGroup const* Affix::GetAffixGroup() const
	{
		throw std::logic_error("Not implemented");
	}

	int Affix::GetId() const
	{
		throw std::logic_error("Not implemented");
	}

	CStr const& Affix::GetName() const
	{
		throw std::logic_error("Not implemented");
	}

	AffixType Affix::GetAffixType() const
	{
		throw std::logic_error("Not implemented");
	}

	CStr Affix::GetLocalizedName(unsigned) const
	{
		throw std::logic_error("Not implemented");
	}

	int AffixManager::GetAffixIdByNameAndResource(CStr const&, int) const
	{
		throw std::logic_error("Not implemented");
	}

	bool AffixManager::LoadFromXmlFile(char const*)
	{
		throw std::logic_error("Not implemented");
	}

	AffixManager::~AffixManager()
	{
		throw std::logic_error("Not implemented");
	}

	AffixManager::AffixManager()
	{
	}

	AffixGroup* AffixManager::GetAffixGroupById(int) const
	{
		throw std::logic_error("Not implemented");
	}

	void AffixManager::GetAffixGroupsByResourceId(int, std::vector<int>&) const
	{
		throw std::logic_error("Not implemented");
	}

	int AffixManager::GetNumAffixes() const
	{
		throw std::logic_error("Not implemented");
	}

	void AffixManager::AddAffix(Affix*)
	{
		throw std::logic_error("Not implemented");
	}

	int AffixManager::GetAffixGroupIdByName(CStr const&) const
	{
		throw std::logic_error("Not implemented");
	}

	Affix* AffixManager::GetAffixById(int) const
	{
		throw std::logic_error("Not implemented");
	}

	int AffixGroup::GetId() const
	{
		throw std::logic_error("Not implemented");
	}

	AffixGroup::AffixGroup(AffixManager*)
	{
		throw std::logic_error("Not implemented");
	}

	AffixType AffixGroup::GetAffixType() const
	{
		throw std::logic_error("Not implemented");
	}

	int AffixGroup::GetRandomAffixId() const
	{
		throw std::logic_error("Not implemented");
	}

	std::vector<int> const& AffixGroup::GetAffixIds() const
	{
		throw std::logic_error("Not implemented");
	}

	CStr const& AffixGroup::GetName() const
	{
		throw std::logic_error("Not implemented");
	}

	AffixGroup::~AffixGroup()
	{
		throw std::logic_error("Not implemented");
	}

	bool AffixGroup::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	int AffixGroup::GetTargetResourceId() const
	{
		throw std::logic_error("Not implemented");
	}
}
