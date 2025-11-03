#include "objcontainer.h"
#include <core/aiparam.h>
#include <stdexcept>
#include <server/objects/physicbodies/physicbody.h>
#include <core/debugcounter.h>
#include "prototypemanager.h"
#include <core/kernel.h>
#include <core/log.h>

#include "globalproperties.h"
#include "m3dapp.h"
#include "server/queststate.h"
#include "server/statistic/timestatistic.h"
#include <server/server.h>

#include "config.h"
#include "level.h"
#include "world.h"
#include "core/timer.h"
#include "server/passagedata.h"

void ShowCurrentStack()
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, CreateNewObject)
{
    auto* objContainer = (ai::ObjContainer*)context->asObject(0, "ObjContainer");
    auto id = objContainer->CreateNewObject(context->asInt(1), context->asString(2), context->asInt(3), context->asInt(4));
    context->pushInt(id);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, GetEntityByObjId)
{
    auto* objContainer = (ai::ObjContainer*)context->asObject(0, "ObjContainer");
    auto* obj = objContainer->GetEntityByObjId(context->asInt(1));
    context->pushObject(obj);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, size)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, GetEntityByObjName)
{
    auto* objContainer = (ai::ObjContainer*)context->asObject(0, "ObjContainer");
    CStr objName = context->asString(1);

    auto* objByName = objContainer->GetEntityByObjName(objName);
    context->pushObject(objByName);

    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, GetPrototypeId)
{
    context->asObject(0, "ObjContainer");
    CStr protoName = context->asString(1);

    auto res = ai::thePrototypeManager->GetPrototypeId(protoName);
    context->pushInt(res);

    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, MessageBox)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, SetTolerance)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, GetTolerance)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, IncTolerance)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, SetGameTime)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, GetGameTime)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, Get24HourTime)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, PauseGameTime)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, UnpauseGameTime)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, GetHeight)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, Dump)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, DumpPhysicInfo)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, AllowSave)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, IsSaveAllowed)
{
    RETRUXX_NOT_IMPLEMENTED;
}

namespace ai
{
    namespace
    {
        int ObjContainerSize = 0x4000;
    }

    RT_CLASS_EXPORTS_BEGIN(ObjContainer)
        RT_CLASS_EXPORT(ObjContainer, m3d::METHOD, CreateNewObject, "", "", "")
        RT_CLASS_EXPORT(ObjContainer, m3d::METHOD, GetEntityByObjId, "", "", "")
        RT_CLASS_EXPORT(ObjContainer, m3d::METHOD, size, "", "", "")
        RT_CLASS_EXPORT(ObjContainer, m3d::METHOD, GetEntityByObjName, "", "", "")
        RT_CLASS_EXPORT(ObjContainer, m3d::METHOD, GetPrototypeId, "", "", "")
        RT_CLASS_EXPORT(ObjContainer, m3d::METHOD, MessageBox, "", "", "")
        RT_CLASS_EXPORT(ObjContainer, m3d::METHOD, SetTolerance, "", "", "")
        RT_CLASS_EXPORT(ObjContainer, m3d::METHOD, GetTolerance, "", "", "")
        RT_CLASS_EXPORT(ObjContainer, m3d::METHOD, IncTolerance, "", "", "")
        RT_CLASS_EXPORT(ObjContainer, m3d::METHOD, SetGameTime, "", "", "")
        RT_CLASS_EXPORT(ObjContainer, m3d::METHOD, GetGameTime, "", "", "")
        RT_CLASS_EXPORT(ObjContainer, m3d::METHOD, Get24HourTime, "", "", "")
        RT_CLASS_EXPORT(ObjContainer, m3d::METHOD, PauseGameTime, "", "", "")
        RT_CLASS_EXPORT(ObjContainer, m3d::METHOD, UnpauseGameTime, "", "", "")
        RT_CLASS_EXPORT(ObjContainer, m3d::METHOD, GetHeight, "", "", "")
        RT_CLASS_EXPORT(ObjContainer, m3d::METHOD, Dump, "", "", "")
        RT_CLASS_EXPORT(ObjContainer, m3d::METHOD, DumpPhysicInfo, "", "", "")
        RT_CLASS_EXPORT(ObjContainer, m3d::METHOD, AllowSave, "", "", "")
        RT_CLASS_EXPORT(ObjContainer, m3d::METHOD, IsSaveAllowed, "", "", "")
	RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(ObjContainer);

    long long GameTime::asInt64() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void GameTime::operator+=(float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::AIParam GameTime::asAIParam() const
    {
        // TODO: generated code
        // Extract time components from milliseconds
        const int64_t totalMilliseconds = m_milliSeconds;

        // Constants for time calculations
        const int64_t MILLISECONDS_PER_YEAR = 32140800000LL;   // 372 * 24 * 60 * 60 * 1000
        const int64_t MILLISECONDS_PER_MONTH = 2678400000LL;   // 31 * 24 * 60 * 60 * 1000
        const int64_t MILLISECONDS_PER_DAY = 86400000LL;
        const int64_t MILLISECONDS_PER_HOUR = 3600000LL;
        const int64_t MILLISECONDS_PER_MINUTE = 60000LL;

        // Calculate time components
        const int year = static_cast<int>(totalMilliseconds / MILLISECONDS_PER_YEAR);
        const int64_t remainderAfterYears = totalMilliseconds % MILLISECONDS_PER_YEAR;

        const int month = static_cast<int>(remainderAfterYears / MILLISECONDS_PER_MONTH) + 1;
        const int64_t remainderAfterMonths = remainderAfterYears % MILLISECONDS_PER_MONTH;

        const int day = static_cast<int>(remainderAfterMonths / MILLISECONDS_PER_DAY) + 1;
        const int64_t remainderAfterDays = remainderAfterMonths % MILLISECONDS_PER_DAY;

        const int hour = static_cast<int>(remainderAfterDays / MILLISECONDS_PER_HOUR);
        const int64_t remainderAfterHours = remainderAfterDays % MILLISECONDS_PER_HOUR;

        const int minute = static_cast<int>(remainderAfterHours / MILLISECONDS_PER_MINUTE);

        // Create vector with time components in order: hour, minute, day, month, year
        std::vector<int> timeComponents;
        timeComponents.reserve(5);

        timeComponents.push_back(hour);
        timeComponents.push_back(minute);
        timeComponents.push_back(day);
        timeComponents.push_back(month);
        timeComponents.push_back(year);

        return { timeComponents };
    }

    float GameTime::GameDiff(ObjContainer const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    GameTime::GameTime(int, int, int, int, int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    GameTime::GameTime(long long)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    GameTime::GameTime()
    {
    }

    void GameTime::setInt64(long long)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void GameTime::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void GameTime::setExpanded(int hour, int minute, int day, int month, int year)
    {
        //TODO: check this
        uint64_t res = (uint64_t)60000 * (uint64_t)(minute + 60 * (uint64_t)(hour + 24 * (uint64_t)(day + 31 * (uint64_t)(month + 12 * year) - 32)));
        m_milliSeconds = res;
        m_milliSeconds0 = res;
    }

    float GameTime::Diff() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::AIParam GameTime::asAIParam24Hour() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void GameTime::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ObjContainer::Node::Node()
    {
        this->m_id = -1;
        this->m_prevId = -1;
        this->m_nextId = -1;
        this->m_value = 0;
        this->m_isValid = 0;
        this->m_totalObjects = 0;
    }

    ObjContainer::InnerContainer::InnerContainer()
    {
        Clear();
    }

    void ObjContainer::InnerContainer::EraseNode(Node& node, bool deleteObj)
    {
        if (node.m_totalObjects < 0x1FFFF)
        {
            m_freePlaces.push_back(node.m_id);
        }

        Node* prevNode = nullptr;
        auto prevId = node.m_prevId;
        if (prevId == -1)
        {
            prevNode = nullptr;
        }
        else
        {
            prevNode = &m_records[prevId];
        }

        Node* nextNode = nullptr;
        auto nextId = node.m_nextId;
        if (nextId == -1)
        {
            nextNode = nullptr;
        }
        else
        {
            nextNode = &m_records[nextId];
        }

        if (prevNode)
        {
            prevNode->m_nextId = nextId;
        }
        else
        {
            m_firstNodeId = nextId;
        }

        if (nextNode)
        {
            nextNode->m_prevId = node.m_prevId;
        }
        else
        {
            m_lastNodeId = node.m_prevId;
        }

        if (deleteObj)
        {
            if (node.m_value)
            {
                // TODO: check this
                delete node.m_value;
            }
        }
        node.m_value = nullptr;
        ++node.m_totalObjects;
        node.m_nextId = -1;
        node.m_prevId = -1;
        node.m_isValid = false;
        --m_size;
    }

    int ObjContainer::InnerContainer::Add(Obj* pObj)
    {
        int nodeId = -1;
        if (!m_freePlaces.empty())
        {
            nodeId = m_freePlaces.back();
            m_freePlaces.pop_back();
        }
        else
        {
            auto msg = "Error: maximum objects count exceeded: " + CStr(ObjContainerSize) + " when attempting to add " + pObj->GetDebugDescription() + " to ObjContainer";
            M3D_LOG_CRIT(msg);
            ShowCurrentStack();
            theObjects->Dump();
            M3D_CRITICAL_ERROR(msg);
        }

        auto& record = m_records[nodeId];
        record.m_isValid = true;
        record.m_value = pObj;
        if (m_size)
        {
            m_records[m_lastNodeId].m_nextId = nodeId;
            record.m_prevId = m_lastNodeId;
        }
        else
        {
            m_firstNodeId = nodeId;
        }

        m_lastNodeId = nodeId;
        ++m_size;
        return (record.m_totalObjects << 14) + nodeId;
    }

    bool ObjContainer::InnerContainer::AddWithOwnObjId(Obj* pObj, int id)
    {
        M3D_ASSERT(id >= 0);


        RETRUXX_NOT_IMPLEMENTED;
    }

    bool ObjContainer::InnerContainer::empty() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Obj* ObjContainer::InnerContainer::GetObjById(int objId)
    {
        if (objId >= 0)
        {
            // TODO: check this
            const auto& record = m_records[objId & 0x3FFF];
            if (objId >> 14 == record.m_totalObjects && record.m_isValid)
            {
                return record.m_value;
            }
        }
        return nullptr;
    }

    void ObjContainer::InnerContainer::Clear()
    {
        m_records.clear();
        m_freePlaces.clear();

        m_firstNodeId = -1;
        m_lastNodeId = -1;
        m_size = 0;

        m_records.resize(MAX_OBJECTS, {});
        for (int i = m_records.size() - 1; i != 0; --i)
        {
            m_records[i].m_id = i;
            m_freePlaces.push_back(i);
        }
    }

    unsigned ObjContainer::InnerContainer::size() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ObjContainer::Node* ObjContainer::InnerContainer::_GetNodeById(int id)
    {
        if (id == -1 && id >= m_records.size())
        {
            return nullptr;
        }
        return &m_records[id];
    }

    ObjContainer::const_iterator::const_iterator(retruxx::vector<Node> const* pRecords, int nodeId)
    {
        this->m_nodeId = nodeId;
        this->m_pRecords = pRecords;
    }

    void ObjContainer::const_iterator::_Inc()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Obj const* ObjContainer::const_iterator::operator*() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Obj const* ObjContainer::const_iterator::operator->() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool ObjContainer::const_iterator::operator==(const_iterator const&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool ObjContainer::const_iterator::operator!=(const_iterator const& rhs) const
    {
        return this->m_nodeId != rhs.m_nodeId;
    }

    ObjContainer::const_iterator& ObjContainer::const_iterator::operator++()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ObjContainer::iterator::iterator(retruxx::vector<Node>* pRecords, int nodeId) :
        ObjContainer::const_iterator(pRecords, nodeId)
    {
    }

    Obj* ObjContainer::iterator::operator->() const
    {
        return (*this->m_pRecords)[this->m_nodeId].m_value;
    }

    ObjContainer::iterator ObjContainer::iterator::operator++(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ObjContainer::iterator& ObjContainer::iterator::operator++()
    {
        auto result = this;
        this->m_nodeId = (*this->m_pRecords)[this->m_nodeId].m_nextId;
        return *result;
    }

    Obj* ObjContainer::iterator::operator*() const
    {
        return (*this->m_pRecords)[this->m_nodeId].m_value;
    }

    Obj* ObjContainer::GetEntityByObjName(CStr const& name)
    {
        if (name.empty())
        {
            return nullptr;
        }

        auto objId = GetObjIdByObjName(name);
        if (objId >= 0)
        {
            return m_allObjects.GetObjById(objId);
        }
        return nullptr;
    }

    void ObjContainer::Purge()
    {
        // TODO: check this
        m_inPurge = true;
        m_numRemovalsLastFrame = 0;

        for (const auto objId : m_objIdsToRemove)
        {
            auto* node = m_allObjects._GetNodeById(objId);
            if (node && node->m_isValid && node->m_value)
            {
                auto* obj = node->m_value;

                ++m_numRemovalsLastFrame;;
                auto* parent = obj->GetParent();
                if (parent)
                {
                    if (obj->m_hierarchyType)
                    {
                        parent->RemoveComponent(obj);
                    }
                    else
                    {
                        parent->RemoveChild(obj);
                    }
                }
                else
                {
                    obj->SetParentInvalid();
                }

                    
                m_nameToIdMap.erase(obj->GetName());
                auto updatingObjId = node->m_value->m_updatingObjId;
                m_allObjects.EraseNode(*node, true);
                auto* updatingNode = m_updatingObjects._GetNodeById(updatingObjId & 0x3fff);
                if (updatingNode)
                {
                    m_updatingObjects.EraseNode(*updatingNode, false);
                }
            }
        }

        m_inPurge = false;
        m_objIdsToRemove.clear();

        for (const auto objId : m_objIdsToUpdate)
        {
            AddObjToUpdate(GetEntityByObjId(objId));
        }

        m_objIdsToUpdate.clear();

        for (const auto objId : m_objIdsToNotUpdate)
        {
            _SetObjNotUpdating(objId);
        }

        m_objIdsToNotUpdate.clear();
    }

    ObjContainer::iterator ObjContainer::updatingEnd()
    {
        return iterator(&m_updatingObjects.m_records, -1);
    }

    ObjContainer::~ObjContainer()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr ObjContainer::GetObjectFullName(CStr const&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ObjContainer::RelinkGeomsToCollisionCells()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ObjContainer::AddObjToUpdate(Obj* obj)
    {
        if (obj)
        {
            if (obj->m_objId != -1)
            {
                obj->m_bMustBeUpdating = true;
                m_objIdsToUpdate.push_back(obj->m_objId);
            }
        }
    }

    m3d::Object* ObjContainer::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ObjContainer::PermitCreation()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int ObjContainer::CreateNewObjectWithSuspendedPostLoad(int prototypeId, char const* name, int parentId, int belongId)
    {
        auto objId = CreateEntityForLoad(prototypeId, name, parentId, -1);
        if (objId >= 0)
        {
            auto* node = m_allObjects._GetNodeById(objId);
            // TODO: check this
            if (objId >> 14 == node->m_totalObjects && node->m_isValid)
            {
                if (node->m_value)
                {
                    node->m_value->SetBelong(belongId);
                    node->m_value->CreateChildren();
                }
            }
        }
        return objId;
    }

    ObjContainer::iterator ObjContainer::updatingBegin()
    {
        return ObjContainer::iterator(&m_updatingObjects.m_records, m_updatingObjects.m_firstNodeId);
    }

    void ObjContainer::LoadNodeStatesFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool ObjContainer::IsSaveAllowed() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ObjContainer::PauseGameTime()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ObjContainer::DeleteAll()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ObjContainer::PassToMap(CStr const&, CStr const&, int, bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float ObjContainer::GetHeight(float, float) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* ObjContainer::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Object);
    }

    int ObjContainer::GetPrototypeId(char const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ObjContainer::Dump()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ObjContainer::SetGameTimeInt64(long long)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int ObjContainer::GetObjIdByObjName(CStr const& name)
    {
        if (name.empty())
        {
            return -1;
        }

        auto it = m_nameToIdMap.find(name);
        if (it != m_nameToIdMap.end())
        {
            return it->second;
        }
        return -1;
    }

    float ObjContainer::GetTolerance(int, int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ObjContainer::SetGameTime(int, int, int, int, int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* ObjContainer::CreateObject()
    {
        return new ObjContainer;
    }

    void ObjContainer::TransferPhysicParamsToSceneGraph()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ObjContainer::UnpauseGameTime()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ObjContainer::UnlinkGeomsFromCollisionCells()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ObjContainer::SetTolerance(int, int, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool ObjContainer::AddWithOwnObjId(Obj* pObj)
    {
        M3D_ASSERT(pObj);

        auto result = m_allObjects.AddWithOwnObjId(pObj, pObj->GetId());
        if (result)
        {
            if (pObj->m_bIsUpdating)
            {
                pObj->m_updatingObjId = m_updatingObjects.Add(pObj);
            }
            CStr name = pObj->GetName();
            if (!name.empty())
            {
                m_nameToIdMap[name] = pObj->GetId();
            }

            M3D_APP->ImmediateMessage(66541, pObj->GetId(), 0, 0, 0, {}, {});
            return 1;
        }
        return result;
    }

    void ObjContainer::SetObjName(int objId, CStr const& name)
    {
        if (objId >= 0)
        {
            auto* obj = m_allObjects.GetObjById(objId);
            if (obj && obj->m_name != name)
            {
                m_nameToIdMap.erase(obj->GetName());
                m_nameToIdMap[name] = objId;
                obj->m_name = name;
            }
        }
    }

    GameTime& ObjContainer::getGameTime()
    {
        return this->m_GameTime;
    }

    m3d::AIParam ObjContainer::GetGameTime() const
    {
        return m_GameTime.asAIParam();
    }

    void ObjContainer::AddObjToPostCollideList(Obj*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ObjContainer::DumpPhysicInfo(CStr const&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    long long ObjContainer::GetGameTimeInt64() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned ObjContainer::GetNumUpdatingObjects() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool ObjContainer::empty() const
    {
        return this->m_allObjects.m_size == 0;
    }

    Obj* ObjContainer::GetEntityByObjId(int objId)
    {
        return m_allObjects.GetObjById(objId);
    }

    void ObjContainer::DenyCreation()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ObjContainer::LoadObjectNamesFromXML(CStr const&)
    {
        // TODO: implement ObjContainer::LoadObjectNamesFromXML
        //RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::AIParam ObjContainer::Get24HourTime() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int ObjContainer::CreateEntityForLoad(int prototypeId, char const* name, int parentId, int objId)
    {
        if (prototypeId < 0 || prototypeId >= ai::thePrototypeManager->GetNumOfPrototypes())
        {
            return -1;
        }

        auto* prototype = ai::thePrototypeManager->GetPrototypeInfo(prototypeId);
        auto* obj = prototype->CreateTargetObject();
        if (!obj)
        {
            return -1;
        }

        obj->m_name = name;
        if (parentId == -1)
        {
            obj->SetParentInvalid();
        }
        else
        {
            obj->LinkToParent(parentId, ai::Obj::HierarchyType::HIERARCHY_CHILD);
        }

        obj->m_prototypeId = prototypeId;
        if (objId == -1)
            return _Add(obj);
        obj->m_objId = objId;

        if (!AddWithOwnObjId(obj))
        {
            M3D_CRITICAL_ERROR("Could not add object with own objId: " + obj->GetDebugDescription());
        }
        return objId;
    }

    void ObjContainer::MessageBoxA(int, int, Obj*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ObjContainer::AllowSave(bool allow)
    {
        m_bSaveAllowed = allow;
    }

    float ObjContainer::GetGameTimeDiff() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ObjContainer::iterator ObjContainer::begin()
    {
        return ObjContainer::iterator(&m_allObjects.m_records, m_allObjects.m_firstNodeId);
    }

    ObjContainer::const_iterator ObjContainer::begin() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int ObjContainer::GetNumRemovalsLastFrame() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ObjContainer::Update(float elapsedTime, unsigned workTime, bool bCinematic)
    {
        // TODO: generated code
        // Reset debug counters
        PhysicBody::GetCountNodeRelinks()->SetI(0);
        PhysicObj::GetRelinksToCollisionCounter()->SetI(0);

        // Update all objects in the container
        m_inUpdate = true;

        for (auto it = updatingBegin(); it != updatingEnd(); ++it)
        {
            if (bCinematic && !it->NeedCinematicUpdate())
            {
                continue;
            }

            // Update the object
            it->Update(elapsedTime, workTime);
        }

        m_inUpdate = false;

        // Update quest state manager
        theQuestStateManager->Update(elapsedTime);

        // Update game time if not in cinematic mode and game time is not paused
        if (!bCinematic && !m_GameTimePaused)
        {
            // TODO: check this time
            m_GameTime += ai::theGlobProp.m_gameTimeMult * elapsedTime;

            // Update global game time statistic
            auto* gameTimeStat = dynamic_cast<ai::TimeStatistic*>(theStatisticManager->GetStatistic("GameTime", "TimeStatistic"));

            gameTimeStat->m_bGlobalFlag = true;
            gameTimeStat->IncreaseByMilliseconds(ai::theGlobProp.m_gameTimeMult * elapsedTime * 1000.0);

            // Update level-specific game time statistic
            auto levelStatName = "GameTime"+ ai::pServer->GetWorld()->m_level->m_levelName;
            auto* levelGameTimeStat = dynamic_cast<ai::TimeStatistic*>(theStatisticManager->GetStatistic(levelStatName, "TimeStatistic"));

            levelGameTimeStat->m_bGlobalFlag = false;
            levelGameTimeStat->IncreaseByMilliseconds(ai::theGlobProp.m_gameTimeMult * elapsedTime * 1000.0);
        }

        // Update real time statistics (always updated)
        auto* realTimeStat = dynamic_cast<ai::TimeStatistic*>(theStatisticManager->GetStatistic("RealTime", "TimeStatistic"));
        realTimeStat->m_bGlobalFlag = true;
        uint64_t realTimeDelta = static_cast<uint64_t>(elapsedTime * 1000.0);
        realTimeStat->IncreaseByMilliseconds(realTimeDelta);

        // Update level-specific real time statistic
        CStr levelRealTimeName("RealTime");
        levelRealTimeName += ai::pServer->GetWorld()->m_level->m_levelName;
        TimeStatistic* levelRealTimeStat = dynamic_cast<ai::TimeStatistic*>(ai::theStatisticManager->GetStatistic(levelRealTimeName, "TimeStatistic"));

       levelRealTimeStat->m_bGlobalFlag = false;
       levelRealTimeStat->IncreaseByMilliseconds(elapsedTime * 1000.0);

        // Handle map transition after fading
        if (ai::thePassageData)
        {
            float fadingTime = m3d::g_Kernel->GetEngineCfg().m_fadingTimeBeforeNextMap.GetF();

            m3d::cmn::Timer& timer = m3d::g_Kernel->GetTimer();
            auto currentTime = timer.GetCurTimeUnscaled();
            auto elapsedFadingTime = (currentTime - ai::thePassageData->m_fadingStartTime) * 0.001;

            if (elapsedFadingTime >= fadingTime)
            {
                _PassToMapAfterFading();
            }
        }
    }

    int ObjContainer::CreateNewObject(int prototypeId, char const* name, int parentId, int belongId)
    {
        auto objId = CreateNewObjectWithSuspendedPostLoad(prototypeId, name, parentId, belongId);
        if (objId != -1)
        {
            auto* obj = m_allObjects.GetObjById(objId);
            obj->PostLoad();
            obj->CreateVisualPart();
        }
        return objId;
    }

    void ObjContainer::Clear(bool bDeleteObjectsPassedToAnotherMap)
    {
        for (auto iter = begin(); iter != end(); ++iter)
        {
            M3D_ASSERT(*iter);
            M3D_ASSERT(GetEntityByObjId(iter.m_nodeId + (m_allObjects.m_records[iter.m_nodeId].m_totalObjects << BITS_IN_MAX_OBJECTS)));
            if (!bDeleteObjectsPassedToAnotherMap)
            {
                if (iter->m_bPassedToAnotherMap)
                    continue;
            }

            iter->Remove();
        }
       
        Purge();
        m_allObjects.Clear();
        m_updatingObjects.Clear();
        m_nameToIdMap.clear();
        m_ObjectFullNames.clear();
        m_objIdsToUpdate.clear();
        m_objIdsToNotUpdate.clear();
        m_objectsToPostCollide.clear();
        m_objIdsToRelinkSceneGraphNode.clear();
        m_objIdsToRemove.clear();
        M3D_APP->ImmediateMessage(66543, 0, 0, 0, 0, {}, {});
    }

    m3d::Class* ObjContainer::GetClass() const
    {
        return RT_CLASS_LOCAL(ObjContainer);
    }

    void ObjContainer::SaveNodeStatesToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ObjContainer::SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ObjContainer::const_iterator ObjContainer::end() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ObjContainer::iterator ObjContainer::end()
    {
        return iterator(&m_allObjects.m_records, -1);
    }

    void ObjContainer::LinkGeomsToCollisionCells()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ObjContainer::RelinkSceneGraphNodes()
    {
        PhysicBody::GetCountNodeRelinks()->SetI(0);
        for (auto& objId : m_objIdsToRelinkSceneGraphNode)
        {
            auto* obj = m_allObjects.GetObjById(objId);
            if (obj)
            {
                obj->RelinkSceneGraphNode();
            }
        }
        m_objIdsToRelinkSceneGraphNode.clear();
    }

    void ObjContainer::AddObjIdToRemove(int objId)
    {
        objId = objId & 0x3FFF;
        m_objIdsToRemove.push_back(objId);
    }

    unsigned ObjContainer::size() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::AIParam ObjContainer::GetObjList(char const*, CVector const&, float) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ObjContainer::IncTolerance(int, int, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ObjContainer::AddObjToNotUpdate(Obj*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ObjContainer::PostCollide()
    {
        for (auto& obj : m_objectsToPostCollide)
        {
            obj->PostCollide();
        }
        m_objectsToPostCollide.clear();
    }

    void ObjContainer::AddObjIdToRelinkSceneGraphNode(int objId)
    {
        m_objIdsToRelinkSceneGraphNode.push_back(objId);
    }

    ObjContainer::ObjContainer()
    {
        m_GameTime.setExpanded(8, 0, 14, 9, 1211);
        m_objIdsToUpdate.reserve(0x3E8);
        m_objIdsToNotUpdate.reserve(0x3E8);
        m_objectsToPostCollide.reserve(0x3E8);
        m_objIdsToRelinkSceneGraphNode.reserve(0x3E8);
        m_objIdsToRemove.reserve(0x3E8);
    }

    ObjContainer::ObjContainer(ObjContainer const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ObjContainer::_SetObjNotUpdating(int objId)
    {
        // TODO: check this
        auto* node = m_allObjects._GetNodeById(objId & 0x3FFF);
        if (node && node->m_isValid && node->m_value)
        {
            auto* obj = node->m_value;
            if (!obj->m_bMustBeUpdating && obj->m_updatingObjId != -1)
            {
                auto* updatingNode = m_updatingObjects._GetNodeById(obj->m_updatingObjId & 0x3FFF);
                if (updatingNode)
                {
                    if (updatingNode->m_isValid)
                    {
                        m_updatingObjects.EraseNode(*updatingNode, false);
                    }
                    obj->m_updatingObjId = -1;
                    obj->m_bIsUpdating = false;
                }
            }
        }
    }

    int ObjContainer::_Add(Obj* pObj)
    {
        M3D_ASSERT(pObj);
        M3D_ASSERT(pObj->GetId() == -1);

        auto objId = m_allObjects.Add(pObj);
        pObj->m_objId = objId;
        if (pObj->m_bIsUpdating)
        {
            pObj->m_updatingObjId = m_updatingObjects.Add(pObj);
        }

        CStr name = pObj->GetName();
        if (!name.empty())
        {
            m_nameToIdMap[name] = objId;
        }

        M3D_APP->ImmediateMessage(66541, objId, 0, 0, 0, {}, {});
        return objId;
    }

    void ObjContainer::_PassToMapAfterFading()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ObjContainer::_SetObjUpdating(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ObjContainer::_DeleteObj(Obj*&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void SetObjects(ObjContainer* objects)
    {
        theObjects = objects;
    }
}
