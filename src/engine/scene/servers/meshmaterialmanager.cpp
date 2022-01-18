#include "meshmaterialmanager.h"
#include <config.h>
#include <core/kernel.h>
#include <core/log.h>
#include <core/scoped_ptr.h>
#include <file/fileserver.h>
#include <file/filestream.h>

namespace m3d
{
    MeshMaterialManager::~MeshMaterialManager()
    {
        throw std::logic_error("Not implemented");
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
            if (!xmlFile->GetFirstChild_(node, "Belongs"))
            {
                return;
            }

            for (node->GetFirstChild_(node, "Belong"); !node->IsEmpty(); node->GetNextSibling_(node, "Belong"))
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
        throw std::logic_error("Not implemented");
    }

    DSurfaceMaterial& MeshMaterialManager::GetMaterial(SgNode&, AnimatedModel::Mesh&)
    {
        throw std::logic_error("Not implemented");
    }
}
