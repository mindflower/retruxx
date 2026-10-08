#define NOMINMAX
#include "weather/weather.h"
#include <stdexcept>
#include <world.h>
#include <core/kernel.h>
#include <core/ini.h>

#include "config.h"
#include "level.h"
#include "m3dapp.h"
#include "core/log.h"
#include "core/timer.h"
#include "server/dynamicscene.h"
#include "server/server.h"
#include "server/playerpassmap.h"
#include "server/obstacle.h"
#include <ode/collision.h>
#include <core/scoped_ptr.h>
#include <file/fileserver.h>
#include <file/filestream.h>

#include "scene/nodes/sgnodestaticmodel.h"

namespace ai
{
    extern CServer* pServer;
}

namespace
{
    // A cvar's value as a float, whatever its type.
    float CVarAsFloat(m3d::CVar const& var)
    {
        return var.GetType() == m3d::CVar::CVAR_FLOAT ? var.GetF() : static_cast<float>(var.GetI());
    }
}  // namespace

namespace m3d
{
    bool CWorld::GetShadowVisibilityFromWeather() const
    {
        // RVA 0x7A4470
        return m_weatherManager.GetShadowVisibilityFromWeather();
    }

    WeatherManager& CWorld::GetWeatherManager()
    {
        return m_weatherManager;
    }

    void CWorld::Invalidate()
    {
        // RVA 0x5BFB60
        m_landscape.Invalidate();
    }

    WheelTraceMgr& CWorld::GetWheelTracesMgr()
    {
        return m_wheelTracesMgr;
    }

    void CWorld::Render()
    {
        m_landscape.UpdateVis(true);
        m_landscape.Render();
        ai::gDynamicScene->RenderDebugInfo();
    }

    int CWorld::RenderSky(Landscape::LandRenderMode rendMode)
    {
        return m_weatherManager.RenderWeather(rendMode);
    }

    void CWorld::New(CCamera& cam, int lsSize, float hgt0)
    {
        // RVA 0x5C7F90 - an empty editor world.
        // NOTE: the current time is read and not used.
        g_Kernel->GetTimer().GetCurTime();
        m_level->New(cam, lsSize);
        ai::pServer->Init(this);
        m_landscape.New(hgt0);
        m_weatherManager.CreateSky();
        m_weatherManager.ReadFromXmlFile(M3D_KERNEL->GetEngineCfg().m_weather_ConfigFile.GetS());
        m_weatherManager.UpdateDayTime();
        m_roadManager.Init();
        m_roadManager.ReadRoadSetConfigFromXmlFile("data/models/Roads.xml");
    }

    SceneGraph& CWorld::GetGraph()
    {
        return m_sceneGraph;
    }

    void CWorld::SetOwner(CClient* client)
    {
        m_owner = client;
    }

    RoadManager& CWorld::GetRoadManager()
    {
        return m_roadManager;
    }

    unsigned CWorld::GetWeatherFogColor() const
    {
        return m_weatherManager.GetWeatherColor(CI_FOG);
    }

    int CWorld::Load(CStr const& levelname, CCamera& cam, bool bQuiet)
    {
        // RVA 0x5C9F50
        auto timeStart = M3D_KERNEL->GetTimer().GetCurTime();
        {
            M3D_LOG_INFO("----------------------- World Loading");
        }
        // The level is loaded by the name stored in the cvar, not by the argument.
        M3D_KERNEL->GetEngineCfg().m_levFileName.Set(levelname.c_str(), true);
        if (m_level->Load(CStr(M3D_KERNEL->GetEngineCfg().m_levFileName.GetS()), cam, bQuiet) == 0)
        {
            M3D_LOG_INFO(
                CStr("Level file ") + CStr(M3D_KERNEL->GetEngineCfg().m_levFileName.GetS()) + CStr(" not found"));
            return 0;
        }
        ai::pServer->Init(this);
        m_weatherManager.CreateSky();

        auto const landscapeStart = M3D_KERNEL->GetTimer().GetCurTime();
        if (!m_landscape.Load())
        {
            return 0;
        }
        auto const landscapeEnd = M3D_KERNEL->GetTimer().GetCurTime();
        {
            M3D_LOG_INFO(
                "----------------------- Landscape loaded in: " +
                CStr(static_cast<int>(landscapeEnd - landscapeStart)));
        }

        auto const serversStart = M3D_KERNEL->GetTimer().GetCurTime();
        bool res = true;
        if (m_level->m_serversname.empty() || !M3D_APP->LoadServers(m_level->m_serversname, bQuiet))
        {
            res = M3D_APP->LoadServers("data\\models\\servers.xml", bQuiet);
        }
        if (m_level->m_staticServers.empty() || !M3D_APP->LoadServers(m_level->m_staticServers, bQuiet))
        {
            res |= M3D_APP->LoadServers("data\\models\\commonservers.xml", bQuiet);
        }
        m_landscape.PostServersLoad();
        if (!res)
        {
            return 0;
        }

        if (!CreatePrefabsFromFile(m_level->GetFullPathNameA("prefabs.xml").c_str()) &&
            !CreatePrefabsFromFile("data\\models\\prefabs.xml"))
        {
            M3D_LOG_INFO("Could not find prefabs.xml");
        }

        M3D_APP->PostLoadServers();

        auto const serversEnd = M3D_KERNEL->GetTimer().GetCurTime();
        {
            M3D_LOG_INFO(
                "----------------------- Servers loaded in: " + CStr(static_cast<int>(serversEnd - serversStart)));
        }

        auto const worldBegin = M3D_KERNEL->GetTimer().GetCurTime();
        if (!bQuiet)
        {
            auto const str = M3D_APP->GetStringByStringId0("LoadWorld");
            M3D_APP->PutSplash(100, str.c_str());
        }

        LoadWorld(m_level->GetFullPathNameA("world.xml"));

        auto const worldEnd = M3D_KERNEL->GetTimer().GetCurTime();
        {
            M3D_LOG_INFO("----------------------- Nodes loaded in: " + CStr(static_cast<int>(worldEnd - worldBegin)));
        }

        auto const initWorldBegin = M3D_KERNEL->GetTimer().GetCurTime();
        if (!bQuiet)
        {
            auto const str = M3D_APP->GetStringByStringId0("InitWorld");
            M3D_APP->PutSplash(100, str.c_str());
        }

        auto const initWorldEnd = M3D_KERNEL->GetTimer().GetCurTime();
        {
            M3D_LOG_INFO(
                "----------------------- Prefabs loaded in: " + CStr(static_cast<int>(initWorldEnd - initWorldBegin)));
        }

        ProcessCollisionStuff();
        m_weatherManager.ReadFromXmlFile(M3D_KERNEL->GetEngineCfg().m_weather_ConfigFile.GetS());
        m_weatherManager.UpdateDayTime();

        auto const worldLoadedEnd = M3D_KERNEL->GetTimer().GetCurTime();
        {
            M3D_LOG_INFO(
                "----------------------- World loaded in: " + CStr(static_cast<int>(worldLoadedEnd - timeStart)));
        }

        if (m_borderWallGeoms[0])
        {
            dGeomPlaneSetParams(m_borderWallGeoms[0], 1.0, 0.0, 0.0, m_level->m_minSafex);
        }
        else
        {
            m_borderWallGeoms[0] = dCreatePlane(ai::gGlobalSpace, 1.0, 0.0, 0.0, m_level->m_minSafex);
        }
        dGeomSetCategoryBits(this->m_borderWallGeoms[0], 1u);
        dGeomSetCollideBits(this->m_borderWallGeoms[0], 0xFFFFFFFE);
        if (m_borderWallGeoms[1])
            dGeomPlaneSetParams(m_borderWallGeoms[1], -1.0, 0.0, 0.0, 0.0 - this->m_level->m_maxSafex);
        else
            this->m_borderWallGeoms[1] =
                dCreatePlane(ai::gGlobalSpace, -1.0, 0.0, 0.0, 0.0 - this->m_level->m_maxSafex);
        dGeomSetCategoryBits(this->m_borderWallGeoms[1], 1u);
        dGeomSetCollideBits(this->m_borderWallGeoms[1], 0xFFFFFFFE);
        if (m_borderWallGeoms[3])
            dGeomPlaneSetParams(m_borderWallGeoms[3], 0.0, 0.0, 1.0, this->m_level->m_minSafey);
        else
            this->m_borderWallGeoms[3] = dCreatePlane(ai::gGlobalSpace, 0.0, 0.0, 1.0, this->m_level->m_minSafey);
        dGeomSetCategoryBits(this->m_borderWallGeoms[3], 1u);
        dGeomSetCollideBits(this->m_borderWallGeoms[3], 0xFFFFFFFE);
        if (m_borderWallGeoms[4])
            dGeomPlaneSetParams(m_borderWallGeoms[4], 0.0, 0.0, -1.0, 0.0 - this->m_level->m_maxSafey);
        else
            this->m_borderWallGeoms[4] =
                dCreatePlane(ai::gGlobalSpace, 0.0, 0.0, -1.0, 0.0 - this->m_level->m_maxSafey);
        dGeomSetCategoryBits(this->m_borderWallGeoms[4], 1u);
        dGeomSetCollideBits(this->m_borderWallGeoms[4], 0xFFFFFFFE);

        auto const roadsLoadedBegin = M3D_KERNEL->GetTimer().GetCurTime();

        m_roadManager.Init();
        m_roadManager.ReadRoadSetConfigFromXmlFile((m_level->m_levelPath + "\\" + m_level->m_roadsetName).c_str());
        m_roadManager.ReadRoadsFromXmlFile((m_level->m_levelPath + "\\" + m_level->m_roadmapName).c_str());

        auto const roadsLoadedEnd = M3D_KERNEL->GetTimer().GetCurTime();
        {
            M3D_LOG_INFO(
                "----------------------- Roads loaded in: " +
                CStr(static_cast<int>(roadsLoadedEnd - roadsLoadedBegin)));
        }

        return 1;
    }

    bool CWorld::SaveWorld(CStr const& filename)
    {
        // RVA 0x5C84E0 - writes the scene graph under a "World" element, with the last node id.
        ref_ptr xmlFile = M3D_KERNEL->CreateXmlFile();
        ref_ptr worldNode = xmlFile->CreateNode(cmn::XML_NODE_ELEMENT, "World");
        m_sceneGraph.m_rootNode.WriteToXmlNode(xmlFile, worldNode);
        xmlFile->AddChild(worldNode);
        worldNode->SetAttribute("LastId", CStr(m_lastId).c_str());
        CStr errorStr;
        return WriteXmlFile(filename.c_str(), xmlFile, &errorStr) != 0;
    }

    void CWorld::RefreshObjectsOnLandscapeRect(CVector const& org, float r)
    {
        // RVA 0x5C78A0 - refreshes the visibility cells (128 units wide) covering the square of half-size r around org.
        // NOTE: the square is taken in x and y.
        float const VISCELL_EDGE_LENGTH = 128.0f;
        float const inv = 1.0f / VISCELL_EDGE_LENGTH;
        m_sceneGraph.RefreshObjectsInRect(
            static_cast<int>((org.x - r) * inv),
            static_cast<int>((org.y - r) * inv),
            static_cast<int>((org.x + r) * inv),
            static_cast<int>((r + org.y) * inv));
    }

    CWorld::CWorld() :
        m_lsInscatterCoeff("lsIC", "50", CVar::CVAR_INT, CVar::CVAR_ARCHIVE),
        m_lsOutscatterCoeff("lsOC", "50", CVar::CVAR_INT, CVar::CVAR_ARCHIVE),
        m_skyInscatterCoeff("skyIC", "50", CVar::CVAR_INT, CVar::CVAR_ARCHIVE),
        m_skyOutscatterCoeff("skyOC", "50", CVar::CVAR_INT, CVar::CVAR_ARCHIVE),
        m_sunColorR("sunColorR", "100", CVar::CVAR_INT, CVar::CVAR_ARCHIVE),
        m_sunColorG("sunColorG", "40", CVar::CVAR_INT, CVar::CVAR_ARCHIVE),
        m_sunColorB("sunColorB", "10", CVar::CVAR_INT, CVar::CVAR_ARCHIVE),
        m_fogStart("lsFogStart", "70", CVar::CVAR_INT, CVar::CVAR_ARCHIVE)
    {
        this->m_sceneGraph.SetOwner(this);
        this->m_landscape.setOwner(this);
        this->m_weatherManager.SetOwner(this);
        this->m_roadManager.SetOwner(&m_landscape);
        this->m_sunAzimuth = 45.0;
        this->m_lastId = 0;
        this->m_level = dynamic_cast<Level*>(m3d::g_Kernel->New("Level"));
        m3d::g_Kernel->UnRegisterGlobal("CurrentLevel");
        m3d::g_Kernel->RegisterGlobal(this->m_level, "CurrentLevel");
        g_Kernel->GetEngineCfg().m_console->RegisterCVar(&this->m_lsInscatterCoeff, 0);
        g_Kernel->GetEngineCfg().m_console->RegisterCVar(&this->m_lsOutscatterCoeff, 0);
        g_Kernel->GetEngineCfg().m_console->RegisterCVar(&this->m_skyInscatterCoeff, 0);
        g_Kernel->GetEngineCfg().m_console->RegisterCVar(&this->m_skyOutscatterCoeff, 0);
        g_Kernel->GetEngineCfg().m_console->RegisterCVar(&this->m_sunColorR, 0);
        g_Kernel->GetEngineCfg().m_console->RegisterCVar(&this->m_sunColorG, 0);
        g_Kernel->GetEngineCfg().m_console->RegisterCVar(&this->m_sunColorB, 0);
        g_Kernel->GetEngineCfg().m_console->RegisterCVar(&this->m_fogStart, 0);
        this->m_profilerUpdateOde = Application::g_pApp->GetProfilerStack().GetProfiler(
            Application::g_pApp->GetProfilerStack().AddProfiler("AI other update", 0x1Eu));
        this->m_borderWallGeoms[0] = 0;
        this->m_borderWallGeoms[1] = 0;
        this->m_borderWallGeoms[2] = 0;
        this->m_borderWallGeoms[3] = 0;
        this->m_borderWallGeoms[4] = 0;
        this->m_borderWallGeoms[5] = 0;
        this->m_texClouds = 0;
        this->m_isWeatherActual = 1;
    }

    void CWorld::UpdateSun()
    {
        // RVA 0x5C7760 - the sun's elevation comes from the level, per time of day; any
        // other time of day puts the sun on the horizon. The trigonometry runs on the x87
        // stack in the original, hence the doubles.
        switch (m_weatherManager.GetCurrentDayTime())
        {
            case GTP_SUNRISE_TIME:
                m_sunAscention = m_level->m_sunRiseAscention;
                break;
            case GTP_DAY_TIME:
                m_sunAscention = m_level->m_sunDayAscention;
                break;
            case GTP_SUNSET_TIME:
                m_sunAscention = m_level->m_sunSetAscention;
                break;
            default:
                m_sunAscention = 0.0f;
                break;
        }

        float const degToRad = 0.017453292f;
        double const ascention = static_cast<double>(m_sunAscention) * degToRad;
        m_sunAzimuth = m_level->m_sunAzimuth;
        double const cosAscention = cos(ascention);
        m_sunDir.x = static_cast<float>(cos(static_cast<double>(m_sunAzimuth) * degToRad) * cosAscention * 20000.0);
        m_sunDir.z = static_cast<float>(sin(static_cast<double>(m_sunAzimuth) * degToRad) * cosAscention * 20000.0);
        m_sunDir.y = static_cast<float>(sin(ascention) * 20000.0);

        double const x = m_sunDir.x;
        double const y = m_sunDir.y;
        double const z = m_sunDir.z;
        double const invLength = 1.0 / sqrt(x * x + y * y + z * z + 1.1920929e-7);
        m_sunDir.x = static_cast<float>(invLength * m_sunDir.x);
        m_sunDir.y = static_cast<float>(invLength * m_sunDir.y);
        m_sunDir.z = static_cast<float>(invLength * m_sunDir.z);
    }

    void CWorld::ReleasePrefabs()
    {
        for (auto& data : m_effectsFactory)
        {
            if (data.onlyOne)
            {
                m_sceneGraph.RemoveNode(data.effect);
            }
            else
            {
                for (auto& effect : data.effects)
                {
                    m_sceneGraph.RemoveNode(effect);
                }
            }
        }

        m_effectsFactory.clear();
        fxNames.clear();
        fxRemap.clear();
    }

    int CWorld::CreateGeomsRepresentingSceneNodes()
    {
        // RVA 0x5C7910 - does nothing.
        return 1;
    }

    dxSpace* CWorld::GetOdeSpace()
    {
        return ai::gGlobalSpace;
    }

    unsigned CWorld::GetWeatherSunColor() const
    {
        // RVA 0x5C7980
        return m_weatherManager.GetWeatherColor(CI_SUN);
    }

    unsigned CWorld::GetWeatherPlantColor() const
    {
        return m_weatherManager.GetWeatherColor(CI_PLANT);
    }

    void CWorld::Release()
    {
        m_landscape.Release();
        m_wheelTracesMgr.Release();
        M3D_RENDERER->ReleaseTexture(m_texMiniMap);
        ReleasePrefabs();
        m_weatherManager.DoneSky();
        for (auto& geom : m_borderWallGeoms)
        {
            if (geom)
            {
                dGeomDestroy(geom);
                geom = nullptr;
            }
        }
        m_roadManager.Release();
        m_sceneGraph.UnlinkAndDeleteAll();
    }

    unsigned CWorld::GetWeatherSpecularColor() const
    {
        return m_weatherManager.GetWeatherColor(CI_SPECULAR);
    }

    ai::Vehicle* CWorld::GetVehicleControlledByPlayer()
    {
        return ai::gDynamicScene->GetVehicleControlledByPlayer();
    }

    float CWorld::GetSForShadowsFromWeather() const
    {
        // RVA 0x5C7990 - how far the shadow texture is shifted for the time of day.
        switch (m_weatherManager.m_curDayTime)
        {
        case GTP_SUNRISE_TIME:
            return 0.25f;
        case GTP_DAY_TIME:
            return 1.0f;
        case GTP_SUNSET_TIME:
            return 0.75f;
        default:
            return 0.0f;
        }
    }

    void CWorld::ProcessCollisionStuffOnNode(SgNode* node)
    {
        if (node->IsKindOf(&m3d::SgStaticModelNode::m_classSgStaticModelNode))
        {
            assert(!"obsolete");
        }
    }

    float CWorld::GetShadowTransparencyFromWeather() const
    {
        // RVA 0x8A1D00
        return m_weatherManager.GetShadowTransparencyFromWeather();
    }

    GlobalTimeParams CWorld::GetCurDayTimeFromWeatherManager() const
    {
        // RVA 0x5C7400
        return m_weatherManager.m_curDayTime;
    }

    void CWorld::Restore()
    {
        // RVA 0x5BFB70
        m_landscape.Restore();
    }

    CVector CWorld::GetScatteringSunColor() const
    {
        // RVA 0x5C8120 - the sun colour cvars are percentages of a maximum of 3 per channel.
        CVector const sunColorMax(3.0f, 3.0f, 3.0f);
        float const r = sunColorMax.x * CVarAsFloat(m_sunColorR) * 0.0099999998f;
        float const g = sunColorMax.y * CVarAsFloat(m_sunColorG) * 0.0099999998f;
        float const b = sunColorMax.z * CVarAsFloat(m_sunColorB) * 0.0099999998f;
        return CVector(r, g, b);
    }

    retruxx::vector<CStr, retruxx::allocator<CStr>> const& CWorld::GetFxNames() const
    {
        // RVA 0x5BFB80
        return fxNames;
    }

    unsigned CWorld::GetWeatherAmbientColor() const
    {
        return m_weatherManager.GetWeatherColor(CI_AMBIENT);
    }

    int CWorld::GetFxId(CStr const& name)
    {
        auto it = fxRemap.find(name);
        if (it != fxRemap.end())
        {
            return it->second;
        }
        return -1;
    }

    CVector const& CWorld::GetSun(float) const
    {
        return m_sunDir;
    }

    CWorld::~CWorld()
    {
        // RVA 0x5CB450 - the rest is destroyed as members.
        IConsole* const console = M3D_KERNEL->GetEngineCfg().m_console;
        console->UnregisterCVar(&m_lsInscatterCoeff);
        console->UnregisterCVar(&m_lsOutscatterCoeff);
        console->UnregisterCVar(&m_skyInscatterCoeff);
        console->UnregisterCVar(&m_skyOutscatterCoeff);
        console->UnregisterCVar(&m_sunColorR);
        console->UnregisterCVar(&m_sunColorG);
        console->UnregisterCVar(&m_sunColorB);
        console->UnregisterCVar(&m_fogStart);
        delete m_level;
        m_level = nullptr;
    }

    void CWorld::Update()
    {
        if (M3D_KERNEL->GetEngineCfg().m_dbg_doNotUpdateAI.GetB())
        {
            m_weatherManager.UpdateWheatherParticles();
            m_landscape.Update();
            m_sceneGraph.Update();
        }
        else
        {
            m_weatherManager.UpdateWheatherParticles();
            m_sceneGraph.UpdateThinkNodes();
            ai::pServer->Update(0.0);
            m_landscape.Update();
            m_sceneGraph.Update();

            m_profilerUpdateOde->StartCountdown();
            ai::pServer->RelinkSceneGraphNodes();
            m_profilerUpdateOde->EndCountdown();

            m_landscape.ManageLandScapeCollisionTriMeshes();
        }
    }

    unsigned CWorld::GetWeatherDiffuseColor() const
    {
        return m_weatherManager.GetWeatherColor(CI_DIFFUSE);
    }

    Landscape& CWorld::GetLandscape()
    {
        return m_landscape;
    }

    void CWorld::ProcessCollisionStuff()
    {
        // RVA 0x5C9E20 - drops every collision triangle tag, then visits every node below
        // the root (the root itself excluded).
        m_landscape.RemoveCollisionTris(-1);

        retruxx::vector<Object*> stack;
        stack.push_back(&m_sceneGraph.m_rootNode);
        while (!stack.empty())
        {
            Object* const parent = stack.back();
            stack.pop_back();

            for (auto* child = static_cast<SgNode*>(parent->GetFirstChild()); child;
                 child = static_cast<SgNode*>(child->GetNextSibling()))
            {
                ProcessCollisionStuffOnNode(child);
                if (child->GetFirstChild())
                {
                    stack.push_back(child);
                }
            }
        }
    }

    int CWorld::CreatePrefabsFromFile(char const* fileName)
    {
        // RVA 0x7AE710 - the file lists the prefab files, each of which holds "Node" elements.
        // A "multi" node groups variants that CreatePrefabsNode picks from at random.
        std::vector<CStr> PrefabFiles;

        m3d::fs::FileServer& fileServer = M3D_KERNEL->GetFileServer();
        scoped_ptr fileStream = fileServer.CreateFileStream();

        if (!fileStream->Open(fileName, m3d::fs::IStream::OpenFlags::OPEN_READ))
        {
            return 0;
        }

        ref_ptr<m3d::cmn::XmlFile> xmlFile = m3d::g_Kernel->CreateXmlFile();

        xmlFile->Read(*fileStream);
        fileStream->Close();

        char const* error = xmlFile->GetError();
        if (error)
        {
            CStr errorMsg = "CreatePrefabsFromFile(): Can't parse: " + CStr(error);
            M3D_LOG_INFO(errorMsg);
            return 0;
        }

        ref_ptr<m3d::cmn::XmlNode> filesNode = xmlFile->CreateNode();
        xmlFile->GetFirstChild(filesNode, "PrefabFiles");

        filesNode->GetFirstChild(filesNode, "Item");

        while (!filesNode->IsEmpty())
        {
            char const* file = filesNode->GetAttribute("file");
            PrefabFiles.push_back(CStr(file));
            filesNode->GetNextSibling(filesNode, "Item");
        }

        for (unsigned int i = 0; i < PrefabFiles.size(); ++i)
        {
            CStr const& prefabFile = PrefabFiles[i];

            if (!fileStream->Open(prefabFile.c_str(), m3d::fs::IStream::OpenFlags::OPEN_READ))
                continue;

            ref_ptr<m3d::cmn::XmlFile> prefabXml = m3d::g_Kernel->CreateXmlFile();

            prefabXml->Read(*fileStream);
            fileStream->Close();

            if (char const* const error = prefabXml->GetError())
            {
                // NOTE: the original builds "Can't parse: " and then assigns the parser's
                // error over it, so the box shows the bare error text.
                M3D_APP->RunMsgBoxDlg("error", CStr(error), 1, false);
                return 0;
            }

            ref_ptr<m3d::cmn::XmlNode> prefabsNode = prefabXml->CreateNode();
            prefabXml->GetFirstChild(prefabsNode, "Prefabs");

            prefabsNode->GetFirstChild(prefabsNode, "Node");

            while (!prefabsNode->IsEmpty())
            {
                bool isMulti = false;
                m3d::SafeBoolAttrib(isMulti, prefabsNode, "multi");

                if (isMulti)
                {
                    char const* name = prefabsNode->GetAttribute("name");

                    m3d::CWorld::EffectsData effData;
                    effData.onlyOne = false;

                    ref_ptr<m3d::cmn::XmlNode> childNode = prefabXml->CreateNode();
                    prefabsNode->GetFirstChild(childNode, "Node");

                    while (!childNode->IsEmpty())
                    {
                        m3d::SgNode* prefab = ReadPrefab(prefabXml, childNode);
                        if (prefab)
                        {
                            effData.effects.push_back(prefab);

                            if (!strstr(prefab->GetName(), name))
                            {
                                CStr warning =
                                    "Warning: multieffect child names doesn't match for parent name. For effect " +
                                    CStr(name);
                                M3D_LOG_INFO(warning);
                            }
                        }

                        childNode->GetNextSibling(childNode, "Node");
                    }

                    M3D_ASSERT(!effData.effects.empty());

                    m_effectsFactory.push_back(effData);
                    fxNames.push_back(CStr(name));
                }
                else
                {
                    m3d::SgNode* prefab = ReadPrefab(prefabXml, prefabsNode);
                    if (prefab)
                    {
                        m3d::CWorld::EffectsData effData;
                        effData.onlyOne = true;
                        effData.effect = prefab;

                        m_effectsFactory.push_back(effData);
                        fxNames.push_back(prefab->GetName());
                    }
                }

                prefabsNode->GetNextSibling(prefabsNode, "Node");
            }
        }

        fxRemap.clear();
        for (unsigned int i = 0; i < fxNames.size(); ++i)
        {
            fxRemap[fxNames[i]] = i;
        }

        return 1;
    }

    SgNode* CWorld::CreatePrefabsNode(int type)
    {
        if (type < 0 || type >= fxNames.size())
        {
            return nullptr;
        }

        if (type >= m_effectsFactory.size())
        {
            return nullptr;
        }

        SgNode* result = nullptr;
        auto& factory = m_effectsFactory[type];
        if (factory.onlyOne)
        {
            result = factory.effect;
        }
        else
        {
            result = factory.effects[rand() % factory.effects.size()];
        }
        if (result)
        {
            return (SgNode*)result->Clone();
        }
        return result;
    }

    void CWorld::UpdateSkyParams()
    {
        m_weatherManager.SetupSkyParams();
    }

    float CWorld::GetSunAscention() const
    {
        // RVA 0x5C7890 - in radians.
        return static_cast<float>(m_sunAscention * 0.017453292f);
    }

    float CWorld::GetFogReduceFactorFromWeather() const
    {
        return m_weatherManager.GetFogReduceFactorFromWeather();
    }

    SgNode* CWorld::ReadPrefab(ref_ptr<cmn::XmlFile> file, ref_ptr<cmn::XmlNode> pnode)
    {
        // RVA 0x7ADD00 - a prefab is a template for CreatePrefabsNode to clone, so neither it
        // nor anything below it may think.
        char const* const name = pnode->GetAttribute("name");
        SgNode* prefab = static_cast<SgNode*>(g_Kernel->New(pnode->GetAttribute("class")));

        if (!prefab->ReadFromXmlNode(file, pnode) || !prefab->ReadFromXmlNodeAfterAdd(file, pnode))
        {
            // NOTE: the message has no closing quote after the name, as in the original.
            M3D_LOG_ERR(
                CStr("Error: Couldn't load prefab '") + CStr(name) +
                CStr(". Check if you have child nodes with duplicate names."));
            GetGraph().RemoveNode(prefab);
            return nullptr;
        }

        SceneGraph* const graph = prefab->GetGraph();
        graph->UnlinkThinkNode(prefab);

        retruxx::vector<Object*> stack;
        stack.push_back(prefab);
        while (!stack.empty())
        {
            Object* const parent = stack.back();
            stack.pop_back();

            for (auto* child = static_cast<SgNode*>(parent->GetFirstChild()); child;
                 child = static_cast<SgNode*>(child->GetNextSibling()))
            {
                graph->UnlinkThinkNode(child);
                if (child->GetFirstChild())
                {
                    stack.push_back(child);
                }
            }
        }

        return prefab;
    }

    bool CWorld::LoadWorld(CStr const& filename)
    {
        // RVA 0x5C9380 - reads the scene graph, registers the static obstacles and the
        // impassable pass map cells with the collision cells, links the top level nodes and
        // works out the id for the next new node.
        M3D_LOG_INFO(CStr("LoadWorld begin..."));
        unsigned const readStart = M3D_KERNEL->GetTimer().GetCurTime();

        CStr err;
        ref_ptr<cmn::XmlFile> xmlFile = ReadXmlFile(filename.c_str(), &err);
        if (!xmlFile)
        {
            M3D_LOG_INFO(CStr("LoadWorld: ") + err);
            return false;
        }
        M3D_LOG_INFO(CStr("%%%% read/parse xml: ") + CStr(M3D_KERNEL->GetTimer().GetCurTime() - readStart));

        ref_ptr<cmn::XmlNode> root = xmlFile->CreateNode();
        xmlFile->GetFirstChild(root, "World");
        int lastSavedId = 0;
        SafeIntAttrib(lastSavedId, root, "LastId");

        unsigned const parseStart = M3D_KERNEL->GetTimer().GetCurTime();
        SgNode* const rootNode = &m_sceneGraph.m_rootNode;
        rootNode->ReadFromXmlNode(xmlFile, root);
        M3D_LOG_INFO(CStr("%%%% read from xml: ") + CStr(M3D_KERNEL->GetTimer().GetCurTime() - parseStart));

        unsigned const linkStart = M3D_KERNEL->GetTimer().GetCurTime();
        if (M3D_KERNEL->GetEngineCfg().m_ai_static_obstacles_enabled.GetB())
        {
            // Every node below the root that answers the "is obstacle" property is one.
            retruxx::vector<SgNode*> stack;
            stack.push_back(rootNode);
            while (!stack.empty())
            {
                SgNode* const parent = stack.back();
                stack.pop_back();

                for (auto* child = static_cast<SgNode*>(parent->GetFirstChild()); child;
                     child = static_cast<SgNode*>(child->GetNextSibling()))
                {
                    char isObstacle = 1;
                    if (child->GetProperty(8717, &isObstacle))
                    {
                        child->UpdateXForm(0, 1);
                        child->SetProperty(8717, &isObstacle);
                        m_landscape.LinkNodeObstacleToCells(child);
                    }
                    if (child->GetFirstChild())
                    {
                        stack.push_back(child);
                    }
                }
            }

            LoadStaticObstacles();
        }

        // The pass map has two cells per tile side.
        ai::PlayerPassMap playerPassMap;
        playerPassMap.LoadFromBinaryFile(m_level->m_levelPath + CStr("\\") + m_level->m_playerPassMapFileName);
        int const passMapSize = 2 * m_landscape.GetTileSize();
        if (!playerPassMap.IsEmpty())
        {
            for (int x = 0; x < passMapSize; ++x)
            {
                for (int y = 0; y < passMapSize; ++y)
                {
                    if (!playerPassMap.GetValue(x, y))
                    {
                        m_landscape.LinkPassMapCellToCollisionCell(PointBase<int>(x, y));
                    }
                }
            }
        }
        playerPassMap.Clear();

        // The next id is past the saved one, the number of top level nodes and the largest
        // id in their names.
        m_lastId = 0;
        int maxNameId = 0;
        int numNodes = 0;
        int nameId = 0;
        for (auto* node = static_cast<SgNode*>(m_sceneGraph.m_rootNode.GetFirstChild()); node;
             node = static_cast<SgNode*>(node->GetNextSibling()))
        {
            node->UpdateXForm(0, 1);
            m_sceneGraph.LinkNode(node);
            m_landscape.LinkNodeAndChildrenCollisionGeomsToCell(node);

            // NOTE: the index moves on after a deletion too, so of two non-digits in a row
            // the second survives ("ab12" leaves "b12"), and sscanf then fails and leaves
            // the previous node's id in place.
            CStr digits(node->GetName());
            for (int i = 0; i < static_cast<int>(digits.length()); ++i)
            {
                if (digits[i] < '0' || digits[i] > '9')
                {
                    digits.del(i, 1);
                }
            }
            if (digits.length() == 0)
            {
                nameId = 0;
            }
            else
            {
                sscanf(digits.c_str(), "%d", &nameId);
            }

            if (nameId > maxNameId)
            {
                maxNameId = nameId;
            }
            ++numNodes;
        }

        m_lastId = std::max(std::max(lastSavedId, numNodes), maxNameId);
        if (lastSavedId == 0)
        {
            // NOTE: without a saved LastId the original adds the address of the root node
            // (lea eax, [eax + this + 319158h]), so the next id depends on where the world
            // happens to be allocated.
            m_lastId += static_cast<int>(reinterpret_cast<intptr_t>(&m_sceneGraph.m_rootNode));
        }

        M3D_LOG_INFO(CStr("%%%% link nodes: ") + CStr(M3D_KERNEL->GetTimer().GetCurTime() - linkStart));
        return true;
    }

    void CWorld::LoadStaticObstacles()
    {
        // RVA 0x5C8670 - each Box of the static obstacles file becomes an oriented box obstacle.
        scoped_ptr fileStream = g_Kernel->GetFileServer().CreateFileStream();
        if (fileStream->Open(
                m_level->GetFullPathNameA(m_level->m_staticObstaclesFileName).c_str(), fs::IStream::OPEN_READ))
        {
            ref_ptr xmlFile = M3D_KERNEL->CreateXmlFile();
            if (xmlFile->Read(*fileStream))
            {
                fileStream->Close();
                ref_ptr node = xmlFile->CreateNode();
                xmlFile->GetFirstChild(node, "Boxes");
                if (!node->IsEmpty())
                {
                    ref_ptr box = xmlFile->CreateNode();
                    for (node->GetFirstChild(box, "Box"); !box->IsEmpty(); box->GetNextSibling(box, "Box"))
                    {
                        CStr buf;

                        SafeStrAttrib(buf, box, "min");
                        auto min = strToVec(buf);

                        SafeStrAttrib(buf, box, "max");
                        auto max = strToVec(buf);

                        SafeStrAttrib(buf, box, "origin");
                        auto origin = strToVec(buf);

                        SafeStrAttrib(buf, box, "rotation");
                        auto rotation = strToQuat(buf);

                        CMatrix mat;
                        mat.rotTranslate(rotation, origin);

                        Obb newBox;
                        newBox.Create(min, max, mat, true);

                        auto obstacle = new ai::Obstacle(newBox);
                        m_landscape.LinkObstacleToCells(obstacle);
                    }
                }
            }
            else
            {
                M3D_LOG_ERR("Error!!!! Bad static obstacles file: " + m_level->m_staticObstaclesFileName);
            }
        }
    }

    void CWorld::Register()
    {
        g_Kernel->AddClass(RT_CLASS_LOCAL(Weather));
        g_Kernel->AddClass(RT_CLASS_LOCAL(WeatherClear));
        g_Kernel->AddClass(RT_CLASS_LOCAL(WeatherInclement));
        g_Kernel->AddClass(RT_CLASS_LOCAL(WeatherThunderstorm));
        g_Kernel->AddClass(RT_CLASS_LOCAL(WeatherFoggy));
    }
}  // namespace m3d
