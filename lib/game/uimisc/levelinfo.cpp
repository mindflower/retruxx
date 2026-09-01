#include "levelinfo.h"
#include <stdexcept>

#include <cstdlib>

#include "client.h"
#include "config.h"
#include "level.h"
#include "world.h"
#include "core/kernel.h"
#include "core/console/console.h"
#include "core/ref_ptr.h"
#include "game/m3dgame.h"
#include "core/log.h"
#include "core/ini.h"
#include "core/scoped_ptr.h"
#include "file/fileserver.h"
#include "file/filestream.h"
#include "game/music/blockmusicmanager.h"
#include "game/uimisc/guihelper.h"
#include "game/uimisc/visibilitymap.h"
#include "server/server.h"
#include "server/objects/base/obj.h"
#include "server/objects/base/objcontainer.h"
#include "server/objects/base/physicobj.h"
#include "server/objects/base/prototypeinfo.h"
#include "server/objects/base/prototypemanager.h"
#include "server/objects/location.h"
#include "server/objects/town.h"
#include "server/utils.h"

RT_CLASS_EXPORT_METHOD_DEFINE(LevelInfoManager, AddKnownLevel)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(LevelInfoManager, AddVisibilityCircleForLevel)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(LevelInfoManager, AddVisibilityRectForLevel)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(LevelInfoManager, IsLevelKnown)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(LevelInfoManager, IsLevelVisited)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(LevelInfoManager, ClearVisibilityMapForLevel)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORTS_BEGIN(LevelInfoManager)
RT_CLASS_EXPORT(LevelInfoManager, m3d::METHOD, AddKnownLevel, "", "", "")
RT_CLASS_EXPORT(LevelInfoManager, m3d::METHOD, AddVisibilityCircleForLevel, "", "", "")
RT_CLASS_EXPORT(LevelInfoManager, m3d::METHOD, AddVisibilityRectForLevel, "", "", "")
RT_CLASS_EXPORT(LevelInfoManager, m3d::METHOD, IsLevelKnown, "", "", "")
RT_CLASS_EXPORT(LevelInfoManager, m3d::METHOD, IsLevelVisited, "", "", "")
RT_CLASS_EXPORT(LevelInfoManager, m3d::METHOD, ClearVisibilityMapForLevel, "", "", "")
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(LevelInfoManager);

// =================================================================================================
// LevelInfo
// =================================================================================================

LevelInfo::LevelInfo()
{
    m_north.x = 0.0f;
    m_north.y = 0.0f;
    m_north.z = -1.0f;
}

LevelInfo::~LevelInfo()
{
    auto* renderer = M3D_APP->m_renderer;
    renderer->ReleaseTexture(m_image0);
    renderer->ReleaseTexture(m_image1);
    for (auto& [name, tex] : m_splashes)
    {
        renderer->ReleaseTexture(tex);
    }
}

CStr const& LevelInfo::GetName() const
{
    return m_name;
}

CStr const& LevelInfo::GetFullName() const
{
    return m_fullName;
}

CStr const& LevelInfo::GetFile() const
{
    return m_file;
}

CStr const& LevelInfo::GetDiz0() const
{
    return m_diz0;
}

CStr const& LevelInfo::GetDiz1() const
{
    return m_diz1;
}

m3d::rend::TexHandle LevelInfo::GetImage0() const
{
    return m_image0;
}

m3d::rend::TexHandle LevelInfo::GetImage1() const
{
    return m_image1;
}

CVector const& LevelInfo::GetNorth() const
{
    return m_north;
}

CStr LevelInfo::GetRandomMusicBlock() const
{
    if (m_musicBlockNames.empty())
    {
        return {};
    }
    return m_musicBlockNames[static_cast<size_t>(rand()) % m_musicBlockNames.size()];
}

retruxx::vector<m3d::rend::TexHandle> LevelInfo::GetSplashes()
{
    retruxx::vector<m3d::rend::TexHandle> result;
    for (auto const& splashName : m_splasheNames)
    {
        if (auto const it = m_splashes.find(splashName); it == m_splashes.end())
        {
            auto splash = M3D_RENDERER->AddTexture(splashName, 4u);
            if (splash.IsValid())
            {
                m_splashes.insert({splashName, splash});
                result.push_back(splash);
            }
        }
        else
        {
            if (it->second.IsValid())
            {
                result.push_back(it->second);
            }
        }
    }
    return result;
}

int LevelInfo::LoadFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
{
    if (!xmlFile || !xmlNode || xmlNode->IsEmpty())
    {
        M3D_LOG_INFO("Error loading a LevelInfo - invalid node or file");
        return 0;
    }

    m3d::SafeStrAttrib(m_name, xmlNode, "name");
    if (m_name.empty())
    {
        M3D_LOG_INFO("Error loading a LevelInfo - empty level name");
        return 0;
    }
    m3d::SafeStrAttrib(m_file, xmlNode, "file");
    if (m_file.empty())
    {
        M3D_LOG_INFO("Error loading a LevelInfo - empty file name");
        return 0;
    }
    m3d::SafeStrAttrib(m_fullName, xmlNode, "fullName");
    m3d::SafeStrAttrib(m_diz0, xmlNode, "diz0");
    m3d::SafeStrAttrib(m_diz1, xmlNode, "diz1");
    m3d::SafeStrAttrib(m_imageFile0, xmlNode, "image0");
    m3d::SafeStrAttrib(m_imageFile1, xmlNode, "image1");
    m3d::SafeVectorAttrib(m_north, xmlNode, "north");

    CStr splashes;
    m3d::SafeStrAttrib(splashes, xmlNode, "splashes");
    ai::StrToStringVector(splashes, m_splasheNames);

    CStr musicBlocks;
    m3d::SafeStrAttrib(musicBlocks, xmlNode, "musicBlocks");
    ai::StrToStringVector(musicBlocks, m_musicBlockNames);

    auto* app = dynamic_cast<CMiracle3d*>(m3d::Application::g_pApp);
    if (m3d::g_Kernel->GetEngineCfg().m_mus_Enable.GetB())
    {
        for (auto const& block : m_musicBlockNames)
        {
            if (!app->GetBlockMusicManager()->IsMusicBlockNameValid(block))
            {
                SYS_ERROR("Error: invalid music block name: '" + block + "' for level info '" + m_name);
            }
        }
    }

    if (!m_imageFile1.empty())
    {
        m_image1 = app->m_renderer->AddTexture(m_imageFile1, 2);
    }
    return 1;
}

int LevelInfo::LoadBigImage()
{
    if (m_imageFile0.empty() || m_image0.IsValid())
    {
        return 1;
    }
    m_image0 = M3D_APP->m_renderer->AddTexture(m_imageFile0, 4);
    return m_image0.IsValid() ? 1 : 0;
}

// =================================================================================================
// LevelInfoManager
// =================================================================================================

LevelInfoManager::LevelInfoManager() :
    m_nextLevelInfoId(0),
    m_cvVisibilityRadius("minimapVisibilityRadius", "512", m3d::CVar::CVAR_INT, m3d::CVar::CVAR_ARCHIVE)
{
    m3d::g_Kernel->GetEngineCfg().m_console->RegisterCVar(&m_cvVisibilityRadius, nullptr);
}

LevelInfoManager::LevelInfoManager(LevelInfoManager const&) : m_nextLevelInfoId(0)
{
}

LevelInfoManager::~LevelInfoManager()
{
    ClearBeforeNewLevel();
    ClearOnce();
}

m3d::Class* LevelInfoManager::GetBaseClass()
{
    return RT_CLASS_LOCAL(Object);
}

m3d::Class* LevelInfoManager::GetClass() const
{
    return RT_CLASS_LOCAL(LevelInfoManager);
}

m3d::Object* LevelInfoManager::CreateObject()
{
    return new LevelInfoManager;
}

m3d::Object* LevelInfoManager::Clone()
{
    return new LevelInfoManager(*this);
}

int LevelInfoManager::Init()
{
    ClearBeforeNewLevel();
    ClearOnce();
    auto* app = dynamic_cast<CMiracle3d*>(m3d::Application::g_pApp);
    auto fileName = app->m_pInterfaceManager->GetPathToLevelInfoFile();

    auto res = LoadLevelInfoFromXml(fileName) & 1;
    res = LoadLevelSizes() & res;
    if (res)
    {
        M3D_LOG_INFO("LevelInfoManager was inited successfully");
    }
    else
    {
        M3D_LOG_ERR("LevelInfoManager was inited with errors");
    }
    return res;
}

int LevelInfoManager::GameDataUpdate(void* data, int dataType)
{
    switch (dataType - 71)
    {
    case 0:
        OnLocationStateChanged(data);
        return 1;
    case 7:
        OnTownRuined(data);
        return 1;
    case 0xE:
        OnStartLevel();
        return 1;
    case 0xF:
        ClearBeforeNewLevel();
        return 1;
    case 0x10:
        ClearBeforeContinuousLevel();
        return 1;
    case 0x13:
        UpdateVisibilityMaps();
        return 1;
    default:
        return 1;
    }
}

// --- lookup / accessors --------------------------------------------------------------------------

LevelInfo* LevelInfoManager::GetLevelInfoById(int id) const
{
    auto const it = m_levels.find(id);
    return it != m_levels.end() ? it->second : nullptr;
}

int LevelInfoManager::GetLevelInfoId(CStr const& name) const
{
    for (auto const& [id, info] : m_levels)
    {
        if (info && info->GetName() == name)
        {
            return id;
        }
    }
    return -1;
}

CStr LevelInfoManager::GetLevelInfoName(int id) const
{
    auto const it = m_levels.find(id);
    if (it == m_levels.end() || !it->second)
    {
        return {};
    }
    return it->second->GetName();
}

LevelInfo* LevelInfoManager::GetLevelInfoByName(CStr const& levelName)
{
    auto const it = m_levels.find(GetLevelInfoId(levelName));
    return it != m_levels.end() ? it->second : nullptr;
}

LevelInfo const* LevelInfoManager::GetLevelInfoByName(CStr const& levelName) const
{
    return const_cast<LevelInfoManager*>(this)->GetLevelInfoByName(levelName);
}

float LevelInfoManager::GetLevelSize(CStr const& levelName) const
{
    auto const it = m_levelSizes.find(levelName);
    return it != m_levelSizes.end() ? it->second : 0.0f;
}

retruxx::map<CStr, ObjectInfo*>* LevelInfoManager::GetObjectsForLevel(CStr const& levelName)
{
    auto const it = m_levelObjects.find(levelName);
    return it != m_levelObjects.end() ? &it->second : nullptr;
}

retruxx::map<CStr, ObjectInfo*> const* LevelInfoManager::GetObjectsForLevel(CStr const& levelName) const
{
    auto const it = m_levelObjects.find(levelName);
    return it != m_levelObjects.end() ? &it->second : nullptr;
}

ObjectInfo* LevelInfoManager::GetObjectInfo(CStr const& objectName, CStr const& levelName) const
{
    auto const* objects = GetObjectsForLevel(levelName);
    if (!objects)
    {
        return nullptr;
    }
    auto const it = objects->find(objectName);
    return it != objects->end() ? it->second : nullptr;
}

VisibilityMap* LevelInfoManager::GetVisibilityMapForLevel(CStr const& levelName) const
{
    auto const it = m_visibilityMaps.find(levelName);
    return it != m_visibilityMaps.end() ? it->second : nullptr;
}

int LevelInfoManager::GetVisibilityRadius() const
{
    return m_cvVisibilityRadius.GetI();
}

void LevelInfoManager::GetAllLevelNames(retruxx::vector<CStr>& allLevelNames) const
{
    allLevelNames.clear();
    for (auto const& [id, info] : m_levels)
    {
        if (info)
        {
            allLevelNames.push_back(info->GetName());
        }
    }
}

void LevelInfoManager::GetVisitedLevelNames(retruxx::vector<CStr>& visitedLevelNames) const
{
    visitedLevelNames.clear();
    for (auto const& level : m_visitedLevels)
    {
        visitedLevelNames.push_back(level);
    }
}

void LevelInfoManager::GetKnownLevelNames(retruxx::vector<CStr>& knownLevelNames) const
{
    knownLevelNames.clear();
    for (auto const& level : m_knownLevels)
    {
        knownLevelNames.push_back(level);
    }
}

bool LevelInfoManager::IsLevelKnown(CStr const& levelName) const
{
    return m_knownLevels.find(levelName) != m_knownLevels.end();
}

bool LevelInfoManager::IsLevelVisited(CStr const& levelName) const
{
    return m_visitedLevels.find(levelName) != m_visitedLevels.end();
}

// --- known / visited levels --------------------------------------------------------------------

int LevelInfoManager::AddKnownLevel(CStr const& levelName)
{
    retruxx::vector<CStr> levels;
    GetAllLevelNames(levels);
    if (std::find(levels.begin(), levels.end(), levelName) == levels.end())
    {
        M3D_LOG_INFO("LevelInfoManager::AddKnownLevel error - level with name '" + levelName + "' does not exist");
        return 0;
    }

    auto const res = m_knownLevels.insert(levelName);
    if (res.second)
    {
        M3D_APP->EnqueueMessage(65675 /* UM_KNOWN_LEVEL_ADDED */, 0, 0, 0, 0, levelName, {});
    }
    return 1;
}

int LevelInfoManager::AddVisitedLevel(CStr const& levelName)
{
    retruxx::vector<CStr> levelNames;
    GetAllLevelNames(levelNames);
    if (std::find(levelNames.begin(), levelNames.end(), levelName) == levelNames.end())
    {
        M3D_LOG_INFO("LevelInfoManager::AddVisitedLevel error - level with name '" + levelName + "' does not exist");
        return 0;
    }
    m_visitedLevels.insert(levelName);
    return 1;
}

void LevelInfoManager::ClearKnownLevels()
{
    m_knownLevels.clear();
}

void LevelInfoManager::ClearVisitedLevels()
{
    m_visitedLevels.clear();
}

void LevelInfoManager::UpdateKnownLevels()
{
    CStr curLevelName;
    if (m3d::pClient && m3d::pClient->GetWorld().m_level)
    {
        curLevelName = m3d::pClient->GetWorld().m_level->GetLevelName();
    }
    AddKnownLevel(curLevelName);

    if (auto const* objects = GetObjectsForLevel(curLevelName))
    {
        for (auto const& [name, info] : *objects)
        {
            if (info && info->IsLocation() && info->IsActive())
            {
                if (CStr const* passage = info->GetPassageAddress())
                {
                    AddKnownLevel(ai::Location::GetLevelNameFromPassageAddress(*passage));
                }
            }
        }
    }
}

// --- visibility maps ----------------------------------------------------------------------------

VisibilityMap* LevelInfoManager::AddVisibilityMapForLevel(CStr const& levelName)
{
    if (levelName.empty())
    {
        return nullptr;
    }
    auto const it = m_visibilityMaps.find(levelName);
    if (it != m_visibilityMaps.end())
    {
        return it->second;
    }
    auto* visMap = new VisibilityMap;
    if (!visMap->SetUpForLevel(levelName))
    {
        delete visMap;
        return nullptr;
    }
    m_visibilityMaps.insert({levelName, visMap});
    return visMap;
}

int LevelInfoManager::AddVisibilityCircleForLevel(CStr const& levelName, CVector const& origin, float radius)
{
    if (VisibilityMap* map = AddVisibilityMapForLevel(levelName))
    {
        return map->ShowArea(origin, radius);
    }
    M3D_LOG_INFO(
        "LevelInfoManager::AddVisibilityRadiusForLevel error - cannot create visibility map for level " + levelName);
    return 0;
}

int LevelInfoManager::AddVisibilityRectForLevel(CStr const& levelName, float x0, float y0, float w, float h)
{
    if (VisibilityMap* map = AddVisibilityMapForLevel(levelName))
    {
        return map->ShowArea(BoundsBase<float>{x0, y0, w, h});
    }
    M3D_LOG_INFO(
        "LevelInfoManager::AddVisibilityRadiusForLevel error - cannot create visibility map for level " + levelName);
    return 0;
}

void LevelInfoManager::ClearVisibilityMapForLevel(CStr const& levelName)
{
    if (VisibilityMap* map = GetVisibilityMapForLevel(levelName))
    {
        map->SetUpForLevel(levelName);
    }
    else
    {
        M3D_LOG_INFO(
            "LevelInfoManager::AddVisibilityRadiusForLevel error - cannot find visibility map for level " + levelName);
    }
}

void LevelInfoManager::ClearVisibilityMaps()
{
    for (auto& [name, visMap] : m_visibilityMaps)
    {
        delete visMap;
    }
    m_visibilityMaps.clear();
}

void LevelInfoManager::UpdateVisibilityMaps()
{
    CStr curLevel;
    if (m3d::pClient && m3d::pClient->GetWorld().m_level)
    {
        curLevel = m3d::pClient->GetWorld().m_level->GetLevelName();
    }
    if (VisibilityMap* map = GetVisibilityMapForLevel(curLevel))
    {
        map->UpdateGrid();
    }
}

// --- level objects ----------------------------------------------------------------------------

int LevelInfoManager::AddObjectInfo(ObjectInfo* oi)
{
    if (!oi || oi->m_levelName.empty() || oi->m_name.empty())
    {
        M3D_LOG_INFO("LevelInfoManager::AddObjectInfo error - invalid param");
        return 0;
    }
    auto& objects = m_levelObjects[oi->m_levelName];
    if (objects.find(oi->m_name) != objects.end())
    {
        M3D_LOG_INFO(
            "LevelInfoManager::AddObjectInfo error - ObjectInfo (m_name = " + oi->m_name +
            ", m_levelName = " + oi->m_levelName + " already exists");
        return 0;
    }
    objects[oi->m_name] = oi;
    return 1;
}

void LevelInfoManager::ClearLevelObjects()
{
    for (auto& [levelName, objects] : m_levelObjects)
    {
        for (auto& [name, info] : objects)
        {
            delete info;
        }
    }
    m_levelObjects.clear();
}

void LevelInfoManager::ClearObjectIds(CStr const& levelName)
{
    if (auto* objects = GetObjectsForLevel(levelName))
    {
        for (auto& [name, info] : *objects)
        {
            if (info)
            {
                info->m_objectId = -1;
            }
        }
    }
}

void LevelInfoManager::ClearNonPersistantPriceInfoForLevel(CStr const& levelName)
{
    if (auto* objects = GetObjectsForLevel(levelName))
    {
        for (auto& [name, info] : *objects)
        {
            if (info && !info->m_bSavedPricesPersistant)
            {
                info->m_savedPrices.clear();
            }
        }
    }
}

int LevelInfoManager::SaveObjectInfoForObject(ai::Obj const* o)
{
    if (!o)
    {
        return 0;
    }

    CStr curLevel;
    if (m3d::pClient && m3d::pClient->GetWorld().m_level)
    {
        curLevel = m3d::pClient->GetWorld().m_level->GetLevelName();
    }

    // Already tracking this object on this level? Just refresh its live object id.
    if (ObjectInfo* existing = GetObjectInfo(CStr(o->GetName()), curLevel))
    {
        existing->m_objectId = o->GetId();
        return 1;
    }

    ai::PrototypeInfo const* proto = o->GetPrototypeInfo();
    if (!proto)
    {
        return 0;
    }

    auto* oi = new ObjectInfo;
    oi->m_name = o->GetName();
    oi->m_objectId = o->GetId();
    oi->m_fullName = ai::pServer->GetFullNameByObjID(o->GetId());
    oi->m_prototypeName = proto->m_prototypeName;
    oi->m_prototypeId = o->GetPrototypeId();
    oi->m_levelName = help::GetCurrentLevelName();
    if (o->IsKindOf(&ai::PhysicObj::m_classPhysicObj))
    {
        oi->m_position = static_cast<ai::PhysicObj const*>(o)->GetPosition();
    }
    oi->m_belong = o->GetBelong();

    if (o->IsKindOf(&ai::Town::m_classTown))
    {
        oi->m_bRuined = static_cast<ai::Town const*>(o)->IsRuined();
    }
    else if (o->IsKindOf(&ai::Location::m_classLocation))
    {
        oi->m_passageAddress = static_cast<ai::Location const*>(o)->GetPassageAddress();
        oi->m_bIsActive = o->GetPropertyById(53).GetAsID() != 0;  // property 53 = "active"
    }

    if (AddObjectInfo(oi))
    {
        return 1;
    }
    delete oi;
    return 0;
}

bool LevelInfoManager::IsObjectCharted(ai::Obj const* o) const
{
    if (!o)
    {
        return false;
    }
    m3d::Class const* cl = o->GetClass();
    if (!cl || !cl->m_className)
    {
        return false;
    }

    bool const nameMatches = CStr::my_strcmp(cl->m_className, "Town") == 0 ||
        CStr::my_strcmp(cl->m_className, "Lair") == 0 || CStr::my_strcmp(cl->m_className, "Location") == 0;
    if (!nameMatches)
    {
        return false;
    }

    // Towns and lairs are always charted; a Location is charted only when it is a passage.
    if (!o->IsKindOf(&ai::Location::m_classLocation))
    {
        return true;
    }
    return static_cast<ai::Location const*>(o)->GetLocationType() == ai::Location::LOCATION_PASSAGE;
}

void LevelInfoManager::UpdateObjectInfoForCurrentLevel()
{
    for (ai::Obj const* obj : *ai::theObjects)
    {
        if (IsObjectCharted(obj))
        {
            SaveObjectInfoForObject(obj);
        }
    }
}

void LevelInfoManager::UpdateObjectPositions()
{
    // TODO: refresh m_position of current-level ObjectInfos from their live PhysicObj.
    RETRUXX_NOT_IMPLEMENTED;
}

// --- images -----------------------------------------------------------------------------------

void LevelInfoManager::UpdateLevelImages()
{
    retruxx::vector<CStr> visitedLevelNames;
    GetVisitedLevelNames(visitedLevelNames);
    for (auto const& [id, info] : m_levels)
    {
        if (info)
        {
            auto const it = std::find(visitedLevelNames.begin(), visitedLevelNames.end(), info->GetName());
            if (it != visitedLevelNames.end())
            {
                info->LoadBigImage();
            }
        }
    }
}

// --- XML ------------------------------------------------------------------------------------

int LevelInfoManager::LoadLevelInfoFromXml(CStr const& fileName)
{
    scoped_ptr stream = m3d::g_Kernel->GetFileServer().CreateFileStream();
    if (!stream->Open(fileName.c_str(), m3d::fs::IStream::OPEN_READ))
    {
        M3D_LOG_INFO("Can't open file " + fileName + " for read.");
        return 0;
    }

    ref_ptr xmlFile = m3d::g_Kernel->CreateXmlFile();
    if (!xmlFile->Read(*stream))
    {
        M3D_LOG_INFO("Error: cannot parse " + fileName);
        return 0;
    }
    stream->Close();

    ref_ptr rootNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
    xmlFile->GetFirstChild(rootNode, "LevelInfoResource");
    if (rootNode->IsEmpty())
    {
        M3D_LOG_INFO("LevelInfoManager: file " + fileName + " is empty");
        return 1;
    }

    ref_ptr node = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
    for (rootNode->GetFirstChild(node, "LevelInfo"); !node->IsEmpty(); node->GetNextSibling(node, "LevelInfo"))
    {
        auto* info = new LevelInfo;
        if (info->LoadFromXml(xmlFile, node))
        {
            m_levels.insert({++m_nextLevelInfoId, info});
        }
        else
        {
            delete info;
        }
    }
    M3D_LOG_INFO("Level infos were loaded successfully from file " + fileName);
    return 1;
}

int LevelInfoManager::LoadLevelSizes()
{
    m_levelSizes.clear();
    int res = 1;
    for (auto const& [id, info] : m_levels)
    {
        res = LoadLevelSizeFromXml(info->GetFile()) & res;
    }
    return res;
}

int LevelInfoManager::LoadLevelSizeFromXml(CStr const& fileName)
{
    ref_ptr file = m3d::g_Kernel->CreateIniFile();
    scoped_ptr stream = m3d::g_Kernel->GetFileServer().CreateFileStream();
    if (!stream->Open(fileName.c_str(), m3d::fs::IStream::OPEN_READ))
    {
        M3D_LOG_INFO("Error: could't open " + fileName + " file");
        return 0;
    }

    file->Read(*stream);
    stream->Close();
    if (auto const* error = file->GetError())
    {
        M3D_LOG_INFO("Error: Parse error file " + fileName + " err " + error);
        return 0;
    }

    CStr level = file->GetString("LEVEL", "LEVELNAME");
    int levelSize = file->GetInteger("LEVEL", "LEVELSIZE");
    float size = static_cast<float>(levelSize) * 128.0f;  // cell edge length -> world units
    m_levelSizes.insert({level, size});
    return 1;
}

int LevelInfoManager::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
{
    // TODO: save-game restore of <VisibilityMaps>/KnownLevels/VisitedLevels/<LevelObjects>.
    RETRUXX_NOT_IMPLEMENTED;
}

int LevelInfoManager::SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
{
    // TODO: mirror of LoadFromXml.
    RETRUXX_NOT_IMPLEMENTED;
}

// --- clear / hooks --------------------------------------------------------------------------

void LevelInfoManager::ClearBeforeNewLevel()
{
    ClearLevelObjects();
    ClearVisibilityMaps();
    m_knownLevels.clear();
    m_visitedLevels.clear();
}

void LevelInfoManager::ClearBeforeContinuousLevel()
{
    CStr curLevel;
    if (m3d::pClient && m3d::pClient->GetWorld().m_level)
    {
        curLevel = m3d::pClient->GetWorld().m_level->GetLevelName();
    }
    ClearNonPersistantPriceInfoForLevel(curLevel);
    ClearObjectIds(curLevel);
}

void LevelInfoManager::ClearOnce()
{
    for (auto& [id, info] : m_levels)
    {
        delete info;
    }
    m_levels.clear();
    m_nextLevelInfoId = 0;
    m_levelSizes.clear();
}

void LevelInfoManager::OnStartLevel()
{
    CStr curLevel;
    if (m3d::pClient && m3d::pClient->GetWorld().m_level)
    {
        curLevel = m3d::pClient->GetWorld().m_level->GetLevelName();
    }
    AddVisitedLevel(curLevel);
    AddVisibilityMapForLevel(curLevel);
    UpdateLevelImages();
    UpdateObjectInfoForCurrentLevel();
    UpdateKnownLevels();
}

void LevelInfoManager::OnEndLevel()
{
    ClearBeforeNewLevel();
}

void LevelInfoManager::OnEndLevelBeforeContinuousLevel()
{
    ClearBeforeContinuousLevel();
}

void LevelInfoManager::OnNewFrame()
{
    UpdateVisibilityMaps();
}

void LevelInfoManager::OnLocationStateChanged(void*)
{
    // TODO: resolve ai::Location from event payload; update ObjectInfo::m_bIsActive (prop 53);
    //       AddKnownLevel(GetLevelNameFromPassageAddress(...)) while active.
    RETRUXX_NOT_IMPLEMENTED;
}

void LevelInfoManager::OnTownRuined(void*)
{
    // TODO: resolve ai::Town from event payload; ObjectInfo::m_bRuined = Town::IsRuined();
    //       on ruined -> ObjectInfo::DeleteSavedPrices().
    RETRUXX_NOT_IMPLEMENTED;
}

// =================================================================================================
// ObjectInfo
// =================================================================================================

ObjectInfo::ObjectInfo() :
    m_prototypeId(-1),
    m_bSavedPricesPersistant(false),
    m_bRuined(false),
    m_bIsActive(false),
    m_objectId(-1),
    m_belong(1000)
{
    m_position.x = 0.0f;
    m_position.y = 0.0f;
    m_position.z = 0.0f;
}

ObjectInfo::~ObjectInfo() = default;

int ObjectInfo::GetId() const
{
    return m_objectId;
}

CStr const& ObjectInfo::GetName() const
{
    return m_name;
}

CStr const& ObjectInfo::GetFullName() const
{
    return m_fullName;
}

CStr const& ObjectInfo::GetPrototypeName() const
{
    return m_prototypeName;
}

int ObjectInfo::GetPrototypeId() const
{
    return m_prototypeId;
}

CVector const& ObjectInfo::GetPosition() const
{
    return m_position;
}

CStr const& ObjectInfo::GetLevelName() const
{
    return m_levelName;
}

int ObjectInfo::GetBelong() const
{
    return m_belong;
}

bool ObjectInfo::IsRuined() const
{
    return m_bRuined;
}

bool ObjectInfo::IsActive() const
{
    return m_bIsActive;
}

bool ObjectInfo::IsSavedPricesPersistant() const
{
    return m_bSavedPricesPersistant;
}

void ObjectInfo::SetSavedPricesPersistant(bool bPersistant)
{
    m_bSavedPricesPersistant = bPersistant;
}

CStr const* ObjectInfo::GetPassageAddress() const
{
    return (IsLocation() && m_bIsActive) ? &m_passageAddress : nullptr;
}

retruxx::map<int, CVector2> const* ObjectInfo::GetSavedPrices() const
{
    return IsTown() ? &m_savedPrices : nullptr;
}

void ObjectInfo::DeleteSavedPrices()
{
    m_savedPrices.clear();
}

int ObjectInfo::AddSavedPriceByPrototypeId(int prototypeId, int sellPrice, int buyPrice)
{
    if (!IsTown())
    {
        return 0;
    }
    m_savedPrices[prototypeId] = CVector2{static_cast<float>(sellPrice), static_cast<float>(buyPrice)};
    return 1;
}

m3d::Class const* ObjectInfo::GetObjectClass() const
{
    int const id = ai::thePrototypeManager->GetPrototypeId(m_prototypeName);
    ai::PrototypeInfo const* proto = ai::thePrototypeManager->GetPrototypeInfo(id);
    if (!proto)
    {
        return nullptr;
    }
    return m3d::g_Kernel->FindClass(proto->m_className.c_str());
}

bool ObjectInfo::IsTown() const
{
    m3d::Class const* cl = GetObjectClass();
    return cl && cl->IsKindOf(&ai::Town::m_classTown);
}

bool ObjectInfo::IsLocation() const
{
    m3d::Class const* cl = GetObjectClass();
    return cl && cl->IsKindOf(&ai::Location::m_classLocation);
}

int ObjectInfo::SavePrices()
{
    // TODO: needs help::GetWarePricesForTown(town, m_savedPrices) once the pricing helper exists.
    RETRUXX_NOT_IMPLEMENTED;
}

int ObjectInfo::LoadFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
{
    if (!xmlFile || !xmlNode)
    {
        M3D_LOG_INFO("ObjectInfo::LoadFromXml error - invalid params");
        return 0;
    }

    m3d::SafeStrAttrib(m_name, xmlNode, "Name");
    if (m_name.empty())
    {
        M3D_LOG_INFO("ObjectInfo::LoadFromXml error - empty name");
        return 0;
    }
    m3d::SafeStrAttrib(m_levelName, xmlNode, "LevelName");
    if (m_levelName.empty())
    {
        M3D_LOG_INFO("ObjectInfo::LoadFromXml error - empty level name");
        return 0;
    }

    m_objectId = -1;
    m3d::SafeStrAttrib(m_fullName, xmlNode, "FullName");
    m3d::SafeStrAttrib(m_prototypeName, xmlNode, "PrototypeName");
    m_prototypeId = ai::thePrototypeManager->GetPrototypeId(m_prototypeName);
    m3d::SafeVectorAttrib(m_position, xmlNode, "Position");
    m3d::SafeIntAttrib(m_belong, xmlNode, "Belong");

    if (IsTown())
    {
        ref_ptr savePricesNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
        xmlNode->GetFirstChild(savePricesNode, "SavePrices");
        if (!savePricesNode->IsEmpty())
        {
            ref_ptr priceNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
            for (savePricesNode->GetFirstChild(priceNode, "Price"); !priceNode->IsEmpty();
                 priceNode->GetNextSibling(priceNode, "Price"))
            {
                int protoId = -1;
                m3d::SafeIntAttrib(protoId, priceNode, "PrototypeId");
                CVector2 value{-1.0f, -1.0f};
                m3d::SafeVector2Attrib(value, priceNode, "Value");
                m_savedPrices[protoId] = value;
            }
        }
        m3d::SafeBoolAttrib(m_bRuined, xmlNode, "IsRuined");
        m3d::SafeBoolAttrib(m_bSavedPricesPersistant, xmlNode, "IsPricesPersistant");
    }
    else if (IsLocation())
    {
        m3d::SafeStrAttrib(m_passageAddress, xmlNode, "PassageAddress");
        m3d::SafeBoolAttrib(m_bIsActive, xmlNode, "IsActive");
    }
    return 1;
}

int ObjectInfo::SaveToXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
{
    if (!xmlFile || !xmlNode)
    {
        M3D_LOG_INFO("ObjectInfo::SaveToXml error - invalid params");
        return 0;
    }

    xmlNode->SetAttribute("Name", m_name.c_str());
    xmlNode->SetAttribute("FullName", m_fullName.c_str());
    xmlNode->SetAttribute("PrototypeName", m_prototypeName.c_str());
    xmlNode->SetAttribute("Position", CStr(m_position).c_str());
    xmlNode->SetAttribute("LevelName", m_levelName.c_str());
    xmlNode->SetAttribute("Belong", CStr(m_belong).c_str());

    if (IsTown())
    {
        if (!m_savedPrices.empty())
        {
            ref_ptr savePricesNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "SavePrices");
            xmlNode->AddChild(savePricesNode);
            for (auto const& [protoId, value] : m_savedPrices)
            {
                ref_ptr priceNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Price");
                savePricesNode->AddChild(priceNode);
                priceNode->SetAttribute("PrototypeId", CStr(protoId).c_str());
                priceNode->SetAttribute("Value", CStr::format_("%.3f %.3f", value.x, value.y).c_str());
            }
        }
        xmlNode->SetAttribute("IsRuined", CStr(m_bRuined).c_str());
        xmlNode->SetAttribute("IsPricesPersistant", CStr(m_bSavedPricesPersistant).c_str());
    }
    else if (IsLocation())
    {
        xmlNode->SetAttribute("PassageAddress", m_passageAddress.c_str());
        xmlNode->SetAttribute("IsActive", CStr(m_bIsActive).c_str());
    }
    return 1;
}
