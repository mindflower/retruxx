#include "physicunit.h"
#include "scene/servers/dataserver.h"
#include "game/m3dgame.h"
#include "core/log.h"
#include "base/prototypemanager.h"

#include <ode/objects.h>
#include "ode/odecpp.h"
#include "physicbodies/geoms/box.h"
#include <server/utils.h>
#include <server/server.h>
#include <server/externalpaths.h>

RT_CLASS_EXPORT_METHOD_DEFINE(PhysicUnit, AddWalkPathByName)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(PhysicUnit, SetWalkPathByName)
{
    RETRUXX_NOT_IMPLEMENTED;
}

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(PhysicUnit)
    RT_CLASS_EXPORT(PhysicUnit, m3d::METHOD, AddWalkPathByName, "", "", "")
    RT_CLASS_EXPORT(PhysicUnit, m3d::METHOD, SetWalkPathByName, "", "", "")
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(PhysicUnit);

    bool PhysicUnitPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = SimplePhysicObjPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            _SetGeomType(GEOM_TYPE_FROM_MODEL);
            m3d::SafeFloatAttrib(m_walkSpeed, xmlNode, "WalkSpeed");
            m3d::SafeFloatAttrib(m_maxStandTime, xmlNode, "MaxStandTime");
            m3d::SafeFloatAttrib(m_turnSpeed, xmlNode, "TurnSpeed");
            m_turnSpeed *= 0.017453292;
        }
        return result;
    }

    Obj* PhysicUnitPrototypeInfo::CreateTargetObject() const
    {
        return new PhysicUnit(*this);
    }

    PhysicUnitPrototypeInfo::PhysicUnitPrototypeInfo()
    {
        m_walkSpeed = 1.0f;
        m_turnSpeed = 1.0f;
        m_maxStandTime = 1.0f;
    }

    void PhysicUnit::SetCauseForce(float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicUnit::OnCollideWithStandingVehicle()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    eGObjPropertySaveStatus PhysicUnit::GetPropertySaveStatus(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    PhysicUnit::PhysicUnit(PhysicUnitPrototypeInfo const& prototype) :
        SimplePhysicObj(prototype),
        m_standTtl(0.0, 0.0, m_maxStandTime, -1.0),
        m_walkTtl(0.0, 0.0, 1.0, 0.0)
    {
        m_walkSpeed = prototype.m_walkSpeed;
        m_turnSpeed = prototype.m_turnSpeed;
        m_maxStandTime = prototype.m_maxStandTime;
        m_State = LIVE;
        m_causePos = ZeroVector;
        m_causeForce = 0.0f;
        m_initVelocities = false;
        m_curWayPointNum = 0u;
        m_prevWayPoint = ZeroVector;
        m_walkState = STAND;
        m_curPath = nullptr;
        m_bMustChangePath = false;
        m_bMustWalk = true;

        auto& serverAnimatedModels = M3D_APP->GetAnimatedModelsServer();
        int const itemByName = serverAnimatedModels.GetItemByName(prototype.GetEngineModelName().c_str(), true);

        m3d::AnimatedModel* mdl = nullptr;
        serverAnimatedModels.GetItemProperty(itemByName, m3d::PROP_INTERNAL_GETMODEL, &mdl);
        if (mdl)
        {
            // TODO: check this
            auto* geom = mdl->GetGeom(0);
            if (geom && geom->Type == m3d::DRAFT_GeomType::BOX)
            {
                m_physicBody->_ClearGeoms();

                CVector const size(geom->Sizes.BoxSizes.x, geom->Sizes.BoxSizes.y, geom->Sizes.BoxSizes.z);
                auto* box = ai::Box::CreateObject(0, size, 0);
                dGeomSetPosition(box->GetGeomId(), geom->Translation.x, geom->Translation.y, geom->Translation.z);

                float quat[4] = {};
                quat[0] = geom->Rotation.w;
                quat[1] = geom->Rotation.x;
                quat[2] = geom->Rotation.y;
                quat[3] = geom->Rotation.z;
                dGeomSetQuaternion(box->GetGeomId(), quat);

                auto* object = ai::GeomTransform::CreateObject(m_spaceId, CommonGeomMovedCallback);
                dGeomSetData(object->GetGeomId(), m_physicBody);
                object->SetGeom(box);
                m_physicBody->m_pGeoms.push_back(object);
                dGeomSetBody(m_physicBody->m_pGeoms.front()->GetGeomId(), m_body->id());
                dMassSetBoxTotal(&m_physicBody->m_mass, prototype.GetMassValue(), size.x, size.y, size.z);
            }
        }
        else
        {
            M3D_LOG_ERR("Error: no model for PhysicUnit: '" + prototype.GetEngineModelName() + "' is missing");
        }
        DisablePhysics();
    }

    CStr PhysicUnit::GetPropertyName(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicUnit::SetState(States)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicUnit::Registration()
    {
        m_propertiesMap["PathsNames"] = 77;
    }

    void PhysicUnit::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicUnit::RenderDebugInfo() const
    {
        // TODO: implement PhysicUnit::RenderDebugInfo
        // RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* PhysicUnit::GetClass() const
    {
        return RT_CLASS_LOCAL(PhysicUnit);
    }

    void PhysicUnit::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicUnit::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int PhysicUnit::GetPropertyId(char const* propName) const
    {
        auto it = PhysicUnit::m_propertiesMap.find(propName);
        if (it != PhysicUnit::m_propertiesMap.end())
        {
            return it->second;
        }

        return SimplePhysicObj::GetPropertyId(propName);
    }

    void PhysicUnit::SetCausePos(CVector const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool PhysicUnit::SetWalkPathByName(char const*)
    {
        // TODO: implement PhysicUnit::SetWalkPathByName
        // RETRUXX_NOT_IMPLEMENTED;
        return true;
    }

    void PhysicUnit::SetPositionSelf(CVector const& pos)
    {
        PhysicObj::SetPositionSelf(GetGroundPos(pos, 1, 0));
    }

    void PhysicUnit::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        Obj::LoadFromXML(xmlFile, xmlNode);
    }

    bool PhysicUnit::SetPropertyById(int propertyId, m3d::AIParam const& newValue)
    {
        if (propertyId != 77)
        {
            return SimplePhysicObj::SetPropertyById(propertyId, newValue);
        }

        m_pathsMap.clear();

        CStr const pathsNames = newValue.GetAsStr();
        retruxx::vector<CStr> names;
        m3d::Tokenize(pathsNames, names, "(), ;\t");

        bool res = true;
        if (!names.empty())
        {
            for (CStr const& name : names)
            {
                res = res && AddWalkPathByName(name.c_str());
            }
            m_curPathName = names[(names.size() * rand()) >> 15];
            res = res && SetWalkPathByName(m_curPathName.c_str());
        }

        return res;
    }

    void PhysicUnit::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    PhysicUnitPrototypeInfo const* PhysicUnit::GetPrototypeInfo() const
    {
        return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()), PhysicUnitPrototypeInfo const);
    }

    bool PhysicUnit::AddWalkPathByName(char const* name)
    {
        // TODO: check this
        CStr const nameStr = name;
        auto it = m_pathsMap.find(nameStr);
        if (it != m_pathsMap.end())
        {
            M3D_LOG_ERR("Error: duplicate path name '" + nameStr + "' is not added in PhysicUnit " + m_name);
            return false;
        }

        auto const& path = ai::pServer->GetExternalPaths()->GetPath(name);
        if (path.empty())
        {
            M3D_LOG_ERR("Error: empty path '" + nameStr + "' is not added in  " + GetDebugDescription());
            return false;
        }

        auto& resultPath = m_pathsMap[name];
        resultPath.reserve(path.size());
        for (CVector2 const& pathElem : path)
        {
            resultPath.push_back(CVector(pathElem.x, 0.0, pathElem.y));
        }
        return true;
    }

    m3d::Class* PhysicUnit::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SimplePhysicObj);
    }

    void PhysicUnit::SetInitVelocities(bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicUnit::Update(float, unsigned)
    {
        // TODO: implement PhysicUnit::Update
        // RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicUnit::SetDirection(CVector const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicUnit::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicUnit::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool PhysicUnit::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    PhysicUnit::~PhysicUnit() = default;

    bool PhysicUnit::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicUnit::_SetWalkState(WalkState)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* PhysicUnit::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* PhysicUnit::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai
