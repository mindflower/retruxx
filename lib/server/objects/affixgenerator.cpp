#include "affixgenerator.h"
#include "core/ini.h"
#include "core/ref_ptr.h"

namespace ai
{
    void AffixGeneratorPrototypeInfo::_InternalCopyFrom(PrototypeInfo const& rhs)
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        // TODO: implement ::PostLoad
    }

    Obj* AffixGeneratorPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void AffixGeneratorPrototypeInfo::GenerateAffixesForObj(Obj* obj, unsigned int desiredNumAffixes) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai
