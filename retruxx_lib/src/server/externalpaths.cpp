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

    bool ExternalPaths::Save(const CStr& filename)
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool ExternalPaths::AddPath(const CStr& pathName,
        const retruxx::vector<CVector2, retruxx::allocator<CVector2>>& thePath)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool ExternalPaths::DelPath(const CStr& pathName)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    const retruxx::vector<CStr, retruxx::allocator<CStr>>& ExternalPaths::GetPathNames() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}
