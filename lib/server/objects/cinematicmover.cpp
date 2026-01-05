#include "cinematicmover.h"

#include <stdexcept>

RT_CLASS_EXPORT_METHOD_DEFINE(CinematicMover, SetObjAndPath)
{
    RETRUXX_NOT_IMPLEMENTED;
}

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(CinematicMover)
        RT_CLASS_EXPORT(CinematicMover, m3d::METHOD, SetObjAndPath, "", "", "")
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(CinematicMover);

    CinematicMoverPrototypeInfo::CinematicMoverPrototypeInfo()
    {
    }

    bool CinematicMoverPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        return ai::PrototypeInfo::LoadFromXML(xmlFile, xmlNode) != 0;
    }

    Obj* CinematicMoverPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* CinematicMover::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CinematicMover::CinematicMover(CinematicMoverPrototypeInfo const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CinematicMover::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CinematicMover::SetObjAndPath(int, CStr const&, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* CinematicMover::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    void CinematicMover::Update(float, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CinematicMover::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CinematicMoverPrototypeInfo const* CinematicMover::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CinematicMover::~CinematicMover()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    PhysicObj* CinematicMover::_GetControlledObj() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* CinematicMover::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* CinematicMover::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}
