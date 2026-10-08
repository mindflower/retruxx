#include "affix.h"

#include <stdexcept>

#include "retruxx/common.h"

#include "resourcemanager.h"
#include "objects/base/globalproperties.h"
#include "objects/base/obj.h"
#include "objects/guns/gun.h"

#include <config.h>
#include <core/aiparam.h>
#include <core/ini.h>
#include <core/kernel.h>
#include <core/log.h>
#include <core/ref_ptr.h>
#include <m3dapp.h>
#include <math/vector2.h>

#include <math.h>

namespace ai
{
	bool Affix::ModificationInfo::ApplyToObj(Affix const* affix, Obj* pObj) const
	{
		// RVA 0x727E80 - rolls this one modification onto the object and charges for it.
		// NOTE: a gun that fires one shell at a time is exempt from rate-of-fire affixes.
		if (!pObj ||
			(IS_KIND_OF(pObj, Gun) && static_cast<Gun const*>(pObj)->GetChargeSize() == 1 &&
				(m_propertyName == "FiringRate" || m_propertyName == "ReChargingTime")))
		{
			return false;
		}

		int const propertyId = pObj->GetPropertyId(m_propertyName.c_str());
		if (propertyId == -1)
		{
			M3D_LOG_ERR(CStr("Error: attempt to apply affix '") + affix->m_name + CStr("' for property '") +
				m_propertyName + CStr("' of ") + pObj->GetDebugDescription());
			return false;
		}

		m3d::AIParam const propDefault = pObj->GetPropertyDefaultById(propertyId);
		if (propDefault.GetType() != m3d::AIPARAM_FLOAT)
		{
			M3D_LOG_ERR(CStr("Error: attempt to apply affix '") + affix->m_name +
				CStr("' to non-float property '") + m_propertyName + CStr("' of ") +
				pObj->GetDebugDescription());
			return false;
		}

		// Raising a maximum drags the matching current value up with it, keeping the ratio.
		int pairedPropertyId = -1;
		if (propertyId == PROPERTY_MAX_DURABILITY)
		{
			pairedPropertyId = PROPERTY_DURABILITY;
		}
		else if (propertyId == PROPERTY_MAX_HEALTH)
		{
			pairedPropertyId = PROPERTY_HEALTH;
		}

		float const oldValue = pObj->GetPropertyById(propertyId).GetAsFloat();
		float const coeff = CVector2(m_lowCoeff, m_highCoeff).randomValue();
		m3d::AIParam const newValue(
			pObj->GetPropertyById(propertyId).GetAsFloat() + propDefault.GetAsFloat() * coeff);

		if (pairedPropertyId != -1 && fabs(oldValue) > 0.0000099999997f)
		{
			float const pairedValue = pObj->GetPropertyById(pairedPropertyId).GetAsFloat();
			m3d::AIParam const pairedNewValue(newValue.GetAsFloat() * (pairedValue / oldValue));
			pObj->SetPropertyById(pairedPropertyId, pairedNewValue);
		}
		pObj->SetPropertyById(propertyId, newValue);

		// A better item is worth more - unless price is the very thing the affix changed.
		if (propertyId != PROPERTY_PRICE && !pObj->GetPropertyName(PROPERTY_PRICE).empty())
		{
			float const priceCoeff = theGlobProp.m_property2PriceCoeff * coeff;
			m3d::AIParam const priceDefault = pObj->GetPropertyDefaultById(PROPERTY_PRICE);
			float const currentPrice = pObj->GetPropertyById(PROPERTY_PRICE).GetAsFloat();
			m3d::AIParam const priceNewValue(priceDefault.GetAsFloat() * priceCoeff + currentPrice);
			pObj->SetPropertyById(PROPERTY_PRICE, priceNewValue);
		}
		return true;
	}

	Affix::ModificationInfo::ModificationInfo(CStr const& str)
	{
		// RVA 0x727C60 - a modification reads as "Property(low, high)", the two numbers being the
		// percentage range of the property's default value that the affix may add.
		retruxx::vector<CStr> tokens;
		m3d::Tokenize(str, tokens, "(), ;\t");

		m_propertyName = tokens[0];

		m_lowCoeff = 0.0f;
		if (!tokens[1].empty())
		{
			sscanf(tokens[1].c_str(), "%f", &m_lowCoeff);
		}
		m_highCoeff = 0.0f;
		if (!tokens[2].empty())
		{
			sscanf(tokens[2].c_str(), "%f", &m_highCoeff);
		}

		if (m_lowCoeff > m_highCoeff)
		{
			std::swap(m_lowCoeff, m_highCoeff);
		}
		m_lowCoeff = m_lowCoeff * 0.0099999998f;
		m_highCoeff = m_highCoeff * 0.0099999998f;
	}

	bool Affix::ApplyToObj(Obj* pObj) const
	{
		// RVA 0x728BF0 - NOTE: the first modification that fails abandons the rest, leaving the
		// object with a partially applied affix.
		for (auto const& modification : m_modifications)
		{
			if (!modification.ApplyToObj(this, pObj))
			{
				return false;
			}
		}
		return true;
	}

	void Affix::PostLoad()
	{
		// RVA 0x725900 - nothing to resolve; an affix is complete as soon as it is read.
	}

	Affix::Affix(AffixGroup* pAffixGroup)
	{
		// RVA 0x7285D0 - the id is handed out by AffixManager::AddAffix.
		m_pAffixGroup = pAffixGroup;
		m_affixId = -1;
	}

	bool Affix::LoadFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
	{
		// RVA 0x728600
		m3d::SafeStrAttrib(m_name, xmlNode, "Name");

		// Prefixes are declined, so every localized form the language needs must be present;
		// suffixes only ever have the one.
		int const formsQuantity = m_pAffixGroup->m_affixType == AFFIXTYPE_PREFIX
			? M3D_ENGINE_CFG.m_loc_formsQuantity.GetI()
			: 1;
		for (int form = 0; form < formsQuantity; ++form)
		{
			CStr locName;
			if (!M3D_APP->GetStringByStringId(
					locName, m_name + CStr("_localizedform_") + CStr(form)))
			{
				M3D_LOG_ERR(CStr("Error: missing localized form ") + CStr(form) + CStr(" for affix ") +
					m_name);
			}
		}

		CStr strModifications;
		m3d::SafeStrAttrib(strModifications, xmlNode, "modifications");

		retruxx::vector<CStr> tokens;
		m3d::Tokenize(strModifications, tokens, ";");
		for (auto const& token : tokens)
		{
			m_modifications.push_back(ModificationInfo(token));
		}
		return true;
	}

	AffixGroup const* Affix::GetAffixGroup() const
	{
		// RVA 0x6891C0
		return m_pAffixGroup;
	}

	int Affix::GetId() const
	{
		// RVA 0x6891B0
		return m_affixId;
	}

	CStr const& Affix::GetName() const
	{
		// RVA 0x4D4400
		return m_name;
	}

	AffixType Affix::GetAffixType() const
	{
		// RVA 0x725910 - an affix is a prefix or a suffix because its group is.
		return m_pAffixGroup->m_affixType;
	}

	CStr Affix::GetLocalizedName(unsigned localizationIndex) const
	{
		// RVA 0x4D4410
		return M3D_APP->GetStringByStringId0(
			m_name + CStr("_localizedform_") + CStr(localizationIndex));
	}

	int AffixManager::GetAffixIdByNameAndResource(CStr const& affixName, int resourceId) const
	{
		// RVA 0x7264A0 - the same affix name exists once per resource kind, so the resource is
		// part of the key.
		if (resourceId == -1)
		{
			return -1;
		}
		for (auto const* affix : m_affixes)
		{
			if (affix->m_name == affixName &&
				theResourceManager->bResourceIsKindOf(resourceId, affix->m_pAffixGroup->m_targetResourceId))
			{
				return affix->m_affixId;
			}
		}
		return -1;
	}

	bool AffixManager::LoadFromXmlFile(char const* fileName)
	{
		// RVA 0x728E00 - the file is grouped by resource kind, then split into prefix and suffix
		// groups, each of which owns its affixes.
		CStr err;
		ref_ptr xmlFile = m3d::ReadXmlFile(fileName, &err);
		if (!xmlFile)
		{
			M3D_LOG_INFO(CStr("No file: ") + CStr(fileName) + err);
			return false;
		}

		ref_ptr rootNode = xmlFile->CreateNode();
		ref_ptr resourceNode = xmlFile->CreateNode();
		ref_ptr affixesNode = xmlFile->CreateNode();
		ref_ptr groupNode = xmlFile->CreateNode();
		ref_ptr affixNode = xmlFile->CreateNode();

		xmlFile->GetFirstChild(rootNode, "Affixes");
		for (rootNode->GetFirstChild(resourceNode, "ForResource"); !resourceNode->IsEmpty();
			 resourceNode->GetNextSibling(resourceNode, "ForResource"))
		{
			CStr const resourceName(resourceNode->GetAttribute("Name"));
			int const resourceId = theResourceManager->GetResourceId(resourceName);
			if (resourceId == -1)
			{
				M3D_LOG_ERR(CStr("Error loading affixes: invalid resource name: '") + resourceName +
					CStr("'"));
				return false;
			}

			struct
			{
				char const* m_sectionName;
				AffixType m_affixType;
			} const sections[] = {{"Prefixes", AFFIXTYPE_PREFIX}, {"Suffixes", AFFIXTYPE_SUFFIX}};

			for (auto const& section : sections)
			{
				resourceNode->GetFirstChild(affixesNode, section.m_sectionName);
				if (affixesNode->IsEmpty())
				{
					continue;
				}
				for (affixesNode->GetFirstChild(groupNode, "AffixGroup"); !groupNode->IsEmpty();
					 groupNode->GetNextSibling(groupNode, "AffixGroup"))
				{
					AffixGroup* const affixGroup = new AffixGroup(this);
					affixGroup->m_affixType = section.m_affixType;
					if (affixGroup->LoadFromXml(xmlFile, groupNode))
					{
						affixGroup->m_affixGroupId = static_cast<int>(m_affixGroups.size());
						affixGroup->m_targetResourceId = resourceId;
						m_affixGroups.push_back(affixGroup);
					}
					// NOTE: a group that fails to load is neither stored nor deleted - it leaks.
				}
			}
		}
		return true;
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

	AffixGroup* AffixManager::GetAffixGroupById(int affixGroupId) const
	{
		// RVA 0x7269B0 - the id is the index.
		if (affixGroupId < 0 || affixGroupId >= static_cast<int>(m_affixGroups.size()))
		{
			return nullptr;
		}
		return m_affixGroups[affixGroupId];
	}

	void AffixManager::GetAffixGroupsByResourceId(int resourceId, std::vector<int>& affixGroupIds) const
	{
		// RVA 0x727310 - NOTE: the list handed in is cleared first, so anything already in it is
		// discarded rather than appended to.
		affixGroupIds.clear();
		if (resourceId == -1)
		{
			return;
		}
		for (auto const* affixGroup : m_affixGroups)
		{
			if (theResourceManager->bResourceIsKindOf(resourceId, affixGroup->m_targetResourceId))
			{
				affixGroupIds.push_back(affixGroup->m_affixGroupId);
			}
		}
	}

	int AffixManager::GetNumAffixes() const
	{
		// RVA 0x725FE0
		return static_cast<int>(m_affixes.size());
	}

	void AffixManager::AddAffix(Affix* affix)
	{
		// RVA 0x727490 - the affix's id is its index in the manager's flat list.
		if (GetAffixIdByNameAndResource(affix->m_name, affix->m_pAffixGroup->m_targetResourceId) != -1)
		{
			M3D_LOG_ERR(CStr("Error: affix '") + affix->m_name + CStr("' already exists for resource ") +
				theResourceManager->GetResourceName(affix->m_pAffixGroup->m_targetResourceId));
			SYS_ERROR("0");
		}
		affix->m_affixId = static_cast<int>(m_affixes.size());
		m_affixes.push_back(affix);
	}

	int AffixManager::GetAffixGroupIdByName(CStr const& affixGroupName) const
	{
		// RVA 0x726510 - NOTE: unlike affixes, group names are not qualified by resource, so the
		// first group of that name anywhere in the file wins.
		for (auto const* affixGroup : m_affixGroups)
		{
			if (affixGroup->m_name == affixGroupName)
			{
				return affixGroup->m_affixGroupId;
			}
		}
		return -1;
	}

	Affix* AffixManager::GetAffixById(int affixId) const
	{
		// RVA 0x726970 - the id is the index.
		if (affixId < 0 || affixId >= static_cast<int>(m_affixes.size()))
		{
			return nullptr;
		}
		return m_affixes[affixId];
	}

	int AffixGroup::GetId() const
	{
		// RVA 0x7258D0
		return m_affixGroupId;
	}

	AffixGroup::AffixGroup(AffixManager* pAffixManager)
	{
		// RVA 0x727050 - the id, resource and type are filled in by AffixManager::LoadFromXmlFile.
		m_pAffixManager = pAffixManager;
		m_affixGroupId = -1;
		m_order = -1;
		m_targetResourceId = -1;
	}

	AffixType AffixGroup::GetAffixType() const
	{
		// RVA 0x7258F0
		return m_affixType;
	}

	int AffixGroup::GetRandomAffixId() const
	{
		// RVA 0x7268C0
		if (m_affixIds.empty())
		{
			return -1;
		}
		return m_affixIds[m_affixIds.size() * rand() / 0x8000];
	}

	std::vector<int> const& AffixGroup::GetAffixIds() const
	{
		// RVA 0x873620
		return m_affixIds;
	}

	CStr const& AffixGroup::GetName() const
	{
		// RVA 0x7258E0
		return m_name;
	}

	AffixGroup::~AffixGroup()
	{
		// RVA 0x726870 - NOTE: a group does not own its affixes; the manager deletes those.
	}

	bool AffixGroup::LoadFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
	{
		// RVA 0x728C30 - each affix is registered with the manager, which is what gives it its id;
		// the group keeps only the ids.
		m3d::SafeStrAttrib(m_name, xmlNode, "Name");
		m3d::SafeIntAttrib(m_order, xmlNode, "order");

		ref_ptr affixNode = xmlFile->CreateNode();
		for (xmlNode->GetFirstChild(affixNode, "Affix"); !affixNode->IsEmpty();
			 affixNode->GetNextSibling(affixNode, "Affix"))
		{
			Affix* const affix = new Affix(this);
			if (affix->LoadFromXml(xmlFile, affixNode))
			{
				m_pAffixManager->AddAffix(affix);
				m_affixIds.push_back(affix->m_affixId);
			}
			// NOTE: an affix that fails to load is neither registered nor deleted - it leaks.
		}
		return true;
	}

	int AffixGroup::GetTargetResourceId() const
	{
		// RVA 0x6891D0
		return m_targetResourceId;
	}
}
