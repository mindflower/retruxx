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
        return RT_DYNCAST(theObjects->GetEntityByObjId(m_gunObjId), Gun);
    }

    GunPrototypeInfo const* Shell::GetGunPrototypeInfo() const
    {
        return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(m_gunPrototypeId), GunPrototypeInfo const);
    }

    void Shell::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x7E5D30 - a shell in flight has to remember the gun that fired it, since the
        // damage it deals is credited back to that gun's owner.
        SimplePhysicObj::LoadRuntimeValues(xmlFile, xmlNode);

        m3d::SafeIntAttrib(m_gunObjId, xmlNode, "GunObjId");
        m3d::SafeIntAttrib(m_emittedObjId, xmlNode, "EmittedObjId");

        // Prototype ids are not stable across runs, so the gun's prototype travels by name.
        CStr gunPrototypeName;
        m3d::SafeStrAttrib(gunPrototypeName, xmlNode, "GunPrototypeName");
        m_gunPrototypeId = thePrototypeManager->GetPrototypeId(gunPrototypeName);
    }

    m3d::Class* Shell::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SimplePhysicObj);
    }

    int Shell::GetGunObjId() const
    {
        return m_gunObjId;
    }

    m3d::Class* Shell::GetClass() const
    {
        // RVA 0x7E5C70
        return RT_CLASS_LOCAL(Shell);
    }

    int Shell::GetEmittedObjId() const
    {
        return m_emittedObjId;
    }

    ShellPrototypeInfo const* Shell::GetPrototypeInfo() const
    {
        // RVA 0x7E6180
        return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()), ShellPrototypeInfo const);
    }

    void Shell::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x7E6580
        SimplePhysicObj::SaveRuntimeValues(xmlFile, xmlNode);

        xmlNode->SetAttribute("GunObjId", CStr(m_gunObjId).c_str());
        xmlNode->SetAttribute("EmittedObjId", CStr(m_emittedObjId).c_str());

        // An unknown prototype id leaves the name empty rather than failing the save.
        CStr gunPrototypeName;
        auto const* gunPrototypeInfo = thePrototypeManager->GetPrototypeInfo(m_gunPrototypeId);
        if (gunPrototypeInfo)
        {
            gunPrototypeName = gunPrototypeInfo->m_prototypeName;
        }
        xmlNode->SetAttribute("GunPrototypeName", gunPrototypeName.c_str());
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
        // RVA 0x7E64D0
        return theObjects->GetEntityByObjId(m_emittedObjId);
    }

    Shell::~Shell() = default;

    m3d::Object* Shell::CreateObject()
    {
        // RVA 0x7E5FC0
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Object* Shell::Clone()
    {
        // RVA 0x7E5E00
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }
}
