#include "weathermanager.h"
#include <stdexcept>

#include "config.h"
#include "level.h"
#include "world.h"
#include "core/ini.h"
#include "core/kernel.h"
#include "core/log.h"
#include "core/scoped_ptr.h"
#include "file/fileserver.h"
#include "file/filestream.h"
#include "game/m3dgame.h"

namespace m3d
{
    int WeatherManager::DeleteWeather(unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    int WeatherManager::CreateSky()
    {
        m_vbSky = M3D_APP->m_renderer->AddVb(rend::VERTEX_XYZCT2, 400, "Sky", 0);
        m_ibSky = M3D_APP->m_renderer->AddIb(2166, false);
        auto mem = static_cast<WORD*>(M3D_APP->m_renderer->LockIb(m_ibSky, 0, 0, 0));
        //TODO: check this and refactor
        auto v3 = 0;
        auto v7 = 19;
        do
        {
            auto v4 = v3 + 21;
            auto v5 = 19;
            do
            {
                *mem = v3;
                mem[1] = v4;
                mem[3] = v3;
                mem[5] = v4;
                mem[2] = v4 - 20 + 19;
                mem[4] = v4 - 20;
                mem += 6;
                ++v3;
                ++v4;
                --v5;
            } while (v5);
            ++v3;
            --v7;
        } while (v7);
        M3D_APP->m_renderer->UnlockIb(m_ibSky);
        return 1;
    }

    int WeatherManager::WriteToXmlFile(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    void WeatherManager::ChangeCloudsTexture()
    {
        throw std::logic_error("Not implemented");
    }

    float WeatherManager::SetGlobalTimeParam(unsigned, float)
    {
        throw std::logic_error("Not implemented");
    }

    int WeatherManager::SetupSkyParams()
    {
        throw std::logic_error("Not implemented");
    }

    float WeatherManager::GetShadowTransparencyFromWeather() const
    {
        throw std::logic_error("Not implemented");
    }

    GlobalTimeParams WeatherManager::GetCurrentDayTime() const
    {
        throw std::logic_error("Not implemented");
    }

    Weather const* WeatherManager::GetActiveWeather() const
    {
        throw std::logic_error("Not implemented");
    }

    unsigned WeatherManager::GetWeatherColor(ColorItems) const
    {
        throw std::logic_error("Not implemented");
    }

    void WeatherManager::ChangeLightmapTexture()
    {
        throw std::logic_error("Not implemented");
    }

    float WeatherManager::GetGlobalTimeParam(unsigned) const
    {
        throw std::logic_error("Not implemented");
    }

    int WeatherManager::LoadWeatherStateFromXMLNode(cmn::XmlFile*, cmn::XmlNode*)
    {
        throw std::logic_error("Not implemented");
    }

    int WeatherManager::UpdateDayTime()
    {
        throw retruxx::logic_error("Not implemented");
        return 1;
    }

    float WeatherManager::GetFogReduceFactorFromWeather() const
    {
        throw retruxx::logic_error("Not implemented");
        //return m_currentWeather->m_reduceDistFactor;
        return 0.0;
    }

    void WeatherManager::SetActiveWeather(unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    int WeatherManager::RenderWeatherParticles()
    {
        throw retruxx::logic_error("Not implemented");
        return 1;
    }

    int WeatherManager::RenderWeather(Landscape::LandRenderMode)
    {
        throw std::logic_error("Not implemented");
    }

    void WeatherManager::SetOwner(CWorld* world)
    {
        m_owner = world;
    }

    int WeatherManager::SaveWeatherStateToXMLNode(cmn::XmlFile*, cmn::XmlNode*)
    {
        throw std::logic_error("Not implemented");
    }

    bool WeatherManager::GetShadowVisibilityFromWeather() const
    {
        return this->m_currentWeather->m_shadowVisibility[this->m_curDayTime];
    }

    int WeatherManager::UpdateWheatherParticles()
    {
        throw retruxx::logic_error("Not implemented");
        //auto idList = ai::theObjects->GetGameTime().GetAsIdList();
        //m_currentWeather->Update(1.0, )
        //throw std::logic_error("Not implemented");
        return 1;
    }

    namespace
    {
        const char* m_globalTimeParamsNames[4] = { "sunriseTime", "dayTime", "sunsetTime", "nightTime" };
    }

    int WeatherManager::ReadFromXmlFile(char const* name)
    {
        ref_ptr<m3d::cmn::XmlFile> rootXmlFile;
        ref_ptr<m3d::cmn::XmlNode> rootNode;
        {
            scoped_ptr fileStream = g_Kernel->GetFileServer().CreateFileStream();
            if (!fileStream->Open(name, fs::IStream::OPEN_READ))
            {
                M3D_LOG_INFO("Error:WeatherManager can't read file: " + CStr(name));
                return 0;
            }

            rootXmlFile = g_Kernel->CreateXmlFile();
            if (!rootXmlFile->Read(*fileStream))
            {
                M3D_LOG_INFO("WeatherManager: error in parsing: " + CStr(name) + " (" + CStr(rootXmlFile->GetError()) + ")");
                return 0;
            }
            fileStream->Close();

            rootNode = rootXmlFile->CreateNode();
            rootXmlFile->GetFirstChild(rootNode, "Weather");
            if (rootNode->IsEmpty())
            {
                M3D_LOG_INFO("Error: WeatherManager can't find Root node in " + CStr(name));
                return 0;
            }

            auto weatherCommonSetsNode = rootXmlFile->CreateNode();
            rootNode->GetFirstChild(weatherCommonSetsNode, "WeatherCommonSets");
            if (weatherCommonSetsNode->IsEmpty())
            {
                M3D_LOG_INFO("Error: WeatherManager can't find WeatherCommonSets node in " + CStr(name));
                return 0;
            }

            for (int i = 0; i < 4; ++i)
            {
                float time = 0.0;
                m3d::SafeFloatAttrib(time, weatherCommonSetsNode, m_globalTimeParamsNames[i]);
                m_globalTimeParams[i] = time * 3600;
            }

            m3d::SafeStrAttrib(m_StarsTextureName, weatherCommonSetsNode, "starsTexture");
            m_starsTexture = M3D_RENDERER->AddTexture(M3D_KERNEL->GetEngineCfg().m_weather_PathToTextures.GetS() + m_StarsTextureName, 0);
        }

        {

            const auto weatherDetailName = m_owner->m_level->GetFullPathNameA(m_owner->m_level->m_weatherDetailName);
            scoped_ptr fileStream = g_Kernel->GetFileServer().CreateFileStream();
            if (!fileStream->Open(weatherDetailName.c_str(), fs::IStream::OPEN_READ))
            {
                M3D_LOG_INFO("Error:WeatherManager can't read file: " + weatherDetailName);
                return 0;
            }

            ref_ptr xmlFile = g_Kernel->CreateXmlFile();
            if (!xmlFile->Read(*fileStream))
            {
                M3D_LOG_INFO("WeatherManager: error in parsing: " + weatherDetailName + " (" + CStr(xmlFile->GetError()) + ")");
                return 0;
            }
            fileStream->Close();

            auto weatherDetNode = xmlFile->CreateNode();
            xmlFile->GetFirstChild(weatherDetNode, "Weather");
            if (weatherDetNode->IsEmpty())
            {
                M3D_LOG_INFO("Error: WeatherManager can't find Root node in " + CStr(name));
                return 0;
            }

            retruxx::set<CStr> weatherNames;
            auto weatherDetItemNode = xmlFile->CreateNode();
            for (weatherDetNode->GetFirstChild(weatherDetItemNode, "WeatherItem"); !weatherDetItemNode->IsEmpty(); weatherDetItemNode->GetNextSibling(weatherDetItemNode, "WeatherItem"))
            {
                weatherNames.insert(weatherDetItemNode->GetAttribute("name"));
            }


            auto itemNode = rootXmlFile->CreateNode();
            for (rootNode->GetFirstChild(itemNode, "WeatherItem"); !itemNode->IsEmpty(); itemNode->GetNextSibling(itemNode, "WeatherItem"))
            {
                CStr name;
                m3d::SafeStrAttrib(name, itemNode, "name");

                CStr cls;
                m3d::SafeStrAttrib(cls, itemNode, "class");

                Weather* weather = (Weather*)M3D_KERNEL->New(cls.c_str());
                weather->DefaultInitialize();
                if (weather->ReadFromXmlNode(rootXmlFile, itemNode))
                {
                    if (weatherNames.find(name) != weatherNames.end())
                    {
                        m_weatherStorage.push_back(weather);
                        for (weatherDetNode->GetFirstChild(weatherDetItemNode, "WeatherItem"); !weatherDetItemNode->IsEmpty(); weatherDetItemNode->GetNextSibling(weatherDetItemNode, "WeatherItem"))
                        {
                            if (weather->m_Name == weatherDetItemNode->GetAttribute("name"))
                            {
                                weather->ReadDetailFromXmlNode(xmlFile, weatherDetItemNode);
                                break;
                            }
                        }
                    }
                }
            }
        }

        if (m_weatherStorage.empty())
        {
            M3D_LOG_INFO("There are no active weather!!!! Check WeatherDetail.xml!!!");
        }

        if (m_curWeatherStorage.empty())
        {
            m_curWeatherStorage = m_weatherStorage;
        }

        auto modulo = 0;
        if (!m_bEdit)
        {
            modulo = m_curWeatherStorage.size();
        }
        else
        {
            modulo = m_weatherStorage.size();
        }

        SetActiveWeather(rand() % modulo);

        return 1;
    }

    WeatherManager::WeatherManager()
    {
        this->m_owner = 0;
        this->m_currentWeather = 0;
        this->m_cloudsOffset = 0.0;
        this->m_bEdit = 0;
    }

    void WeatherManager::DoneSky()
    {
        throw std::logic_error("Not implemented");
    }

    void WeatherManager::AddWeather(CStr const&, CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    unsigned WeatherManager::GetNumWeathers() const
    {
        throw std::logic_error("Not implemented");
    }

    char const* WeatherManager::GetGlobalTimeParamName(unsigned) const
    {
        throw std::logic_error("Not implemented");
    }

    void WeatherManager::SetActiveWeatherByName(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    void WeatherManager::ChangeStarsTexture(CStr&)
    {
        throw std::logic_error("Not implemented");
    }

    Weather* WeatherManager::GetWeather(unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    Weather* WeatherManager::GetWeatherByName(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }
}
