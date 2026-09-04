#include "navpoint.h"

#include <stdexcept>

#include "retruxx/common.h"

#include "core/ini.h"
#include "core/kernel.h"
#include "core/log.h"
#include "core/ref_ptr.h"

#include <math/vector.h>

#include "game/m3dgame.h"
#include "game/uimanager/truxxuimanager.h"
#include "game/uimisc/guihelper.h"
#include "game/uimisc/objectcollection.h"
#include "game/uimisc/questinfo.h"

#include "server/objects/base/objcontainer.h"
#include "server/objects/dynamicquest.h"
#include "server/quest.h"
#include "server/queststate.h"

// ============================================================================
//  NavPoint
// ============================================================================

namespace
{
    // RVA 0x16B300 / 0x16B340 - the shipped build keeps a {type, name} table
    // populated by a static initializer ("MAIN_QUEST"/"USER_QUEST"/... etc);
    // reproduced here as straight switches.
    CStr NavPointType2Str(NavPoint::NavPointType type)
    {
        switch (type)
        {
        case NavPoint::NAVPOINT_TYPE_MAIN_QUEST:
            return "MAIN_QUEST";
        case NavPoint::NAVPOINT_TYPE_USER_QUEST:
            return "USER_QUEST";
        case NavPoint::NAVPOINT_TYPE_USER_LOCATION:
            return "USER_LOCATION";
        default:
            return {};
        }
    }

    NavPoint::NavPointType Str2NavPointType(CStr const& str)
    {
        if (str == "MAIN_QUEST")
        {
            return NavPoint::NAVPOINT_TYPE_MAIN_QUEST;
        }
        if (str == "USER_QUEST")
        {
            return NavPoint::NAVPOINT_TYPE_USER_QUEST;
        }
        if (str == "USER_LOCATION")
        {
            return NavPoint::NAVPOINT_TYPE_USER_LOCATION;
        }
        return NavPoint::NAVPOINT_TYPE_NUM_NAVPOINT_TYPES;
    }

    // RVA 0x16B400 / 0x16B440
    CStr ObjectType2Str(NavPoint::ObjectType type)
    {
        switch (type)
        {
        case NavPoint::OBJECT_TYPE_STATIC_QUEST:
            return "STATIC_QUEST";
        case NavPoint::OBJECT_TYPE_DYNAMIC_QUEST:
            return "DYNAMIC_QUEST";
        default:
            return {};
        }
    }

    NavPoint::ObjectType Str2ObjectType(CStr const& str)
    {
        if (str == "STATIC_QUEST")
        {
            return NavPoint::OBJECT_TYPE_STATIC_QUEST;
        }
        if (str == "DYNAMIC_QUEST")
        {
            return NavPoint::OBJECT_TYPE_DYNAMIC_QUEST;
        }
        return NavPoint::OBJECT_TYPE_NUM_OBJECT_TYPES;
    }
}  // namespace

NavPoint::NavPoint()
{
    Invalidate();
}

NavPoint::~NavPoint()
{
    // m_levelName cleans itself up.
}

void NavPoint::Invalidate()
{
    // RVA 0x16B2A0 - note m_levelName is deliberately left untouched.
    m_navPointType = NAVPOINT_TYPE_INVALID;
    m_objectType = OBJECT_TYPE_INVALID;
    m_id = -1;
    m_objectId = -1;
    m_coordinate = CVector{0.0f, 0.0f, 0.0f};
}

int NavPoint::GetId() const
{
    // RVA 0x16B150
    return m_id;
}

int NavPoint::GetObjectId() const
{
    // RVA 0x16B140
    return m_objectId;
}

NavPoint::NavPointType NavPoint::GetNavPointType() const
{
    // RVA 0x16B120
    return m_navPointType;
}

NavPoint::ObjectType NavPoint::GetObjectType() const
{
    // RVA 0x16B130
    return m_objectType;
}

CStr const& NavPoint::GetLevelName() const
{
    // RVA 0x16B160
    return m_levelName;
}

CVector const* NavPoint::GetCoordinate() const
{
    // RVA 0x16B170
    if (m_objectType == OBJECT_TYPE_STATIC_QUEST)
    {
        QuestInfoManager* qim = M3D_APP->m_pInterfaceManager->GetQuestInfoManager();
        if (ai::Quest* quest = ai::theQuestManager->GetQuestById(m_objectId))
        {
            if (QuestInfo const* questInfo = qim->GetQuestInfoByQuestInfoName(quest->GetName()))
            {
                return questInfo->GetCoordinateForMap(m_levelName);
            }
        }
        return nullptr;
    }

    if (m_objectType == OBJECT_TYPE_DYNAMIC_QUEST)
    {
        if (m_objectId >= 0)
        {
            ai::Obj* obj = ai::theObjects->GetEntityByObjId(m_objectId);
            if (obj && obj->IsKindOf(&ai::DynamicQuest::m_classDynamicQuest))
            {
                QuestInfoManager* qim = M3D_APP->m_pInterfaceManager->GetQuestInfoManager();
                if (QuestInfo const* questInfo = qim->GetQuestInfoForDynamicQuest(m_objectId))
                {
                    return questInfo->GetCoordinateForMap(m_levelName);
                }
            }
        }
        return nullptr;
    }

    // User location - the coordinate is stored directly.
    return &m_coordinate;
}

bool NavPoint::IsValid() const
{
    // RVA 0x16B250
    if (m_navPointType == NAVPOINT_TYPE_INVALID || m_id == -1)
    {
        return false;
    }
    if (m_levelName.empty())
    {
        return false;
    }
    if (!GetCoordinate())
    {
        return false;
    }
    return m_objectType == OBJECT_TYPE_INVALID || m_objectId != -1;
}

int NavPoint::SaveToXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
{
    // RVA 0x16B4D0
    if (!xmlFile || !xmlNode)
    {
        M3D_LOG_INFO("NavPoint::SaveToXml error - null xmlFile or xmlNode");
        return 0;
    }
    xmlNode->SetAttribute("NavPointType", NavPointType2Str(m_navPointType).c_str());
    xmlNode->SetAttribute("ObjectType", ObjectType2Str(m_objectType).c_str());
    xmlNode->SetAttribute("ObjectId", CStr(m_objectId).c_str());
    xmlNode->SetAttribute("LevelName", m_levelName.c_str());
    xmlNode->SetAttribute("Coordinate", CStr(m_coordinate).c_str());
    return 1;
}

int NavPoint::LoadFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
{
    // RVA 0x16B640
    Invalidate();
    if (!xmlFile || !xmlNode)
    {
        M3D_LOG_INFO("NavPoint::LoadFromXml error - null xmlFile or xmlNode");
        return 0;
    }

    CStr strNavPointType;
    m3d::SafeStrAttrib(strNavPointType, xmlNode, "NavPointType");
    m_navPointType = Str2NavPointType(strNavPointType);

    CStr strObjectType;
    m3d::SafeStrAttrib(strObjectType, xmlNode, "ObjectType");
    m_objectType = Str2ObjectType(strObjectType);

    m3d::SafeIntAttrib(m_objectId, xmlNode, "ObjectId");
    m3d::SafeStrAttrib(m_levelName, xmlNode, "LevelName");
    m3d::SafeVectorAttrib(m_coordinate, xmlNode, "Coordinate");
    return 1;
}

// ============================================================================
//  NavPointManager
// ============================================================================

NavPointManager::NavPointManager()
{
}

NavPointManager::~NavPointManager()
{
    Clear();
}

int NavPointManager::Init()
{
    // RVA 0x16C0C0
    Clear();
    return 1;
}

void NavPointManager::Clear()
{
    // RVA 0x16B880
    while (!m_navPoints.empty())
    {
        RemoveNavPointById(m_navPoints.begin()->first);
    }
    m_nextNavPointId = 0;
}

// --- private helpers -------------------------------------------------------------------------

int NavPointManager::AddNavPoint(NavPoint* np)
{
    // RVA 0x16BE80
    if (!np)
    {
        return -1;
    }
    int const id = m_nextNavPointId;
    np->m_id = id;
    if (!np->IsValid())
    {
        return -1;
    }
    if (m_navPoints.find(id) != m_navPoints.end())
    {
        return -1;
    }

    // Each (level, type) pair holds at most one nav point - drop the old one.
    if (np->m_navPointType <= NavPoint::NAVPOINT_TYPE_USER_LOCATION)
    {
        std::vector<int> const existing = GetNavPointsByType(np->m_levelName, np->m_navPointType);
        if (!existing.empty())
        {
            RemoveNavPointById(existing[0]);
        }
    }

    m_navPoints[id] = np;
    ++m_nextNavPointId;

    m3d::AIParam param5;
    M3D_APP->ImmediateMessage(65666, id, np->m_objectId, np->m_objectType, np->m_navPointType, np->m_levelName, param5);
    return id;
}

// --- queries -------------------------------------------------------------------------------

NavPoint const* NavPointManager::GetNavPointById(int id) const
{
    // RVA 0x16BB10
    auto it = m_navPoints.find(id);
    return it == m_navPoints.end() ? nullptr : it->second;
}

NavPoint const*
    NavPointManager::GetNavPointByObjectId(CStr const& levelName, int objectId, NavPoint::ObjectType objectType) const
{
    // RVA 0x16BB40
    if (levelName.empty() || objectId == -1 || objectType == NavPoint::OBJECT_TYPE_NUM_OBJECT_TYPES)
    {
        return nullptr;
    }
    NavPoint const* found = nullptr;
    for (auto const& kv : m_navPoints)
    {
        NavPoint* np = kv.second;
        if (np && np->m_objectId == objectId && np->m_objectType == objectType && np->m_levelName == levelName)
        {
            found = np;
        }
    }
    return found;
}

std::vector<int> NavPointManager::GetNavPointsForLevel(CStr const& levelName) const
{
    // RVA 0x16B920
    std::vector<int> levelNavPointIds;
    for (auto const& kv : m_navPoints)
    {
        NavPoint* np = kv.second;
        if (np && np->m_levelName == levelName)
        {
            levelNavPointIds.push_back(np->m_id);
        }
    }
    return levelNavPointIds;
}

std::vector<int> NavPointManager::GetNavPointsByType(CStr const& levelName, NavPoint::NavPointType npType) const
{
    // RVA 0x16BA10
    std::vector<int> navPointIds;
    for (auto const& kv : m_navPoints)
    {
        NavPoint* np = kv.second;
        if (np && np->m_levelName == levelName && np->m_navPointType == npType)
        {
            navPointIds.push_back(np->m_id);
        }
    }
    return navPointIds;
}

std::vector<int> NavPointManager::GetAllNavPointsByQuestId(int questId) const
{
    // RVA 0x16BBE0
    std::vector<int> npIds;
    if (questId == -1)
    {
        return npIds;
    }
    for (auto const& kv : m_navPoints)
    {
        NavPoint* np = kv.second;
        if (np && np->m_objectId == questId && np->m_objectType == NavPoint::OBJECT_TYPE_STATIC_QUEST)
        {
            npIds.push_back(np->m_id);
        }
    }
    return npIds;
}

int NavPointManager::GetMaxNavPointsNumByType(NavPoint::NavPointType npType) const
{
    // RVA 0x16C740
    return (npType == NavPoint::NAVPOINT_TYPE_MAIN_QUEST || static_cast<unsigned int>(npType - 1) < 2) ? 1 : 0;
}

// --- "can add" predicates -----------------------------------------------------------------

bool NavPointManager::CanNavPointOfTypeBeAdded(CStr const& levelName, NavPoint::NavPointType npType, bool bByUser) const
{
    // RVA 0x16C760
    if (static_cast<unsigned int>(npType) > 2)
    {
        return false;
    }
    if (npType != NavPoint::NAVPOINT_TYPE_USER_QUEST)
    {
        return true;
    }
    // Only one user-quest nav point per level - a second one needs a user gesture.
    if (GetNavPointsByType(levelName, npType).empty())
    {
        return true;
    }
    return bByUser;
}

bool NavPointManager::CanNavPointBeAdded(
    CStr const& levelName,
    int objectId,
    NavPoint::ObjectType objectType,
    NavPoint::NavPointType npType,
    bool bByUser) const
{
    // RVA 0x16C7E0
    if (!CanNavPointOfTypeBeAdded(levelName, npType, bByUser) || GetNavPointByObjectId(levelName, objectId, objectType))
    {
        return false;
    }

    if (objectType == NavPoint::OBJECT_TYPE_STATIC_QUEST)
    {
        if (!ai::theQuestManager->GetQuestById(objectId))
        {
            return false;
        }
        ai::QuestState const* qs = ai::theQuestStateManager->GetQuestStateById(objectId);
        if (!qs || qs->GetCompleteStatus() != ai::QuestState::NOT_COMPLETE || !qs->bIsTaken())
        {
            return false;
        }
        QuestInfoManager* qim = M3D_APP->m_pInterfaceManager->GetQuestInfoManager();
        QuestInfo const* qi = qim->GetQuestInfoForStaticQuest(objectId);
        if (!qi || !qi->GetCoordinateForMap(levelName))
        {
            return false;
        }
    }
    else if (objectType == NavPoint::OBJECT_TYPE_DYNAMIC_QUEST)
    {
        if (levelName != help::GetCurrentLevelName())
        {
            return false;
        }
        ai::Obj* obj = ai::theObjects->GetEntityByObjId(objectId);
        if (!obj || !obj->IsKindOf(&ai::DynamicQuest::m_classDynamicQuest))
        {
            return false;
        }
        // TODO(RVA 0x16C7E0: the shipped build also requires the dynamic quest
        //  status to equal 1 ("taken"); DynamicQuest::GetQuestStatus() and its
        //  QuestStatus enum are not yet available in retruxx)
    }
    return true;
}

// --- public add / remove ------------------------------------------------------------------

int NavPointManager::AddNavPointObjectDependend(
    CStr const& levelName,
    NavPoint::NavPointType npType,
    NavPoint::ObjectType objectType,
    int objectId,
    bool bByUser)
{
    // RVA 0x16BC90
    if (!CanNavPointBeAdded(levelName, objectId, objectType, npType, bByUser))
    {
        return -1;
    }
    NavPoint* np = new NavPoint;
    np->m_objectId = objectId;
    np->m_navPointType = npType;
    np->m_objectType = objectType;
    np->m_levelName = levelName;

    int const result = AddNavPoint(np);
    if (result == -1)
    {
        delete np;
    }
    return result;
}

int NavPointManager::AddNavPointUserLocation(CStr const& levelName, CVector const& coordinate)
{
    // RVA 0x16BDB0
    NavPoint* np = new NavPoint;
    np->m_navPointType = NavPoint::NAVPOINT_TYPE_USER_LOCATION;
    np->m_levelName = levelName;
    np->m_coordinate = coordinate;

    int const result = AddNavPoint(np);
    if (result == -1)
    {
        delete np;
    }
    return result;
}

int NavPointManager::RemoveNavPointById(int id)
{
    // RVA 0x16BFA0
    auto it = m_navPoints.find(id);
    if (it == m_navPoints.end())
    {
        return 0;
    }
    NavPoint* np = it->second;
    if (!np)
    {
        return 0;
    }
    int const objectId = np->m_objectId;
    NavPoint::ObjectType const objectType = np->m_objectType;
    NavPoint::NavPointType const npType = np->m_navPointType;
    CStr const levelName = np->m_levelName;

    delete np;
    m_navPoints.erase(it);

    m3d::AIParam param5;
    M3D_APP->ImmediateMessage(65667, id, objectId, objectType, npType, levelName, param5);
    return 1;
}

// --- quest-driven updates -----------------------------------------------------------------

int NavPointManager::MakeNavPointOnFirstFitStaticQuest(CStr const& levelName, bool bIsMainQuest)
{
    // RVA 0x16C500
    if (levelName.empty())
    {
        return -1;
    }
    int newNpId = -1;
    std::vector<int> const& takenQuestIds = M3D_APP->m_pInterfaceManager->GetTakenQuestIds();
    for (int i = static_cast<int>(takenQuestIds.size()) - 1; i >= 0; --i)
    {
        int const questId = takenQuestIds[i];
        QuestInfoManager* qim = M3D_APP->m_pInterfaceManager->GetQuestInfoManager();
        ai::Quest* quest = ai::theQuestManager->GetQuestById(questId);
        if (!quest)
        {
            continue;
        }
        QuestInfo const* qi = qim->GetQuestInfoByQuestInfoName(quest->GetName());
        if (qi && qi->IsMainQuest() == bIsMainQuest && qi->GetCoordinateForMap(levelName))
        {
            newNpId = AddNavPointObjectDependend(
                levelName,
                qi->IsMainQuest() ? NavPoint::NAVPOINT_TYPE_MAIN_QUEST : NavPoint::NAVPOINT_TYPE_USER_QUEST,
                NavPoint::OBJECT_TYPE_STATIC_QUEST,
                questId,
                false);
            if (newNpId != -1)
            {
                return newNpId;
            }
        }
    }
    return newNpId;
}

int NavPointManager::MakeNavPointOnFirstFitDynamicQuest()
{
    // RVA 0x16C620
    std::set<int> const* dqIds =
        M3D_APP->m_pInterfaceManager->GetObjectCollection().GetObjectsByClass(&ai::DynamicQuest::m_classDynamicQuest);
    if (!dqIds)
    {
        return -1;
    }
    int result = -1;
    for (int dqId : *dqIds)
    {
        result = AddNavPointObjectDependend(
            help::GetCurrentLevelName(),
            NavPoint::NAVPOINT_TYPE_USER_QUEST,
            NavPoint::OBJECT_TYPE_DYNAMIC_QUEST,
            dqId,
            false);
        if (result != -1)
        {
            break;
        }
    }
    return result;
}

int NavPointManager::UpdateOnQuestTaken(void* data)
{
    // RVA 0x16C110
    if (!data)
    {
        return 0;
    }
    int const questId = static_cast<int*>(data)[13];
    QuestInfoManager* qim = M3D_APP->m_pInterfaceManager->GetQuestInfoManager();
    ai::Quest* quest = ai::theQuestManager->GetQuestById(questId);
    if (!quest)
    {
        return 0;
    }
    QuestInfo const* qi = qim->GetQuestInfoByQuestInfoName(quest->GetName());
    if (!qi)
    {
        return 0;
    }
    NavPoint::NavPointType const npType =
        qi->IsMainQuest() ? NavPoint::NAVPOINT_TYPE_MAIN_QUEST : NavPoint::NAVPOINT_TYPE_USER_QUEST;
    for (CStr const& mapName : qi->GetMapNames())
    {
        AddNavPointObjectDependend(mapName, npType, NavPoint::OBJECT_TYPE_STATIC_QUEST, questId, false);
    }
    return 1;
}

int NavPointManager::UpdateOnQuestStateChanged(void* data)
{
    // RVA 0x16C200
    if (!data)
    {
        return 0;
    }
    int const questId = static_cast<int*>(data)[13];
    if (!ai::theQuestStateManager->GetQuestStateById(questId))
    {
        return 0;
    }
    QuestInfoManager* qim = M3D_APP->m_pInterfaceManager->GetQuestInfoManager();
    ai::Quest* quest = ai::theQuestManager->GetQuestById(questId);
    if (!quest)
    {
        return 0;
    }
    QuestInfo const* qi = qim->GetQuestInfoByQuestInfoName(quest->GetName());
    if (!qi)
    {
        return 0;
    }

    std::vector<int> const nps = GetAllNavPointsByQuestId(questId);
    for (int npId : nps)
    {
        RemoveNavPointById(npId);
        if (!qi->GetCoordinateForMap(help::GetCurrentLevelName()))
        {
            continue;
        }
        NavPoint::NavPointType const npType =
            qi->IsMainQuest() ? NavPoint::NAVPOINT_TYPE_MAIN_QUEST : NavPoint::NAVPOINT_TYPE_USER_QUEST;
        if (CanNavPointOfTypeBeAdded(help::GetCurrentLevelName(), npType, false))
        {
            int const r = MakeNavPointOnFirstFitStaticQuest(help::GetCurrentLevelName(), qi->IsMainQuest());
            if (r == -1 && !qi->IsMainQuest())
            {
                MakeNavPointOnFirstFitDynamicQuest();
            }
        }
    }
    return 1;
}

int NavPointManager::UpdateOnDynamicQuestStateChanged(void*)
{
    // TODO(RVA 0x16C3A0: branches entirely on the DynamicQuest "taken" status
    //  (== 1); DynamicQuest::GetQuestStatus() and its QuestStatus enum are not
    //  yet available in retruxx)
    RETRUXX_NOT_IMPLEMENTED;
}

int NavPointManager::GameDataUpdate(void* data, int dataType)
{
    // RVA 0x16C0D0
    switch (dataType)
    {
    case 59:
        UpdateOnQuestTaken(data);
        break;
    case 60:
        UpdateOnQuestStateChanged(data);
        break;
    case 67:
        UpdateOnDynamicQuestStateChanged(data);
        break;
    }
    return 1;
}

// --- persistence --------------------------------------------------------------------------

int NavPointManager::LoadFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
{
    // RVA 0x16C8E0
    Clear();
    if (!xmlFile || !xmlNode)
    {
        M3D_LOG_INFO("NavPointManager::LoadFromXml error - null xmlFile or xmlNode");
        return 0;
    }

    int res = 1;
    ref_ptr navPointNode = xmlFile->CreateNode();
    xmlNode->GetFirstChild(navPointNode, "NavPoint");
    while (!navPointNode->IsEmpty())
    {
        NavPoint* np = new NavPoint;
        if (!np->LoadFromXml(xmlFile, navPointNode) || AddNavPoint(np) == -1)
        {
            delete np;
            res = 0;
        }
        navPointNode->GetNextSibling(navPointNode, "NavPoint");
    }
    if (!res)
    {
        M3D_LOG_INFO("NavPointManager was loaded with errors");
    }
    return res;
}

int NavPointManager::SaveToXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
{
    // RVA 0x16CAF0
    if (!xmlFile || !xmlNode)
    {
        // NOTE: the shipped log message has the "xnlFile" typo; kept verbatim.
        M3D_LOG_INFO("NavPointManager::SaveToXml error - null xnlFile or xmlNode");
        return 0;
    }

    int res = 1;
    for (auto const& kv : m_navPoints)
    {
        NavPoint* np = kv.second;
        if (!np)
        {
            res = 0;
            continue;
        }
        ref_ptr npNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "NavPoint");
        xmlNode->AddChild(npNode);
        res &= np->SaveToXml(xmlFile, npNode);
    }
    if (!res)
    {
        M3D_LOG_INFO("NavPointManager saved with errors");
    }
    return res;
}
