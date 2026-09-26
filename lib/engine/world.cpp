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
        // TODO: check this
        auto m_curDayTime = this->m_weatherManager.GetCurrentDayTime();
        float m_sunRiseAscention = 0.0;
        if (m_curDayTime == GTP_SUNRISE_TIME)
        {
            m_sunRiseAscention = this->m_level->m_sunRiseAscention;
            this->m_sunAscention = m_sunRiseAscention;
        }
        auto v2 = m_curDayTime - 1;
        if (!v2)
        {
            m_sunRiseAscention = this->m_level->m_sunDayAscention;
            this->m_sunAscention = m_sunRiseAscention;
        }
        if (v2 == 1)
        {
            m_sunRiseAscention = this->m_level->m_sunSetAscention;
            this->m_sunAscention = m_sunRiseAscention;
        }

        auto v4 = this->m_sunAscention * 0.017453292;
        m_sunAzimuth = this->m_level->m_sunAzimuth;
        auto v6 = cos(v4);
        this->m_sunDir.x = cos(m_sunAzimuth * 0.017453292) * v6 * 20000.0;
        this->m_sunDir.z = sin(this->m_sunAzimuth * 0.017453292) * v6 * 20000.0;
        this->m_sunDir.y = sin(v4) * 20000.0;
        auto v7 = sqrt(
            this->m_sunDir.x * this->m_sunDir.x + this->m_sunDir.y * this->m_sunDir.y +
            this->m_sunDir.z * this->m_sunDir.z + 0.00000011920929);
        this->m_sunDir.x = 1.0 / v7 * this->m_sunDir.x;
        this->m_sunDir.y = 1.0 / v7 * this->m_sunDir.y;
        this->m_sunDir.z = 1.0 / v7 * this->m_sunDir.z;
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
        // TODO: generated code
        // Clear collision triangles from landscape
        m_landscape.RemoveCollisionTris(-1);

        // Stack for depth-first traversal of scene graph
        std::vector<m3d::Object*> stack;

        // Start with the root node
        m3d::Object* rootNode = &m_sceneGraph.m_rootNode;
        stack.push_back(rootNode);

        // Depth-first traversal
        while (!stack.empty())
        {
            // Pop the last node from stack
            m3d::Object* currentNode = stack.back();
            stack.pop_back();

            // Process all children of current node
            m3d::SgNode* child = dynamic_cast<m3d::SgNode*>(currentNode->GetFirstChild());

            while (child != nullptr)
            {
                // Process collision for this child node
                ProcessCollisionStuffOnNode(child);

                // If child has children of its own, push to stack for processing
                if (child->GetFirstChild() != nullptr)
                {
                    stack.push_back(child);
                }

                // Move to next sibling
                child = dynamic_cast<m3d::SgNode*>(child->GetNextSibling());
            }
        }
    }

    int CWorld::CreatePrefabsFromFile(char const* fileName)
    {
        // TODO: generated code
        std::vector<CStr> PrefabFiles;

        // Create file stream
        m3d::fs::FileServer& fileServer = M3D_KERNEL->GetFileServer();
        scoped_ptr fileStream = fileServer.CreateFileStream();

        // Open file
        if (!fileStream->Open(fileName, m3d::fs::IStream::OpenFlags::OPEN_READ))
        {
            return 0;
        }

        // Create XML file and parse
        ref_ptr<m3d::cmn::XmlFile> xmlFile = m3d::g_Kernel->CreateXmlFile();

        xmlFile->Read(*fileStream);
        fileStream->Close();

        // Check for XML parsing errors
        char const* error = xmlFile->GetError();
        if (error)
        {
            CStr errorMsg = "CreatePrefabsFromFile(): Can't parse: " + CStr(error);
            M3D_LOG_INFO(errorMsg);
            return 0;
        }

        // Get PrefabFiles section
        ref_ptr<m3d::cmn::XmlNode> filesNode = xmlFile->CreateNode();
        xmlFile->GetFirstChild(filesNode, "PrefabFiles");

        // Iterate through all Item nodes
        filesNode->GetFirstChild(filesNode, "Item");

        while (!filesNode->IsEmpty())
        {
            char const* file = filesNode->GetAttribute("file");
            PrefabFiles.push_back(CStr(file));
            filesNode->GetNextSibling(filesNode, "Item");
        }

        // Process each prefab file
        for (unsigned int i = 0; i < PrefabFiles.size(); ++i)
        {
            CStr const& prefabFile = PrefabFiles[i];

            // Reopen file stream for this prefab file
            if (!fileStream->Open(prefabFile.c_str(), m3d::fs::IStream::OpenFlags::OPEN_READ))
                continue;

            // Parse prefab XML
            ref_ptr<m3d::cmn::XmlFile> prefabXml = m3d::g_Kernel->CreateXmlFile();

            prefabXml->Read(*fileStream);
            fileStream->Close();

            // Check for errors
            if (prefabXml->GetError())
            {
                CStr errorMsg = "Can't parse: " + CStr(prefabXml->GetError());
                M3D_APP->RunMsgBoxDlg("error", errorMsg, 1, false);
                return 0;
            }

            // Get Prefabs section
            ref_ptr<m3d::cmn::XmlNode> prefabsNode = prefabXml->CreateNode();
            prefabXml->GetFirstChild(prefabsNode, "Prefabs");

            // Process each Node
            prefabsNode->GetFirstChild(prefabsNode, "Node");

            while (!prefabsNode->IsEmpty())
            {
                bool isMulti = false;
                m3d::SafeBoolAttrib(isMulti, prefabsNode, "multi");

                if (isMulti)
                {
                    // Multi-effect prefab
                    char const* name = prefabsNode->GetAttribute("name");

                    m3d::CWorld::EffectsData effData;
                    effData.onlyOne = false;

                    // Process child nodes
                    ref_ptr<m3d::cmn::XmlNode> childNode = prefabXml->CreateNode();
                    prefabsNode->GetFirstChild(childNode, "Node");

                    while (!childNode->IsEmpty())
                    {
                        m3d::SgNode* prefab = ReadPrefab(prefabXml, childNode);
                        if (prefab)
                        {
                            effData.effects.push_back(prefab);

                            // Validate name matching
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
                    // Single effect prefab
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

        // Rebuild FX remap table
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
        // TODO: generated code
        // Get node attributes
        char const* name = pnode->GetAttribute("name");
        char const* className = pnode->GetAttribute("class");

        // Create new node instance
        m3d::SgNode* loader = dynamic_cast<m3d::SgNode*>(m3d::g_Kernel->New(className));

        // Try to load from XML
        if (loader->ReadFromXmlNode(file, pnode) && loader->ReadFromXmlNodeAfterAdd(file, pnode))
        {
            // Successfully loaded - process the node hierarchy
            m3d::SceneGraph* graph = loader->GetGraph();

            // Remove loader from think list temporarily
            graph->UnlinkThinkNode(loader);

            // Process children using iterative DFS
            std::vector<m3d::Object*> stack;
            stack.push_back(dynamic_cast<m3d::Object*>(loader->GetFirstChild()));

            while (!stack.empty())
            {
                m3d::Object* current = stack.back();
                stack.pop_back();

                // Process all siblings of the current node
                m3d::SgNode* sibling = dynamic_cast<m3d::SgNode*>(current);
                while (sibling)
                {
                    // Link the sibling node
                    graph->UnlinkThinkNode(sibling);

                    // If this sibling has children, add to stack for processing
                    if (sibling->GetFirstChild())
                    {
                        stack.push_back(sibling->GetFirstChild());
                    }

                    // Move to next sibling
                    sibling = dynamic_cast<m3d::SgNode*>(sibling->GetNextSibling());
                }
            }
        }
        else
        {
            CStr errorMsg =
                "Error: Couldn't load prefab '" + CStr(name) + "'. Check if you have child nodes with duplicate names.";

            M3D_LOG_ERR(errorMsg);

            // Remove the failed node from the scene graph
            GetGraph().RemoveNode(loader);

            return nullptr;
        }

        return loader;
    }

    bool CWorld::LoadWorld(CStr const& filename)
    {
        // TODO: check this!!!
        M3D_LOG_INFO("LoadWorld begin...");

        // Start timing
        unsigned int startTime = M3D_KERNEL->GetTimer().GetCurTime();

        // Read XML file
        CStr err;
        ref_ptr<m3d::cmn::XmlFile> xmlFile = m3d::ReadXmlFile(filename.c_str(), &err);
        if (xmlFile)
        {
            // Log XML parsing time
            CStr fileName("%%%% read/parse xml: ");

            unsigned int parseTime = M3D_KERNEL->GetTimer().GetCurTime();
            CStr timeStr(parseTime - startTime);
            CStr name(fileName);
            name += timeStr;

            CStr logMsg(name);
            M3D_LOG_INFO(logMsg);

            // Get root node
            ref_ptr<m3d::cmn::XmlNode> root = xmlFile->CreateNode();
            xmlFile->GetFirstChild(root, "World");

            // Get last saved ID if available
            int lastId = 0;
            SafeIntAttrib(lastId, root, "LastId");

            // Load world data from XML
            unsigned int loadStartTime = M3D_KERNEL->GetTimer().GetCurTime();

            m3d::SgNode* rootNode = GetGraph().GetRootNode();
            rootNode->ReadFromXmlNode(xmlFile, root);

            // Log loading time
            CStr loadTimeMsg("%%%% read from xml: ");
            unsigned int loadEndTime = M3D_KERNEL->GetTimer().GetCurTime();

            CStr loadTimeStr(loadEndTime - loadStartTime);
            CStr loadMsg(loadTimeMsg + loadTimeStr);
            M3D_LOG_INFO(loadMsg);

            // Process static obstacles if enabled
            unsigned int processStartTime = M3D_KERNEL->GetTimer().GetCurTime();

            if (M3D_KERNEL->GetEngineCfg().m_ai_static_obstacles_enabled.GetB())
            {
                std::vector<m3d::SgNode*> stack;
                stack.push_back(rootNode);

                while (!stack.empty())
                {
                    m3d::SgNode* currentNode = stack.back();
                    stack.pop_back();

                    m3d::SgNode* child = dynamic_cast<m3d::SgNode*>(currentNode->GetFirstChild());
                    while (child)
                    {
                        char isObstacle = 1;
                        if (child->GetProperty(8717u, &isObstacle))
                        {
                            child->UpdateXForm(0, 1);
                            child->SetProperty(8717u, &isObstacle);
                            m_landscape.LinkNodeObstacleToCells(child);
                        }

                        if (child->GetFirstChild())
                        {
                            stack.push_back(child);
                        }

                        child = dynamic_cast<m3d::SgNode*>(child->GetNextSibling());
                    }
                }

                LoadStaticObstacles();
            }

            // Load player pass map
            ai::PlayerPassMap playerPassMap;
            CStr pathSep("\\");
            CStr levelPath(m_level->m_levelPath);
            levelPath += pathSep;

            CStr passMapPath(levelPath);
            passMapPath += m_level->m_playerPassMapFileName;

            playerPassMap.LoadFromBinaryFile(passMapPath);

            // Process pass map
            int tileSize = 2 * m_landscape.GetTileSize();
            if (!playerPassMap.IsEmpty())
            {
                for (int j = 0; j < tileSize; ++j)
                {
                    for (int k = 0; k < tileSize; ++k)
                    {
                        if (!playerPassMap.GetValue(j, k))
                        {
                            PointBase<int> point(j, k);
                            m_landscape.LinkPassMapCellToCollisionCell(point);
                        }
                    }
                }
            }

            playerPassMap.Clear();

            // Process scene graph nodes
            m3d::SgNode* firstChild = dynamic_cast<m3d::SgNode*>(GetGraph().GetRootNode()->GetFirstChild());
            m_lastId = 0;
            int maxIdNode = 0;
            int nodeCount = 0;

            while (firstChild)
            {
                firstChild->UpdateXForm(0, 1);
                m_sceneGraph.LinkNode(firstChild);
                m_landscape.LinkNodeAndChildrenCollisionGeomsToCell(firstChild);

                // Extract ID from node name
                CStr nameStr(firstChild->GetName());
                for (size_t m = 0; m < nameStr.length(); ++m)
                {
                    // TODO: refactor this shit
                    char c = nameStr[m];
                    if (c < '0' || c > '9')
                    {
                        nameStr.del(m, 1);
                        --m;
                    }
                }

                int idNode = 0;
                if (nameStr.length() > 0)
                {
                    int id;
                    sscanf(nameStr.c_str(), "%d", &id);
                    idNode = id;
                }

                if (idNode > maxIdNode)
                    maxIdNode = idNode;

                nodeCount++;
                firstChild = dynamic_cast<m3d::SgNode*>(firstChild->GetNextSibling());
            }

            // Set last ID
            int lastSavedId = lastId;
            int maxId = std::max(nodeCount, std::max(lastSavedId, maxIdNode));
            m_lastId = maxId;

            // Log processing time

            CStr processMsg("%%%% link nodes: ");
            unsigned int processEndTime = M3D_KERNEL->GetTimer().GetCurTime();

            CStr processTimeStr(processEndTime - processStartTime);
            CStr finalMsg(processMsg);
            finalMsg += processTimeStr;

            CStr finalLogMsg(finalMsg);
            M3D_LOG_INFO(finalLogMsg);

            // Cleanup
            return 1;
        }
        else
        {
            // Error handling
            CStr errorMsg("LoadWorld: ");
            errorMsg += err;

            M3D_LOG_INFO(errorMsg);

            return 0;
        }
    }

    void CWorld::LoadStaticObstacles()
    {
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

                        SafeStrAttrib(buf, node, "min");
                        auto min = strToVec(buf);

                        SafeStrAttrib(buf, node, "max");
                        auto max = strToVec(buf);

                        SafeStrAttrib(buf, node, "origin");
                        auto origin = strToVec(buf);

                        SafeStrAttrib(buf, node, "rotation");
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
