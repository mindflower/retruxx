#include "levelinfo.h"
#include <stdexcept>

#include "client.h"
#include "config.h"
#include "level.h"
#include "world.h"
#include "core/kernel.h"
#include "core/console/console.h"
#include "game/m3dgame.h"
#include "core/log.h"
#include "core/ini.h"
#include "core/scoped_ptr.h"
#include "file/fileserver.h"
#include "file/filestream.h"
#include "game/music/blockmusicmanager.h"
#include "game/uimisc/visibilitymap.h"
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

CStr const& LevelInfo::GetName() const
{
    return m_name;
}

CStr const& LevelInfo::GetFullName() const
{
	RETRUXX_NOT_IMPLEMENTED;
}

m3d::rend::TexHandle LevelInfo::GetImage0() const
{
	RETRUXX_NOT_IMPLEMENTED;
}

m3d::rend::TexHandle LevelInfo::GetImage1() const
{
	RETRUXX_NOT_IMPLEMENTED;
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

CStr const& LevelInfo::GetFile() const
{
    return m_file;
}

LevelInfo::LevelInfo()
{
}

CStr const& LevelInfo::GetDiz0() const
{
	RETRUXX_NOT_IMPLEMENTED;
}

LevelInfo::~LevelInfo()
{
	RETRUXX_NOT_IMPLEMENTED;
}

CStr const& LevelInfo::GetDiz1() const
{
	RETRUXX_NOT_IMPLEMENTED;
}

CVector const& LevelInfo::GetNorth() const
{
	RETRUXX_NOT_IMPLEMENTED;
}

int LevelInfo::LoadFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
{
    if (xmlFile && xmlNode && !xmlNode->IsEmpty())
    {
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
        auto app = dynamic_cast<CMiracle3d*>(m3d::Application::g_pApp);
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
            return 1;
        }
    }
    else
    {
        M3D_LOG_INFO("Error loading a LevelInfo - invalid node or file");
    }
    return 0;
}

CStr LevelInfo::GetRandomMusicBlock() const
{
	RETRUXX_NOT_IMPLEMENTED;
}

int LevelInfo::LoadBigImage()
{
    if (m_imageFile0.empty() || m_image0.IsValid())
    {
        return 1;
    }
    m_image0 = M3D_APP->m_renderer->AddTexture(m_imageFile0, 4);
    if (!m_image0.IsValid())
    {
        return 0;
    }
    return 1;
}

void LevelInfoManager::ClearBeforeNewLevel()
{
    ClearLevelObjects();
    ClearVisibilityMaps();
    m_knownLevels.clear();
    m_visitedLevels.clear();
}

void LevelInfoManager::GetVisitedLevelNames(retruxx::vector<CStr>& visitedLevelNames) const
{
    visitedLevelNames.clear();
    for (auto const& level : m_visitedLevels)
    {
        visitedLevelNames.push_back(level);
    }
}

void LevelInfoManager::ClearLevelObjects()
{
    for (auto obj : m_levelObjects)
    {
	    for (auto info : obj.second)
	    {
            RETRUXX_NOT_IMPLEMENTED;
            delete info.second;
	    }
    }
    m_levelObjects.clear();
}

void LevelInfoManager::GetAllLevelNames(retruxx::vector<CStr>& allLevelNames) const
{
    allLevelNames.clear();
    for (auto const& level : m_levels)
    {
        if (level.second)
        {
            allLevelNames.push_back(level.second->GetName());
        }
    }
}

int LevelInfoManager::AddVisibilityCircleForLevel(CStr const&, CVector const&, float)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void LevelInfoManager::OnTownRuined(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void LevelInfoManager::UpdateKnownLevels()
{
    CStr curLevelName;
    if (m3d::pClient && m3d::pClient->GetWorld().m_level)
    {
        curLevelName = m3d::pClient->GetWorld().m_level->GetLevelName();
    }
    AddKnownLevel(curLevelName);

    retruxx::vector<CStr> allLevels;
    GetAllLevelNames(allLevels);

    auto it = m_levelObjects.find(curLevelName);
    if (it != m_levelObjects.end())
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}

m3d::Object* LevelInfoManager::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void LevelInfoManager::UpdateLevelImages()
{
    retruxx::vector<CStr> visitedLevelNames;
    GetVisitedLevelNames(visitedLevelNames);
    //TODO: check this!!
    for (auto const& level : m_levels)
    {
        if (level.second)
        {
            auto const it = std::find(visitedLevelNames.begin(), visitedLevelNames.end(), level.second->GetName());
            if (it != visitedLevelNames.end())
            {
                level.second->LoadBigImage();
            }
        }
    }
}

void LevelInfoManager::UpdateObjectInfoForCurrentLevel()
{
    for (auto it = ai::theObjects->begin(); it != ai::theObjects->end(); ++it)
    {
        bool tasd = true;
    }
    //RETRUXX_NOT_IMPLEMENTED;
}

void LevelInfoManager::UpdateObjectPositions()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void LevelInfoManager::OnEndLevel()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int LevelInfoManager::AddVisibilityRectForLevel(CStr const&, float, float, float, float)
{
    RETRUXX_NOT_IMPLEMENTED;
}

LevelInfo* LevelInfoManager::GetLevelInfoById(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int LevelInfoManager::SaveObjectInfoForObject(ai::Obj const*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* LevelInfoManager::CreateObject()
{
    return new LevelInfoManager;
}

void LevelInfoManager::ClearVisibilityMapForLevel(CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

VisibilityMap* LevelInfoManager::GetVisibilityMapForLevel(CStr const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool LevelInfoManager::IsObjectCharted(ai::Obj const*) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

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
    auto visMap = new VisibilityMap;
    if (!visMap->SetUpForLevel(levelName))
    {
        delete visMap;
        return nullptr;
    }
    m_visibilityMaps.insert({levelName, visMap});
    return visMap;
}

m3d::Class* LevelInfoManager::GetBaseClass()
{
    return RT_CLASS_LOCAL(Object);
}

void LevelInfoManager::ClearNonPersistantPriceInfoForLevel(CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void LevelInfoManager::ClearOnce()
{
    for (auto level : m_levels)
    {
        delete level.second;
    }
    m_levels.clear();
    m_levelSizes.clear();
    m_nextLevelInfoId = 0;
}

int LevelInfoManager::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void LevelInfoManager::ClearObjectIds(CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool LevelInfoManager::IsLevelKnown(CStr const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int LevelInfoManager::Init()
{
    ClearBeforeNewLevel();
    ClearOnce();
    auto app = dynamic_cast<CMiracle3d*>(m3d::Application::g_pApp);
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

int LevelInfoManager::GetVisibilityRadius() const
{
    return m_cvVisibilityRadius.GetI();
}

void LevelInfoManager::OnNewFrame()
{
    RETRUXX_NOT_IMPLEMENTED;
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

retruxx::map<CStr, ObjectInfo*> const* LevelInfoManager::GetObjectsForLevel(CStr const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

retruxx::map<CStr, ObjectInfo*>* LevelInfoManager::GetObjectsForLevel(CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int LevelInfoManager::LoadLevelSizes()
{
    m_levelSizes.clear();
    int res = 1;
    for (auto const& level : m_levels)
    {
        res = LoadLevelSizeFromXml(level.second->GetFile()) & res;
    }
    return res;
}

void LevelInfoManager::ClearBeforeContinuousLevel()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void LevelInfoManager::ClearVisitedLevels()
{
    RETRUXX_NOT_IMPLEMENTED;
}

LevelInfoManager::~LevelInfoManager()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void LevelInfoManager::OnEndLevelBeforeContinuousLevel()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int LevelInfoManager::GameDataUpdate(void* data, int dataType)
{
    int result = 0;
    switch (dataType - 71)
    {
    case 0u:
        OnLocationStateChanged(data);
        result = 1;
        break;
    case 7u:
        OnTownRuined( data);
        result = 1;
        break;
    case 0xEu:
        OnStartLevel();
        result = 1;
        break;
    case 0xFu:
        ClearBeforeNewLevel();
        result = 1;
        break;
    case 0x10u:
        ClearBeforeContinuousLevel();
        result = 1;
        break;
    case 0x13u:
        UpdateVisibilityMaps();
        result = 1;
        break;
    default:
        result = 1;
        break;
    }
    return result;
}

int LevelInfoManager::LoadLevelInfoFromXml(CStr const& fileName)
{
    int result = 0;
    scoped_ptr stream = m3d::g_Kernel->GetFileServer().CreateFileStream();
    if (stream->Open(fileName.c_str(), m3d::fs::IStream::OPEN_READ))
    {
        ref_ptr xmlFile = m3d::g_Kernel->CreateXmlFile();
        if (xmlFile->Read(*stream))
        {
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
                auto info = new LevelInfo;
                if (info->LoadFromXml(xmlFile, node))
                {
                    //TODO: check this
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
        else
        {
            M3D_LOG_INFO("Error: cannot parse " + fileName);
            return 0;
        }
    }
    M3D_LOG_INFO("Can't open file " + fileName + " for read.");
    return 0;
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

LevelInfo* LevelInfoManager::GetLevelInfoByName(CStr const& levelName)
{
    auto id = GetLevelInfoId(levelName);
    auto it = m_levels.find(id);
    if (it != m_levels.end())
    {
        return it->second;
    }
    return nullptr;
}

LevelInfo const* LevelInfoManager::GetLevelInfoByName(CStr const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int LevelInfoManager::AddObjectInfo(ObjectInfo*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void LevelInfoManager::OnLocationStateChanged(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int LevelInfoManager::LoadLevelSizeFromXml(CStr const& fileName)
{
    ref_ptr file = m3d::g_Kernel->CreateIniFile();
    scoped_ptr stream = m3d::g_Kernel->GetFileServer().CreateFileStream();
    if (stream->Open(fileName.c_str(), m3d::fs::IStream::OPEN_READ))
    {
        file->Read(*stream);
        stream->Close();
        auto error = file->GetError();
        if (error)
        {
            M3D_LOG_INFO("Error: Parse error file " + fileName + " err " + error);
            return 0;
        }
        CStr level = file->GetString("LEVEL", "LEVELNAME");
        int levelSize = file->GetInteger("LEVEL", "LEVELSIZE");
        float size = levelSize* 128.0;
        m_levelSizes.insert({level, size});
        return 1;
    }
    M3D_LOG_INFO("Error: could't open " + fileName + " file");
    return 0;
}

ObjectInfo* LevelInfoManager::GetObjectInfo(CStr const&, CStr const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int LevelInfoManager::AddKnownLevel(CStr const& levelName)
{
    retruxx::vector<CStr> levels;
    GetAllLevelNames(levels);
    if (std::find(levels.begin(), levels.end(), levelName) == levels.end())
    {
        M3D_LOG_INFO("LevelInfoManager::AddKnownLevel error - level with name '" + levelName + "' does not exist");
        return 0;
    }

    m_knownLevels.insert(levelName);
    M3D_APP->EnqueueMessage(65675, 0, 0, 0, 0, levelName, {});
    return 1;
}

int LevelInfoManager::GetLevelInfoId(CStr const& name) const
{
    for (auto&[ id, info ] : m_levels)
    {
        if (info->GetName() == name)
        {
            return id;
        }
    }
    return -1;
}

void LevelInfoManager::ClearKnownLevels()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void LevelInfoManager::GetKnownLevelNames(retruxx::vector<CStr>&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr LevelInfoManager::GetLevelInfoName(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int LevelInfoManager::SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void LevelInfoManager::UpdateVisibilityMaps()
{
    // TODO: implement LevelInfoManager::UpdateVisibilityMaps
    //RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* LevelInfoManager::GetClass() const
{
    return RT_CLASS_LOCAL(LevelInfoManager);
}

float LevelInfoManager::GetLevelSize(CStr const& levelName) const
{
    auto const it = m_levelSizes.find(levelName);
    if (it != m_levelSizes.end())
    {
        return it->second;
    }
    return 0.0;
}

void LevelInfoManager::ClearVisibilityMaps()
{
    for (auto visMap : m_visibilityMaps)
    {
        delete visMap.second;
    }
    m_visibilityMaps.clear();
}

bool LevelInfoManager::IsLevelVisited(CStr const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

LevelInfoManager::LevelInfoManager() :
	m_cvVisibilityRadius("minimapVisibilityRadius", "512", m3d::CVar::CVAR_INT, m3d::CVar::CVAR_ARCHIVE)
{
    m3d::g_Kernel->GetEngineCfg().m_console->RegisterCVar(&m_cvVisibilityRadius, nullptr);
}

LevelInfoManager::LevelInfoManager(LevelInfoManager const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ObjectInfo::ObjectInfo()
{
	RETRUXX_NOT_IMPLEMENTED;
}

bool ObjectInfo::IsSavedPricesPersistant() const
{
	RETRUXX_NOT_IMPLEMENTED;
}

int ObjectInfo::SavePrices()
{
	RETRUXX_NOT_IMPLEMENTED;
}

int ObjectInfo::GetId() const
{
	RETRUXX_NOT_IMPLEMENTED;
}

int ObjectInfo::GetBelong() const
{
	RETRUXX_NOT_IMPLEMENTED;
}

int ObjectInfo::SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
{
	RETRUXX_NOT_IMPLEMENTED;
}

void ObjectInfo::SetSavedPricesPersistant(bool)
{
	RETRUXX_NOT_IMPLEMENTED;
}

CStr const* ObjectInfo::GetPassageAddress() const
{
	RETRUXX_NOT_IMPLEMENTED;
}

int ObjectInfo::GetPrototypeId() const
{
	RETRUXX_NOT_IMPLEMENTED;
}

bool ObjectInfo::IsTown() const
{
	RETRUXX_NOT_IMPLEMENTED;
}

int ObjectInfo::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
{
	RETRUXX_NOT_IMPLEMENTED;
}

bool ObjectInfo::IsLocation() const
{
	RETRUXX_NOT_IMPLEMENTED;
}

ObjectInfo::~ObjectInfo()
{
	RETRUXX_NOT_IMPLEMENTED;
}

CStr const& ObjectInfo::GetLevelName() const
{
	RETRUXX_NOT_IMPLEMENTED;
}

CStr const& ObjectInfo::GetName() const
{
	RETRUXX_NOT_IMPLEMENTED;
}

int ObjectInfo::AddSavedPriceByPrototypeId(int, int, int)
{
	RETRUXX_NOT_IMPLEMENTED;
}

retruxx::map<int, CVector2> const* ObjectInfo::GetSavedPrices() const
{
	RETRUXX_NOT_IMPLEMENTED;
}

CStr const& ObjectInfo::GetPrototypeName() const
{
	RETRUXX_NOT_IMPLEMENTED;
}

CVector const& ObjectInfo::GetPosition() const
{
	RETRUXX_NOT_IMPLEMENTED;
}

bool ObjectInfo::IsActive() const
{
	RETRUXX_NOT_IMPLEMENTED;
}

bool ObjectInfo::IsRuined() const
{
	RETRUXX_NOT_IMPLEMENTED;
}

void ObjectInfo::DeleteSavedPrices()
{
	RETRUXX_NOT_IMPLEMENTED;
}

CStr const& ObjectInfo::GetFullName() const
{
	RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class const* ObjectInfo::GetObjectClass() const
{
	RETRUXX_NOT_IMPLEMENTED;
}
