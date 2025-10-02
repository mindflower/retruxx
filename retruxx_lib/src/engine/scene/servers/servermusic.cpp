#include <stdexcept>
#include <scene/servers/servermusic.h>

#include "config.h"
#include "m3dapp.h"
#include "core/kernel.h"
#include "core/log.h"
#include <core/ini.h>
#include "core/ref_ptr.h"
#include "core/scoped_ptr.h"

namespace
{
    struct MusicItem
    {
        MusicItem(int id)
        {
            m_soundId = id;
        }
        /* 0x0000 */ int m_soundId;
    }; /* size: 0x0004 */
}

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
        throw retruxx::logic_error("Not implemented");
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

    void MusicServer::AddItemsList(retruxx::vector<m3d::DataServer::ServerItem>& itemsList)
    {
        if (!M3D_KERNEL->GetEngineCfg().m_mus_Enable.GetB())
        {
            return;
        }

        size_t beginSize = itemsList.size();
        size_t last = 0;

        for (auto modelIt = m_models.begin(); modelIt != m_models.end();)
        {
            bool readFlag = false;
            for (auto& item : itemsList)
            {
                if (item.m_id == modelIt->m_name)
                {
                    item.m_fileWasRead = true;
                    readFlag = true;
                    ++last;
                    break;
                }
            }
            if (readFlag)
            {
                ++modelIt;
                continue;
            }

            if (modelIt->m_ptr)
            {
                delete modelIt->m_ptr;
                modelIt->m_ptr = nullptr;
            }
            else
            {
                M3D_LOG_INFO("Warning: Something goes wrong!");
            }


            modelIt = m_models.erase(modelIt);
        }

        auto numLeftItems = beginSize - last;
        if (numLeftItems < 1)
        {
            if (m_fnLoadCallback)
            {
                m_fnLoadCallback(100, m_fnLoadCallbackData);
            }
            return;
        }

        Proto proto;
        int protoPos;
        ParseProto(itemsList.front().m_filename.c_str(), &proto, &protoPos);
        if (proto != PROTO_FILE)
        {
            M3D_LOG_ERR("Error: protocol is not supported " + CStr(proto));
            return;
        }

        CStr err;
        if (ref_ptr xmlFile = m3d::ReadXmlFile(M3D_KERNEL->GetEngineCfg().m_snd_pathToMusic.GetS(), &err))
        {
            ref_ptr xmlNode = xmlFile->CreateNode();
            xmlFile->GetFirstChild(xmlNode, "sounds");
            if (xmlNode->IsEmpty())
            {
                return;
            }

            size_t lasta = 0;
            for (xmlNode->GetFirstChild(xmlNode, "model"); !xmlNode->IsEmpty(); xmlNode->GetNextSibling(xmlNode, "model"), ++lasta)
            {
                if (m_fnLoadCallback && lasta < numLeftItems)
                {
                    m_fnLoadCallback(100 * lasta / numLeftItems, m_fnLoadCallbackData);
                }

                CStr id = xmlNode->GetAttribute("id");
                if (GetItemByName(id.c_str(), false) != -1)
                {
                    continue;
                }

                for (auto& item : itemsList)
                {
                    if (!item.m_fileWasRead && id == item.m_id)
                    {
                        CStr file = xmlNode->GetAttribute("file");
                        auto res = M3D_APP->m_sound->AddSound(file.c_str(), snd::SND_TYPE_MUSIC, 0, 1, snd::SND_PRIORITY_EXTRAHIGH);
                        if (res != -1)
                        {
                            auto musicItem = new MusicItem(res);
                            DataServer::Model model(musicItem, file.c_str(), {}, id.c_str());
                            m_models.push_back(std::move(model));
                        }
                        item.m_fileWasRead = true;
                    }
                }
            }

            for (const auto& item : itemsList)
            {
                if (!item.m_fileWasRead)
                {
                    M3D_LOG_ERR("MusicServer: cannot read file: " + CStr(M3D_KERNEL->GetEngineCfg().m_snd_pathToSounds.GetS()) + " id = '" + item.m_id + "'");
                }
            }
        }
        else
        {
            M3D_LOG_ERR("ServerMusic: " + err);
        }
    }

    int MusicServer::_GetSoundIdByServerHandle(int) const
    {
        throw retruxx::logic_error("Not implemented");
    }
}
