#include "externalpaths.h"

#include "core/ini.h"
#include "core/kernel.h"
#include "core/log.h"
#include "core/ref_ptr.h"
#include "core/scoped_ptr.h"
#include "file/fileserver.h"
#include "file/filestream.h"

namespace ai
{
    bool ExternalPaths::Load(const CStr& fileName)
    {
        scoped_ptr stream = M3D_KERNEL->GetFileServer().CreateFileStream();
        if (!stream->Open(fileName.c_str(), m3d::fs::IStream::OPEN_READ))
        {
            M3D_LOG_ERR("Error: ExternalPaths can't read file " + CStr(fileName));
            return false;
        }

        ref_ptr xmlFile = M3D_KERNEL->CreateXmlFile();
        if (!xmlFile->Read(*stream))
        {
            M3D_LOG_ERR("Error: ExternalPaths can't read xml " + CStr(fileName) + "(" + CStr(xmlFile->GetError()) + ")");
            return false;
        }

        ref_ptr pathsNode = xmlFile->CreateNode();
        xmlFile->GetFirstChild(pathsNode, "Paths");
        if (pathsNode->IsEmpty())
        {
            M3D_LOG_ERR("Error: ExternalPaths can't find root node Paths in file " + CStr(fileName));
            return false;
        }

        ref_ptr pathNode = xmlFile->CreateNode();
        for (pathsNode->GetFirstChild(pathNode, "Path"); !pathNode->IsEmpty(); pathNode->GetNextSibling(pathNode, "Path"))
        {
            CStr name = pathNode->GetAttribute("Name");
            if (name.empty())
            {
                M3D_LOG_ERR("Error reading external paths: empty path name");
                continue;
            }

            if (m_paths.find(name) != m_paths.end())
            {
                M3D_LOG_ERR("Error: duplicate path name '" + name + "' found in ExternalPaths");
                continue;
            }

            retruxx::vector<CVector2> paths;
            ref_ptr pointNode = xmlFile->CreateNode();
            for (pathNode->GetFirstChild(pointNode, "Point"); !pointNode->IsEmpty(); pointNode->GetNextSibling(pointNode, "Point"))
            {
                CVector2 point(0.0, 0.0);
                if (m3d::SafeVector2Attrib(point, pointNode, "coord"))
                {
                    paths.push_back(std::move(point));
                }
                else
                {
                    M3D_LOG_ERR("Error reading path name = '" + name);
                }
            }

            m_paths.emplace(std::move(name), std::move(paths));
        }
        return true;
    }

    // RVA 0x7C8750
    bool ExternalPaths::Save(const CStr& filename)
    {
        if (filename.empty())
        {
            return false;
        }

        ref_ptr xmlFile = M3D_KERNEL->CreateXmlFile();
        ref_ptr rootNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Paths");
        for (PathMap::const_iterator it = m_paths.begin(); it != m_paths.end(); ++it)
        {
            ref_ptr pathNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Path");
            pathNode->SetAttribute("Name", it->first.c_str());
            for (CVector2 const& point : it->second)
            {
                ref_ptr pointNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Point");
                pointNode->SetAttribute("coord", CStr(point).c_str());
                pathNode->AddChild(pointNode.get());
            }
            rootNode->AddChild(pathNode.get());
        }
        xmlFile->AddChild(rootNode.get());

        scoped_ptr stream = M3D_KERNEL->GetFileServer().CreateFileStream();
        if (!stream->Open(filename.c_str(), m3d::fs::IStream::OPEN_WRITE))
        {
            M3D_LOG_INFO(CStr("Could not save external paths into file ") + CStr(filename));
            return false;
        }
        xmlFile->Write(*stream);
        stream->Close();
        return true;
    }

    const retruxx::vector<CVector2, retruxx::allocator<CVector2>>& ExternalPaths::GetPath(const CStr& pathName) const
    {
        auto it = m_paths.find(pathName);
        if (it != m_paths.end())
        {
            return it->second;
        }

        M3D_LOG_WARN("Warning: external path name = '" + pathName + "' doesn't exist");
        return m_emptyPath;
    }

    bool ExternalPaths::SetPath(const CStr& pathName,
        const retruxx::vector<CVector2, retruxx::allocator<CVector2>>& thePath)
    {
        // RVA 0x7CA370
        if (pathName.empty())
        {
            M3D_LOG_INFO("Warning: attempting to set path with empty name!");
            return false;
        }
        m_paths[pathName] = thePath;
        return true;
    }

    bool ExternalPaths::AddPath(const CStr& pathName,
        const retruxx::vector<CVector2, retruxx::allocator<CVector2>>& thePath)
    {
        // RVA 0x7CA410 - NOTE: unlike SetPath, an empty name is not rejected.
        if (m_paths.find(pathName) != m_paths.end())
        {
            M3D_LOG_ERR(CStr("Error: duplicate path name '") + pathName + CStr("' is no added in ExternalPaths"));
            return false;
        }
        m_paths[pathName] = thePath;
        return true;
    }

    bool ExternalPaths::DelPath(const CStr& pathName)
    {
        // RVA 0x7C9660
        PathMap::iterator const it = m_paths.find(pathName);
        if (it == m_paths.end())
        {
            M3D_LOG_INFO(CStr("Warning: external path name = '") + pathName + CStr("' doesn't exist"));
            return false;
        }
        m_paths.erase(it);
        return true;
    }

    const retruxx::vector<CStr, retruxx::allocator<CStr>>& ExternalPaths::GetPathNames() const
    {
        // RVA 0x7C9230 - NOTE: the names are collected into m_emptyNames, so this const method
        // rebuilds that member on every call.
        retruxx::vector<CStr>& names = const_cast<retruxx::vector<CStr>&>(m_emptyNames);
        names.clear();
        for (PathMap::const_iterator it = m_paths.begin(); it != m_paths.end(); ++it)
        {
            names.push_back(it->first);
        }
        return names;
    }
}
