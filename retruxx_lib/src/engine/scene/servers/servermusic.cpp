#include <stdexcept>
#include <scene/servers/servermusic.h>

#include "config.h"
#include "m3dapp.h"
#include "core/kernel.h"
#include "core/log.h"
#include <core/ini.h>
#include "core/ref_ptr.h"
#include "core/scoped_ptr.h"

namespace m3d
{
    MusicServer::~MusicServer()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int MusicServer::GetItemProperty(int, int, void*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int MusicServer::SetItemProperty(int, int, void*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void MusicServer::RenderItem(int, void*)
    {
        // TODO: implement MusicServer::RenderItem
        //throw retruxx::logic_error("Not implemented");
    }

    int MusicServer::Release()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void MusicServer::PostLoad()
    {
    }

    int MusicServer::RemoveItem(int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int MusicServer::SaveAllLoadedEntities(char const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int MusicServer::AddItem(char const* params, char const* id)
    {
        if (!g_Kernel->GetEngineCfg().m_mus_Enable.GetB())
        {
            return -1;
        }
        if (auto res = GetItemByName(id, false); res != -1)
        {
            return res;
        }

        Proto proto = PROTO_NONE;
        int paramsPos = 0;
        ParseProto(params, &proto, &paramsPos);
        if (proto != PROTO_FILE)
        {
            M3D_LOG_INFO("protocol is not supported " + CStr(proto));
            return - 1;
        }
        CStr err;
        ref_ptr xmlFile = ReadXmlFile(g_Kernel->GetEngineCfg().m_snd_pathToMusic.GetS(), &err);
        if (!xmlFile)
        {
            M3D_LOG_INFO("ServerMusic: " + err);
            return -1;
        }
        auto musicNode = xmlFile->CreateNode(cmn::XML_NODE_EMPTY, nullptr);
        xmlFile->GetFirstChild(musicNode, "Music");
        if (musicNode->IsEmpty())
        {
            return -1;
        }

        auto modelNode = xmlFile->CreateNode(cmn::XML_NODE_EMPTY, nullptr);
        for (musicNode->GetFirstChild(modelNode, "model"); !modelNode->IsEmpty(); modelNode->GetNextSibling(modelNode, "model"))
        { 
	        if (modelNode->GetAttribute("id") == CStr(id))
	        {
                break;
	        }
        }

        if (modelNode->IsEmpty())
        {
            return -1;
        }

        CStr fileName = modelNode->GetAttribute("file");
        auto res = m3d::Application::g_pApp->m_sound->AddSound(fileName.c_str(), snd::SND_TYPE_MUSIC, 0, 1, snd::SND_PRIORITY_HIGH);
        if (res ==-1)
        {
            M3D_LOG_INFO("MusicServer: cannot read file: " + fileName + ", id = " + CStr(id));
            return -1;
        }

        //TODO: check this
        auto snd = new int(res);
        Model model(snd, fileName.c_str(), {}, id);
        m_models.push_back(std::move(model));
        return m_models.size();
    }

    void MusicServer::AddItemsList(retruxx::vector<m3d::DataServer::ServerItem>&)
    {
        // TODO: implement MusicServer::AddItemsList
        //throw retruxx::logic_error("Not implemented");
    }

    int MusicServer::_GetSoundIdByServerHandle(int) const
    {
        throw retruxx::logic_error("Not implemented");
    }
}
