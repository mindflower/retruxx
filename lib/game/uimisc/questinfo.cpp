#include "questinfo.h"

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

int QuestInfoManager::SaveModifiedQuestInfosToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

QuestInfoManager::QuestInfoManager() = default;

QuestInfo const* QuestInfoManager::GetQuestInfoByQuestInfoName(CStr const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void QuestInfoManager::SetCoordinateForQuest(CStr const&, CStr const&, CVector const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr QuestInfoManager::GetDynamicQuestInfoPatternNameByType(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

QuestInfo const* QuestInfoManager::GetQuestInfoForStaticQuest(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void QuestInfoManager::Clear()
{
    RETRUXX_NOT_IMPLEMENTED;
}

QuestInfoManager::~QuestInfoManager()
{
    RETRUXX_NOT_IMPLEMENTED;
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

int QuestInfoManager::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool QuestInfoManager::IsQuestDynamic(CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
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

QuestInfo const* QuestInfoManager::GetQuestInfoForDynamicQuest(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int QuestInfoManager::LoadModifiedQuestInfosFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void QuestInfoManager::OnEndLevel()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void QuestInfoManager::ClearQuestInfos()
{
    for (auto& questInfo : m_questInfos)
    {
        delete questInfo.second;
    }

    m_questInfos.clear();
}

QuestInfo* QuestInfoManager::CreateQuestInfoForDynamicQuest(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
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

void QuestInfoManager::OnDynamicQuestStateChanged(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void QuestInfoManager::GetAllQuestInfosByNamePattern(CStr const&, std::vector<CStr>&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int QuestInfoManager::LoadFromFile(CStr const&)
{
    // TODO: implement QuestInfoManager::LoadFromFile
    // RETRUXX_NOT_IMPLEMENTED;
    return 1;
}

QuestInfo const* QuestInfoManager::AddQuestInfoForDynamicQuest(int)
{
    RETRUXX_NOT_IMPLEMENTED;
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
    RETRUXX_NOT_IMPLEMENTED;
}

int QuestInfoManager::RemoveQuestInfoForDynamicQuest(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}
