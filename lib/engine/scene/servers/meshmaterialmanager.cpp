#include "meshmaterialmanager.h"
#include <config.h>
#include <core/kernel.h>
#include <core/log.h>
#include "core/ini.h"
#include <core/scoped_ptr.h>
#include <file/fileserver.h>
#include <file/filestream.h>

#include "scene/nodes/sgnode.h"
#include "server/objects/base/obj.h"

namespace m3d
{
    MeshMaterialManager::~MeshMaterialManager()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void MeshMaterialManager::Init(CStr const& logoFileName, CStr const& belongsToLogoFileName)
    {
        m_pLogos = new AnimatedModel;
        if (g_Kernel->GetEngineCfg().m_loadFromGAM.GetB())
        {
            m_pLogos->LoadGAM(logoFileName, true);
        }
        else
        {
            m_pLogos->LoadSAM(logoFileName, true);
        }

        scoped_ptr fileStream = g_Kernel->GetFileServer().CreateFileStream();
        if (fileStream->Open(belongsToLogoFileName.c_str(), fs::IStream::OPEN_READ) == 0)
        {
            M3D_LOG_ERR("Error: BelongsToLogos: Can't open file: " + belongsToLogoFileName);
            return;
        }

        ref_ptr xmlFile = g_Kernel->CreateXmlFile();
        if (xmlFile->Read(*fileStream))
        {
            fileStream->Close();
            ref_ptr node = xmlFile->CreateNode(cmn::XML_NODE_EMPTY, nullptr);
            if (!xmlFile->GetFirstChild(node, "Belongs"))
            {
                return;
            }

            for (node->GetFirstChild(node, "Belong"); !node->IsEmpty(); node->GetNextSibling(node, "Belong"))
            {
                int id = 0;
                SafeIntAttrib(id, node, "id");
                int logo = 0;
                SafeIntAttrib(logo, node, "logo");
                m_mapBelongToLogo[id] = logo;
            }
        }
        else
        {
            M3D_LOG_ERR("Error: BelongsToLogos: Error while reading file: " + belongsToLogoFileName);
        }
    }

    MeshMaterialManager::MeshMaterialManager()
    {
    }

    void MeshMaterialManager::Release()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    DSurfaceMaterial& MeshMaterialManager::GetMaterial(SgNode& node, AnimatedModel::Mesh& mh)
    {
        if (mh.m_MaterialNumber < 0)
        {
            if (m_pLogos)
            {
                int belong = -1;
                node.GetProperty(4353, &belong);
                auto it = m_mapBelongToLogo.find(belong);
                if (it == m_mapBelongToLogo.end())
                {
                    return m_pLogos->GetMaterial(0, 0);
                }
                return m_pLogos->GetMaterial(0, it->second);
            }
            return mh.m_pModelSkins->front().front();
        }
        int skin = 0;
        node.GetProperty(8706, &skin);
        return mh.GetMaterial(skin);
    }
}
