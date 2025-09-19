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


namespace ai
{
    extern CServer* pServer;
}

namespace m3d
{
    bool CWorld::GetShadowVisibilityFromWeather() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    WeatherManager& CWorld::GetWeatherManager()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void CWorld::Invalidate()
    {
        throw retruxx::logic_error("Not implemented");
    }

    WheelTraceMgr& CWorld::GetWheelTracesMgr()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void CWorld::Render()
    {
        m_landscape.UpdateVis(true);
        m_landscape.Render();
        ai::gDynamicScene->RenderDebugInfo();
    }

    int CWorld::RenderSky(Landscape::LandRenderMode)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void CWorld::New(CCamera&, int, float)
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    int CWorld::Load(CStr const& levelname, CCamera& cam, bool bQuiet)
    {
        //TODO: implement CWorld::Load

        auto timeStart = M3D_KERNEL->GetTimer().GetCurTime();
        M3D_LOG_INFO("----------------------- World Loading");
        M3D_KERNEL->GetEngineCfg().m_levFileName.Set(levelname.c_str(), true);
        if (m_level->Load(levelname, cam, bQuiet) == 0)
        {
            M3D_LOG_INFO("Level file " + levelname + " not found");
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
        M3D_LOG_INFO("----------------------- Landscape loaded in: " + CStr(landscapeEnd - landscapeStart));

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

        if (!CreatePrefabsFromFile("prefabs.xml") && !CreatePrefabsFromFile("data\\models\\prefabs.xml"))
        {
            M3D_LOG_INFO("Could not find prefabs.xml");
        }

        M3D_APP->PostLoadServers();

        auto const serversEnd = M3D_KERNEL->GetTimer().GetCurTime();
        M3D_LOG_INFO("----------------------- Servers loaded in: " + CStr(serversEnd - landscapeEnd));

        const auto worldBegin = M3D_KERNEL->GetTimer().GetCurTime();
        if (!bQuiet)
        {
            const auto str = M3D_APP->GetStringByStringId0("LoadWorld");
            M3D_APP->PutSplash(100, str.c_str());
        }

        LoadWorld(m_level->GetFullPathNameA("world.xml"));

        const auto worldEnd = M3D_KERNEL->GetTimer().GetCurTime();
        M3D_LOG_INFO("----------------------- Nodes loaded in: " + CStr(worldEnd - worldBegin));

        const auto initWorldBegin = M3D_KERNEL->GetTimer().GetCurTime();
        if (!bQuiet)
        {
            const auto str = M3D_APP->GetStringByStringId0("InitWorld");
            M3D_APP->PutSplash(100, str.c_str());
        }

        const auto initWorldEnd = M3D_KERNEL->GetTimer().GetCurTime();
        M3D_LOG_INFO("----------------------- Prefabs loaded in: " + CStr(initWorldEnd - initWorldBegin));

        ProcessCollisionStuff();
        m_weatherManager.ReadFromXmlFile(M3D_KERNEL->GetEngineCfg().m_weather_ConfigFile.GetS());
        m_weatherManager.UpdateDayTime();

        const auto worldLoadedEnd = M3D_KERNEL->GetTimer().GetCurTime();
        M3D_LOG_INFO("----------------------- World loaded in: " + CStr(worldLoadedEnd - timeStart));

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
            this->m_borderWallGeoms[1] = dCreatePlane(ai::gGlobalSpace, -1.0, 0.0, 0.0, 0.0 - this->m_level->m_maxSafex);
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
            this->m_borderWallGeoms[4] = dCreatePlane(ai::gGlobalSpace, 0.0, 0.0, -1.0, 0.0 - this->m_level->m_maxSafey);
        dGeomSetCategoryBits(this->m_borderWallGeoms[4], 1u);
        dGeomSetCollideBits(this->m_borderWallGeoms[4], 0xFFFFFFFE);




        throw retruxx::logic_error("Not implemented");
        return 1;
    }

    bool CWorld::SaveWorld(CStr const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void CWorld::RefreshObjectsOnLandscapeRect(CVector const&, float)
    {
        throw retruxx::logic_error("Not implemented");
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
        this->m_profilerUpdateOde = Application::g_pApp->GetProfilerStack().GetProfiler(Application::g_pApp->GetProfilerStack().AddProfiler("AI other update", 0x1Eu));
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
        throw retruxx::logic_error("Not implemented");
    }

    void CWorld::ReleasePrefabs()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int CWorld::CreateGeomsRepresentingSceneNodes()
    {
        throw retruxx::logic_error("Not implemented");
    }

    dxSpace* CWorld::GetOdeSpace()
    {
        throw retruxx::logic_error("Not implemented");
    }

    unsigned CWorld::GetWeatherSunColor() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    unsigned CWorld::GetWeatherPlantColor() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void CWorld::Release()
    {
        throw retruxx::logic_error("Not implemented");
    }

    unsigned CWorld::GetWeatherSpecularColor() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    ai::Vehicle* CWorld::GetVehicleControlledByPlayer()
    {
        throw retruxx::logic_error("Not implemented");
    }

    float CWorld::GetSForShadowsFromWeather() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void CWorld::ProcessCollisionStuffOnNode(SgNode*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    float CWorld::GetShadowTransparencyFromWeather() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    GlobalTimeParams CWorld::GetCurDayTimeFromWeatherManager() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void CWorld::Restore()
    {
        throw retruxx::logic_error("Not implemented");
    }

    CVector CWorld::GetScatteringSunColor() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    retruxx::vector<CStr, retruxx::allocator<CStr>> const& CWorld::GetFxNames() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    unsigned CWorld::GetWeatherAmbientColor() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int CWorld::GetFxId(CStr const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    CVector const& CWorld::GetSun(float) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    CWorld::~CWorld()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void CWorld::Update()
    {
        throw retruxx::logic_error("Not implemented");
    }

    unsigned CWorld::GetWeatherDiffuseColor() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    Landscape& CWorld::GetLandscape()
    {
        return m_landscape;
    }

    void CWorld::ProcessCollisionStuff()
    {
        throw retruxx::logic_error("Not implemented");
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
        const char* error = xmlFile->GetError();
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
            const char* file = filesNode->GetAttribute("file");
            PrefabFiles.push_back(CStr(file));
            filesNode->GetNextSibling(filesNode, "Item");
        }

        // Process each prefab file
        for (unsigned int i = 0; i < PrefabFiles.size(); ++i)
        {
            const CStr& prefabFile = PrefabFiles[i];

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
                    const char* name = prefabsNode->GetAttribute("name");

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
                                CStr warning = "Warning: multieffect child names doesn't match for parent name. For effect " + CStr(name);
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

    SgNode* CWorld::CreatePrefabsNode(int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void CWorld::UpdateSkyParams()
    {
        throw retruxx::logic_error("Not implemented");
    }

    float CWorld::GetSunAscention() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    float CWorld::GetFogReduceFactorFromWeather() const
    {
        return m_weatherManager.GetFogReduceFactorFromWeather();
    }

    SgNode* CWorld::ReadPrefab(ref_ptr<cmn::XmlFile> file, ref_ptr<cmn::XmlNode> pnode)
    {
        // TODO: generated code

        // Get node attributes
        const char* name = pnode->GetAttribute("name");
        const char* className = pnode->GetAttribute("class");

        // Create new node instance
        m3d::SgNode* loader = dynamic_cast<m3d::SgNode*>(m3d::g_Kernel->New(className));

        // Try to load from XML
        if (loader->ReadFromXmlNode(file, pnode) &&
            loader->ReadFromXmlNodeAfterAdd(file, pnode))
        {
            // Successfully loaded - process the node hierarchy
            m3d::SceneGraph* graph = loader->GetGraph();
            std::set<m3d::SgNode*>& thinkList = graph->m_thinkList;

            // Remove loader from think list temporarily
            thinkList.erase(loader);

            // Process node hierarchy using a stack
            std::vector<m3d::Object*> stack;
            stack.push_back(loader);

            while (!stack.empty())
            {
                m3d::Object* current = stack.back();
                stack.pop_back();

                // Process children
                m3d::SgNode* child = static_cast<m3d::SgNode*>(current->GetFirstChild());
                while (child)
                {
                    // Remove child from think list
                    auto range = thinkList.equal_range(child);
                    thinkList.erase(range.first, range.second);

                    // If this child has children, add to stack for processing
                    if (child->GetFirstChild())
                    {
                        stack.push_back(child);
                    }

                    child = static_cast<m3d::SgNode*>(child->GetNextSibling());
                }
            }
        }
        else
        {
            CStr errorMsg = "Error: Couldn't load prefab '" + CStr(name) +
                "'. Check if you have child nodes with duplicate names.";

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
            CStr loadMsg(loadTimeMsg);;
            M3D_LOG_INFO(logMsg);

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
        if (fileStream->Open(m_level->GetFullPathNameA(m_level->m_staticObstaclesFileName).c_str(), fs::IStream::OPEN_READ))
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
}
