#include "blockmusicmanager.h"
#include <config.h>
#include <stdexcept>
#include <core/kernel.h>
#include <core/log.h>
#include <core/ref_ptr.h>
#include <core/ini.h>

#include "game/m3dgame.h"
#include "game/uimisc/levelinfo.h"
#include <client.h>
#include <level.h>
#include <world.h>
#include "scene/servers/DataServer.h"

namespace m3d
{
    void BlockMusicManager::SetMusicType(BlockMusicType musicType)
    {
        M3D_ASSERT((int)musicType >= 0 && (int)musicType < NUM_MUSIC_TYPES);

        if (musicType != m_curMusicType)
        {
            if (musicType == DRIVING)
            {
                Reset();
            }
            m_curMusicType = musicType;
            m_bMustPlayNewMusic = true;
        }
    }

    bool BlockMusicManager::IsMusicBlockNameValid(CStr const& musicBlockName) const
    {
        return m_blockNamesToIds.find(musicBlockName) != m_blockNamesToIds.cend();
    }

    void BlockMusicManager::Reset()
    {
        // RVA 0x424040
        if (!m_bMustPlayNewMusic)
        {
            M3D_LOG_INFO("BlockMusicManager: resetting");
        }

        if (m_blocks.empty())
        {
            m_curBlockNum = -1;
            return;
        }

        // The current level may name a set of music blocks to pick from;
        // otherwise the first block is used.
        m_curBlockNum = 0;
        auto* levelInfoManager = M3D_APP->m_pInterfaceManager->GetLevelInfoManager();
        if (levelInfoManager)
        {
            CStr levelName;
            if (pClient && pClient->GetWorld().m_level)
            {
                levelName = pClient->GetWorld().m_level->m_levelName;
            }
            auto const* levelInfo = levelInfoManager->GetLevelInfoByName(levelName);
            if (levelInfo)
            {
                CStr const blockNameFromLevel = levelInfo->GetRandomMusicBlock();
                if (!blockNameFromLevel.empty())
                {
                    auto const it = m_blockNamesToIds.find(blockNameFromLevel);
                    if (it == m_blockNamesToIds.end())
                    {
                        M3D_CRITICAL_ERROR("unknown music block: " + blockNameFromLevel);
                    }
                    // NOTE: like the original, this reads through end() when the
                    // critical error above returns rather than aborting.
                    m_curBlockNum = it->second;
                }
            }
        }

        M3D_ASSERT(static_cast<unsigned>(m_curBlockNum) < m_blocks.size());
        m_bMustPlayNewMusic = true;
    }

    void BlockMusicManager::Init()
    {
        for (auto const& block : m_blocks)
        {
            auto item = M3D_APP->GetMusicServer().GetItemByName(block.front().c_str(), true);
            if (item != -1)
            {
                M3D_APP->GetMusicServer().SetItemProperty(item, 9856, &BlockMusicManager::_MusicEndCallback);
            }
        }
    }

    void BlockMusicManager::InitOnce()
    {
        m_instance = this;
        m_blocks.clear();
        m_blockNamesToIds.clear();
        m_curBlockNum = -1;
        if (g_Kernel->GetEngineCfg().m_mus_Enable.GetB())
        {
            CStr err;
            CStr fileName = g_Kernel->GetEngineCfg().m_snd_pathToMusicBlocks.GetS();
            ref_ptr xmlFile = ReadXmlFile(fileName.c_str(), &err);
            if (xmlFile)
            {
                ref_ptr musicBlocksNode = xmlFile->CreateNode(cmn::XML_NODE_EMPTY, nullptr);
                xmlFile->GetFirstChild(musicBlocksNode, "MusicBlocks");

                ref_ptr blockNode = xmlFile->CreateNode(cmn::XML_NODE_EMPTY, nullptr);
                for (musicBlocksNode->GetFirstChild(blockNode, "Block"); !blockNode->IsEmpty(); blockNode->GetNextSibling(blockNode, "Block"))
                {
                    std::vector<CStr> block;
                    const char* musicNames[3] = { "driving", "alarm", "battle" };
                    for (auto musicName : musicNames)
                    {
                        block.push_back(blockNode->GetAttribute(musicName));
                    }
                    M3D_ASSERT(block.size() == NUM_MUSIC_TYPES);

                    CStr blockName;
                    SafeStrAttrib(blockName, blockNode, "name");
                    if (blockName.empty())
                    {
                        SYS_ERROR("!\"Error: empty music block name\"");
                    }
                    auto it = m_blockNamesToIds.find(blockName);
                    if (it != end(m_blockNamesToIds))
                    {
                        M3D_CRITICAL_ERROR("duplicate music block name: " + blockName);
                    }

                    m_blockNamesToIds[blockName] = m_blocks.size();
                    m_blocks.push_back(block);
                }
                if (!m_blocks.empty())
                {
                    m_curBlockNum = 0;
                }
            }
            else
            {
                M3D_LOG_ERR("Error: No BlockMusicManager file: " + fileName + " err: " + err);
            }
        }
    }

    int BlockMusicManager::PlayCurrentMusic()
    {
        if (!m_bMustPlayNewMusic)
        {
            return m_curChannelId;
        }
        if (m_curBlockNum == -1)
        {
            return -1;
        }
        if (m_curMusicType >= NUM_MUSIC_TYPES)
        {
            return -1;
        }

        if (!M3D_ENGINE_CFG.m_mus_Enable.GetB())
        {
            return -1;
        }

        auto& block = m_blocks[m_curBlockNum][m_curMusicType];
        M3D_LOG_INFO("BlockMusicManager: start playing new music: blockNum = " + CStr(m_curBlockNum) + ", musicType = " + CStr(m_curMusicType) + ", name = '" + block + "'");
        M3D_APP->StartPlayingMusic(block.c_str(), true, m_curMusicType != DRIVING);

        m_bMustPlayNewMusic = false;
        return m_curChannelId;
    }

    void BlockMusicManager::_MusicEndCallback(int)
    {
        // RVA 0x424470
        M3D_LOG_INFO("BlockMusicManager: current music ended");
        m_instance->m_bMustPlayNewMusic = true;
    }
}
