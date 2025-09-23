#include "objcontainer.h"
#include <core/aiparam.h>
#include <stdexcept>
#include <server/objects/physicbodies/physicbody.h>
#include <core/debugcounter.h>

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, CreateNewObject)
{
    throw retruxx::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, GetEntityByObjId)
{
    throw retruxx::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, size)
{
    throw retruxx::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, GetEntityByObjName)
{
    throw retruxx::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, GetPrototypeId)
{
    throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
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
        __int64 res = 60000 * (minute + 60 * (hour + 24 * (day + 31 * (month + 12 * year) - 32)));
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
    }

    ObjContainer::InnerContainer::InnerContainer()
    {
        m_records.resize(0x4000, {});
        m_freePlaces.reserve(0x4000);
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

    int ObjContainer::InnerContainer::Add(Obj*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool ObjContainer::InnerContainer::AddWithOwnObjId(Obj*, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool ObjContainer::InnerContainer::empty() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    Obj* ObjContainer::InnerContainer::GetObjById(int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ObjContainer::InnerContainer::Clear()
    {
        throw retruxx::logic_error("Not implemented");
    }

    unsigned ObjContainer::InnerContainer::size() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    ObjContainer::Node* ObjContainer::InnerContainer::_GetNodeById(int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    ObjContainer::const_iterator::const_iterator(retruxx::vector<Node> const*, int)
    {
        throw retruxx::logic_error("Not implemented");
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

    bool ObjContainer::const_iterator::operator!=(const_iterator const&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    ObjContainer::const_iterator& ObjContainer::const_iterator::operator++()
    {
        throw retruxx::logic_error("Not implemented");
    }

    ObjContainer::iterator::iterator(retruxx::vector<Node>* pRecords, int nodeId) :
        ObjContainer::const_iterator(pRecords, nodeId)
    {
        throw retruxx::logic_error("Not implemented");
    }

    Obj* ObjContainer::iterator::operator->() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    ObjContainer::iterator ObjContainer::iterator::operator++(int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    ObjContainer::iterator& ObjContainer::iterator::operator++()
    {
        throw retruxx::logic_error("Not implemented");
    }

    Obj* ObjContainer::iterator::operator*() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    Obj* ObjContainer::GetEntityByObjName(CStr const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ObjContainer::Purge()
    {
        throw retruxx::logic_error("Not implemented");
    }

    ObjContainer::iterator ObjContainer::updatingEnd()
    {
        throw retruxx::logic_error("Not implemented");
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

    void ObjContainer::AddObjToUpdate(Obj*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Object* ObjContainer::Clone()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ObjContainer::PermitCreation()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int ObjContainer::CreateNewObjectWithSuspendedPostLoad(int, char const*, int, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    ObjContainer::iterator ObjContainer::updatingBegin()
    {
        throw retruxx::logic_error("Not implemented");
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

    int ObjContainer::GetObjIdByObjName(CStr const&)
    {
        throw retruxx::logic_error("Not implemented");
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

    bool ObjContainer::AddWithOwnObjId(Obj*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ObjContainer::SetObjName(int, CStr const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    GameTime& ObjContainer::getGameTime()
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::AIParam ObjContainer::GetGameTime() const
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    Obj* ObjContainer::GetEntityByObjId(int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ObjContainer::DenyCreation()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ObjContainer::LoadObjectNamesFromXML(CStr const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::AIParam ObjContainer::Get24HourTime() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int ObjContainer::CreateEntityForLoad(int, char const*, int, int)
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    ObjContainer::const_iterator ObjContainer::begin() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int ObjContainer::GetNumRemovalsLastFrame() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ObjContainer::Update(float, unsigned, bool)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int ObjContainer::CreateNewObject(int, char const*, int, int)
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    void ObjContainer::LinkGeomsToCollisionCells()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ObjContainer::RelinkSceneGraphNodes()
    {
        // TODO: implement ObjContainer::RelinkSceneGraphNodes
        //throw retruxx::logic_error("Not implemented");
        //auto* countNode = ai::PhysicBody::GetCountNodeRelinks();
        //countNode->SetI(0);
        //for ()
    }

    void ObjContainer::AddObjIdToRemove(int)
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    void ObjContainer::AddObjIdToRelinkSceneGraphNode(int)
    {
        throw retruxx::logic_error("Not implemented");
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

    int ObjContainer::_Add(Obj*)
    {
        throw retruxx::logic_error("Not implemented");
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
