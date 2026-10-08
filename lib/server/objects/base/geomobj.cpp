#include "geomobj.h"

#include "prototypemanager.h"
#include "server/objects/physicbodies/geoms/box.h"
#include "server/objects/physicbodies/physichelpers.h"

#include <core/kernel.h>
#include <core/log.h>
#include <landscape.h>
#include <server/server.h>
#include <world.h>

#include <ode/objects.h>
#include <ode/odecpp.h>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(GeomObj)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(GeomObj);

    Obj* GeomObjPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x61F4B0
        return new GeomObj(*this);
    }

    GeomObjPrototypeInfo::GeomObjPrototypeInfo() = default;
    GeomObjPrototypeInfo::~GeomObjPrototypeInfo() = default;

    bool GeomObjPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        return PhysicObjPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
    }

    void GeomObj::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        // RVA 0x61ED50
        // NOTE: empty, and deliberately does not chain to PhysicObj - a GeomObj takes none of
        // its state from the level file, it is built entirely by SetGeom. The same goes for
        // LoadRuntimeValues, SaveToXML and SaveRuntimeValues below.
    }

    Geom* GeomObj::GetGeom()
    {
        // RVA 0x80E1C0
        return m_pGeom;
    }

    void GeomObj::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        // RVA 0x61ED60 - empty, see LoadFromXML.
    }

    void GeomObj::RenderDebugInfo() const
    {
        // RVA 0x61F0E0 - an object with no space of its own counts as enabled.
        if (m_spaceId && !dGeomIsEnabled(m_spaceId))
            return;

        if (m_pGeom && dGeomIsEnabled(m_pGeom->GetGeomId()))
        {
            ai::pServer->GetWorld()->GetLandscape().DrawGeom(m_pGeom->GetGeomId());
        }
    }

    void GeomObj::UnlinkGeomsFromCollisionCells()
    {
        // RVA 0x61F010
        if (m_pGeom)
        {
            m_pGeom->UnlinkFromCollisionCells(GetId());
        }
    }

    Geom::CellAabb GeomObj::GetCollisionCellAabb() const
    {
        // RVA 0x61F050
        if (m_pGeom)
        {
            return m_pGeom->GetCollisionCellAabb();
        }

        // An inverted box, so that anything tested against it comes out empty.
        Geom::CellAabb aabb;
        aabb.x0 = 0;
        aabb.z0 = 0;
        aabb.x1 = -1;
        aabb.z1 = -1;
        return aabb;
    }

    void GeomObj::EnableGeometry(bool changePhysicState)
    {
        // RVA 0x61EFC0
        PhysicObj::EnableGeometry(changePhysicState);
        if (m_pGeom)
        {
            dGeomEnable(m_pGeom->GetGeomId());
        }
    }

    void GeomObj::SetGeom(CVector size, float massValue, float massSizeCoeff, bool bSameMassForm)
    {
        // RVA 0x61EE90 - the collision shape is always a box; bSameMassForm only picks whether
        // the mass is distributed as that same box or as a sphere enclosing it.
        delete m_pGeom;
        m_pGeom = nullptr;

        m_pGeom = ai::Box::CreateObject(nullptr, size, ai::CommonGeomMovedCallback);
        dGeomSetData(m_pGeom->GetGeomId(), this);

        if (bSameMassForm)
        {
            dMassSetBoxTotal(&m_mass, massValue, size.x * massSizeCoeff, size.y * massSizeCoeff,
                size.z * massSizeCoeff);
        }
        else
        {
            float maxSide = size.x;
            if (size.y > maxSide)
                maxSide = size.y;
            if (size.z > maxSide)
                maxSide = size.z;
            dMassSetSphereTotal(&m_mass, massValue, massSizeCoeff * maxSide);
        }
        _Construct();
    }

    void GeomObj::RelinkGeomsToCollisionCells()
    {
        // RVA 0x61F030
        if (m_pGeom)
        {
            m_pGeom->RelinkToCollisionCells(GetId());
        }
    }

    GeomObjPrototypeInfo const* GeomObj::GetPrototypeInfo() const
    {
        // RVA 0x61F7C0
        return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()), GeomObjPrototypeInfo const);
    }

    GeomObj::GeomObj(GeomObjPrototypeInfo const& prototype) : PhysicObj(prototype)
    {
        // RVA 0x61EE10 - the geom itself only appears once SetGeom is called.
        m_pGeom = nullptr;
        dMassSetZero(&m_mass);
    }

    m3d::Class* GeomObj::GetClass() const
    {
        // RVA 0x61ECA0
        return RT_CLASS_LOCAL(GeomObj);
    }

    void GeomObj::TransferToSpace(dxSpace* newSpace)
    {
        // RVA 0x61F090
        if (m_pGeom)
        {
            if (dGeomGetSpace(m_pGeom->GetGeomId()))
            {
                dSpaceRemove(dGeomGetSpace(m_pGeom->GetGeomId()), m_pGeom->GetGeomId());
            }
            dSpaceAdd(newSpace, m_pGeom->GetGeomId());
        }
        PhysicObj::TransferToSpace(newSpace);
    }

    m3d::Class* GeomObj::GetBaseClass()
    {
        return RT_CLASS_LOCAL(PhysicObj);
    }

    void GeomObj::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        // RVA 0x61ED80 - empty, see LoadFromXML.
    }

    void GeomObj::DisableGeometry(bool changePhysicState)
    {
        // RVA 0x61EF90
        PhysicObj::DisableGeometry(changePhysicState);
        if (m_pGeom)
        {
            dGeomDisable(m_pGeom->GetGeomId());
        }
    }

    void GeomObj::LinkGeomsToCollisionCells()
    {
        // RVA 0x61EFF0
        if (m_pGeom)
        {
            m_pGeom->LinkToCollisionCells(GetId(), nullptr);
        }
    }

    void GeomObj::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        // RVA 0x61ED70 - empty, see LoadFromXML.
    }

    void GeomObj::_UnlinkBodyFromGeoms()
    {
        // RVA 0x61ED10 - overridden only to forward; the single geom is bound to the body once
        // in _Construct and never unbound. The three below are the same.
        PhysicObj::_UnlinkBodyFromGeoms();
    }

    void GeomObj::_LinkBodyToGeoms()
    {
        // RVA 0x61ED20
        PhysicObj::_LinkBodyToGeoms();
    }

    void GeomObj::_SetRotationToGeoms(Quaternion const& rot)
    {
        // RVA 0x61ED40
        PhysicObj::_SetRotationToGeoms(rot);
    }

    void GeomObj::_Construct()
    {
        // RVA 0x61F500 - puts the geom in this object's own space, binds it to the body and
        // recentres the mass on its centre of gravity.
        if (!m_pGeom)
            return;

        _CreateSpace(false);

        if (dGeomGetSpace(m_pGeom->GetGeomId()))
        {
            dSpaceRemove(dGeomGetSpace(m_pGeom->GetGeomId()), m_pGeom->GetGeomId());
        }
        dSpaceAdd(m_spaceId, m_pGeom->GetGeomId());
        dGeomSetBody(m_pGeom->GetGeomId(), m_body->id());

        dMass mass;
        dMassSetZero(&mass);
        // NOTE: the shipped build really does zero it twice before adding.
        dMassSetZero(&mass);
        dMassAdd(&mass, &m_mass);

        // ODE wants the body's mass centred on the origin, so the offset is remembered and
        // subtracted out here.
        m_massCenter.x = mass.c[0];
        m_massCenter.y = mass.c[1];
        m_massCenter.z = mass.c[2];
        dMassTranslate(&mass, -m_massCenter.x, -m_massCenter.y, -m_massCenter.z);

        if (mass.mass < 0.001)
        {
            M3D_LOG_ERR("Error: mass is too low: " + CStr(mass.mass) + " for " + GetDebugDescription());
            dMassSetSphereTotal(&mass, 1.0, 1.0);
        }
        dBodySetMass(m_body->id(), &mass);
    }

    void GeomObj::_SetPositionToGeoms(CVector const& pos)
    {
        // RVA 0x61ED30
        PhysicObj::_SetPositionToGeoms(pos);
    }

    GeomObj::~GeomObj()
    {
        // RVA 0x61ECE0
        delete m_pGeom;
        m_pGeom = nullptr;
    }

    m3d::Object* GeomObj::CreateObject()
    {
        // RVA 0x61F2F0
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Object* GeomObj::Clone()
    {
        // RVA 0x61F130
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }
}  // namespace ai
