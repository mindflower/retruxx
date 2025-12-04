#include "affix.h"

#include <stdexcept>

#include "retruxx/common.h"

namespace ai
{
	bool Affix::ModificationInfo::ApplyToObj(Affix const*, Obj*) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	Affix::ModificationInfo::ModificationInfo(CStr const&)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	bool Affix::ApplyToObj(Obj*) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Affix::PostLoad()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	Affix::Affix(AffixGroup*)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	bool Affix::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	AffixGroup const* Affix::GetAffixGroup() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	int Affix::GetId() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	CStr const& Affix::GetName() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	AffixType Affix::GetAffixType() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	CStr Affix::GetLocalizedName(unsigned) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	int AffixManager::GetAffixIdByNameAndResource(CStr const&, int) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	bool AffixManager::LoadFromXmlFile(char const*)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	AffixManager::~AffixManager()
	{
		for (auto& group : m_affixGroups)
		{
			delete group;
		}

		for (auto& affix : m_affixes)
		{
			delete affix;
		}
	}

	AffixManager::AffixManager()
	{
	}

	AffixGroup* AffixManager::GetAffixGroupById(int) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void AffixManager::GetAffixGroupsByResourceId(int, std::vector<int>&) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	int AffixManager::GetNumAffixes() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void AffixManager::AddAffix(Affix*)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	int AffixManager::GetAffixGroupIdByName(CStr const&) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	Affix* AffixManager::GetAffixById(int) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	int AffixGroup::GetId() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	AffixGroup::AffixGroup(AffixManager*)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	AffixType AffixGroup::GetAffixType() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	int AffixGroup::GetRandomAffixId() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	std::vector<int> const& AffixGroup::GetAffixIds() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	CStr const& AffixGroup::GetName() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	AffixGroup::~AffixGroup()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	bool AffixGroup::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	int AffixGroup::GetTargetResourceId() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}
}
