#include "radioengine.h"
#include <stdexcept>

#include "config.h"
#include "m3dapp.h"
#include "core/kernel.h"
#include "core/log.h"
#include <core/ini.h>
#include "core/ref_ptr.h"
#include "server/utils.h"
#include "core/timer.h"
#include "game/m3dgame.h"
#include "game/profile.h"
#include <cmath>

namespace m3d
{
    void RadioEngine::Init()
    {
        m_instance = this;
        if (g_Kernel->GetEngineCfg().m_snd_Enable.GetB())
        {
            auto pathToSamples = g_Kernel->GetEngineCfg().m_snd_pathToRadioSamples.GetS();
            CStr err;
            ref_ptr samplesXmlFile = ReadXmlFile(pathToSamples, &err);
            if (samplesXmlFile)
            {
                m_soundIdName.clear();
                m_soundIdSound.clear();

                ref_ptr radioSamples = samplesXmlFile->CreateNode(cmn::XML_NODE_EMPTY, nullptr);
                samplesXmlFile->GetFirstChild(radioSamples, "RadioSamples");

                ref_ptr sample = samplesXmlFile->CreateNode(cmn::XML_NODE_EMPTY, nullptr);
                for (radioSamples->GetFirstChild(sample, "Sample"); !sample->IsEmpty(); sample->GetNextSibling(sample, "Sample"))
                {
                    auto idAttr = sample->GetAttribute("id");
                    if (idAttr)
                    {
                        auto id = atoi(idAttr);
                        if (id != -1)
                        {
                            auto textAttr = sample->GetAttribute("text");
                            m_soundIdName[id] = textAttr;

                            auto soundAttr = sample->GetAttribute("sound");
                            auto snd =
                                Application::g_pApp->m_sound->AddSound(soundAttr, snd::SND_TYPE_2DSOUND, 1, 1, snd::SND_PRIORITY_NORMAL);
                            m_soundIdSound[id] = snd;
                        }
                    }
                }

                auto pathToRadioSounds = g_Kernel->GetEngineCfg().m_snd_pathToRadioSounds.GetS();
                ref_ptr soundsXmlFile = ReadXmlFile(pathToRadioSounds, &err);
                if (soundsXmlFile)
                {
                    m_correctIds.clear();
                    m_groupNameByBelong.clear();
                    ref_ptr radioSounds = soundsXmlFile->CreateNode(cmn::XML_NODE_EMPTY, nullptr);
                    soundsXmlFile->GetFirstChild(radioSounds, "RadioSounds");

                    ref_ptr group = soundsXmlFile->CreateNode(cmn::XML_NODE_EMPTY, nullptr);
                    for (radioSounds->GetFirstChild(group, "Group"); !group->IsEmpty(); group->GetNextSibling(group, "Group"))
                    {
                        auto nameAttr = group->GetAttribute("name");
                        auto belongsAttr = group->GetAttribute("belongs");
                        retruxx::vector<int> belongs;
                        ai::StrToIntVector(belongsAttr, belongs);
                        for (auto belong : belongs)
                        {
                            if (m_groupNameByBelong.find(belong) != m_groupNameByBelong.end())
                            {
                                SYS_ERROR("Error: duplicate belong " + CStr(belong));
                            }
                            m_groupNameByBelong[belong] = nameAttr;
                        }

                        ref_ptr enemy = soundsXmlFile->CreateNode(cmn::XML_NODE_EMPTY, nullptr);
                        group->GetFirstChild(enemy, "Enemy");
                        ref_ptr sound = soundsXmlFile->CreateNode(cmn::XML_NODE_EMPTY, nullptr);
                        for (enemy->GetFirstChild(sound, "Sound"); !sound->IsEmpty(); sound->GetNextSibling(sound, "Sound"))
                        {
                            auto idAttr = sound->GetAttribute("id");
                            auto probAttr = sound->GetAttribute("probability");
                            auto samplesAttr = sound->GetAttribute("samples");

                            SCurId curId;
                            curId.probability = atoi(probAttr);
                            ai::StrToIntVector(samplesAttr, curId.ids);

                            auto fullName = nameAttr + CStr("_enemy_") + idAttr;
                            m_correctIds.insert(retruxx::pair<CStr, SCurId>(fullName, curId));
                        }

                        ref_ptr neutral = soundsXmlFile->CreateNode(cmn::XML_NODE_EMPTY, nullptr);
                        group->GetFirstChild(neutral, "Neutral");
                        ref_ptr sound2 = soundsXmlFile->CreateNode(cmn::XML_NODE_EMPTY, nullptr);
                        for (neutral->GetFirstChild(sound2, "Sound"); !sound2->IsEmpty(); sound2->GetNextSibling(sound2, "Sound"))
                        {
                            auto idAttr = sound2->GetAttribute("id");
                            auto probAttr = sound2->GetAttribute("probability");
                            auto samplesAttr = sound2->GetAttribute("samples");

                            SCurId curId;
                            curId.probability = atoi(probAttr);
                            ai::StrToIntVector(samplesAttr, curId.ids);

                            auto fullName = nameAttr + CStr("_neutral_") + idAttr;
                            m_correctIds.insert(retruxx::pair<CStr, SCurId>(fullName, curId));
                        }
                    }
                    m_soundDeque.clear();
                    m_curChannelId = -1;
                    m_curTextToShow = {};
                }
                else
                {
                    M3D_LOG_ERR("Error reading RadioSounds file '" + CStr(pathToRadioSounds));
                }
            }
            else
            {
                M3D_LOG_ERR("Error reading RadioSamples file '" + CStr(pathToSamples));
            }
        }
    }

    RadioEngine::~RadioEngine()
    {
        if (m_instance == this)
        {
            m_instance = 0;
        }
    }

    void RadioEngine::PlayNextSoundMessage()
    {
        // RVA 0x428470
        if (!m_instance || !g_Kernel->GetEngineCfg().m_snd_Enable.GetB())
        {
            return;
        }

        if (m_curChannelId != -1)
        {
            if (M3D_APP->m_sound->IsChannelPlaying(m_curChannelId))
            {
                return;
            }
            m_curChannelId = -1;
            m_curTextToShow = CStr("");
        }

        if (m_soundDeque.empty())
        {
            return;
        }

        RadioSoundItem const sslFront = m_soundDeque.front();
        m_soundDeque.pop_front();

        // A message type is repeated at most once every 10 seconds, and only
        // during normal gameplay.
        if (g_Kernel->GetTimer().GetCurTime() - m_lastTime[sslFront.type] < 10000 || M3D_APP->GetCurGameMode() != 0)
        {
            return;
        }
        if (m_correctIds.find(sslFront.sound) == m_correctIds.end())
        {
            return;
        }

        int probability = m_correctIds[sslFront.sound].probability;
        auto* profile = M3D_APP->GetProfileManager()->GetCurProfile();
        if (profile)
        {
            AIParam probabilityCoeff;
            profile->GetParam(PP_NUM_RADIO_REPLIES_COEFF, probabilityCoeff);
            float const scaled = probabilityCoeff.GetAsFloat() * static_cast<float>(probability);
            // NOTE: the original converts with a bare fistp, i.e. rounding to
            // nearest rather than truncating.
            probability = static_cast<int>(std::lrint(scaled));
        }

        if (rand() % 100 > probability)
        {
            return;
        }

        auto const numIds = static_cast<unsigned>(m_correctIds[sslFront.sound].ids.size());
        auto const idx = static_cast<unsigned>(rand()) % numIds;
        auto const sampleId = m_correctIds[sslFront.sound].ids[idx];
        auto const sound = m_soundIdSound[sampleId];
        m_curTextToShow = m_soundIdName[sampleId];
        m_curChannelId = M3D_APP->m_sound->PlaySound2D(sound, false);
        M3D_APP->m_pInterfaceManager->AddFadingMsg(m_curTextToShow, {});
        m_lastTime[sslFront.type] = g_Kernel->GetTimer().GetCurTime();
    }

    RadioEngine* RadioEngine::GetInstance()
    {
        return m_instance;
    }

    void RadioEngine::PlaySoundMessage(int belongId, int messageType, CStr const& soundId)
    {
        // RVA 0x4282C0
        if (!g_Kernel->GetEngineCfg().m_snd_Enable.GetB())
        {
            return;
        }

        if (belongId == -1)
        {
            // No speaker: only restart this message type's repeat timer.
            m_lastTime[messageType] = g_Kernel->GetTimer().GetCurTime();
        }
        else if (m_groupNameByBelong.find(belongId) != m_groupNameByBelong.end())
        {
            RadioSoundItem sl;
            sl.sound = m_groupNameByBelong[belongId] + CStr("_") + soundId;
            sl.type = messageType;
            m_soundDeque.push_back(sl);
        }
    }
}  // namespace m3d
