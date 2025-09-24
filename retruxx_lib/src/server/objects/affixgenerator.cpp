#include "affixgenerator.h"

namespace ai
{
    void AffixGeneratorPrototypeInfo::_InternalCopyFrom(const PrototypeInfo& rhs)
    {
        throw std::logic_error("Not implemented");
    }

    AffixGeneratorPrototypeInfo::AffixGeneratorPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    bool AffixGeneratorPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode)
    {
        throw std::logic_error("Not implemented");
    }

    void AffixGeneratorPrototypeInfo::PostLoad()
    {
        throw std::logic_error("Not implemented");
    }

    Obj* AffixGeneratorPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    void AffixGeneratorPrototypeInfo::GenerateAffixesForObj(Obj* obj, unsigned int desiredNumAffixes) const
    {
        throw std::logic_error("Not implemented");
    }
}
