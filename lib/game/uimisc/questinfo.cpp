#include "questinfo.h"
#include "core/scoped_ptr.h"
#include "file/fileserver.h"
#include "file/filestream.h"
#include "server/objects/dynamicquest.h"
#include "server/objects/dynamicquestconvoy.h"

#include "core/ini.h"
#include "core/log.h"
#include "core/ref_ptr.h"

#include <math/vector.h>

#include <stdexcept>

#include "game/m3dgame.h"
#include "server/objects/base/objcontainer.h"
#include "server/objects/base/physicobj.h"
#include "server/objects/team.h"
#include "server/objects/vehicle.h"
#include "server/quest.h"

// =================================================================================================
// QuestInfo::AuxLevelInfo
// =================================================================================================

QuestInfo::AuxLevelInfo::AuxLevelInfo() : m_coordinate(nullptr), m_coordType(COORDTYPE_NUM_COORDTYPES)
{
}

QuestInfo::AuxLevelInfo::AuxLevelInfo(AuxLevelInfo const& rhs) :
    m_coordinate(nullptr),
    m_coordType(COORDTYPE_NUM_COORDTYPES)
{
    m_levelName = rhs.m_levelName;
    m_coordType = rhs.m_coordType;
    m_targetObjName = rhs.m_targetObjName;
    SetCoordinate(rhs.m_coordinate);
}

QuestInfo::AuxLevelInfo& QuestInfo::AuxLevelInfo::operator=(AuxLevelInfo const& rhs)
{
    if (&rhs != this)
    {
        m_levelName = rhs.m_levelName;
        m_coordType = rhs.m_coordType;
        m_targetObjName = rhs.m_targetObjName;
        SetCoordinate(rhs.m_coordinate);
    }
    return *this;
}

QuestInfo::AuxLevelInfo::~AuxLevelInfo()
{
    delete m_coordinate;
}

void QuestInfo::AuxLevelInfo::SetCoordinate(CVector const* coord)
{
    if (coord)
    {
        if (!m_coordinate)
        {
            m_coordinate = new CVector;
        }
        *m_coordinate = *coord;
    }
    else
    {
        delete m_coordinate;
        m_coordinate = nullptr;
    }
}

void QuestInfo::AuxLevelInfo::Invalidate()
{
    delete m_coordinate;
    m_coordinate = nullptr;
    m_targetObjName = "";
    m_coordType = COORDTYPE_NUM_COORDTYPES;
}

void QuestInfo::AuxLevelInfo::SetUp(CStr const& levelName)
{
    Invalidate();
    m_levelName = levelName;
    m_coordType = COORDTYPE_NONE;
}

void QuestInfo::AuxLevelInfo::SetUp(CStr const& levelName, CVector const& coord)
{
    Invalidate();
    m_levelName = levelName;
    m_coordinate = new CVector(coord);
    m_coordType = COORDTYPE_DIRECT;
}

void QuestInfo::AuxLevelInfo::SetUpForObj(CStr const& levelName, CStr const& targetObjName)
{
    Invalidate();
    m_coordType = COORDTYPE_OBJECT;
    m_levelName = levelName;
    m_targetObjName = targetObjName;
}

QuestInfo::AuxLevelInfo::CoordType QuestInfo::AuxLevelInfo::GetCoordType() const
{
    return m_coordType;
}

CStr const& QuestInfo::AuxLevelInfo::GetLevelName() const
{
    return m_levelName;
}

bool QuestInfo::AuxLevelInfo::IsValid() const
{
    switch (m_coordType)
    {
    case COORDTYPE_DIRECT:
        return !m_levelName.empty() && m_coordinate != nullptr;
    case COORDTYPE_OBJECT:
        return !m_levelName.empty() && !m_targetObjName.empty();
    case COORDTYPE_NONE:
        return !m_levelName.empty();
    default:
        return false;
    }
}

CVector const* QuestInfo::AuxLevelInfo::GetCoordinate() const
{
    // Logically const: an object-bound entry caches the resolved world position.
    auto* self = const_cast<AuxLevelInfo*>(this);

    if (m_coordType == COORDTYPE_DIRECT)
    {
        return m_coordinate;
    }
    if (m_coordType != COORDTYPE_OBJECT)
    {
        return nullptr;
    }

    // COORDTYPE_OBJECT resolves a live position only while the player is on this level.
    if (help::GetCurrentLevelName() == m_levelName)
    {
        ai::Obj* entity = ai::theObjects->GetEntityByObjName(m_targetObjName);
        if (!entity)
        {
            return nullptr;
        }

        ai::PhysicObj* physicObj = nullptr;
        if (entity->IsKindOf(&ai::PhysicObj::m_classPhysicObj))
        {
            physicObj = static_cast<ai::PhysicObj*>(entity);
        }
        else if (entity->IsKindOf(&ai::Team::m_classTeam))
        {
            ai::Vehicle* vehicle = static_cast<ai::Team*>(entity)->GetVehicle(0);
            if (!vehicle)
            {
                return nullptr;
            }
            physicObj = static_cast<ai::PhysicObj*>(vehicle);
        }
        else
        {
            return nullptr;
        }

        if (!self->m_coordinate)
        {
            self->m_coordinate = new CVector;
        }
        *self->m_coordinate = physicObj->GetPosition();
        return m_coordinate;
    }

    // A different level: hand back the cached coordinate (zeroed on first use).
    if (!self->m_coordinate)
    {
        self->m_coordinate = new CVector;
        self->m_coordinate->x = 0.0f;
        self->m_coordinate->y = 0.0f;
        self->m_coordinate->z = 0.0f;
    }
    return m_coordinate;
}

int QuestInfo::AuxLevelInfo::LoadFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
{
    Invalidate();

    if (!xmlFile || !xmlNode)
    {
        M3D_LOG_INFO("QuestInfo::AuxLevelInfo::LoadFromXml error - null xmlFile or xmlNode");
        return 0;
    }

    CStr levelName;
    m3d::SafeStrAttrib(levelName, xmlNode, "name");
    if (levelName.empty())
    {
        M3D_LOG_INFO("QuestInfo::AuxLevelInfo::LoadFromXml error - empty map name");
        return 0;
    }

    CStr strCoord;
    CStr targetObjName;
    m3d::SafeStrAttrib(strCoord, xmlNode, "coordinate");
    m3d::SafeStrAttrib(targetObjName, xmlNode, "targetObjName");

    if (!strCoord.empty() && !targetObjName.empty())
    {
        M3D_LOG_INFO(
            "QuestInfo::AuxLevelInfo::LoadFromXml warning - both coordinate and targetObjName were "
            "specified; coordinate would be ignored (map name: " +
            levelName + ")");
    }

    if (!targetObjName.empty())
    {
        SetUpForObj(levelName, targetObjName);
    }
    else if (strCoord.empty())
    {
        SetUp(levelName);
    }
    else
    {
        SetUp(levelName, strToVec(strCoord));
    }

    if (!IsValid())
    {
        M3D_LOG_INFO("QuestInfo::AuxLevelInfo::LoadFromXml error - invalid (map name: " + levelName + ")");
        return 0;
    }
    return 1;
}

int QuestInfo::AuxLevelInfo::SaveToXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
{
    if (!xmlFile || !xmlNode)
    {
        M3D_LOG_INFO("QuestInfo::AuxLevelInfo::SaveToXml error - null xmlFile or xmlNode");
        return 0;
    }

    xmlNode->SetAttribute("name", m_levelName.c_str());
    if (m_coordType == COORDTYPE_DIRECT)
    {
        if (m_coordinate)
        {
            xmlNode->SetAttribute("coordinate", CStr(*m_coordinate).c_str());
        }
    }
    else if (m_coordType == COORDTYPE_OBJECT)
    {
        xmlNode->SetAttribute("targetObjName", m_targetObjName.c_str());
    }
    return 1;
}

// =================================================================================================
// QuestInfo
// =================================================================================================

QuestInfo::QuestInfo() :
    m_questId(-1),
    m_questType(help::QUESTTYPE_NUM_QUEST_TYPES),
    m_isMainQuest(false),
    m_bCoordinateModified(false)
{
}

QuestInfo::~QuestInfo() = default;

CStr const& QuestInfo::GetQuestInfoName() const
{
    return m_questInfoName;
}

int QuestInfo::GetQuestId() const
{
    return m_questId;
}

help::QuestType QuestInfo::GetQuestType() const
{
    return m_questType;
}

CStr const& QuestInfo::GetBriefDiz() const
{
    return m_briefDiz;
}

CStr const& QuestInfo::GetFullDiz() const
{
    return m_fullDiz;
}

CStr const& QuestInfo::GetLiteraryDiz() const
{
    return m_literaryDiz;
}

CStr const& QuestInfo::GetSmallImageFie() const
{
    return m_smallImageFile;
}

CStr const& QuestInfo::GetLargeImageFile() const
{
    return m_largeImageFile;
}

CStr const& QuestInfo::GetHirerName() const
{
    return m_hirerName;
}

bool QuestInfo::IsMainQuest() const
{
    return m_isMainQuest;
}

bool QuestInfo::IsCoordinateModified() const
{
    return m_bCoordinateModified;
}

std::vector<CStr, std::allocator<CStr>> QuestInfo::GetMapNames() const
{
    std::vector<CStr> names;
    for (auto const& entry : m_levelInfo)
    {
        names.push_back(entry.first);
    }
    return names;
}

bool QuestInfo::ExistsOnMap(CStr const& mapName) const
{
    return m_levelInfo.find(mapName) != m_levelInfo.end();
}

CVector const* QuestInfo::GetCoordinateForMap(CStr const& mapName) const
{
    auto it = m_levelInfo.find(mapName);
    if (it == m_levelInfo.end())
    {
        return nullptr;
    }
    return it->second.GetCoordinate();
}

void QuestInfo::SetCoordinateForMap(CStr const& mapName, CVector const& coord)
{
    auto it = m_levelInfo.find(mapName);
    if (it == m_levelInfo.end())
    {
        M3D_LOG_ERR("Error: no data for level '" + mapName + "' exists for quest '" + m_questInfoName + "'");
        return;
    }
    it->second.SetUp(mapName, coord);
    m_bCoordinateModified = true;
}

void QuestInfo::UpdateQuestId()
{
    if (m_questType == help::QUESTTYPE_STATIC)
    {
        m_questId = ai::theQuestManager->GetQuestIdByName(m_questInfoName);
    }
}

void QuestInfo::OnStartLevel()
{
    UpdateQuestId();
}

int QuestInfo::LoadFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
{
    if (!xmlFile || !xmlNode || xmlNode->IsEmpty())
    {
        M3D_LOG_ERR("Error loading a QuestInfo - invalid node or file");
        return 0;
    }

    m3d::SafeStrAttrib(m_questInfoName, xmlNode, "questName");
    if (m_questInfoName.empty())
    {
        M3D_LOG_ERR("Error loading a QuestInfo - empty quest name");
        return 0;
    }

    m_questType = QuestInfoManager::IsQuestDynamic(m_questInfoName) ? help::QUESTTYPE_DYNAMIC : help::QUESTTYPE_STATIC;

    m3d::SafeStrAttrib(m_briefDiz, xmlNode, "briefDiz");
    m3d::SafeStrAttrib(m_fullDiz, xmlNode, "fullDiz");
    m3d::SafeStrAttrib(m_literaryDiz, xmlNode, "literaryDiz");
    m3d::SafeStrAttrib(m_smallImageFile, xmlNode, "smallImage");
    m3d::SafeStrAttrib(m_largeImageFile, xmlNode, "largeImage");
    m3d::SafeStrAttrib(m_hirerName, xmlNode, "hirerName");

    LoadCoordinatesFromXml(xmlFile, xmlNode);
    m3d::SafeBoolAttrib(m_isMainQuest, xmlNode, "isMainQuest");
    return 1;
}

int QuestInfo::LoadCoordinatesFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
{
    if (!xmlFile || !xmlNode || xmlNode->IsEmpty())
    {
        M3D_LOG_ERR("Error loading a QuestInfo - invalid node or file");
        return 0;
    }

    m_levelInfo.clear();

    ref_ptr mapNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
    for (xmlNode->GetFirstChild(mapNode, "Map"); !mapNode->IsEmpty(); mapNode->GetNextSibling(mapNode, "Map"))
    {
        AuxLevelInfo levelInfo;
        if (!levelInfo.LoadFromXml(xmlFile, mapNode))
        {
            M3D_LOG_INFO(
                "QuestInfo::LoadCoordinatesFromXml warning - errors while loading level coordinate info "
                "(questName: " +
                m_questInfoName + ")");
            continue;
        }

        CStr const& mapKey = levelInfo.GetLevelName();
        if (m_levelInfo.find(mapKey) != m_levelInfo.end())
        {
            M3D_LOG_INFO(
                "QuestInfo::LoadCoordinatesFromXml warning - coordinate info for map " + mapKey +
                " were specified multiple times. All infos except the first would be ignored.");
            continue;
        }
        m_levelInfo.insert({mapKey, levelInfo});
    }
    return 1;
}

int QuestInfo::SaveCoordinatesToXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
{
    if (!xmlFile || !xmlNode)
    {
        M3D_LOG_ERR("QuestInfo::SaveToXml error - invalid node or file");
        return 0;
    }

    xmlNode->SetAttribute("questName", m_questInfoName.c_str());
    for (auto const& entry : m_levelInfo)
    {
        ref_ptr mapNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Map");
        xmlNode->AddChild(mapNode);
        entry.second.SaveToXml(xmlFile, mapNode);
    }
    return 1;
}

// =================================================================================================
// QuestInfoManager
// =================================================================================================

namespace
{
    // Quest info patterns used for each dynamic quest type; a concrete info is
    // one of "<pattern>_0" .. "<pattern>_4".
    struct DQuestTypeToName
    {
        int m_dQuestType;
        char const* m_dQuestName;
    };

    DQuestTypeToName const l_dQuestTypeToName[] = {
        {0, "Dynamic_Destroy"},
        {1, "Dynamic_Reach"},
        {2, "Dynamic_Convoy"},
        {3, "Dynamic_Peace"},
        {4, "Dynamic_Hunt"},
    };
}

int QuestInfoManager::SaveModifiedQuestInfosToXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
{
    // RVA 0x573E90 - only quest infos whose coordinates were changed are saved.
    for (auto const& [name, info] : m_questInfos)
    {
        if (info && info->m_bCoordinateModified)
        {
            ref_ptr qiNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "QuestInfo");
            xmlNode->AddChild(qiNode);
            info->SaveCoordinatesToXml(xmlFile, qiNode);
        }
    }
    return 1;
}

QuestInfoManager::QuestInfoManager() = default;

QuestInfo const* QuestInfoManager::GetQuestInfoByQuestInfoName(CStr const& questInfoName) const
{
    // RVA 0x573F50
    auto const it = m_questInfos.find(questInfoName);
    return it != m_questInfos.end() ? it->second : nullptr;
}

void QuestInfoManager::SetCoordinateForQuest(CStr const& questName, CStr const& mapName, CVector const& coordinate)
{
    // RVA 0x574080
    auto const it = m_questInfos.find(questName);
    if (it == m_questInfos.end())
    {
        M3D_LOG_ERR("Error: no quest exists with name '" + questName + "'");
        return;
    }
    it->second->SetCoordinateForMap(mapName, coordinate);
}

CStr QuestInfoManager::GetDynamicQuestInfoPatternNameByType(int type)
{
    // RVA 0x5741D0
    for (auto const& entry : l_dQuestTypeToName)
    {
        if (entry.m_dQuestType == type)
        {
            return entry.m_dQuestName;
        }
    }
    return CStr();
}

QuestInfo const* QuestInfoManager::GetQuestInfoForStaticQuest(int questId) const
{
    // RVA 0x573FA0
    auto* quest = ai::theQuestManager->GetQuestById(questId);
    if (!quest)
    {
        return nullptr;
    }
    return GetQuestInfoByQuestInfoName(quest->GetName());
}

void QuestInfoManager::Clear()
{
    // RVA 0x5731D0
    ClearQuestInfos();
    ClearDynamicQuestInfos();
    m_isInited = false;
}

QuestInfoManager::~QuestInfoManager()
{
    // RVA 0x573160
    ClearQuestInfos();
    ClearDynamicQuestInfos();
    m_isInited = false;
}

int QuestInfoManager::Init()
{
    if (m_isInited)
    {
        ClearQuestInfos();
        ClearDynamicQuestInfos();
        m_isInited = false;
    }

    auto const pathToDlg = M3D_APP->m_pInterfaceManager->GetPathToQuestInfoFileGlobal();
    auto const res = LoadFromFile(pathToDlg);

    m_isInited = true;
    if (res)
    {
        M3D_LOG_INFO("QuestInfo manager was inited successfully");
    }
    else
    {
        M3D_LOG_INFO("QuestInfo manager was inited with errors");
    }

    return 1;
}

int QuestInfoManager::LoadFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* rootNode)
{
    // RVA 0x5733E0 - the first quest info of a given name wins.
    ref_ptr qiNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
    for (rootNode->GetFirstChild(qiNode, "QuestInfo"); !qiNode->IsEmpty(); qiNode->GetNextSibling(qiNode, "QuestInfo"))
    {
        auto* info = new QuestInfo;
        if (info->LoadFromXml(xmlFile, qiNode) && m_questInfos.find(info->m_questInfoName) == m_questInfos.end())
        {
            m_questInfos.insert({info->m_questInfoName, info});
        }
        else
        {
            delete info;
        }
    }
    return 1;
}

bool QuestInfoManager::IsQuestDynamic(CStr const& questInfoName)
{
    // RVA 0x574000
    for (auto const& entry : l_dQuestTypeToName)
    {
        if (questInfoName == entry.m_dQuestName)
        {
            return true;
        }
    }
    return false;
}

int QuestInfoManager::GameDataUpdate(void* data, int dataType)
{
    switch (dataType)
    {
    case 67:
    {
        OnDynamicQuestStateChanged(data);
        return 1;
    }
    case 85:
    {
        OnStartLevel();
        return 1;
    }
    case 86:
    {
        ClearQuestInfos();
        ClearDynamicQuestInfos();
        m_isInited = false;
        return 1;
    }
    case 87:
    {
        ClearDynamicQuestInfos();
        return 1;
    }
    default:
    {
        return 1;
    }
    }
}

QuestInfo const* QuestInfoManager::GetQuestInfoForDynamicQuest(int dQuestId)
{
    // RVA 0x573FC0 - created on first request.
    if (dQuestId == -1)
    {
        return nullptr;
    }
    auto const it = m_dynamicQuestInfos.find(dQuestId);
    if (it != m_dynamicQuestInfos.end())
    {
        return it->second;
    }
    return AddQuestInfoForDynamicQuest(dQuestId);
}

int QuestInfoManager::LoadModifiedQuestInfosFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
{
    // RVA 0x573B90
    ref_ptr qiNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
    for (xmlNode->GetFirstChild(qiNode, "QuestInfo"); !qiNode->IsEmpty(); qiNode->GetNextSibling(qiNode, "QuestInfo"))
    {
        CStr questInfoName;
        m3d::SafeStrAttrib(questInfoName, qiNode, "questName");
        auto const it = m_questInfos.find(questInfoName);
        if (it == m_questInfos.end())
        {
            M3D_LOG_ERR("Error: invalid quest info name: '" + questInfoName + "' in save file");
            continue;
        }
        it->second->LoadCoordinatesFromXml(xmlFile, qiNode);
        it->second->m_bCoordinateModified = true;
    }
    return 1;
}

void QuestInfoManager::OnEndLevel()
{
    // RVA 0x574380
    ClearQuestInfos();
    ClearDynamicQuestInfos();
    m_isInited = false;
}

void QuestInfoManager::ClearQuestInfos()
{
    for (auto& questInfo : m_questInfos)
    {
        delete questInfo.second;
    }

    m_questInfos.clear();
}

QuestInfo* QuestInfoManager::CreateQuestInfoForDynamicQuest(int dQuestId) const
{
    // RVA 0x574530 - a random "<pattern>_N" quest info for the quest's type,
    // with its texts filled in and a map mark for the current level.
    if (dQuestId == -1)
    {
        return nullptr;
    }
    auto* obj = ai::theObjects->GetEntityByObjId(dQuestId);
    if (!obj || !obj->IsKindOf(&ai::DynamicQuest::m_classDynamicQuest))
    {
        return nullptr;
    }
    auto* dQuest = static_cast<ai::DynamicQuest*>(obj);
    auto const type = static_cast<int>(dQuest->GetQuestType());

    std::vector<CStr> patternQuestInfoNames;
    GetAllQuestInfosByNamePattern(GetDynamicQuestInfoPatternNameByType(type), patternQuestInfoNames);
    if (patternQuestInfoNames.empty())
    {
        return nullptr;
    }
    auto const* pattern =
        GetQuestInfoByQuestInfoName(patternQuestInfoNames[rand() % patternQuestInfoNames.size()]);
    if (!pattern)
    {
        return nullptr;
    }

    auto* dQuestInfo = new QuestInfo;
    *dQuestInfo = *pattern;
    dQuestInfo->m_briefDiz = pattern->m_briefDiz;
    dQuestInfo->m_fullDiz = pattern->m_fullDiz;
    dQuestInfo->m_literaryDiz = pattern->m_literaryDiz;
    auto const& stringParser = M3D_APP->m_pInterfaceManager->GetStringParser();
    stringParser.FormatStringForDynamicQuest(dQuestInfo->m_briefDiz, dQuest);
    stringParser.FormatStringForDynamicQuest(dQuestInfo->m_fullDiz, dQuest);
    stringParser.FormatStringForDynamicQuest(dQuestInfo->m_literaryDiz, dQuest);
    dQuestInfo->m_hirerName = dQuest->GetHirerName();

    QuestInfo::AuxLevelInfo levelInfo;
    switch (type)
    {
    case 0:
    case 1:
    case 3:
        levelInfo.SetUpForObj(help::GetCurrentLevelName(), dQuest->GetTargetName());
        break;
    case 2:
        // A convoy quest points at the caravan.
        if (dQuest->IsKindOf(&ai::DynamicQuestConvoy::m_classDynamicQuestConvoy))
        {
            auto const caravanId = static_cast<ai::DynamicQuestConvoy*>(dQuest)->GetCaravanId();
            if (caravanId != -1)
            {
                auto* caravan = ai::theObjects->GetEntityByObjId(caravanId);
                if (caravan && caravan->IsKindOf(&ai::Obj::m_classObj) && !CStr(caravan->GetName()).empty())
                {
                    levelInfo.SetUpForObj(help::GetCurrentLevelName(), CStr(caravan->GetName()));
                }
            }
        }
        break;
    case 4:
        levelInfo.SetUp(help::GetCurrentLevelName());
        break;
    default:
        break;
    }

    if (levelInfo.IsValid())
    {
        dQuestInfo->m_levelInfo.insert({help::GetCurrentLevelName(), levelInfo});
    }
    return dQuestInfo;
}

void QuestInfoManager::OnStartLevel()
{
    //TODO: check this
    for (auto& info : m_questInfos)
    {
        if (info.second)
        {
            if (info.second->m_questType == help::QUESTTYPE_STATIC)
            {
                info.second->m_questId = ai::theQuestManager->GetQuestIdByName(info.second->m_questInfoName);
            }
        }
    }
}

void QuestInfoManager::OnDynamicQuestStateChanged(void* data)
{
    // RVA 0x5743B0
    if (!data)
    {
        return;
    }
    int const dQuestId = static_cast<m3d::Event*>(data)->m_intEv[0];
    auto* obj = ai::theObjects->GetEntityByObjId(dQuestId);
    if (!obj || !obj->IsKindOf(&ai::DynamicQuest::m_classDynamicQuest))
    {
        return;
    }
    if (static_cast<ai::DynamicQuest*>(obj)->GetQuestStatus() == ai::DynamicQuest::STATUS_PROCESSING)
    {
        AddQuestInfoForDynamicQuest(dQuestId);
    }
    else
    {
        RemoveQuestInfoForDynamicQuest(dQuestId);
    }
}

void QuestInfoManager::GetAllQuestInfosByNamePattern(CStr const& questInfoPatternName, std::vector<CStr>& questInfoNames) const
{
    // RVA 0x5749E0 - "<pattern>_0" .. "<pattern>_4" that exist.
    questInfoNames.clear();
    for (int i = 0; i < 5; ++i)
    {
        CStr const qiName = questInfoPatternName + CStr("_") + CStr(i);
        auto const it = m_questInfos.find(qiName);
        if (it != m_questInfos.end() && it->second)
        {
            questInfoNames.push_back(qiName);
        }
    }
}

int QuestInfoManager::LoadFromFile(CStr const& fileName)
{
    // RVA 0x573620
    scoped_ptr stream = m3d::g_Kernel->GetFileServer().CreateFileStream();
    if (!stream->Open(fileName.c_str(), m3d::fs::IStream::OPEN_READ))
    {
        // NOTE: a missing file still counts as success, as in the original.
        M3D_LOG_INFO("Can't open file " + fileName + " for read.");
        return 1;
    }

    ref_ptr xmlFile = m3d::g_Kernel->CreateXmlFile();
    if (!xmlFile->Read(*stream))
    {
        M3D_LOG_INFO("Error: cannot parse " + fileName + " (" + CStr(xmlFile->GetError()) + ") ");
        return 0;
    }
    stream->Close();

    ref_ptr rootNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
    xmlFile->GetFirstChild(rootNode, "QuestInfoResource");
    if (rootNode->IsEmpty())
    {
        M3D_LOG_INFO("QuestInfoManager: file " + fileName + " is empty");
    }
    else
    {
        LoadFromXml(xmlFile, rootNode);
        M3D_LOG_INFO("Quest infos were loaded successfully from file " + fileName);
    }
    return 1;
}

QuestInfo const* QuestInfoManager::AddQuestInfoForDynamicQuest(int dQuestId)
{
    // RVA 0x574420
    if (dQuestId == -1)
    {
        return nullptr;
    }
    auto const it = m_dynamicQuestInfos.find(dQuestId);
    if (it != m_dynamicQuestInfos.end())
    {
        return it->second;
    }
    auto* info = CreateQuestInfoForDynamicQuest(dQuestId);
    if (!info)
    {
        return nullptr;
    }
    m_dynamicQuestInfos.insert({dQuestId, info});
    return info;
}

void QuestInfoManager::ClearDynamicQuestInfos()
{
    for (auto& questInfo : m_dynamicQuestInfos)
    {
        delete questInfo.second;
    }

    m_dynamicQuestInfos.clear();
}

void QuestInfoManager::OnEndLevelBeforeContinuousLevel()
{
    // RVA 0x5743A0
    ClearDynamicQuestInfos();
}

int QuestInfoManager::RemoveQuestInfoForDynamicQuest(int dQuestId)
{
    // RVA 0x5744B0
    if (dQuestId == -1)
    {
        return 0;
    }
    auto const it = m_dynamicQuestInfos.find(dQuestId);
    if (it == m_dynamicQuestInfos.end())
    {
        return 0;
    }
    delete it->second;
    it->second = nullptr;
    m_dynamicQuestInfos.erase(it);
    return 1;
}
