#include "townmusicmanager.h"
#include <stdexcept>

#include "config.h"
#include "m3dapp.h"
#include "core/kernel.h"
#include "core/log.h"
#include "core/ref_ptr.h"
#include <core/ini.h>

namespace m3d
{
    void TownMusicManager::Activate()
    {
        throw std::logic_error("Not implemented");
    }

    int TownMusicManager::LaunchAmbientWorkshop()
    {
        throw std::logic_error("Not implemented");
    }

    void TownMusicManager::StopAmbient()
    {
        throw std::logic_error("Not implemented");
    }

    void TownMusicManager::Init()
    {
        m_ambientShopSounds.clear();
        m_ambientWorkshopSounds.clear();
        if (!g_Kernel->GetEngineCfg().m_mus_Enable.GetB())
        {
            return;
        }
        auto fileName = g_Kernel->GetEngineCfg().m_snd_pathToTownAmbient.GetS();
        CStr err;
        ref_ptr xmlFile = ReadXmlFile(fileName, &err);
        if (xmlFile)
        {
            ref_ptr rootNode = xmlFile->CreateNode(cmn::XML_NODE_EMPTY, nullptr);
            xmlFile->GetFirstChild(rootNode, "TownAmbience");
            ref_ptr node = xmlFile->CreateNode(cmn::XML_NODE_EMPTY, nullptr);
            for (rootNode->GetFirstChild(node, "Sound"); !node->IsEmpty(); node->GetNextSibling(node, "Sound"))
            {
                auto pathAttr = node->GetAttribute("path");
                //TODO: check this!!
                auto snd = Application::g_pApp->m_sound->AddSound(pathAttr, snd::SND_TYPE_2DSOUND, 1, 1, snd::SND_PRIORITY_NORMAL);
                auto typeAttr = node->GetAttribute("type");
                if (!CStr::my_strcmp(typeAttr, "shop"))
                {
                    m_ambientShopSounds.push_back(snd);
                }
                else if (!CStr::my_strcmp(typeAttr, "workshop"))
                {
                    m_ambientWorkshopSounds.push_back(snd);
                }
            }
        }
        else
        {
            M3D_LOG_ERR("Error: reading file: " + CStr(fileName));
        }
    }

    int TownMusicManager::LaunchAmbientShop()
    {
        throw std::logic_error("Not implemented");
    }
}
