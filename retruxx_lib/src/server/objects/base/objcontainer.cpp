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
    throw retruxx::logic_error("Not implemented");
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
    throw retruxx::logic_error("Not implemented");
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
    throw retruxx::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, SetTolerance)
{
    throw retruxx::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, GetTolerance)
{
    throw retruxx::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, IncTolerance)
{
    throw retruxx::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, SetGameTime)
{
    throw retruxx::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, GetGameTime)
{
    throw retruxx::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, Get24HourTime)
{
    throw retruxx::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, PauseGameTime)
{
    throw retruxx::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, UnpauseGameTime)
{
    throw retruxx::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, GetHeight)
{
    throw retruxx::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, Dump)
{
    throw retruxx::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, DumpPhysicInfo)
{
    throw retruxx::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, AllowSave)
{
    throw retruxx::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, IsSaveAllowed)
{
    throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    void GameTime::operator+=(float)
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    GameTime::GameTime(int, int, int, int, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    GameTime::GameTime(long long)
    {
        throw retruxx::logic_error("Not implemented");
    }

    GameTime::GameTime()
    {
    }

    void GameTime::setInt64(long long)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void GameTime::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    m3d::AIParam GameTime::asAIParam24Hour() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void GameTime::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw retruxx::logic_error("Not implemented");
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
        this->m_firstNodeId = -1;
        this->m_lastNodeId = -1;
        this->m_size = 0;

        m_records.resize(ObjContainerSize, {});
        m_freePlaces.reserve(ObjContainerSize);
        for (int i = m_records.size() - 1; i!=0; --i)
        {
            m_records[i].m_id = i;
            m_freePlaces.push_back(i);
        }
    }

    void ObjContainer::InnerContainer::EraseNode(Node&, bool)
    {
        throw retruxx::logic_error("Not implemented");
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


        throw retruxx::logic_error("Not implemented");
    }

    bool ObjContainer::InnerContainer::empty() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    Obj* ObjContainer::InnerContainer::GetObjById(int objId)
    {
        if (objId >= 0)
        {
            // TODO: check this
            const auto& record = m_records[objId];
            if (objId >> 14 == record.m_totalObjects && record.m_isValid)
            {
                return record.m_value;
            }
        }
        return nullptr;
    }

    void ObjContainer::InnerContainer::Clear()
    {
        throw retruxx::logic_error("Not implemented");
    }

    unsigned ObjContainer::InnerContainer::size() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    ObjContainer::Node* ObjContainer::InnerContainer::_GetNodeById(int id)
    {
        if (id == -1)
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
        throw retruxx::logic_error("Not implemented");
    }

    Obj const* ObjContainer::const_iterator::operator*() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    Obj const* ObjContainer::const_iterator::operator->() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool ObjContainer::const_iterator::operator==(const_iterator const&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool ObjContainer::const_iterator::operator!=(const_iterator const& rhs) const
    {
        return this->m_nodeId != rhs.m_nodeId;
    }

    ObjContainer::const_iterator& ObjContainer::const_iterator::operator++()
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
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
        // TODO: implement ObjContainer::Purge
        //throw retruxx::logic_error("Not implemented");
    }

    ObjContainer::iterator ObjContainer::updatingEnd()
    {
        return iterator(&m_updatingObjects.m_records, -1);
    }

    ObjContainer::~ObjContainer()
    {
        throw retruxx::logic_error("Not implemented");
    }

    CStr ObjContainer::GetObjectFullName(CStr const&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ObjContainer::RelinkGeomsToCollisionCells()
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    void ObjContainer::PermitCreation()
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    bool ObjContainer::IsSaveAllowed() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ObjContainer::PauseGameTime()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ObjContainer::DeleteAll()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ObjContainer::PassToMap(CStr const&, CStr const&, int, bool)
    {
        throw retruxx::logic_error("Not implemented");
    }

    float ObjContainer::GetHeight(float, float) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Class* ObjContainer::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Object);
    }

    int ObjContainer::GetPrototypeId(char const*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ObjContainer::Dump()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ObjContainer::SetGameTimeInt64(long long)
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    void ObjContainer::SetGameTime(int, int, int, int, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Object* ObjContainer::CreateObject()
    {
        return new ObjContainer;
    }

    void ObjContainer::TransferPhysicParamsToSceneGraph()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ObjContainer::UnpauseGameTime()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ObjContainer::UnlinkGeomsFromCollisionCells()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ObjContainer::SetTolerance(int, int, float)
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    void ObjContainer::DumpPhysicInfo(CStr const&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    long long ObjContainer::GetGameTimeInt64() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    unsigned ObjContainer::GetNumUpdatingObjects() const
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    void ObjContainer::LoadObjectNamesFromXML(CStr const&)
    {
        // TODO: implement ObjContainer::LoadObjectNamesFromXML
        //throw retruxx::logic_error("Not implemented");
    }

    m3d::AIParam ObjContainer::Get24HourTime() const
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    void ObjContainer::AllowSave(bool allow)
    {
        m_bSaveAllowed = allow;
    }

    float ObjContainer::GetGameTimeDiff() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    ObjContainer::iterator ObjContainer::begin()
    {
        return ObjContainer::iterator(&m_allObjects.m_records, m_allObjects.m_firstNodeId);
    }

    ObjContainer::const_iterator ObjContainer::begin() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int ObjContainer::GetNumRemovalsLastFrame() const
    {
        throw retruxx::logic_error("Not implemented");
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

    void ObjContainer::Clear(bool)
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Class* ObjContainer::GetClass() const
    {
        return RT_CLASS_LOCAL(ObjContainer);
    }

    void ObjContainer::SaveNodeStatesToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ObjContainer::SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    ObjContainer::const_iterator ObjContainer::end() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    ObjContainer::iterator ObjContainer::end()
    {
        return iterator(&m_allObjects.m_records, -1);
    }

    void ObjContainer::LinkGeomsToCollisionCells()
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    m3d::AIParam ObjContainer::GetObjList(char const*, CVector const&, float) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ObjContainer::IncTolerance(int, int, float)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ObjContainer::AddObjToNotUpdate(Obj*)
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    void ObjContainer::_SetObjNotUpdating(int)
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    void ObjContainer::_SetObjUpdating(int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ObjContainer::_DeleteObj(Obj*&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SetObjects(ObjContainer* objects)
    {
        theObjects = objects;
    }
}
