#include "objcontainer.h"
#include <core/aiparam.h>
#include <stdexcept>

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, CreateNewObject)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, GetEntityByObjId)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, size)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, GetEntityByObjName)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, GetPrototypeId)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, MessageBox)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, SetTolerance)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, GetTolerance)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, IncTolerance)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, SetGameTime)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, GetGameTime)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, Get24HourTime)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, PauseGameTime)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, UnpauseGameTime)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, GetHeight)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, Dump)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, DumpPhysicInfo)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, AllowSave)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(ObjContainer, IsSaveAllowed)
{
    throw std::logic_error("Not implemented");
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

    ObjContainer* theObjects = nullptr;

    long long GameTime::asInt64() const
    {
        throw std::logic_error("Not implemented");
    }

    void GameTime::operator+=(float)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::AIParam GameTime::asAIParam() const
    {
        throw std::logic_error("Not implemented");
    }

    float GameTime::GameDiff(ObjContainer const*) const
    {
        throw std::logic_error("Not implemented");
    }

    GameTime::GameTime(int, int, int, int, int)
    {
        throw std::logic_error("Not implemented");
    }

    GameTime::GameTime(long long)
    {
        throw std::logic_error("Not implemented");
    }

    GameTime::GameTime()
    {
    }

    void GameTime::setInt64(long long)
    {
        throw std::logic_error("Not implemented");
    }

    void GameTime::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    m3d::AIParam GameTime::asAIParam24Hour() const
    {
        throw std::logic_error("Not implemented");
    }

    void GameTime::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    int ObjContainer::InnerContainer::Add(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    bool ObjContainer::InnerContainer::AddWithOwnObjId(Obj*, int)
    {
        throw std::logic_error("Not implemented");
    }

    bool ObjContainer::InnerContainer::empty() const
    {
        throw std::logic_error("Not implemented");
    }

    Obj* ObjContainer::InnerContainer::GetObjById(int)
    {
        throw std::logic_error("Not implemented");
    }

    void ObjContainer::InnerContainer::Clear()
    {
        throw std::logic_error("Not implemented");
    }

    unsigned ObjContainer::InnerContainer::size() const
    {
        throw std::logic_error("Not implemented");
    }

    ObjContainer::Node* ObjContainer::InnerContainer::_GetNodeById(int)
    {
        throw std::logic_error("Not implemented");
    }

    ObjContainer::const_iterator::const_iterator(std::vector<Node> const*, int)
    {
        throw std::logic_error("Not implemented");
    }

    void ObjContainer::const_iterator::_Inc()
    {
        throw std::logic_error("Not implemented");
    }

    Obj const* ObjContainer::const_iterator::operator*() const
    {
        throw std::logic_error("Not implemented");
    }

    Obj const* ObjContainer::const_iterator::operator->() const
    {
        throw std::logic_error("Not implemented");
    }

    bool ObjContainer::const_iterator::operator==(const_iterator const&) const
    {
        throw std::logic_error("Not implemented");
    }

    bool ObjContainer::const_iterator::operator!=(const_iterator const&) const
    {
        throw std::logic_error("Not implemented");
    }

    ObjContainer::const_iterator& ObjContainer::const_iterator::operator++()
    {
        throw std::logic_error("Not implemented");
    }

    ObjContainer::iterator::iterator(std::vector<Node>* pRecords, int nodeId) : const_iterator(pRecords, nodeId)
    {
        throw std::logic_error("Not implemented");
    }

    Obj* ObjContainer::iterator::operator->() const
    {
        throw std::logic_error("Not implemented");
    }

    ObjContainer::iterator ObjContainer::iterator::operator++(int)
    {
        throw std::logic_error("Not implemented");
    }

    ObjContainer::iterator& ObjContainer::iterator::operator++()
    {
        throw std::logic_error("Not implemented");
    }

    Obj* ObjContainer::iterator::operator*() const
    {
        throw std::logic_error("Not implemented");
    }

    Obj* ObjContainer::GetEntityByObjName(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    void ObjContainer::Purge()
    {
        throw std::logic_error("Not implemented");
    }

    ObjContainer::iterator ObjContainer::updatingEnd()
    {
        throw std::logic_error("Not implemented");
    }

    ObjContainer::~ObjContainer()
    {
        throw std::logic_error("Not implemented");
    }

    CStr ObjContainer::GetObjectFullName(CStr const&) const
    {
        throw std::logic_error("Not implemented");
    }

    void ObjContainer::RelinkGeomsToCollisionCells()
    {
        throw std::logic_error("Not implemented");
    }

    void ObjContainer::AddObjToUpdate(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* ObjContainer::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    void ObjContainer::PermitCreation()
    {
        throw std::logic_error("Not implemented");
    }

    int ObjContainer::CreateNewObjectWithSuspendedPostLoad(int, char const*, int, int)
    {
        throw std::logic_error("Not implemented");
    }

    ObjContainer::iterator ObjContainer::updatingBegin()
    {
        throw std::logic_error("Not implemented");
    }

    void ObjContainer::LoadNodeStatesFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*)
    {
        throw std::logic_error("Not implemented");
    }

    bool ObjContainer::IsSaveAllowed() const
    {
        throw std::logic_error("Not implemented");
    }

    void ObjContainer::PauseGameTime()
    {
        throw std::logic_error("Not implemented");
    }

    void ObjContainer::DeleteAll()
    {
        throw std::logic_error("Not implemented");
    }

    void ObjContainer::PassToMap(CStr const&, CStr const&, int, bool)
    {
        throw std::logic_error("Not implemented");
    }

    float ObjContainer::GetHeight(float, float) const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* ObjContainer::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    int ObjContainer::GetPrototypeId(char const*) const
    {
        throw std::logic_error("Not implemented");
    }

    void ObjContainer::Dump()
    {
        throw std::logic_error("Not implemented");
    }

    void ObjContainer::SetGameTimeInt64(long long)
    {
        throw std::logic_error("Not implemented");
    }

    int ObjContainer::GetObjIdByObjName(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    float ObjContainer::GetTolerance(int, int) const
    {
        throw std::logic_error("Not implemented");
    }

    void ObjContainer::SetGameTime(int, int, int, int, int)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* ObjContainer::CreateObject()
    {
        return new ObjContainer;
    }

    void ObjContainer::TransferPhysicParamsToSceneGraph()
    {
        throw std::logic_error("Not implemented");
    }

    void ObjContainer::UnpauseGameTime()
    {
        throw std::logic_error("Not implemented");
    }

    void ObjContainer::UnlinkGeomsFromCollisionCells()
    {
        throw std::logic_error("Not implemented");
    }

    void ObjContainer::SetTolerance(int, int, float)
    {
        throw std::logic_error("Not implemented");
    }

    bool ObjContainer::AddWithOwnObjId(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    void ObjContainer::SetObjName(int, CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    GameTime& ObjContainer::getGameTime()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::AIParam ObjContainer::GetGameTime() const
    {
        throw std::logic_error("Not implemented");
    }

    void ObjContainer::AddObjToPostCollideList(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    void ObjContainer::DumpPhysicInfo(CStr const&) const
    {
        throw std::logic_error("Not implemented");
    }

    long long ObjContainer::GetGameTimeInt64() const
    {
        throw std::logic_error("Not implemented");
    }

    unsigned ObjContainer::GetNumUpdatingObjects() const
    {
        throw std::logic_error("Not implemented");
    }

    bool ObjContainer::empty() const
    {
        throw std::logic_error("Not implemented");
    }

    Obj* ObjContainer::GetEntityByObjId(int)
    {
        throw std::logic_error("Not implemented");
    }

    void ObjContainer::DenyCreation()
    {
        throw std::logic_error("Not implemented");
    }

    void ObjContainer::LoadObjectNamesFromXML(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::AIParam ObjContainer::Get24HourTime() const
    {
        throw std::logic_error("Not implemented");
    }

    int ObjContainer::CreateEntityForLoad(int, char const*, int, int)
    {
        throw std::logic_error("Not implemented");
    }

    void ObjContainer::MessageBoxA(int, int, Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    void ObjContainer::AllowSave(bool)
    {
        throw std::logic_error("Not implemented");
    }

    float ObjContainer::GetGameTimeDiff() const
    {
        throw std::logic_error("Not implemented");
    }

    ObjContainer::iterator ObjContainer::begin()
    {
        throw std::logic_error("Not implemented");
    }

    ObjContainer::const_iterator ObjContainer::begin() const
    {
        throw std::logic_error("Not implemented");
    }

    int ObjContainer::GetNumRemovalsLastFrame() const
    {
        throw std::logic_error("Not implemented");
    }

    void ObjContainer::Update(float, unsigned, bool)
    {
        throw std::logic_error("Not implemented");
    }

    int ObjContainer::CreateNewObject(int, char const*, int, int)
    {
        throw std::logic_error("Not implemented");
    }

    void ObjContainer::Clear(bool)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* ObjContainer::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    void ObjContainer::SaveNodeStatesToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    void ObjContainer::SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    ObjContainer::const_iterator ObjContainer::end() const
    {
        throw std::logic_error("Not implemented");
    }

    ObjContainer::iterator ObjContainer::end()
    {
        throw std::logic_error("Not implemented");
    }

    void ObjContainer::LinkGeomsToCollisionCells()
    {
        throw std::logic_error("Not implemented");
    }

    void ObjContainer::RelinkSceneGraphNodes()
    {
        throw std::logic_error("Not implemented");
    }

    void ObjContainer::AddObjIdToRemove(int)
    {
        throw std::logic_error("Not implemented");
    }

    unsigned ObjContainer::size() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::AIParam ObjContainer::GetObjList(char const*, CVector const&, float) const
    {
        throw std::logic_error("Not implemented");
    }

    void ObjContainer::IncTolerance(int, int, float)
    {
        throw std::logic_error("Not implemented");
    }

    void ObjContainer::AddObjToNotUpdate(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    void ObjContainer::PostCollide()
    {
        throw std::logic_error("Not implemented");
    }

    void ObjContainer::AddObjIdToRelinkSceneGraphNode(int)
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    void ObjContainer::_SetObjNotUpdating(int)
    {
        throw std::logic_error("Not implemented");
    }

    int ObjContainer::_Add(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    void ObjContainer::_PassToMapAfterFading()
    {
        throw std::logic_error("Not implemented");
    }

    void ObjContainer::_SetObjUpdating(int)
    {
        throw std::logic_error("Not implemented");
    }

    void ObjContainer::_DeleteObj(Obj*&)
    {
        throw std::logic_error("Not implemented");
    }

    void SetObjects(ObjContainer* objects)
    {
        theObjects = objects;
    }
}
