#include "shell.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Shell)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Shell);

    bool ShellPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        return SimplePhysicObjPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
    }

    ShellPrototypeInfo::ShellPrototypeInfo() = default;

    Gun* Shell::GetGun() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    GunPrototypeInfo const* Shell::GetGunPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Shell::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Shell::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SimplePhysicObj);
    }

    int Shell::GetGunObjId() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Shell::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Shell::GetEmittedObjId() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ShellPrototypeInfo const* Shell::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Shell::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Shell::SetGunObjId(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Shell::Shell(ShellPrototypeInfo const& prototype) : SimplePhysicObj(prototype)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Obj* Shell::GetEmittedObj() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Shell::~Shell()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Shell::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Shell::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}
