#include "affixgenerator.h"
#include "core/ini.h"
#include "core/ref_ptr.h"

#include "server/affix.h"
#include "server/objects/base/obj.h"
#include "server/server.h"

#include <iterator>
#include <stdlib.h>

namespace ai
{
    void AffixGeneratorPrototypeInfo::_InternalCopyFrom(PrototypeInfo const& rhs)
    {
        // RVA 0x8748F0 - a plain assignment from another prototype of the same class.
        *this = static_cast<AffixGeneratorPrototypeInfo const&>(rhs);
    }

    AffixGeneratorPrototypeInfo::AffixGeneratorPrototypeInfo() = default;

    bool AffixGeneratorPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = PrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            ref_ptr affixNode = xmlFile->CreateNode();
            for (xmlNode->GetFirstChild(affixNode, "Affix"); !affixNode->IsEmpty(); affixNode->GetNextSibling(affixNode, "Affix"))
            {
                AffixDescription newDescription;
                m3d::SafeStrAttrib(newDescription.m_affixName, affixNode, "AffixName");
                m_affixDescriptions.push_back(std::move(newDescription));
            }
        }
        return result;
    }

    void AffixGeneratorPrototypeInfo::PostLoad()
    {
        // RVA 0x873860 - the affix names are looked up lazily in GenerateAffixesForObj, so
        // there is nothing to resolve here. The override exists only to stop the base class
        // from running.
    }

    Obj* AffixGeneratorPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x873630 - an affix generator is a prototype only; it never becomes an object.
        return nullptr;
    }

    void AffixGeneratorPrototypeInfo::GenerateAffixesForObj(Obj* obj, unsigned int desiredNumAffixes) const
    {
        // RVA 0x874170 - rolls affixes onto a freshly created object: at most one from each
        // affix group, and never more than once in the object's life.
        PrototypeInfo const* const objPrototypeInfo = obj->GetPrototypeInfo();
        if (!objPrototypeInfo->m_bApplyAffixes || obj->m_bAffixesWasApplied)
        {
            return;
        }

        int const resourceId = objPrototypeInfo->m_resourceId;
        if (desiredNumAffixes)
        {
            // The count asked for is only an upper bound - a roll of zero leaves the object plain.
            desiredNumAffixes = (desiredNumAffixes + 1) * rand() / 0x8000;
        }

        // Every name this generator knows that resolves against the object's resource.
        AffixManager* const affixManager = pServer->GetAffixManager();
        retruxx::set<int> affixIds;
        for (auto const& description : m_affixDescriptions)
        {
            int const affixId =
                affixManager->GetAffixIdByNameAndResource(description.m_affixName, resourceId);
            if (affixId != -1)
            {
                affixIds.insert(affixId);
            }
        }

        if (desiredNumAffixes)
        {
            unsigned int numApplied = 0;
            while (!affixIds.empty())
            {
                auto candidate = affixIds.begin();
                std::advance(candidate, affixIds.size() * rand() / 0x8000);
                int const affixId = *candidate;

                Affix* const affix = pServer->GetAffixManager()->GetAffixById(affixId);
                if (obj->ApplyAffix(affix))
                {
                    // One affix per group, so the whole group drops out of the running.
                    for (int groupAffixId : affix->m_pAffixGroup->m_affixIds)
                    {
                        affixIds.erase(groupAffixId);
                    }
                    ++numApplied;
                }
                else
                {
                    affixIds.erase(affixId);
                }

                if (numApplied >= desiredNumAffixes)
                {
                    break;
                }
            }
        }

        obj->m_bAffixesWasApplied = true;
    }
}  // namespace ai
