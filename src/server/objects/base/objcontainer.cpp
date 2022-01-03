#include "objcontainer.h"
#include <core/aiparam.h>
#include <stdexcept>

namespace ai
{
    ObjContainer* theObjects = nullptr;

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
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
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
}
