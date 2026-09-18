#include "repositoryobjectsgenerator.h"
#include "base/prototypemanager.h"

#include <core/ini.h>
#include <core/kernel.h>
#include <core/log.h>
#include <core/ref_ptr.h>
#include <stdexcept>

namespace ai
{
    RepositoryObjectsGeneratorPrototypeInfo::RepositoryObjectsGeneratorPrototypeInfo() = default;

    bool RepositoryObjectsGeneratorPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x875730 - one <Object> child per kind of thing this generator may put on a shelf.
        bool const result = ai::PrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            ref_ptr objectNode = xmlFile->CreateNode();
            for (xmlNode->GetFirstChild(objectNode, "Object"); !objectNode->IsEmpty();
                 objectNode->GetNextSibling(objectNode, "Object"))
            {
                CStr prototypeName;
                m3d::SafeStrAttrib(prototypeName, objectNode, "PrototypeName");
                if (prototypeName.empty())
                {
                    M3D_LOG_ERR(
                        "Error: empty prototype name for object in RepositoryObjectsGenerator '" +
                        m_prototypeName + CStr("'"));
                    continue;
                }

                ObjectDescription newDescription;
                newDescription.prototypeName = prototypeName;
                newDescription.prototypeId = -1;
                m_objectDescriptions.push_back(newDescription);
            }
        }
        return result;
    }

    void RepositoryObjectsGeneratorPrototypeInfo::PostLoad()
    {
        // RVA 0x874DF0 - the names can only be turned into ids once every prototype has been read.
        for (auto& description : m_objectDescriptions)
        {
            description.prototypeId = thePrototypeManager->GetPrototypeId(description.prototypeName);
            if (description.prototypeId == -1)
            {
                M3D_LOG_ERR(
                    "Error: Unknown Object prototype: '" + description.prototypeName + CStr("'"));
            }
        }
    }

    void RepositoryObjectsGeneratorPrototypeInfo::Generate(unsigned count, ai::GeomRepository* repository) const
    {
        // RVA 0x874FB0 - fills a shelf by picking kinds at random until it holds the wanted number of
        // items, or until nothing more will fit.
        if (m_objectDescriptions.empty())
        {
            return;
        }
        while (repository->GetNumItems() < count)
        {
            unsigned const index = static_cast<unsigned>(m_objectDescriptions.size()) * rand() / 0x8000;
            if (!repository->AddItems(m_objectDescriptions[index].prototypeId, 1))
            {
                break;
            }
        }
    }

    ai::Obj* RepositoryObjectsGeneratorPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x874910 - a generator is a prototype only; it never becomes an object of its own.
        return nullptr;
    }
}  // namespace ai
