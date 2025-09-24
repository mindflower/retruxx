#include "repositoryobjectsgenerator.h"

#include <stdexcept>

namespace ai
{
    RepositoryObjectsGeneratorPrototypeInfo::RepositoryObjectsGeneratorPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    bool RepositoryObjectsGeneratorPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile,
        const m3d::cmn::XmlNode* xmlNode)
    {
        throw std::logic_error("Not implemented");
    }

    void RepositoryObjectsGeneratorPrototypeInfo::PostLoad()
    {
        throw std::logic_error("Not implemented");
    }

    ai::Obj* RepositoryObjectsGeneratorPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }
}
