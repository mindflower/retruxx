#include "weathermanager.h"
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
#include "server/server.h"

namespace m3d
{
    int WeatherManager::DeleteWeather(unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
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

    float WeatherManager::SetGlobalTimeParam(unsigned, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::rend::VertexXYZCT2 tmp_0[400];

    int WeatherManager::SetupSkyParams()
    {
        if (m_weatherStorage.empty())
        {
            return 0;
        }

        M3D_APP->SetFrameClearColor(GetWeatherColor(CI_FOG));

        auto v4 = 0;

        float VISCELL_EDGE_LENGTH_9 = 128.0;
        auto atmoRadius = M3D_ENGINE_CFG.m_weather_AtmoRadius.GetF();

        auto v22 = (this->m_owner->m_level->land_size + 2) * VISCELL_EDGE_LENGTH_9;

        auto v7 = 0.0;
        auto v8 = 1.0 / atmoRadius;
        auto v9 = v22 * 0.050000001;
        auto v26 = v22 * 0.35350001;
        auto v10 = v22 / (float)(this->m_owner->m_level->m_skyDomeDivider * this->m_currentWeather->m_weatherSkyDomeFactor);
        auto w = v22 * 0.050000001;
        auto v11 = 0.0;
        auto len = 0.0;
        auto v23 = (float)(v22 * 0.35350001) * 0.25;
        auto center_4 = v10;

        auto v27 = v22 / 2;
        auto v29 = v22 / 2;
        auto v30 = v22 / 2;
        while (1)
        {
            auto v12 = len * v9;
            auto v13 = 0.0;
            auto retaddr = len * v9;
            auto v25 = (float)((float)(len * v9) - v30) * (float)((float)(len * v9) - v30);
            auto p_tu0 = &tmp_0[v4].tu0;
            while (1)
            {
                auto v15 = v13 * v9;
                auto lerpHalf = 255.0;
                auto alpha = sqrt((float)((float)(v13 * v9) - v29) * (float)((float)(v13 * v9) - v29) + v25);
                if (alpha > (double)v23)
                {
                    auto v16 = (float)(alpha - v23) / (float)(v26 * 0.75);
                    auto v17 = 0.0;
                    if (v16 < 0.0 || (v17 = 1.0, v16 > 1.0))
                        v16 = v17;
                    v12 = retaddr;
                    lerpHalf = 255.0 - (float)(v16 * 255.0);
                }
                v13 = v13 + 1.0;
                *((int*)p_tu0 - 1) = (int)lerpHalf << 24;
                *p_tu0 = v7 + this->m_cloudsOffset;
                p_tu0[1] = v11 + this->m_cloudsOffset;
                *(p_tu0 - 2) = v12 - *(float*)&v27;
                p_tu0[2] = v7;
                v7 = v7 + 0.25;
                *(p_tu0 - 3) = center_4
                    - (float)((float)((float)((float)(v12 - *(float*)&v27) * (float)(v12 - *(float*)&v27)) * v8)
                              + (float)((float)((float)(v15 - *(float*)&v27) * (float)(v15 - *(float*)&v27)) * v8));
                p_tu0[3] = v11;
                *(p_tu0 - 4) = v15 - *(float*)&v27;
                ++v4;
                p_tu0 += 8;
                if (v13 >= 19.9)
                    break;
                v9 = w;
            }
            auto v18 = (float)(len + 1.0) >= 19.9;
            v11 = v11 + 0.25;
            len = len + 1.0;
            if (v18)
                break;
            v7 = 0.0;
            v9 = w;
        }

        this->m_cloudsOffset = M3D_KERNEL->GetTimer().GetLastFrameTime()
            * 0.001
            * this->m_currentWeather->m_cloudsSpeed[this->m_curDayTime]
            + this->m_cloudsOffset;

        memcpy(
            M3D_RENDERER->LockVb(this->m_vbSky, 0, 0, 0),
            tmp_0,
            0x3200u);
        M3D_RENDERER->UnlockVb(this->m_vbSky);
        return 1;
    }

    float WeatherManager::GetShadowTransparencyFromWeather() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    GlobalTimeParams WeatherManager::GetCurrentDayTime() const
    {
        return this->m_curDayTime;
    }

    Weather const* WeatherManager::GetActiveWeather() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned WeatherManager::GetWeatherColor(ColorItems ci) const
    {
        return (int)this->m_currentWeather->m_currentColors[ci].z | (((int)this->m_currentWeather->m_currentColors[ci].y | (((int)this->m_currentWeather->m_currentColors[ci].x | 0xFFFFFF00) << 8)) << 8);
    }

    void WeatherManager::ChangeLightmapTexture()
    {
        auto path = m_owner->m_level->GetFullPathNameA(m_currentWeather->m_lightmapTextureName[m_curDayTime]);
        m_owner->GetLandscape().ReloadLightmapTexture(path);
    }

    float WeatherManager::GetGlobalTimeParam(unsigned) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int WeatherManager::LoadWeatherStateFromXMLNode(cmn::XmlFile*, cmn::XmlNode*)
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        if (m_weatherStorage.empty())
        {
            return 0;
        }

        M3D_RENDERER->SetToStream0(this->m_vbSky);
        M3D_RENDERER->PushCull(rend::M3DCULL_NONE);
        M3D_RENDERER->PushLighting(0);
        M3D_RENDERER->PushBlend(rend::BM_ALPHA);
        M3D_RENDERER->SetAlphaTest(0);

        auto orgInv = M3D_RENDERER->MatGetOrgInv();
        M3D_RENDERER->MatPushWorld();

        CMatrix translation;
        translation.zero();

        translation.m[3][0] = orgInv.x;
        translation.m[3][1] = orgInv.y;
        translation.m[3][2] = orgInv.z;

        CMatrix rot;
        rot.zero();

        CMatrix vv;
        auto v6 = sin(0.0);
        auto v7 = cos(0.0);
        vv._11 = (float)((float)((float)(translation._21 * v6) + (float)(translation._31 * rot._13))
                         + (float)(rot._14 * translation._41))
            + v7;
        vv._12 = (float)((float)((float)(translation._12 * v7) + (float)(translation._32 * rot._13))
                         + (float)(rot._14 * translation._42))
            + v6;
        vv._14 = translation._14 * v7 + translation._24 * v6 + translation._34 * rot._13 + rot._14;
        vv._13 = (float)((float)((float)(translation._13 * v7) + (float)(translation._23 * v6))
                         + (float)(rot._14 * translation._43))
            + rot._13;
        vv._21 = (float)((float)((float)(translation._21 * v7) + (float)(rot._23 * translation._31))
                         + (float)(rot._24 * translation._41))
            + (float)(0.0 - v6);
        vv._22 = (float)((float)((float)(rot._23 * translation._32) + (float)(translation._12 * (float)(0.0 - v6)))
                         + (float)(rot._24 * translation._42))
            + v7;
        vv._24 = (float)((float)((float)(translation._24 * v7) + (float)(rot._23 * translation._34))
                         + (float)(translation._14 * (float)(0.0 - v6)))
            + rot._24;
        vv._23 = (float)((float)((float)(translation._23 * v7) + (float)(translation._13 * (float)(0.0 - v6)))
                         + (float)(rot._24 * translation._43))
            + rot._23;
        vv._31 = (float)((float)((float)(rot._32 * translation._21) + (float)(rot._34 * translation._41)) + rot._31)
            + translation._31;
        vv._32 = (float)((float)((float)(rot._31 * translation._12) + (float)(rot._34 * translation._42)) + rot._32)
            + translation._32;
        vv._33 = (float)((float)((float)(rot._32 * translation._23) + (float)(rot._31 * translation._13))
                         + (float)(rot._34 * translation._43))
            + 1.0;
        vv._34 = (float)((float)((float)(rot._32 * translation._24) + (float)(rot._31 * translation._14)) + rot._34)
            + translation._34;
        vv._41 = (float)((float)((float)(rot._43 * translation._31) + (float)(rot._42 * translation._21)) + rot._41)
            + translation._41;
        vv._42 = (float)((float)((float)(rot._43 * translation._32) + (float)(rot._41 * translation._12)) + rot._42)
            + translation._42;
        vv._43 = (float)((float)((float)(rot._42 * translation._23) + (float)(rot._41 * translation._13)) + rot._43)
            + translation._43;
        vv._44 = (float)((float)((float)(rot._43 * translation._34) + (float)(rot._42 * translation._24))
                         + (float)(rot._41 * translation._14))
            + 1.0;

        rot = vv;

        M3D_RENDERER->MatSetWorld(rot);
        M3D_RENDERER->SetIndices(this->m_ibSky, 0);
        M3D_RENDERER->TgSetTcSource(0, rend::TC_FROM_VERTEX, 0);
        M3D_RENDERER->SetStageState(0,
                                    rend::BM_COLOR,
                                    rend::TS_TEX_MODULATE2X_TFAC);

        M3D_RENDERER->SetStageState(0,
            rend::BM_ALPHA,
            rend::TS_DIFF_MODULATE_TFAC);
        M3D_RENDERER->SetStageState(1, rend::BM_COLOR, rend::TS_NONE);
        M3D_RENDERER->SetStageState(1, rend::BM_ALPHA, rend::TS_NONE);
        M3D_RENDERER->SetTexture(0,
            this->m_cloudTextureHandle,
            -1.0);
        M3D_RENDERER->SetTFactor(GetWeatherColor(CI_SKY), false);
        M3D_RENDERER->DrawIndexedPrimitive(
            rend::M3DPT_TRIANGLELIST,
            0,
            400u,
            0,
            722u);

        if (this->m_curDayTime == GTP_NIGHT_TIME && rendMode == Landscape::LRM_DIRECT)
        {
            M3D_RENDERER->SetTFactor(-1u, 0);
            M3D_RENDERER->SetStageState(0, rend::BM_COLOR, rend::TS_TEXTURE);
            M3D_RENDERER->SetStageState(
                0,
                rend::BM_ALPHA,
                rend::TS_DIFF_MODULATE_TFAC);
            M3D_RENDERER->SetTexture(
                0,
                this->m_starsTexture,
                -1.0);
            M3D_RENDERER->TgSetTcSource(0, rend::TC_FROM_VERTEX, 1);
            M3D_RENDERER->DrawIndexedPrimitive(
                rend::M3DPT_TRIANGLELIST,
                0,
                400u,
                0,
                722u);
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

    int WeatherManager::SaveWeatherStateToXMLNode(cmn::XmlFile*, cmn::XmlNode*)
    {
        RETRUXX_NOT_IMPLEMENTED;
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

    void WeatherManager::AddWeather(CStr const&, CStr const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned WeatherManager::GetNumWeathers() const
    {
        if (m_bEdit)
        {
            return m_weatherStorage.size();
        }
        return m_curWeatherStorage.size();
    }

    char const* WeatherManager::GetGlobalTimeParamName(unsigned) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WeatherManager::SetActiveWeatherByName(CStr const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WeatherManager::ChangeStarsTexture(CStr&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Weather* WeatherManager::GetWeather(unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Weather* WeatherManager::GetWeatherByName(CStr const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}
