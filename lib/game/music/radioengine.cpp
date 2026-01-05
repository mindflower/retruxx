#include "radioengine.h"
#include <stdexcept>

#include "config.h"
#include "m3dapp.h"
#include "core/kernel.h"
#include "core/log.h"
#include <core/ini.h>
#include "core/ref_ptr.h"
#include "server/utils.h"

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
        // TODO: implmement RadioEngine::PlayNextSoundMessage
        // RETRUXX_NOT_IMPLEMENTED;
    }

    RadioEngine* RadioEngine::GetInstance()
    {
        return m_instance;
    }

    void RadioEngine::PlaySoundMessage(int, int, CStr const&)
    {
        // TODO: implmement RadioEngine::PlaySoundMessage
        // RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace m3d
