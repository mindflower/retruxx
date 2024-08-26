#include "objectsicons.h"
#include <m3dapp.h>
#include <core/kernel.h>
#include <core/log.h>
#include <core/scoped_ptr.h>
#include <file/fileserver.h>
#include <file/filestream.h>

ObjectsIcons::Icon::Icon()
{
}

int ObjectsIcons::Icon::SetIco(int idx, CStr const& fileName)
{
    if (idx < 0 || idx >=2)
    {
        return 0;
    }
    M3D_RENDERER->ReleaseTexture(m_ico[idx]);
    if (!fileName.empty())
    {
        m_ico[idx] = M3D_RENDERER->AddTexture(fileName, 4);
    }
    else
    {
        m_ico[idx].SetInvalid();
    }
    return m_ico[idx].IsValid();
}

m3d::rend::TexHandle ObjectsIcons::Icon::GetIco(int mode) const
{
    if (mode < 0 || mode >=2)
    {
        return {};
    }
    return m_ico[mode];
}

ObjectsIcons::Icon::~Icon()
{
    throw std::logic_error("Not implemented");
}

ObjectsIcons::ObjectsIcons()
{
}

void ObjectsIcons::Clear(bool)
{
    throw std::logic_error("Not implemented");
}

m3d::rend::TexHandle ObjectsIcons::GetIcoByName(CStr const& name, int mode) const
{
    if (auto const icon = m_levelIcons.find(name); icon != m_levelIcons.end() && icon->second)
    {
        return icon->second->GetIco(mode);
    }

    if (auto const icon = m_globalIcons.find(name); icon != m_globalIcons.end() && icon->second)
    {
        return icon->second->GetIco(mode);
    }
    return {};
}

ObjectsIcons::~ObjectsIcons()
{
    throw std::logic_error("Not implemented");
}

int ObjectsIcons::Load(CStr const& fileName, bool bGlobal)
{
    scoped_ptr stream = m3d::g_Kernel->GetFileServer().CreateFileStream();
    if (!stream->Open(fileName.c_str(), m3d::fs::IStream::OPEN_READ))
    {
        M3D_LOG_INFO("Can't open file " + fileName + " for read.");
        return 0;
    }
    ref_ptr xmlFile = m3d::g_Kernel->CreateXmlFile();
    if (!xmlFile->Read(*stream))
    {
        M3D_LOG_INFO("Error: cannot parse " + fileName);
        return 0;
    }
    stream->Close();

    ref_ptr rootNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
    xmlFile->GetFirstChild_(rootNode, "icons");
    if (rootNode->IsEmpty())
    {
        M3D_LOG_INFO("ObjectsIcons::LoadFromXml error - cannot find root node icons");
        return 0;
    }
    auto res = 1;
    ref_ptr node = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
    for (rootNode->GetFirstChild_(node, "Item"); !node->IsEmpty(); node->GetNextSibling_(node, "Item"))
    {
        CStr strId;
        CStr file;
        CStr file1;
        m3d::SafeStrAttrib(strId, node, "id");
        if (!strId.empty())
        {
            m3d::SafeStrAttrib(file, node, "file");
            m3d::SafeStrAttrib(file1, node, "file1");
            auto icon = new ObjectsIcons::Icon;
            icon->SetIco(0, file);
            icon->SetIco(1, file1);
            if (!AddIcon(strId, icon, bGlobal))
            {
                delete icon;
                res = 0;
            }
        }
        else
        {
            M3D_LOG_INFO("ObjectsIcons::Load error - empty string id for icon");
            res = 0;
        }
    }
    if (!res)
    {
        M3D_LOG_INFO("ObjectsIcons inited with errors");
    }
    return res;
}

int ObjectsIcons::AddIcon(CStr const& strId, Icon* ico, bool bGlobal)
{
    //TODO: check this
    if (!ico || strId.empty())
    {
        return 0;
    }
    if (bGlobal)
    {
        auto it = m_globalIcons.find(strId);
        if (it != end(m_globalIcons))
        {
            M3D_LOG_INFO("ObjectsIcons::AddIcon warning - ico with id " + strId + " already exists and wiil be replaced");
            delete it->second;
        }
        m_globalIcons[strId] = ico;
    }
    else
    {
        auto it = m_levelIcons.find(strId);
        if (it != end(m_levelIcons))
        {
            M3D_LOG_INFO("ObjectsIcons::AddIcon warning - ico with id " + strId + " already exists and wiil be replaced");
            delete it->second;
        }
        m_levelIcons[strId] = ico;
    }
    return 1;
}
