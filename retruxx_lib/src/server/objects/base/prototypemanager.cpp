#include "prototypemanager.h"
#include <core/log.h>
#include <file/fileserver.h>
#include <file/filestream.h>
#include "prototypeinfo.h"
#include <core/scoped_ptr.h>
#include <core/ini.h>
#include <server/server.h>
#include <server/resourcemanager.h>


namespace ai
{
    PrototypeManager::PrototypeManager()
    {
    }

    void PrototypeManager::Clear()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned PrototypeManager::GetPrototypeFullNameLocalizedForm(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PrototypeManager::LoadFromXmlFile(CStr const& fileName)
    {
        ++m_loadingLock;
        _LoadGameObjectsFolderFromXML(fileName, ai::PrototypeManager::_ReadNewPrototype);
        for (auto& prototype : m_prototypes)
        {
            prototype->PostLoad();
        }
        --m_loadingLock;
    }

    int PrototypeManager::GetMatrixNum(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PrototypeManager::RefreshFromXmlFile(CStr const& fileName)
    {
        ai::PrototypeManager::_LoadGameObjectsFolderFromXML(fileName, ai::PrototypeManager::_RefreshPrototype);
    }

    CStr PrototypeManager::GetPrototypeFullName(CStr const&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr PrototypeManager::GetPrototypeFullName(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr PrototypeManager::GetPrototypeName(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int PrototypeManager::GetPrototypeId(CStr const& prototypeName) const
    {
        int res = -1;
        if (m_prototypeNamesToIds.get(prototypeName, res))
        {
            return res;
        }

        if (!prototypeName.empty())
        {
            M3D_LOG_ERR("Error: No prototype '" + prototypeName + "' registered");
        }
        return res;
    }

    PrototypeManager::~PrototypeManager()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PrototypeManager::GetPrototypeIdsByResourceId(int resourceId,  retruxx::vector<int, retruxx::allocator<int>>& prototypeIds) const
    {
        prototypeIds.clear();

        for (auto& proto : m_prototypes)
        {
            if (theResourceManager->bResourceIsKindOf(proto->m_resourceId, resourceId))
            {
                prototypeIds.push_back(proto->m_resourceId);
            }
        }
    }

    PrototypeInfo const* PrototypeManager::GetPrototypeInfo(CStr const&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    PrototypeInfo const* PrototypeManager::GetPrototypeInfo(int id) const
    {
        if (id < m_prototypes.size())
        {
            return m_prototypes[id];
        }
        return nullptr;
    }

    int PrototypeManager::GetNumOfPrototypes() const
    {
        return m_prototypes.size();
    }

    void PrototypeManager::_LoadGameObjectsFolderFromXML(CStr const& fileName, bool(* action)(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*))
    {
        scoped_ptr fileStream = M3D_KERNEL->GetFileServer().CreateFileStream();
        if (fileStream->Open(fileName.c_str(), m3d::fs::IStream::OPEN_READ))
        {
            ref_ptr xml = M3D_KERNEL->CreateXmlFile();
            if (xml->Read(*fileStream))
            {
                fileStream->Close();

                ref_ptr xmlNode = xml->CreateNode();
                xml->GetFirstChild(xmlNode, "Prototypes");
                if (!xmlNode->IsEmpty())
                {
                    auto dirName = DirectoryFromFileName(fileName);
                    _LoadFromFolder(xml, xmlNode, dirName, action);
                }
                else
                {
                    M3D_LOG_ERR("Error: Tag <Prototypes> not found in file: " + fileName);
                }
            }
            else
            {
                M3D_LOG_ERR("Error: cannot parse " + fileName);
            }
        }
        else
        {
            M3D_LOG_ERR("Error: cannot open " + fileName);
        }
    }

    PrototypeInfo* PrototypeManager::_InternalGetPrototypeInfo(CStr const& prototypeName)
    {
        for (const auto& proto : m_prototypes)
        {
            if (proto->m_prototypeName == prototypeName)
            {
                return proto;
            }
        }
        return nullptr;
    }

    bool PrototypeManager::_RefreshPrototype(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        CStr prototypeName = xmlNode->GetAttribute("Name");

        auto* prototypeInfo = thePrototypeManager->_InternalGetPrototypeInfo(prototypeName);
        if (prototypeInfo)
        {
            prototypeInfo->RefreshFromXml(xmlFile, xmlNode);
            return 1;
        }

        M3D_LOG_ERR("Error: prototype '" + prototypeName + "' is not loaded");
        return 0;
    }

    void PrototypeManager::_LoadFromFolder(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* RootNode, CStr const& directory, bool(*action)(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*))
    {
        ref_ptr node = xmlFile->CreateNode();
        for (RootNode->GetFirstChild(node, nullptr); !node->IsEmpty(); node->GetNextSibling(node, nullptr))
        {
            if (node->IsOfType(m3d::cmn::XmlNodeType::XML_NODE_ELEMENT))
            {
                CStr value = node->GetValue();
                if (value != "Folder")
                {
                    action(xmlFile, node);
                    continue;
                }

                CStr fileName = node->GetAttribute("File");
                if (!fileName.empty())
                {
                    _LoadGameObjectsFolderFromXML(directory + "/" + fileName, action);
                }
                else
                {
                    _LoadFromFolder(xmlFile, node, directory, action);
                }
            }
        }
    }

    bool PrototypeManager::_ReadNewPrototype(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        CStr className = xmlNode->GetAttribute("Class");
        auto* prototypeInfo = ai::pServer->CreatePrototypeInfoByClassName(className);
        if (prototypeInfo)
        {
            prototypeInfo->m_className = className;
            CStr parentPrototypeName = xmlNode->GetAttribute("ParentPrototype");
            if (!parentPrototypeName.empty())
            {
                auto* parentPrototypeInfo = ai::thePrototypeManager->_InternalGetPrototypeInfo(parentPrototypeName);
                if (!parentPrototypeInfo)
                {
                    M3D_CRITICAL_ERROR("parent prototype '" + parentPrototypeName + "' is not loaded");
                }
                prototypeInfo->CopyFrom(*parentPrototypeInfo);
            }

            prototypeInfo->m_prototypeId = ai::thePrototypeManager->m_prototypes.size();
            if (prototypeInfo->LoadFromXML(xmlFile, xmlNode))
            {
                int id = -1;
                if (ai::thePrototypeManager->m_prototypeNamesToIds.get(prototypeInfo->m_prototypeName, id))
                {
                    M3D_CRITICAL_ERROR("duplicate prototype in game objects: '" + prototypeInfo->m_prototypeName + "'");
                }
                ai::thePrototypeManager->m_prototypeNamesToIds.add(prototypeInfo->m_prototypeName, prototypeInfo->m_prototypeId);
                ai::thePrototypeManager->m_prototypes.push_back(prototypeInfo);
                return 1;
            }
            else
            {
                M3D_LOG_ERR("Error: prototype '" + prototypeInfo->m_prototypeName + "' of class " + prototypeInfo->m_className + " was not loaded");
            }
        }
        else
        {
            M3D_LOG_ERR("Error: Invalid class name : <" + className + ">");
        }
        return 0;
    }
}
