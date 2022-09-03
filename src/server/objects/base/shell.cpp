#include "shell.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Shell)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Shell);

    bool ShellPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    ShellPrototypeInfo::ShellPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    Gun* Shell::GetGun() const
    {
        throw std::logic_error("Not implemented");
    }

    GunPrototypeInfo const* Shell::GetGunPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    void Shell::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* Shell::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SimplePhysicObj);
    }

    int Shell::GetGunObjId() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* Shell::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    int Shell::GetEmittedObjId() const
    {
        throw std::logic_error("Not implemented");
    }

    ShellPrototypeInfo const* Shell::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    void Shell::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    void Shell::SetGunObjId(int)
    {
        throw std::logic_error("Not implemented");
    }

    Shell::Shell(ShellPrototypeInfo const& prototype) : SimplePhysicObj(prototype)
    {
        throw std::logic_error("Not implemented");
    }

    Obj* Shell::GetEmittedObj() const
    {
        throw std::logic_error("Not implemented");
    }

    Shell::~Shell()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* Shell::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* Shell::Clone()
    {
        throw std::logic_error("Not implemented");
    }
}
