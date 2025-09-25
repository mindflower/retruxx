#include "cinematicmover.h"

#include <stdexcept>

RT_CLASS_EXPORT_METHOD_DEFINE(CinematicMover, SetObjAndPath)
{
    throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    m3d::Class* CinematicMover::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    CinematicMover::CinematicMover(CinematicMoverPrototypeInfo const&)
    {
        throw std::logic_error("Not implemented");
    }

    void CinematicMover::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    void CinematicMover::SetObjAndPath(int, CStr const&, float)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* CinematicMover::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    void CinematicMover::Update(float, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    void CinematicMover::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    CinematicMoverPrototypeInfo const* CinematicMover::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    CinematicMover::~CinematicMover()
    {
        throw std::logic_error("Not implemented");
    }

    PhysicObj* CinematicMover::_GetControlledObj() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* CinematicMover::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* CinematicMover::Clone()
    {
        throw std::logic_error("Not implemented");
    }
}
