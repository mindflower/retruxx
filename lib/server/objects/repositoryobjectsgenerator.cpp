#include "repositoryobjectsgenerator.h"

#include <stdexcept>

namespace ai
{
    RepositoryObjectsGeneratorPrototypeInfo::RepositoryObjectsGeneratorPrototypeInfo() = default;

    bool RepositoryObjectsGeneratorPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = ai::PrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            // TODO: implement RepositoryObjectsGeneratorPrototypeInfo::LoadFromXML
        }
        return result;
    }

    void RepositoryObjectsGeneratorPrototypeInfo::PostLoad()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ai::Obj* RepositoryObjectsGeneratorPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai
