#include "levelinfo.h"
#include <stdexcept>

#include "config.h"
#include "core/kernel.h"
#include "core/console/console.h"
#include "game/m3dgame.h"
#include "core/log.h"
#include "core/scoped_ptr.h"
#include "file/fileserver.h"
#include "file/filestream.h"
#include "game/music/blockmusicmanager.h"
#include "game/uimisc/visibilitymap.h"
#include "server/utils.h"

RT_CLASS_EXPORT_METHOD_DEFINE(LevelInfoManager, AddKnownLevel)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(LevelInfoManager, AddVisibilityCircleForLevel)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(LevelInfoManager, AddVisibilityRectForLevel)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(LevelInfoManager, IsLevelKnown)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(LevelInfoManager, IsLevelVisited)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(LevelInfoManager, ClearVisibilityMapForLevel)
{
    throw std::logic_error("Not implemented");
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
	throw std::logic_error("Not implemented");
}

CStr const& LevelInfo::GetFullName() const
{
	throw std::logic_error("Not implemented");
}

m3d::rend::TexHandle LevelInfo::GetImage0() const
{
	throw std::logic_error("Not implemented");
}

m3d::rend::TexHandle LevelInfo::GetImage1() const
{
	throw std::logic_error("Not implemented");
}

std::vector<m3d::rend::TexHandle> LevelInfo::GetSplashes()
{
	throw std::logic_error("Not implemented");
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
	throw std::logic_error("Not implemented");
}

LevelInfo::~LevelInfo()
{
	throw std::logic_error("Not implemented");
}

CStr const& LevelInfo::GetDiz1() const
{
	throw std::logic_error("Not implemented");
}

CVector const& LevelInfo::GetNorth() const
{
	throw std::logic_error("Not implemented");
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
	throw std::logic_error("Not implemented");
}

int LevelInfo::LoadBigImage()
{
	throw std::logic_error("Not implemented");
}

void LevelInfoManager::ClearBeforeNewLevel()
{
    ClearLevelObjects();
    ClearVisibilityMaps();
    m_knownLevels.clear();
    m_visitedLevels.clear();
}

void LevelInfoManager::GetVisitedLevelNames(std::vector<CStr>&) const
{
    throw std::logic_error("Not implemented");
}

void LevelInfoManager::ClearLevelObjects()
{
    for (auto obj : m_levelObjects)
    {
	    for (auto info : obj.second)
	    {
            throw std::logic_error("Not implemented");
            delete info.second;
	    }
    }
    m_levelObjects.clear();
}

void LevelInfoManager::GetAllLevelNames(std::vector<CStr>&) const
{
    throw std::logic_error("Not implemented");
}

int LevelInfoManager::AddVisibilityCircleForLevel(CStr const&, CVector const&, float)
{
    throw std::logic_error("Not implemented");
}

void LevelInfoManager::OnTownRuined(void*)
{
    throw std::logic_error("Not implemented");
}

void LevelInfoManager::UpdateKnownLevels()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* LevelInfoManager::Clone()
{
    throw std::logic_error("Not implemented");
}

void LevelInfoManager::UpdateLevelImages()
{
    throw std::logic_error("Not implemented");
}

void LevelInfoManager::UpdateObjectInfoForCurrentLevel()
{
    throw std::logic_error("Not implemented");
}

void LevelInfoManager::UpdateObjectPositions()
{
    throw std::logic_error("Not implemented");
}

void LevelInfoManager::OnEndLevel()
{
    throw std::logic_error("Not implemented");
}

int LevelInfoManager::AddVisibilityRectForLevel(CStr const&, float, float, float, float)
{
    throw std::logic_error("Not implemented");
}

LevelInfo* LevelInfoManager::GetLevelInfoById(int) const
{
    throw std::logic_error("Not implemented");
}

int LevelInfoManager::SaveObjectInfoForObject(ai::Obj const*)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* LevelInfoManager::CreateObject()
{
    return new LevelInfoManager;
}

void LevelInfoManager::ClearVisibilityMapForLevel(CStr const&)
{
    throw std::logic_error("Not implemented");
}

VisibilityMap* LevelInfoManager::GetVisibilityMapForLevel(CStr const&) const
{
    throw std::logic_error("Not implemented");
}

bool LevelInfoManager::IsObjectCharted(ai::Obj const*) const
{
    throw std::logic_error("Not implemented");
}

VisibilityMap* LevelInfoManager::AddVisibilityMapForLevel(CStr const&)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* LevelInfoManager::GetBaseClass()
{
    return RT_CLASS_LOCAL(Object);
}

void LevelInfoManager::ClearNonPersistantPriceInfoForLevel(CStr const&)
{
    throw std::logic_error("Not implemented");
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
    throw std::logic_error("Not implemented");
}

void LevelInfoManager::ClearObjectIds(CStr const&)
{
    throw std::logic_error("Not implemented");
}

bool LevelInfoManager::IsLevelKnown(CStr const&) const
{
    throw std::logic_error("Not implemented");
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
    throw std::logic_error("Not implemented");
}

void LevelInfoManager::OnNewFrame()
{
    throw std::logic_error("Not implemented");
}

int LevelInfoManager::AddVisitedLevel(CStr const&)
{
    throw std::logic_error("Not implemented");
}

std::map<CStr, ObjectInfo*> const* LevelInfoManager::GetObjectsForLevel(CStr const&) const
{
    throw std::logic_error("Not implemented");
}

std::map<CStr, ObjectInfo*>* LevelInfoManager::GetObjectsForLevel(CStr const&)
{
    throw std::logic_error("Not implemented");
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
    throw std::logic_error("Not implemented");
}

void LevelInfoManager::ClearVisitedLevels()
{
    throw std::logic_error("Not implemented");
}

LevelInfoManager::~LevelInfoManager()
{
    throw std::logic_error("Not implemented");
}

void LevelInfoManager::OnEndLevelBeforeContinuousLevel()
{
    throw std::logic_error("Not implemented");
}

int LevelInfoManager::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
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
            xmlFile->GetFirstChild_(rootNode, "LevelInfoResource");
            if (rootNode->IsEmpty())
            {
                M3D_LOG_INFO("LevelInfoManager: file " + fileName + " is empty");
                return 1;
            }
            ref_ptr node = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
            for (rootNode->GetFirstChild_(node, "LevelInfo"); !node->IsEmpty(); node->GetNextSibling_(node, "LevelInfo"))
            {
                auto info = new LevelInfo;
                if (info->LoadFromXml(xmlFile, node))
                {
                    //TODO: check this
                    m_levels.emplace(++m_nextLevelInfoId, info);
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
    throw std::logic_error("Not implemented");
}

LevelInfo* LevelInfoManager::GetLevelInfoByName(CStr const&)
{
    throw std::logic_error("Not implemented");
}

LevelInfo const* LevelInfoManager::GetLevelInfoByName(CStr const&) const
{
    throw std::logic_error("Not implemented");
}

int LevelInfoManager::AddObjectInfo(ObjectInfo*)
{
    throw std::logic_error("Not implemented");
}

void LevelInfoManager::OnLocationStateChanged(void*)
{
    throw std::logic_error("Not implemented");
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
        m_levelSizes.emplace(level, size);
        return 1;
    }
    M3D_LOG_INFO("Error: could't open " + fileName + " file");
    return 0;
}

ObjectInfo* LevelInfoManager::GetObjectInfo(CStr const&, CStr const&) const
{
    throw std::logic_error("Not implemented");
}

int LevelInfoManager::AddKnownLevel(CStr const&)
{
    throw std::logic_error("Not implemented");
}

int LevelInfoManager::GetLevelInfoId(CStr const&) const
{
    throw std::logic_error("Not implemented");
}

void LevelInfoManager::ClearKnownLevels()
{
    throw std::logic_error("Not implemented");
}

void LevelInfoManager::GetKnownLevelNames(std::vector<CStr>&) const
{
    throw std::logic_error("Not implemented");
}

CStr LevelInfoManager::GetLevelInfoName(int) const
{
    throw std::logic_error("Not implemented");
}

int LevelInfoManager::SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
{
    throw std::logic_error("Not implemented");
}

void LevelInfoManager::UpdateVisibilityMaps()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* LevelInfoManager::GetClass() const
{
    return RT_CLASS_LOCAL(LevelInfoManager);
}

float LevelInfoManager::GetLevelSize(CStr const&) const
{
    throw std::logic_error("Not implemented");
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
    throw std::logic_error("Not implemented");
}

LevelInfoManager::LevelInfoManager() :
	m_cvVisibilityRadius("minimapVisibilityRadius", "512", m3d::CVar::CVAR_INT, m3d::CVar::CVAR_ARCHIVE)
{
    m3d::g_Kernel->GetEngineCfg().m_console->RegisterCVar(&m_cvVisibilityRadius, nullptr);
}

LevelInfoManager::LevelInfoManager(LevelInfoManager const&)
{
    throw std::logic_error("Not implemented");
}

ObjectInfo::ObjectInfo()
{
	throw std::logic_error("Not implemented");
}

bool ObjectInfo::IsSavedPricesPersistant() const
{
	throw std::logic_error("Not implemented");
}

int ObjectInfo::SavePrices()
{
	throw std::logic_error("Not implemented");
}

int ObjectInfo::GetId() const
{
	throw std::logic_error("Not implemented");
}

int ObjectInfo::GetBelong() const
{
	throw std::logic_error("Not implemented");
}

int ObjectInfo::SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
{
	throw std::logic_error("Not implemented");
}

void ObjectInfo::SetSavedPricesPersistant(bool)
{
	throw std::logic_error("Not implemented");
}

CStr const* ObjectInfo::GetPassageAddress() const
{
	throw std::logic_error("Not implemented");
}

int ObjectInfo::GetPrototypeId() const
{
	throw std::logic_error("Not implemented");
}

bool ObjectInfo::IsTown() const
{
	throw std::logic_error("Not implemented");
}

int ObjectInfo::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
{
	throw std::logic_error("Not implemented");
}

bool ObjectInfo::IsLocation() const
{
	throw std::logic_error("Not implemented");
}

ObjectInfo::~ObjectInfo()
{
	throw std::logic_error("Not implemented");
}

CStr const& ObjectInfo::GetLevelName() const
{
	throw std::logic_error("Not implemented");
}

CStr const& ObjectInfo::GetName() const
{
	throw std::logic_error("Not implemented");
}

int ObjectInfo::AddSavedPriceByPrototypeId(int, int, int)
{
	throw std::logic_error("Not implemented");
}

std::map<int, CVector2> const* ObjectInfo::GetSavedPrices() const
{
	throw std::logic_error("Not implemented");
}

CStr const& ObjectInfo::GetPrototypeName() const
{
	throw std::logic_error("Not implemented");
}

CVector const& ObjectInfo::GetPosition() const
{
	throw std::logic_error("Not implemented");
}

bool ObjectInfo::IsActive() const
{
	throw std::logic_error("Not implemented");
}

bool ObjectInfo::IsRuined() const
{
	throw std::logic_error("Not implemented");
}

void ObjectInfo::DeleteSavedPrices()
{
	throw std::logic_error("Not implemented");
}

CStr const& ObjectInfo::GetFullName() const
{
	throw std::logic_error("Not implemented");
}

m3d::Class const* ObjectInfo::GetObjectClass() const
{
	throw std::logic_error("Not implemented");
}
