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
#include "server/objects/player.h"
#include "server/objects/vehicle.h"
#include "server/relationship.h"
#include "server/statistic/statisticmanager.h"

#include <core/ref_ptr.h>
#include <core/scoped_ptr.h>
#include <file/fileserver.h>
#include <file/filestream.h>
#include "world.h"
#include "core/timer.h"
#include "server/passagedata.h"

void ShowCurrentStack()
{
    // RVA 0x75C0F0 - appends the current call stack to the exception log (falling back to stderr), by
    // capturing a CONTEXT that points at this function and handing it to ShowStackRM together with a
    // duplicated handle of the calling thread. ShowStackRM - the symbol-resolving stack walker itself -
    // has not been reimplemented, so there is nothing to call and this stays empty; it is only ever used
    // to add detail to the two fatal paths below, which log and abort on their own.
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, CreateNewObject)
{
    auto* objContainer = (ai::ObjContainer*)context->asObject(0, "ObjContainer");
    auto id =
        objContainer->CreateNewObject(context->asInt(1), context->asString(2), context->asInt(3), context->asInt(4));
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
    auto* objContainer = (ai::ObjContainer*)context->asObject(0, "ObjContainer");
    context->pushInt(objContainer->size());
    return 1;
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
    auto* objContainer = (ai::ObjContainer*)context->asObject(0, "ObjContainer");
    auto* pPlayer = (ai::Obj*)context->asObject(3, "Obj");
    auto command = context->asInt(1);
    auto textId = context->asInt(2);
    objContainer->MessageBoxA(command, textId, pPlayer);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, SetTolerance)
{
    context->asObject(0, "ObjContainer");
    auto first = context->asInt(1);
    auto second = context->asInt(2);
    auto tolerance = context->asFloat(3);
    ai::theRelationship->SetTolerance(first, second, tolerance);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, GetTolerance)
{
    context->asObject(0, "ObjContainer");
    auto first = context->asInt(1);
    auto second = context->asInt(2);
    auto res = ai::theRelationship->GetTolerance(first, second);
    context->pushFloat(res);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, IncTolerance)
{
    context->asObject(0, "ObjContainer");
    auto first = context->asInt(1);
    auto second = context->asInt(2);
    auto increment = context->asFloat(3);
    ai::theRelationship->IncTolerance(first, second, increment);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, SetGameTime)
{
    auto* objContainer = (ai::ObjContainer*)context->asObject(0, "ObjContainer");

    int hour = context->asInt(1);
    int minute = context->asInt(2);
    int day = context->asInt(3);
    int month = context->asInt(4);
    int year = context->asInt(5);

    objContainer->SetGameTime(hour, minute, day, month, year);

    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, GetGameTime)
{
    auto* objContainer = (ai::ObjContainer*)context->asObject(0, "ObjContainer");
    auto time = objContainer->getGameTime().asAIParam();
    context->pushAIParam(time);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, Get24HourTime)
{
    auto* objContainer = (ai::ObjContainer*)context->asObject(0, "ObjContainer");
    auto time = objContainer->getGameTime().asAIParam24Hour();
    context->pushAIParam(time);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, PauseGameTime)
{
    auto* objContainer = (ai::ObjContainer*)context->asObject(0, "ObjContainer");
    objContainer->PauseGameTime();
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, UnpauseGameTime)
{
    auto* objContainer = (ai::ObjContainer*)context->asObject(0, "ObjContainer");
    objContainer->UnpauseGameTime();
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, GetHeight)
{
    auto* objContainer = (ai::ObjContainer*)context->asObject(0, "ObjContainer");
    auto x = context->asFloat(1);
    auto z = context->asFloat(2);
    context->pushFloat(objContainer->GetHeight(x, z));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, Dump)
{
    auto* objContainer = (ai::ObjContainer*)context->asObject(0, "ObjContainer");
    objContainer->Dump();
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, DumpPhysicInfo)
{
    auto* objContainer = (ai::ObjContainer*)context->asObject(0, "ObjContainer");
    CStr fileName = context->asString(1);
    objContainer->DumpPhysicInfo(fileName);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, AllowSave)
{
    auto* objContainer = (ai::ObjContainer*)context->asObject(0, "ObjContainer");
    objContainer->AllowSave(context->asBool(1));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, IsSaveAllowed)
{
    auto* objContainer = (ai::ObjContainer*)context->asObject(0, "ObjContainer");
    context->pushBool(objContainer->IsSaveAllowed());
    return 1;
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
        // RVA 0x62D110
        return m_milliSeconds;
    }

    void GameTime::operator+=(float gameTimeSeconds)
    {
        m_milliSeconds += (unsigned __int64)(gameTimeSeconds * 1000.0);
    }

    m3d::AIParam GameTime::asAIParam() const
    {
        // RVA 0x630F10
        // Extract time components from milliseconds
        int64_t const totalMilliseconds = m_milliSeconds;

        // Constants for time calculations
        int64_t const MILLISECONDS_PER_YEAR = 32140800000LL;  // 372 * 24 * 60 * 60 * 1000
        int64_t const MILLISECONDS_PER_MONTH = 2678400000LL;  // 31 * 24 * 60 * 60 * 1000
        int64_t const MILLISECONDS_PER_DAY = 86400000LL;
        int64_t const MILLISECONDS_PER_HOUR = 3600000LL;
        int64_t const MILLISECONDS_PER_MINUTE = 60000LL;

        // Calculate time components
        int const year = static_cast<int>(totalMilliseconds / MILLISECONDS_PER_YEAR);
        int64_t const remainderAfterYears = totalMilliseconds % MILLISECONDS_PER_YEAR;

        int const month = static_cast<int>(remainderAfterYears / MILLISECONDS_PER_MONTH) + 1;
        int64_t const remainderAfterMonths = remainderAfterYears % MILLISECONDS_PER_MONTH;

        int const day = static_cast<int>(remainderAfterMonths / MILLISECONDS_PER_DAY) + 1;
        int64_t const remainderAfterDays = remainderAfterMonths % MILLISECONDS_PER_DAY;

        int const hour = static_cast<int>(remainderAfterDays / MILLISECONDS_PER_HOUR);
        int64_t const remainderAfterHours = remainderAfterDays % MILLISECONDS_PER_HOUR;

        int const minute = static_cast<int>(remainderAfterHours / MILLISECONDS_PER_MINUTE);

        // Create vector with time components in order: hour, minute, day, month, year
        std::vector<int> timeComponents;
        timeComponents.reserve(5);

        timeComponents.push_back(hour);
        timeComponents.push_back(minute);
        timeComponents.push_back(day);
        timeComponents.push_back(month);
        timeComponents.push_back(year);

        return {timeComponents};
    }

    float GameTime::GameDiff(ObjContainer const*) const
    {
        // RVA 0x62D170 - the elapsed game time expressed in real seconds.
        // NOTE: the container argument is never used.
        return static_cast<float>(
            static_cast<double>(m_milliSeconds - m_milliSeconds0) / ai::theGlobProp.m_gameTimeMult * 0.001);
    }

    GameTime::GameTime(int hour, int minute, int day, int month, int year)
    {
        // RVA 0x62E020
        setExpanded(hour, minute, day, month, year);
    }

    GameTime::GameTime(long long milliSeconds)
    {
        // RVA 0x62DAF0
        m_milliSeconds = milliSeconds;
        m_milliSeconds0 = milliSeconds;
    }

    GameTime::GameTime()
    {
    }

    void GameTime::setInt64(long long milliSeconds)
    {
        // RVA 0x62D0F0 - setting the time also resets the point the Diff functions measure from.
        m_milliSeconds = milliSeconds;
        m_milliSeconds0 = milliSeconds;
    }

    void GameTime::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const* xmlNode)
    {
        m3d::SafeInt64Attrib(m_milliSeconds, xmlNode, "Seconds");
        m3d::SafeInt64Attrib(m_milliSeconds0, xmlNode, "Seconds0");
    }

    void GameTime::setExpanded(int hour, int minute, int day, int month, int year)
    {
        // RVA 0x62DB10 - every field is clamped into range first; a month has 30 days here and a year 12 months.
        // The year is not clamped at all.
        int64_t const mth = month <= 0 ? 1 : (month > 12 ? 12 : month);
        int64_t const d = day <= 0 ? 1 : (day > 30 ? 30 : day);
        int64_t const h = hour < 0 ? 0 : (hour > 23 ? 23 : hour);
        int64_t const min = minute < 0 ? 0 : (minute > 59 ? 59 : minute);

        int64_t const res = 60000 * (min + 60 * (h + 24 * (d + 31 * (mth + 12LL * year) - 32)));
        m_milliSeconds = res;
        m_milliSeconds0 = res;
    }

    float GameTime::Diff() const
    {
        return (m_milliSeconds - m_milliSeconds0) * 0.001;
    }

    m3d::AIParam GameTime::asAIParam24Hour() const
    {
        // RVA 0x630D70 - the time of day alone, as hour, minute and second.
        int64_t const MILLISECONDS_PER_YEAR = 32140800000LL;
        int64_t const MILLISECONDS_PER_MONTH = 2678400000LL;
        int64_t const MILLISECONDS_PER_DAY = 86400000LL;
        int64_t const MILLISECONDS_PER_HOUR = 3600000LL;
        int64_t const MILLISECONDS_PER_MINUTE = 60000LL;

        int64_t const millisecondsInDay =
            m_milliSeconds % MILLISECONDS_PER_YEAR % MILLISECONDS_PER_MONTH % MILLISECONDS_PER_DAY;

        std::vector<int> timeComponents;
        timeComponents.reserve(3);
        timeComponents.push_back(static_cast<int>(millisecondsInDay / MILLISECONDS_PER_HOUR));
        timeComponents.push_back(static_cast<int>(millisecondsInDay % MILLISECONDS_PER_HOUR / MILLISECONDS_PER_MINUTE));
        timeComponents.push_back(
            static_cast<int>(millisecondsInDay % MILLISECONDS_PER_HOUR % MILLISECONDS_PER_MINUTE / 1000));

        return {timeComponents};
    }

    void GameTime::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x62DCA0 - NOTE: both attributes are named after seconds but hold milliseconds, matching
        // what LoadFromXML reads back.
        xmlNode->SetAttribute("Seconds", CStr(m_milliSeconds).c_str());
        xmlNode->SetAttribute("Seconds0", CStr(m_milliSeconds0).c_str());
    }

    ObjContainer::Node::Node()
    {
        m_id = -1;
        m_prevId = -1;
        m_nextId = -1;
        m_value = nullptr;
        m_isValid = false;
        m_totalObjects = 0;
    }

    ObjContainer::InnerContainer::InnerContainer()
    {
        Clear();
    }

    void ObjContainer::InnerContainer::EraseNode(Node& node, bool deleteObj)
    {
        // RVA 0x62FE20
        if (node.m_totalObjects < MAX_OBJECTS_IN_CELL)
        {
            m_freePlaces.push_back(node.m_id);
        }

        Node* prevNode = nullptr;
        auto const prevId = node.m_prevId;
        if (prevId == -1)
        {
            prevNode = nullptr;
        }
        else
        {
            prevNode = &m_records[prevId];
        }

        Node* nextNode = nullptr;
        auto const nextId = node.m_nextId;
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
            auto msg = "Error: maximum objects count exceeded: " + CStr(ObjContainerSize) + " when attempting to add " +
                pObj->GetDebugDescription() + " to ObjContainer";
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

        return (record.m_totalObjects << BITS_IN_MAX_OBJECTS) + nodeId;
    }

    bool ObjContainer::InnerContainer::AddWithOwnObjId(Obj* pObj, int id)
    {
        // RVA 0x62E840 - puts an object back into the slot it had when it was saved. The high bits of the id
        // carry how many objects that slot has already held, so a stale id is recognised and refused.
        M3D_ASSERT(id >= 0);

        auto const nodeId = id & MAX_OBJECTS_MASK;
        auto const totalObjects = id >> BITS_IN_MAX_OBJECTS;
        {
            auto const& existing = m_records[nodeId];
            if (totalObjects == existing.m_totalObjects && existing.m_isValid && existing.m_value)
            {
                return false;
            }
        }

        auto& node = m_records[nodeId];
        M3D_ASSERT(!node.m_isValid);

        node.m_totalObjects = totalObjects;

        auto const freePlace = std::find(m_freePlaces.begin(), m_freePlaces.end(), nodeId);
        if (freePlace != m_freePlaces.end())
        {
            m_freePlaces.erase(freePlace);
        }

        node.m_value = pObj;
        node.m_isValid = true;
        if (m_size)
        {
            m_records[m_lastNodeId].m_nextId = nodeId;
            node.m_prevId = m_lastNodeId;
        }
        else
        {
            m_firstNodeId = nodeId;
        }
        ++m_size;
        m_lastNodeId = nodeId;
        return true;
    }

    bool ObjContainer::InnerContainer::empty() const
    {
        // RVA 0x62D740
        return m_size == 0;
    }

    Obj* ObjContainer::InnerContainer::GetObjById(int objId)
    {
        // RVA 0x40C350
        if (objId >= 0)
        {
            auto const& record = m_records[objId & MAX_OBJECTS_MASK];
            if (objId >> BITS_IN_MAX_OBJECTS == record.m_totalObjects && record.m_isValid)
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
        // RVA 0x62D730
        return m_size;
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
        // RVA 0x40BFA0
        m_nodeId = (*m_pRecords)[m_nodeId].m_nextId;
    }

    Obj const* ObjContainer::const_iterator::operator*() const
    {
        // RVA 0x6067D0
        return (*m_pRecords)[m_nodeId].m_value;
    }

    Obj const* ObjContainer::const_iterator::operator->() const
    {
        // RVA 0x62E780
        return (*m_pRecords)[m_nodeId].m_value;
    }

    bool ObjContainer::const_iterator::operator==(const_iterator const& rhs) const
    {
        // RVA 0x40BF10 - NOTE: only the node id is compared; two iterators into different containers
        // that happen to sit on the same slot compare equal.
        return m_nodeId == rhs.m_nodeId;
    }

    bool ObjContainer::const_iterator::operator!=(const_iterator const& rhs) const
    {
        return m_nodeId != rhs.m_nodeId;
    }

    ObjContainer::const_iterator& ObjContainer::const_iterator::operator++()
    {
        // RVA 0x40BF80
        m_nodeId = (*m_pRecords)[m_nodeId].m_nextId;
        return *this;
    }

    ObjContainer::iterator::iterator(retruxx::vector<Node>* pRecords, int nodeId) :
        ObjContainer::const_iterator(pRecords, nodeId)
    {
    }

    Obj* ObjContainer::iterator::operator->() const
    {
        return (*m_pRecords)[m_nodeId].m_value;
    }

    ObjContainer::iterator ObjContainer::iterator::operator++(int)
    {
        // RVA 0x40BF30
        iterator const old = *this;
        m_nodeId = (*m_pRecords)[m_nodeId].m_nextId;
        return old;
    }

    ObjContainer::iterator& ObjContainer::iterator::operator++()
    {
        m_nodeId = (*m_pRecords)[m_nodeId].m_nextId;
        return *this;
    }

    Obj* ObjContainer::iterator::operator*() const
    {
        return (*m_pRecords)[m_nodeId].m_value;
    }

    Obj* ObjContainer::GetEntityByObjName(CStr const& name)
    {
        if (name.empty())
        {
            return nullptr;
        }

        auto const objId = GetObjIdByObjName(name);
        if (objId >= 0)
        {
            return m_allObjects.GetObjById(objId);
        }
        return nullptr;
    }

    void ObjContainer::Purge()
    {
        // RVA 0x630A40 - the end of a frame: everything queued for removal is unhooked from its parent, taken
        // out of the name table and deleted, and then the objects that asked to start or stop updating are
        // moved between the two lists.
        m_inPurge = true;
        m_numRemovalsLastFrame = 0;

        for (auto const objId : m_objIdsToRemove)
        {
            if (static_cast<unsigned>(objId) >= m_allObjects.m_records.size())
            {
                continue;
            }
            auto& node = m_allObjects.m_records[objId];
            if (!node.m_isValid)
            {
                continue;
            }

            // NOTE: the object is used without a null check; a valid node is assumed to carry one.
            auto* const obj = node.m_value;
            ++m_numRemovalsLastFrame;

            auto* const parent = obj->GetParent();
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

            CStr const name = obj->GetName();
            if (!name.empty())
            {
                m_nameToIdMap.erase(name);
            }

            auto const updatingObjId = obj->m_updatingObjId;
            m_allObjects.EraseNode(node, true);

            if (updatingObjId != -1)
            {
                auto const updatingNodeId = static_cast<unsigned>(updatingObjId & MAX_OBJECTS_MASK);
                if (updatingNodeId < m_updatingObjects.m_records.size())
                {
                    auto& updatingNode = m_updatingObjects.m_records[updatingNodeId];
                    if (updatingNode.m_isValid)
                    {
                        m_updatingObjects.EraseNode(updatingNode, false);
                    }
                }
            }
        }

        m_inPurge = false;
        m_objIdsToRemove.clear();

        for (auto const objId : m_objIdsToUpdate)
        {
            _SetObjUpdating(objId);
        }
        m_objIdsToUpdate.clear();

        for (auto const objId : m_objIdsToNotUpdate)
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
        // RVA 0x632A60 - the objects go first; the bookkeeping containers clean themselves up.
        Clear(true);
    }

    CStr ObjContainer::GetObjectFullName(CStr const& ObjectName) const
    {
        CStr res;
        m_ObjectFullNames.get(ObjectName, res);
        return res;
    }

    void ObjContainer::RelinkGeomsToCollisionCells()
    {
        // RVA 0x62F100 - dead objects, objects already queued for removal and objects sitting inside a
        // repository keep their geoms where they are.
        for (auto it = begin(); it != end(); ++it)
        {
            if (!it->IsKindOf(&ai::PhysicObj::m_classPhysicObj))
            {
                continue;
            }
            auto const flags = it->GetFlags();
            if ((flags & 8) == 0 && (flags & 2) == 0 && !it->GetParentRepository())
            {
                ((PhysicObj*)*it)->RelinkGeomsToCollisionCells();
            }
        }
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
        // RVA 0x633BE0 - NOTE: the copy constructor raises a SysError, so cloning a container never
        // returns.
        return new ObjContainer(*this);
    }

    void ObjContainer::PermitCreation()
    {
        // RVA 0x62D290
        --m_denyCreationCount;
    }

    int ObjContainer::CreateNewObjectWithSuspendedPostLoad(
        int prototypeId,
        char const* name,
        int parentId,
        int belongId)
    {
        auto const objId = CreateEntityForLoad(prototypeId, name, parentId, -1);
        if (objId >= 0)
        {
            auto& node = m_allObjects.m_records[objId & MAX_OBJECTS_MASK];
            if (objId >> BITS_IN_MAX_OBJECTS == node.m_totalObjects && node.m_isValid)
            {
                if (node.m_value)
                {
                    node.m_value->SetBelong(belongId);
                    node.m_value->CreateChildren();
                }
            }
        }
        return objId;
    }

    ObjContainer::iterator ObjContainer::updatingBegin()
    {
        return ObjContainer::iterator(&m_updatingObjects.m_records, m_updatingObjects.m_firstNodeId);
    }

    void ObjContainer::LoadNodeStatesFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode)
    {
        // RVA 0x62E440 - the counterpart of SaveNodeStatesToXml.
        ref_ptr nodeXml = xmlFile->CreateNode();
        for (xmlNode->GetFirstChild(nodeXml, "Node"); !nodeXml->IsEmpty(); nodeXml->GetNextSibling(nodeXml, "Node"))
        {
            char const* const id = nodeXml->GetAttribute("Id");
            if (!id)
            {
                continue;
            }
            unsigned const nodeId = atoi(id);
            if (nodeId >= MAX_OBJECTS)
            {
                continue;
            }
            char const* const totalObjects = nodeXml->GetAttribute("TotalObjects");
            if (totalObjects)
            {
                m_allObjects.m_records[nodeId].m_totalObjects = atoi(totalObjects);
            }
        }
    }

    bool ObjContainer::IsSaveAllowed() const
    {
        return m_bSaveAllowed;
    }

    void ObjContainer::PauseGameTime()
    {
        // RVA 0x62D1D0
        m_GameTimePaused = true;
    }

    void ObjContainer::DeleteAll()
    {
        // RVA 0x6317C0 - every object is asked to go; the actual deletion happens in the next Purge.
        for (auto it = begin(); it != end(); ++it)
        {
            it->Remove();
        }
        M3D_APP->ImmediateMessage(66543, 0, 0, 0, 0, {}, {});
    }

    void ObjContainer::PassToMap(CStr const& mapName, CStr const& locationName, int angle, bool bImmediate)
    {
        // RVA 0x633530 - starts the move to another map. Unless it is asked to happen at once, the
        // player's vehicle is made invulnerable and the screen fades first; Update finishes the job.
        // NOTE: a passage that is already in flight is overwritten and its PassageData leaked.
        ai::thePassageData = new PassageData;
        ai::thePassageData->m_mapName = mapName;
        ai::thePassageData->m_locationName = locationName;
        ai::thePassageData->m_angle = angle;
        ai::thePassageData->m_fadingStartTime = M3D_KERNEL->GetTimer().GetCurTimeUnscaled();

        M3D_LOG_INFO(
            "Passing to map '" + mapName + CStr("', location '") + locationName + CStr("', angle = ") + CStr(angle) +
            CStr(", immediate = ") + CStr(bImmediate));

        if (bImmediate)
        {
            _PassToMapAfterFading();
            return;
        }
        if (ai::thePlayer && ai::thePlayer->GetVehicle())
        {
            ai::thePlayer->GetVehicle()->setGodMode(true);
        }
        M3D_APP->EnqueueMessage(66566, 0, 0, 0, 0, {}, {});
    }

    float ObjContainer::GetHeight(float x, float z) const
    {
        // RVA 0x62D260 - the landscape height, straight out of the engine config.
        return M3D_KERNEL->GetEngineCfg().GetHeight(x, z);
    }

    m3d::Class* ObjContainer::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Object);
    }

    int ObjContainer::GetPrototypeId(char const* prototypeName) const
    {
        // RVA 0x62D7D0
        return ai::thePrototypeManager->GetPrototypeId(CStr(prototypeName));
    }

    void ObjContainer::Dump()
    {
        // RVA 0x62F430
        M3D_LOG_INFO("**************** Dumping all objects *************************************");
        M3D_LOG_INFO("Total objects count: " + CStr(m_allObjects.size()));
        for (auto it = begin(); it != end(); ++it)
        {
            M3D_LOG_INFO(it->GetDebugDescription());
        }
        M3D_LOG_INFO("**************** End dumping all objects *********************************");

        M3D_LOG_INFO("**************** Dumping updating objects ********************************");
        M3D_LOG_INFO("Updating objects count: " + CStr(m_updatingObjects.size()));
        for (auto it = updatingBegin(); it != updatingEnd(); ++it)
        {
            M3D_LOG_INFO(it->GetDebugDescription());
        }
        M3D_LOG_INFO("**************** End dumping updating objects ****************************");
    }

    void ObjContainer::SetGameTimeInt64(long long seconds)
    {
        // RVA 0x62D1F0
        m_GameTime.setInt64(seconds);
    }

    int ObjContainer::GetObjIdByObjName(CStr const& name)
    {
        if (name.empty())
        {
            return -1;
        }

        auto const it = m_nameToIdMap.find(name);
        if (it != m_nameToIdMap.end())
        {
            return it->second;
        }
        return -1;
    }

    float ObjContainer::GetTolerance(int PlayerID1, int PlayerID2) const
    {
        // RVA 0x62D1B0
        return ai::theRelationship->GetTolerance(PlayerID1, PlayerID2);
    }

    void ObjContainer::SetGameTime(int hour, int minute, int day, int month, int year)
    {
        m_GameTime.setExpanded(hour, minute, day, month, year);
    }

    m3d::Object* ObjContainer::CreateObject()
    {
        return new ObjContainer;
    }

    void ObjContainer::TransferPhysicParamsToSceneGraph()
    {
        // RVA 0x62EF80
        for (auto it = begin(); it != end(); ++it)
        {
            it->TransferPhysicParamsToSceneGraphNode();
        }
    }

    void ObjContainer::UnpauseGameTime()
    {
        // RVA 0x62D1E0
        m_GameTimePaused = false;
    }

    void ObjContainer::UnlinkGeomsFromCollisionCells()
    {
        // RVA 0x62F0A0
        for (auto it = begin(); it != end(); ++it)
        {
            if (it->IsKindOf(&ai::PhysicBody::m_classPhysicBody))
            {
                ((PhysicBody*)*it)->UnlinkGeomFromCollisionCells();
            }
        }
    }

    void ObjContainer::SetTolerance(int PlayerID1, int PlayerID2, float Tolerance)
    {
        // RVA 0x62D1A0
        ai::theRelationship->SetTolerance(PlayerID1, PlayerID2, Tolerance);
    }

    bool ObjContainer::AddWithOwnObjId(Obj* pObj)
    {
        M3D_ASSERT(pObj);

        auto const result = m_allObjects.AddWithOwnObjId(pObj, pObj->GetId());
        if (result)
        {
            if (pObj->m_bIsUpdating)
            {
                pObj->m_updatingObjId = m_updatingObjects.Add(pObj);
            }

            CStr const name = pObj->GetName();
            if (!name.empty())
            {
                m_nameToIdMap[name] = pObj->GetId();
            }

            M3D_APP->ImmediateMessage(66541, pObj->GetId(), 0, 0, 0, {}, {});
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
        return m_GameTime;
    }

    m3d::AIParam ObjContainer::GetGameTime() const
    {
        return m_GameTime.asAIParam();
    }

    void ObjContainer::AddObjToPostCollideList(Obj* pObj)
    {
        // RVA 0x5FDDE0
        m_objectsToPostCollide.push_back(pObj);
    }

    void ObjContainer::DumpPhysicInfo(CStr const& fileName) const
    {
        // RVA 0x62F180 - writes one xml file describing every physic object and every physic body, for
        // debugging. Both kinds land side by side under <Root>.
        scoped_ptr fileStream = M3D_KERNEL->GetFileServer().CreateFileStream();
        if (!fileStream->Open(fileName.c_str(), m3d::fs::IStream::OPEN_WRITE))
        {
            return;
        }

        ref_ptr xmlFile = M3D_KERNEL->CreateXmlFile();
        ref_ptr ndRoot = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Root");
        xmlFile->AddChild(ndRoot);

        for (auto it = begin(); it != end(); ++it)
        {
            if (it->IsKindOf(&ai::PhysicObj::m_classPhysicObj))
            {
                ref_ptr ndObj = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "PhysicObj");
                ndRoot->AddChild(ndObj);
                ((PhysicObj const*)*it)->DumpPhysicInfo(xmlFile, ndObj);
            }
            if (it->IsKindOf(&ai::PhysicBody::m_classPhysicBody))
            {
                ref_ptr ndBody = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "PhysicBody");
                ndRoot->AddChild(ndBody);
                ((PhysicBody const*)*it)->DumpPhysicInfo(xmlFile, ndBody);
            }
        }

        xmlFile->Write(*fileStream);
        fileStream->Close();
    }

    long long ObjContainer::GetGameTimeInt64() const
    {
        // RVA 0x62D220
        return m_GameTime.asInt64();
    }

    unsigned ObjContainer::GetNumUpdatingObjects() const
    {
        // RVA 0x41D530
        return m_updatingObjects.size();
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
        // RVA 0x62D280
        ++m_denyCreationCount;
    }

    void ObjContainer::LoadObjectNamesFromXML(CStr const& fileName)
    {
        // RVA 0x631C90 - reads the table that maps an object's internal name to the name shown to the
        // player.
        scoped_ptr fileStream = M3D_KERNEL->GetFileServer().CreateFileStream();
        if (!fileStream->Open(fileName.c_str(), m3d::fs::IStream::OPEN_READ))
        {
            M3D_LOG_ERR("Error: cannot open " + fileName);
            return;
        }

        ref_ptr xmlFile = M3D_KERNEL->CreateXmlFile();
        if (!xmlFile->Read(*fileStream))
        {
            M3D_LOG_ERR("Error: cannot parse " + fileName + CStr(" ( ") + CStr(xmlFile->GetError()) + CStr(" ) "));
            return;
        }
        fileStream->Close();

        ref_ptr rootNode = xmlFile->CreateNode();
        xmlFile->GetFirstChild(rootNode, "ObjectNames");
        if (rootNode->IsEmpty())
        {
            M3D_LOG_ERR("Error: Tag <ObjectNames> not found in file: " + fileName);
            return;
        }

        ref_ptr objectNode = xmlFile->CreateNode();
        for (rootNode->GetFirstChild(objectNode, "Object"); !objectNode->IsEmpty();
             objectNode->GetNextSibling(objectNode, "Object"))
        {
            CStr const name = objectNode->GetAttribute("Name");
            CStr const cFullName = objectNode->GetAttribute("FullName");
            CStr tFullName;
            tFullName = cFullName;
            m_ObjectFullNames.add(name, tFullName);
        }
    }

    m3d::AIParam ObjContainer::Get24HourTime() const
    {
        // RVA 0x631C70
        return m_GameTime.asAIParam24Hour();
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

    void ObjContainer::MessageBoxA(int command, int textId, Obj*)
    {
        // RVA 0x631BB0 - NOTE: the player argument is ignored; the message goes to whoever listens.
        M3D_APP->EnqueueMessage(66550, command, textId, 0, 0, {}, {});
    }

    void ObjContainer::AllowSave(bool allow)
    {
        m_bSaveAllowed = allow;
    }

    float ObjContainer::GetGameTimeDiff() const
    {
        return m_GameTime.Diff();
    }

    ObjContainer::iterator ObjContainer::begin()
    {
        return ObjContainer::iterator(&m_allObjects.m_records, m_allObjects.m_firstNodeId);
    }

    ObjContainer::const_iterator ObjContainer::begin() const
    {
        // RVA 0x62D750
        return const_iterator(&m_allObjects.m_records, m_allObjects.m_firstNodeId);
    }

    int ObjContainer::GetNumRemovalsLastFrame() const
    {
        // RVA 0x41D520
        return m_numRemovalsLastFrame;
    }

    void ObjContainer::Update(float elapsedTime, unsigned workTime, bool bCinematic)
    {
        // RVA 0x632EC0
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
            m_GameTime += ai::theGlobProp.m_gameTimeMult * elapsedTime;

            // Update global game time statistic
            auto* gameTimeStat = dynamic_cast<ai::TimeStatistic*>(
                theStatisticManager->GetStatistic(ai::STATISTIC_GAME_TIME, "TimeStatistic"));

            gameTimeStat->m_bGlobalFlag = true;
            gameTimeStat->IncreaseByMilliseconds(ai::theGlobProp.m_gameTimeMult * elapsedTime * 1000.0);

            // Update level-specific game time statistic
            auto levelStatName = ai::STATISTIC_GAME_TIME + ai::pServer->GetWorld()->m_level->m_levelName;
            auto* levelGameTimeStat =
                dynamic_cast<ai::TimeStatistic*>(theStatisticManager->GetStatistic(levelStatName, "TimeStatistic"));

            levelGameTimeStat->m_bGlobalFlag = false;
            levelGameTimeStat->IncreaseByMilliseconds(ai::theGlobProp.m_gameTimeMult * elapsedTime * 1000.0);
        }

        // Update real time statistics (always updated)
        auto* realTimeStat = dynamic_cast<ai::TimeStatistic*>(
            theStatisticManager->GetStatistic(ai::STATISTIC_REAL_TIME, "TimeStatistic"));
        realTimeStat->m_bGlobalFlag = true;
        uint64_t realTimeDelta = static_cast<uint64_t>(elapsedTime * 1000.0);
        realTimeStat->IncreaseByMilliseconds(realTimeDelta);

        // Update level-specific real time statistic
        CStr levelRealTimeName(ai::STATISTIC_REAL_TIME);
        levelRealTimeName += ai::pServer->GetWorld()->m_level->m_levelName;
        TimeStatistic* levelRealTimeStat =
            dynamic_cast<ai::TimeStatistic*>(ai::theStatisticManager->GetStatistic(levelRealTimeName, "TimeStatistic"));

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
            M3D_ASSERT(GetEntityByObjId(
                iter.m_nodeId + (m_allObjects.m_records[iter.m_nodeId].m_totalObjects << BITS_IN_MAX_OBJECTS)));
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

    void ObjContainer::SaveNodeStatesToXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x62E050 - records, for every slot that has ever held an object, how many it has held. That
        // count is the high half of an object id, so a save can only be reloaded into a container whose
        // slots agree.
        for (auto const& node : m_allObjects.m_records)
        {
            if (node.m_totalObjects > 0)
            {
                ref_ptr nodeXml = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Node");
                nodeXml->SetAttribute("Id", CStr(node.m_id).c_str());
                nodeXml->SetAttribute("TotalObjects", CStr(node.m_totalObjects).c_str());
                xmlNode->AddChild(nodeXml);
            }
        }
    }

    void ObjContainer::SaveToXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x62ECF0 - only the roots are written out: an object that has a parent, sits in a repository
        // or has already travelled to another map is saved by whoever owns it, or not at all.
        if (m_SaveType == SAVE_FULL)
        {
            ref_ptr runtimeNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "ObjContainerRuntime");
            SaveNodeStatesToXml(xmlFile, runtimeNode);
            if (runtimeNode->HasChildOrAttribute())
            {
                xmlNode->AddChild(runtimeNode);
            }
        }

        for (auto it = begin(); it != end(); ++it)
        {
            const_cast<Obj*>(*it)->m_bIsAlreadySaved = false;
        }

        for (auto it = begin(); it != end(); ++it)
        {
            if (it->m_parentId != -1 || it->GetParentRepository() || it->GetPassedToAnotherMapStatus())
            {
                continue;
            }
            ref_ptr objNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Object");
            it->SaveToXML(xmlFile, objNode);
            if (objNode->HasChildOrAttribute())
            {
                xmlNode->AddChild(objNode);
            }
        }
    }

    ObjContainer::const_iterator ObjContainer::end() const
    {
        // RVA 0x62D770
        return const_iterator(&m_allObjects.m_records, -1);
    }

    ObjContainer::iterator ObjContainer::end()
    {
        return iterator(&m_allObjects.m_records, -1);
    }

    void ObjContainer::LinkGeomsToCollisionCells()
    {
        // RVA 0x62F040
        for (auto it = begin(); it != end(); ++it)
        {
            if (it->IsKindOf(&ai::PhysicBody::m_classPhysicBody))
            {
                ((PhysicBody*)*it)->LinkGeomToCollisionCells();
            }
        }
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
        objId = objId & MAX_OBJECTS_MASK;
        m_objIdsToRemove.push_back(objId);
    }

    unsigned ObjContainer::size() const
    {
        // RVA 0x41D540
        return m_allObjects.size();
    }

    m3d::AIParam ObjContainer::GetObjList(char const*, CVector const&, float) const
    {
        // RVA 0x631B80 - NOTE: never implemented in the shipped game. It ignores the class name, the
        // position and the radius and hands back an empty AIParam.
        return m3d::AIParam();
    }

    void ObjContainer::IncTolerance(int PlayerID1, int PlayerID2, float IncTolerance)
    {
        // RVA 0x62D1C0
        ai::theRelationship->IncTolerance(PlayerID1, PlayerID2, IncTolerance);
    }

    void ObjContainer::AddObjToNotUpdate(Obj* obj)
    {
        if (obj)
        {
            if (obj->m_objId != -1)
            {
                obj->m_bMustBeUpdating = 0;
                m_objIdsToNotUpdate.push_back(obj->m_objId);
            }
        }
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
        m_SaveType = SAVE_LEVEL;
        m_GameTimePaused = false;
        m_GameTime.setExpanded(8, 0, 14, 9, 1211);
        m_denyCreationCount = 0;
        m_objIdsToUpdate.reserve(0x3E8);
        m_objIdsToNotUpdate.reserve(0x3E8);
        m_objectsToPostCollide.reserve(0x3E8);
        m_objIdsToRelinkSceneGraphNode.reserve(0x3E8);
        m_objIdsToRemove.reserve(0x3E8);
        m_inPurge = false;
        m_inUpdate = false;
        m_bSaveAllowed = true;
    }

    ObjContainer::ObjContainer(ObjContainer const&)
    {
        // RVA 0x632C10 - the members are all default-constructed and then the copy is refused; nothing
        // of the source container is ever read.
        SYS_ERROR("!\"Not implemented\"");
    }

    void ObjContainer::_SetObjNotUpdating(int objId)
    {
        // RVA 0x630520
        if (objId >= 0)
        {
            auto const& node = m_allObjects.m_records[objId & MAX_OBJECTS_MASK];
            if (objId >> BITS_IN_MAX_OBJECTS == node.m_totalObjects && node.m_isValid)
            {
                auto* value = node.m_value;
                if (value)
                {
                    if (!value->m_bMustBeUpdating && value->m_updatingObjId != -1)
                    {
                        auto const updatingId = value->m_updatingObjId & MAX_OBJECTS_MASK;
                        if (updatingId < m_updatingObjects.m_records.size())
                        {
                            auto& updatingNode = m_updatingObjects.m_records[updatingId];
                            if (updatingNode.m_isValid)
                            {
                                m_updatingObjects.EraseNode(updatingNode, false);
                            }
                            value->m_updatingObjId = -1;
                            value->m_bIsUpdating = false;
                        }
                    }
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
        // RVA 0x6326F0 - the fade is over: take the objects that travel with the player out of this
        // container and tell the game to load the next map.
        if (!ai::thePassageData->m_fadingStartTime)
        {
            return;
        }
        if (ai::thePlayer && ai::thePlayer->GetVehicle())
        {
            ai::thePlayer->GetVehicle()->setGodMode(false);
        }
        ai::thePassageData->TakeNeededObjectsFromObjContainer();
        ai::theQuestStateManager->OnPlayerPassToMap(ai::thePassageData->m_mapName);
        ai::thePassageData->m_fadingStartTime = 0;
        M3D_APP->EnqueueMessage(66549, 0, 0, 0, 0, {}, {});
    }

    void ObjContainer::_SetObjUpdating(int objId)
    {
        // RVA 0x6304D0
        if (objId < 0)
        {
            return;
        }
        auto const& node = m_allObjects.m_records[objId & MAX_OBJECTS_MASK];
        if (objId >> BITS_IN_MAX_OBJECTS != node.m_totalObjects || !node.m_isValid)
        {
            return;
        }
        auto* const value = node.m_value;
        if (value && value->m_bMustBeUpdating && value->m_updatingObjId == -1)
        {
            value->m_updatingObjId = m_updatingObjects.Add(value);
            value->m_bIsUpdating = true;
        }
    }

    void ObjContainer::_DeleteObj(Obj*& pObj)
    {
        // RVA 0x62D820
        delete pObj;
        pObj = nullptr;
    }

    void SetObjects(ObjContainer* objects)
    {
        theObjects = objects;
    }
}  // namespace ai
