#include "weathermanager.h"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <stdexcept>

#include "config.h"
#include "level.h"
#include "world.h"
#include "core/ini.h"
#include "core/kernel.h"
#include "core/log.h"
#include "core/scoped_ptr.h"
#include "core/timer.h"
#include "file/fileserver.h"
#include "file/filestream.h"
#include "game/m3dgame.h"
#include "math/vector2.h"
#include "server/server.h"

namespace m3d
{
    namespace
    {
        const char* m_globalTimeParamsNames[4] = { "sunriseTime", "dayTime", "sunsetTime", "nightTime" };

        // The sky dome is a square grid of this many vertices per side (CreateSky, SetupSkyParams,
        // RenderWeather).
        int const SKY_GRID = 20;
    }

    int WeatherManager::DeleteWeather(unsigned iWeatherIdx)
    {
        // RVA 0x65F5F0 - editor only; the last remaining weather cannot be deleted.
        if (!m_bEdit)
        {
            return 1;
        }
        if (m_weatherStorage.empty())
        {
            return 0;
        }
        std::size_t count = m_weatherStorage.size();
        if (iWeatherIdx >= count || count <= 1)
        {
            return 0;
        }

        unsigned const newActive = iWeatherIdx ? iWeatherIdx - 1 : 0;
        // NOTE: the weather is only Release()d, never destroyed, and m_curWeatherStorage keeps its
        // pointer. The next weather is activated while the deleted one is still in the storage.
        m_weatherStorage[iWeatherIdx]->Release();
        SetActiveWeather(newActive);
        m_weatherStorage.erase(m_weatherStorage.begin() + iWeatherIdx);
        return 1;
    }

    int WeatherManager::CreateSky()
    {
        // RVA 0x65E740 - the sky dome is a 20x20 vertex grid (filled in by SetupSkyParams), drawn as
        // 19x19 quads of two triangles each: 19 * 19 * 6 = 2166 indices.
        m_vbSky = M3D_APP->m_renderer->AddVb(rend::VERTEX_XYZCT2, SKY_GRID * SKY_GRID, "Sky", 0);
        m_ibSky = M3D_APP->m_renderer->AddIb((SKY_GRID - 1) * (SKY_GRID - 1) * 6, false);
        auto mem = static_cast<WORD*>(M3D_APP->m_renderer->LockIb(m_ibSky, 0, 0, 0));
        for (int row = 0; row < SKY_GRID - 1; ++row)
        {
            for (int col = 0; col < SKY_GRID - 1; ++col)
            {
                WORD const base = static_cast<WORD>(row * SKY_GRID + col);
                mem[0] = base;
                mem[1] = base + SKY_GRID + 1;
                mem[2] = base + SKY_GRID;
                mem[3] = base;
                mem[4] = base + 1;
                mem[5] = base + SKY_GRID + 1;
                mem += 6;
            }
        }
        M3D_APP->m_renderer->UnlockIb(m_ibSky);
        return 1;
    }

    int WeatherManager::WriteToXmlFile(char const* name)
    {
        // RVA 0x65EDE0 - the mirror of the Weather.xml half of ReadFromXmlFile. The per-level
        // WeatherDetail.xml data (WriteDetailToXmlNode) is not written here.
        ref_ptr<cmn::XmlFile> xmlFile = g_Kernel->CreateXmlFile();
        ref_ptr<cmn::XmlNode> rootNode = xmlFile->CreateNode(cmn::XML_NODE_ELEMENT, "Weather");
        ref_ptr<cmn::XmlNode> weatherCommonNode = xmlFile->CreateNode(cmn::XML_NODE_ELEMENT, "WeatherCommonSets");

        for (unsigned i = 0; i < GTP_NUM_PARAMS; ++i)
        {
            // The times are kept in seconds and written in hours.
            weatherCommonNode->SetAttribute(
                m_globalTimeParamsNames[i], CStr(GetGlobalTimeParam(i) * 0.00027777778f).c_str());
        }
        weatherCommonNode->SetAttribute("starsTexture", m_StarsTextureName.c_str());
        rootNode->AddChild(weatherCommonNode);

        for (unsigned i = 0; i < m_weatherStorage.size(); ++i)
        {
            ref_ptr<cmn::XmlNode> itemNode = xmlFile->CreateNode(cmn::XML_NODE_ELEMENT, "WeatherItem");
            itemNode->SetAttribute("class", m_weatherStorage[i]->GetClassNameA());
            m_weatherStorage[i]->WriteToXmlNode(xmlFile, itemNode);
            rootNode->AddChild(itemNode);
        }
        xmlFile->AddChild(rootNode);

        scoped_ptr fileStream = g_Kernel->GetFileServer().CreateFileStream();
        if (fileStream->Open(name, fs::IStream::OPEN_WRITE))
        {
            xmlFile->Write(*fileStream);
            fileStream->Close();
        }
        else
        {
            M3D_LOG_INFO("Could not save Weather.xml into file " + CStr(name));
        }
        // NOTE: returns 1 even when the file could not be opened.
        return 1;
    }

    void WeatherManager::ChangeCloudsTexture()
    {
        M3D_RENDERER->ReleaseTexture(m_cloudTextureHandle);

        CStr pathToTex = M3D_ENGINE_CFG.m_weather_PathToTextures.GetS();
        pathToTex += m_currentWeather->m_cloudsTextureName[m_curDayTime];

        m_cloudTextureHandle = M3D_RENDERER->AddTexture(pathToTex, 0);
        M3D_RENDERER->SetTextureParameter(m_cloudTextureHandle, rend::TM_WRAP_S, 1);
        M3D_RENDERER->SetTextureParameter(m_cloudTextureHandle, rend::TM_WRAP_T, 1);
    }

    float WeatherManager::SetGlobalTimeParam(unsigned iParamIdx, float fValue)
    {
        // RVA 0x65D5B0 - returns the previous value, or 0 for an index out of range.
        float const oldValue = GetGlobalTimeParam(iParamIdx);
        if (iParamIdx < GTP_NUM_PARAMS)
        {
            m_globalTimeParams[iParamIdx] = fValue;
        }
        return oldValue;
    }

    int WeatherManager::SetupSkyParams()
    {
        // RVA 0x65DA30 - refills the 20x20 sky dome (see CreateSky): a paraboloid over the level whose
        // height drops with the squared distance from the centre over the atmosphere radius. Stage 0
        // carries the scrolling clouds, stage 1 the same texture unscrolled, and the alpha fades out
        // from a quarter of the lerp radius to the full radius.
        if (m_weatherStorage.empty())
        {
            return 0;
        }
        M3D_APP->SetFrameClearColor(GetWeatherColor(CI_FOG));

        float const VISCELL_EDGE_LENGTH = 128.0f;
        static rend::VertexXYZCT2 tmp[SKY_GRID * SKY_GRID];

        float const w = static_cast<float>(m_owner->m_level->land_size + 2) * VISCELL_EDGE_LENGTH;
        float const invAtmoRadius = 1.0f / M3D_ENGINE_CFG.m_weather_AtmoRadius.GetF();
        float const domeHeight = w / (m_owner->m_level->m_skyDomeDivider * m_currentWeather->m_weatherSkyDomeFactor);
        float const c2 = w * 0.05f;
        float const lerpHalf = w * 0.3535f;
        float const fadeStart = lerpHalf * 0.25f;
        float const fadeLength = lerpHalf * 0.75f;
        CVector2 const center(w * 0.5f, w * 0.5f);

        rend::VertexXYZCT2* v = tmp;
        for (int row = 0; row < SKY_GRID; ++row)
        {
            float const tv = static_cast<float>(row) * 0.25f;
            for (int col = 0; col < SKY_GRID; ++col, ++v)
            {
                float const tu = static_cast<float>(col) * 0.25f;
                CVector2 const point(static_cast<float>(col) * c2, static_cast<float>(row) * c2);
                float const dx = point.x - center.x;
                float const dz = point.y - center.y;
                float const len = std::sqrt(dx * dx + dz * dz);
                float alpha = 255.0f;
                if (len > fadeStart)
                {
                    float const t = std::clamp((len - fadeStart) / fadeLength, 0.0f, 1.0f);
                    alpha = 255.0f - t * 255.0f;
                }

                v->x = dx;
                v->y = domeHeight - (dx * dx + dz * dz) * invAtmoRadius;
                v->z = dz;
                // Rounded to nearest (a bare fistp), not truncated.
                v->c = static_cast<uint32_t>(lrintf(alpha)) << 24;
                v->tu0 = tu + m_cloudsOffset;
                v->tv0 = tv + m_cloudsOffset;
                v->tu1 = tu;
                v->tv1 = tv;
            }
        }

        m_cloudsOffset += static_cast<float>(M3D_KERNEL->GetTimer().GetLastFrameTime()) * 0.001f
            * m_currentWeather->m_cloudsSpeed[m_curDayTime];

        memcpy(M3D_RENDERER->LockVb(m_vbSky, 0, 0, 0), tmp, sizeof(tmp));
        M3D_RENDERER->UnlockVb(m_vbSky);
        return 1;
    }

    float WeatherManager::GetShadowTransparencyFromWeather() const
    {
        return m_currentWeather->m_shadowTransparency[m_curDayTime];
    }

    GlobalTimeParams WeatherManager::GetCurrentDayTime() const
    {
        return this->m_curDayTime;
    }

    Weather const* WeatherManager::GetActiveWeather() const
    {
        return m_currentWeather;
    }

    unsigned WeatherManager::GetWeatherColor(ColorItems ci) const
    {
        // RVA 0x65D6F0. Builds 0xFFRRGGBB by truncating the three 0..255
        // float channels and folding them in through a sign-extended
        // 0xFFFFFF00 seed: (x|0xFFFFFF00)<<8 leaves 0xFFFFxx00, then |y and
        // <<8 gives 0xFFxxyy00, then |z. The channels are OR-ed, not masked,
        // so a component outside 0..255 bleeds into the neighbouring byte.
        return (int)this->m_currentWeather->m_currentColors[ci].z | (((int)this->m_currentWeather->m_currentColors[ci].y | (((int)this->m_currentWeather->m_currentColors[ci].x | 0xFFFFFF00) << 8)) << 8);
    }

    void WeatherManager::ChangeLightmapTexture()
    {
        auto path = m_owner->m_level->GetFullPathNameA(m_currentWeather->m_lightmapTextureName[m_curDayTime]);
        m_owner->GetLandscape().ReloadLightmapTexture(path);
    }

    float WeatherManager::GetGlobalTimeParam(unsigned iParamIdx) const
    {
        // RVA 0x65D580
        if (iParamIdx >= 4)
            return 0.0;
        return m_globalTimeParams[iParamIdx];
    }

    int WeatherManager::LoadWeatherStateFromXMLNode(cmn::XmlFile*, cmn::XmlNode* xmlNode)
    {
        // RVA 0x65FBC0 - restores the state SaveWeatherStateToXMLNode wrote into a save game.
        CStr newWeatherName;
        m3d::SafeStrAttrib(newWeatherName, xmlNode, "WeatherName");
        SetActiveWeatherByName(newWeatherName);

        int dayTime = GTP_SUNRISE_TIME;
        m3d::SafeIntAttrib(dayTime, xmlNode, "DayTime");

        GlobalTimeParams const oldDayTime = m_curDayTime;
        m_curDayTime = static_cast<GlobalTimeParams>(dayTime);
        for (int i = CI_SKY; i < CI_NUM_COLORITEMS; ++i)
        {
            m_currentWeather->UpdateColors(static_cast<ColorItems>(i), static_cast<ColorTypes>(m_curDayTime));
        }

        m_owner->UpdateSun();
        ai::UpdateLights();

        m_owner->m_isWeatherActual = m_owner->m_isWeatherActual && oldDayTime == m_curDayTime;
        if (!m_owner->m_isWeatherActual)
        {
            ChangeLightmapTexture();
            ChangeCloudsTexture();
        }

        M3D_APP->ReloadPostEffects();
        M3D_APP->AddPostEffect(m_currentWeather->m_PostEffectName[m_curDayTime], 0.0f);
        return 1;
    }

    int WeatherManager::UpdateDayTime()
    {
        if (m_weatherStorage.empty())
            return 0;

        auto gameTime = ai::theObjects->GetGameTime().GetAsIdList();
        auto secs = gameTime[2] + 60 * gameTime[1] + 3600 * gameTime[0];
        m_currentWeather->Update(1.0, secs);

        auto oldDayTime = this->m_curDayTime;
        if (secs < this->m_globalTimeParams[0] || this->m_globalTimeParams[1] <= secs)
        {
            if (secs < this->m_globalTimeParams[1] || this->m_globalTimeParams[2] <= secs)
            {
                if (secs < this->m_globalTimeParams[2] || this->m_globalTimeParams[3] <= secs)
                    this->m_curDayTime = GTP_NIGHT_TIME;
                else
                    this->m_curDayTime = GTP_SUNSET_TIME;
            }
            else
            {
                this->m_curDayTime = GTP_DAY_TIME;
            }
        }
        else
        {
            this->m_curDayTime = GTP_SUNRISE_TIME;
        }

        for (int i = CI_SKY; i < CI_NUM_COLORITEMS; ++i)
            this->m_currentWeather->UpdateColors((ColorItems)i, (ColorTypes)this->m_curDayTime);

        m_owner->UpdateSun();
        ai::UpdateLights();

        if (this->m_curDayTime != oldDayTime)
            this->m_owner->m_isWeatherActual = 0;

        if (!this->m_owner->m_isWeatherActual)
        {
            m3d::WeatherManager::ChangeLightmapTexture();
            m3d::WeatherManager::ChangeCloudsTexture();
        }
        if (!this->m_bEdit)
        {
        
            M3D_APP->ReloadPostEffects();
            M3D_APP->AddPostEffect(this->m_currentWeather->m_PostEffectName[this->m_curDayTime], 0.0);
        }

        return 1;
    }

    float WeatherManager::GetFogReduceFactorFromWeather() const
    {
        return m_currentWeather->m_reduceDistFactor;
    }

    void WeatherManager::SetActiveWeather(unsigned cur)
    {
        if (m_currentWeather)
            m_currentWeather->TurnOffEffects();
        if (this->m_bEdit)
        {
            this->m_owner->m_isWeatherActual = (m_currentWeather == this->m_weatherStorage[cur]);
            this->m_currentWeather = this->m_weatherStorage[cur];
        }
        else
        {
            this->m_owner->m_isWeatherActual = (this->m_currentWeather == this->m_curWeatherStorage[cur]);
            this->m_currentWeather = this->m_curWeatherStorage[cur];
        }
        this->m_currentWeather->SetUp();
    }

    int WeatherManager::RenderWeatherParticles()
    {
        return this->m_currentWeather->Render();
    }

    int WeatherManager::RenderWeather(Landscape::LandRenderMode rendMode)
    {
        // RVA 0x65DE10 - the dome is drawn around the camera: clouds from UV set 0 modulated twice by
        // the sky colour, then at night in the direct pass the stars from UV set 1 over them.
        if (m_weatherStorage.empty())
        {
            return 0;
        }

        M3D_RENDERER->SetToStream0(m_vbSky);
        M3D_RENDERER->PushCull(rend::M3DCULL_NONE);
        M3D_RENDERER->PushLighting(0);
        M3D_RENDERER->PushBlend(rend::BM_ALPHA);
        M3D_RENDERER->SetAlphaTest(0);

        CVector const org = M3D_RENDERER->MatGetOrgInv();
        M3D_RENDERER->MatPushWorld();
        CMatrix translation;
        translation.translation(org);
        CMatrix rot;
        // NOTE: the original scales a timer reading by zero here, so the dome never turns.
        rot.rotZ(0.0f);
        rot = rot * translation;
        M3D_RENDERER->MatSetWorld(rot);

        unsigned const SKY_VERTICES = SKY_GRID * SKY_GRID;
        unsigned const SKY_TRIANGLES = (SKY_GRID - 1) * (SKY_GRID - 1) * 2;
        M3D_RENDERER->SetIndices(m_ibSky, 0);
        M3D_RENDERER->TgSetTcSource(0, rend::TC_FROM_VERTEX, 0);
        M3D_RENDERER->SetStageState(0, rend::BM_COLOR, rend::TS_TEX_MODULATE2X_TFAC);
        M3D_RENDERER->SetStageState(0, rend::BM_ALPHA, rend::TS_DIFF_MODULATE_TFAC);
        M3D_RENDERER->SetStageState(1, rend::BM_COLOR, rend::TS_NONE);
        M3D_RENDERER->SetStageState(1, rend::BM_ALPHA, rend::TS_NONE);
        M3D_RENDERER->SetTexture(0, m_cloudTextureHandle, -1.0);
        M3D_RENDERER->SetTFactor(GetWeatherColor(CI_SKY), false);
        M3D_RENDERER->DrawIndexedPrimitive(rend::M3DPT_TRIANGLELIST, 0, SKY_VERTICES, 0, SKY_TRIANGLES);

        if (m_curDayTime == GTP_NIGHT_TIME && rendMode == Landscape::LRM_DIRECT)
        {
            M3D_RENDERER->SetTFactor(0xFFFFFFFFu, false);
            M3D_RENDERER->SetStageState(0, rend::BM_COLOR, rend::TS_TEXTURE);
            M3D_RENDERER->SetStageState(0, rend::BM_ALPHA, rend::TS_DIFF_MODULATE_TFAC);
            M3D_RENDERER->SetTexture(0, m_starsTexture, -1.0);
            M3D_RENDERER->TgSetTcSource(0, rend::TC_FROM_VERTEX, 1);
            M3D_RENDERER->DrawIndexedPrimitive(rend::M3DPT_TRIANGLELIST, 0, SKY_VERTICES, 0, SKY_TRIANGLES);
        }

        M3D_RENDERER->MatPopWorld();
        M3D_RENDERER->PopCull();
        M3D_RENDERER->PopLighting();
        M3D_RENDERER->PopBlend();
        M3D_RENDERER->TgDisable(0);
        M3D_RENDERER->TgDisable(1);
        return 1;
    }

    void WeatherManager::SetOwner(CWorld* world)
    {
        m_owner = world;
    }

    int WeatherManager::SaveWeatherStateToXMLNode(cmn::XmlFile*, cmn::XmlNode* xmlNode)
    {
        // RVA 0x65D680
        xmlNode->SetAttribute("WeatherName", m_currentWeather->m_Name.c_str());
        xmlNode->SetAttribute("DayTime", CStr(static_cast<int>(m_curDayTime)).c_str());
        return 1;
    }

    bool WeatherManager::GetShadowVisibilityFromWeather() const
    {
        return this->m_currentWeather->m_shadowVisibility[this->m_curDayTime];
    }

    int WeatherManager::UpdateWheatherParticles()
    {
        auto time = ai::theObjects->GetGameTime().GetAsIdList();
        m_currentWeather->Update(1.0, time[2] + 60 * time[1] - (int)(float)((float)time[0] * -3600.0));
        for (int i = CI_SKY; i < CI_NUM_COLORITEMS; ++i)
            this->m_currentWeather->UpdateColors((ColorItems)i, (m3d::ColorTypes)this->m_curDayTime);
        return 1;
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

        std::size_t modulo = 0u;
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
        M3D_RENDERER->ReleaseVb(m_vbSky);
        M3D_RENDERER->ReleaseIb(m_ibSky);

        for (auto& weather : m_weatherStorage)
        {
            weather->Release();
            delete weather;
        }
        m_weatherStorage.clear();
        m_curWeatherStorage.clear();
        m_currentWeather = nullptr;

        M3D_RENDERER->ReleaseTexture(m_cloudTextureHandle);
        M3D_RENDERER->ReleaseTexture(m_starsTexture);
    }

    void WeatherManager::AddWeather(CStr const& Name, CStr const& ClassName)
    {
        // RVA 0x65FFA0 - editor only.
        if (m_bEdit)
        {
            // NOTE: an unknown class name makes New return null, which is dereferenced unchecked.
            Weather* weather = static_cast<Weather*>(M3D_KERNEL->New(ClassName.c_str()));
            weather->DefaultInitialize();
            weather->m_Name = Name;
            m_weatherStorage.push_back(weather);
        }
    }

    std::size_t WeatherManager::GetNumWeathers() const
    {
        if (m_bEdit)
        {
            return m_weatherStorage.size();
        }
        return m_curWeatherStorage.size();
    }

    char const* WeatherManager::GetGlobalTimeParamName(unsigned i) const
    {
        // RVA 0x65D740
        return m_globalTimeParamsNames[i];
    }

    void WeatherManager::SetActiveWeatherByName(CStr const& name)
    {
        // RVA 0x65FA00
        Weather* weather = GetWeatherByName(name);
        if (weather)
        {
            if (m_currentWeather)
            {
                m_currentWeather->TurnOffEffects();
            }
            m_owner->m_isWeatherActual = m_currentWeather == weather;
            m_currentWeather = weather;
            weather->SetUp();
        }
        else
        {
            // NOTE: GetWeatherByName has already logged the same message.
            M3D_LOG_INFO("Weather with name '" + name + CStr("' doesn't exists!!!"));
        }
    }

    void WeatherManager::ChangeStarsTexture(CStr& Name)
    {
        // RVA 0x65E9F0
        if (Name.c_str() && strlen(Name.c_str()))
        {
            M3D_RENDERER->ReleaseTexture(m_starsTexture);
            m_StarsTextureName = Name;

            CStr pathToTex = M3D_ENGINE_CFG.m_weather_PathToTextures.GetS();
            pathToTex += Name;

            m_starsTexture = M3D_RENDERER->AddTexture(pathToTex, 0);
            M3D_RENDERER->SetTextureParameter(m_starsTexture, rend::TM_WRAP_S, 1);
            M3D_RENDERER->SetTextureParameter(m_starsTexture, rend::TM_WRAP_T, 1);
        }
    }

    Weather* WeatherManager::GetWeather(unsigned N)
    {
        // RVA 0x65ED40
        if (m_bEdit)
        {
            return m_weatherStorage[N];
        }
        return m_curWeatherStorage[N];
    }

    Weather* WeatherManager::GetWeatherByName(CStr const& name)
    {
        // RVA 0x65F830
        // NOTE: in edit mode the lookup overwrites the level's weather list with the full storage.
        if (m_bEdit)
        {
            m_curWeatherStorage = m_weatherStorage;
        }

        for (Weather* weather : m_curWeatherStorage)
        {
            if (name == weather->m_Name)
            {
                return weather;
            }
        }

        M3D_LOG_INFO("Weather with name '" + name + CStr("' doesn't exists!!!"));
        return nullptr;
    }
}
