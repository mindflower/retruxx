#include <stdexcept>
#include <scene/servers/serversound.h>
#include <core/kernel.h>
#include <config.h>
#include <m3dapp.h>

#include "core/ini.h"
#include "core/log.h"
#include "core/ref_ptr.h"

namespace 
{
    const char* FAKE_ITEM_NAME = "Fake_Sound_Do_Not_Use_It";
    const char* DEFAULT_GROUP = "SOUND3D";
    CStr TYPE_SINGLE = "SINGLE";
    CStr TYPE_DOUBLE = "DOUBLE";
    CStr TYPE_TRIPLE = "TRIPLE";
    CStr TYPE_FAKE = "FAKE";

}

namespace m3d
{
    void Sound3DServer::UnregisterNode(m3d::SgNode* node)
    {
        if (M3D_KERNEL->GetEngineCfg().m_snd_Enable.GetB())
        {
            auto channel = -1;
            node->GetProperty(9729u, &channel);
            if (channel != -1)
                m3d::Application::g_pApp->m_sound->StopChannel(channel);
        }
    }

    int Sound3DServer::RemoveItem(int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    Sound3DServer::~Sound3DServer()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Sound3DServer::AddItem(char const*, char const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Sound3DServer::PostLoad()
    {
    }

    int Sound3DServer::GetItemProperty(int, int, void*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Sound3DServer::RegisterNode(m3d::SgNode*)
    {
    }

    int Sound3DServer::SaveAllLoadedEntities(char const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Sound3DServer::RenderItem(int id, void* params)
    {
        if (M3D_KERNEL->GetEngineCfg().m_snd_Enable.GetB())
        {
            if (id != 2)
            {
                if (id == -3)
                {
                    auto pos = M3D_RENDERER->MatGetOrgInv();

                    CVector up;
                    CVector front;
                    CVector r;
                    M3D_RENDERER->MatGetBasis(r, up, front);

                    // TODO: check this
                    M3D_APP->m_sound->SetListenerPosition(pos, r, front, up);
                    return;
                }
                if (id != -4)
                {
                    throw retruxx::logic_error("Not implemented");
                }
            }
        }
    }

    int Sound3DServer::Release()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Sound3DServer::AddItemsList(retruxx::vector<m3d::DataServer::ServerItem>& itemsList)
    {
        m3d::DataServer::ServerItem fakeItem;
        fakeItem.m_id = FAKE_ITEM_NAME;
        fakeItem.m_filename = "file:";
        itemsList.push_back(fakeItem);

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
        if (ref_ptr xmlFile = m3d::ReadXmlFile(M3D_KERNEL->GetEngineCfg().m_snd_pathToSounds.GetS(), &err))
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
                        _AddItemFromXmlNode(xmlNode);
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

    void Sound3DServer::_AddItemFromXmlNode(m3d::cmn::XmlNode const* xmlNode)
    {
        CStr id = xmlNode->GetAttribute("id");

        CStr type;
        m3d::SafeStrAttrib(type, xmlNode, "type");

        auto* group = xmlNode->GetAttribute("group");
        if (!group)
        {
            group = DEFAULT_GROUP;
        }

        if (type == TYPE_DOUBLE)
        {
            CStr fileStart;
            m3d::SafeStrAttrib(fileStart, xmlNode, "file_start");

            CStr fileEnd;
            m3d::SafeStrAttrib(fileStart, xmlNode, "file_end");

            if (_AddDoubleItem(fileStart, fileEnd, id.c_str(), group) == -1)
            {
                M3D_LOG_ERR("DataServer: cannot read " + fileStart + " id = " + id);
            }

        }
        else if (type == TYPE_TRIPLE)
        {
            CStr fileStart;
            m3d::SafeStrAttrib(fileStart, xmlNode, "file_start");

            CStr fileLoop;
            m3d::SafeStrAttrib(fileStart, xmlNode, "file_loop");

            CStr fileEnd;
            m3d::SafeStrAttrib(fileStart, xmlNode, "file_end");

            if (_AddTripleItem(fileStart, fileLoop, fileEnd, id.c_str(), group) == -1)
            {
                M3D_LOG_ERR("DataServer: cannot read " + fileStart + " id = " + id);
            }
        }
        else if (type == TYPE_FAKE)
        {
            if (_AddFakeItem() == -1)
            {
                M3D_LOG_ERR("DataServer: cannot read " + CStr(FAKE_ITEM_NAME));
            }
        }
        else if (type.empty() || type == TYPE_SINGLE)
        {
            CStr file;
            m3d::SafeStrAttrib(file, xmlNode, "file");

            if (_AddItem(file.c_str(), id.c_str(), group) == -1)
            {
                M3D_LOG_ERR("DataServer: cannot read " + file + " id = " + id);
            }
        }
    }

    int Sound3DServer::_AddFakeItem()
    {
        if (GetItemByName(FAKE_ITEM_NAME, false) == -1)
        {
            auto soundItem = new SoundItem();
            soundItem->type = SOUND_TYPE_SIMPLE;
            soundItem->soundIds[0] = -1;

            m3d::DataServer::Model model(soundItem, nullptr, nullptr, FAKE_ITEM_NAME);
            m_models.push_back(std::move(model));
            return m_models.size() - 1;
        }
        return -1;
    }

    int Sound3DServer::_AddTripleItem(CStr, CStr, CStr, char const*, char const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    Sound3DServer::SoundItem::SoundItem()
    {
        this->type = SOUND_TYPE_SIMPLE;
        this->soundIds[0] = -1;
        this->soundIds[1] = -1;
        this->soundIds[2] = -1;
    }

    int Sound3DServer::_AddItem(char const* fileName, char const* id, char const* groupName)
    {
        if (!M3D_KERNEL->GetEngineCfg().m_snd_Enable.GetB())
        {
            return 0;
        }

        int soundType = 1;
        if (groupName)
        {
            soundType = strcmp(groupName, "SOUND2D") != 0;
        }

        auto soundId = M3D_APP->m_sound->AddSound(fileName, (snd::UserSoundType)soundType, groupName, 8, snd::SND_PRIORITY_NORMAL);
        if (soundId == -1)
        {
            M3D_LOG_INFO("SoundServer: cannot add sound " + CStr(fileName));
            return -1;
        }

        auto soundItem = new SoundItem();
        soundItem->type = SOUND_TYPE_SIMPLE;
        soundItem->soundIds[0] = soundId;
        DataServer::Model model(soundItem, fileName, fileName, id);
        m_models.push_back(std::move(model));
        return m_models.size() - 1;
    }

    int Sound3DServer::_AddDoubleItem(CStr, CStr, char const*, char const*)
    {
        throw retruxx::logic_error("Not implemented");
    }
}
