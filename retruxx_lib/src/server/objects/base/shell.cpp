#include "shell.h"

#include "objcontainer.h"

#include <stdexcept>
#include "prototypemanager.h"
#include "core/kernel.h"
#include "server/objects/guns/gun.h"

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
        return m_emittedObjId;
    }

    ShellPrototypeInfo const* Shell::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Shell::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Shell::SetGunObjId(int gunObjId)
    {
        m_gunObjId = gunObjId;
        if (gunObjId == -1)
        {
            m_emittedObjId = -1;
            m_gunPrototypeId = -1;
            return;
        }

        auto* obj = (PhysicBody*)theObjects->GetEntityByObjId(gunObjId);
        m_emittedObjId = obj->GetOwnerId();
        m_gunPrototypeId = obj->GetPrototypeId();

        auto gunPrototypeInfo = thePrototypeManager->GetPrototypeInfo(m_gunPrototypeId);
        M3D_ASSERT(gunPrototypeInfo);
        M3D_ASSERT(gunPrototypeInfo->IsPrototypeOf(RT_CLASS_LOCAL(Gun)));
    }

    Shell::Shell(ShellPrototypeInfo const& prototype) : SimplePhysicObj(prototype)
    {
        m_gunObjId = -1;
        m_emittedObjId = -1;
    }

    Obj* Shell::GetEmittedObj() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Shell::~Shell() = default;

    m3d::Object* Shell::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Shell::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}
