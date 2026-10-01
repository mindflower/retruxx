#include "physicobj.h"
#include <stdexcept>
#include <core/aiparam.h>
#include <server/obstacle.h>

#include "landscape.h"
#include "world.h"
#include "core/ini.h"
#include "core/log.h"
#include "game/m3dgame.h"
#include "server/dynamicscene.h"
#include "server/ai/aimanager.h"
#include "server/objects/physicbodies/physicbody.h"
#include "server/objects/physicbodies/physichelpers.h"

extern "C"
{
#include "ode/collision.h"
#include "ode/objects.h"
}

#include "ode/odecpp.h"
#include <server/server.h>
#include "prototypemanager.h"

RT_CLASS_EXPORT_METHOD_DEFINE(PhysicObj, SetPosition)
{
    auto* obj = (ai::PhysicObj*)context->asObject(0, "PhysicObj");
    auto& vec = context->asVector(1);
    obj->SetPosition(vec);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(PhysicObj, GetPosition)
{
    auto* obj = (ai::PhysicObj*)context->asObject(0, "PhysicObj");
    auto pos = obj->GetPosition();
    context->pushVector(pos);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(PhysicObj, SetRotation)
{
    auto* obj = (ai::PhysicObj*)context->asObject(0, "PhysicObj");
    auto& quat = context->asQuaternion(1);
    obj->SetRotation(quat);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(PhysicObj, GetRotation)
{
    // RVA 0x5FB4A0
    auto* obj = (ai::PhysicObj*)context->asObject(0, "PhysicObj");
    context->pushQuaternion(obj->GetRotation());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(PhysicObj, SetDirection)
{
    // RVA 0x5F98D0
    auto* obj = (ai::PhysicObj*)context->asObject(0, "PhysicObj");
    obj->SetDirection(context->asVector(1));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(PhysicObj, GetDirection)
{
    // RVA 0x5F9900
    auto* obj = (ai::PhysicObj*)context->asObject(0, "PhysicObj");
    context->pushVector(obj->GetDirection());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(PhysicObj, GetLinearVelocity)
{
    // RVA 0x5F9940
    auto* obj = (ai::PhysicObj*)context->asObject(0, "PhysicObj");
    context->pushVector(obj->GetLinearVelocity());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(PhysicObj, SetLinearVelocity)
{
    // RVA 0x5F9980
    auto* obj = (ai::PhysicObj*)context->asObject(0, "PhysicObj");
    obj->SetLinearVelocity(context->asVector(1));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(PhysicObj, GetAngularVelocity)
{
    // RVA 0x5FB4E0
    auto* obj = (ai::PhysicObj*)context->asObject(0, "PhysicObj");
    context->pushVector(obj->GetAngularVelocity());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(PhysicObj, SetAngularVelocity)
{
    // RVA 0x5FB540
    auto* obj = (ai::PhysicObj*)context->asObject(0, "PhysicObj");
    obj->SetAngularVelocity(context->asVector(1));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(PhysicObj, SetUpdatingByODE)
{
    // RVA 0x5F99B0
    auto* obj = (ai::PhysicObj*)context->asObject(0, "PhysicObj");
    obj->SetUpdatingByODE(context->asBool(1));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(PhysicObj, GetSkin)
{
    // RVA 0x5FA090
    auto* obj = (ai::PhysicObj*)context->asObject(0, "PhysicObj");
    context->pushInt(obj->GetSkin());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(PhysicObj, SetSkin)
{
    auto* obj = dynamic_cast<ai::PhysicObj*>(context->asObject(0, "PhysicObj"));
    auto skin = context->asInt(1);
    obj->SetSkin(skin);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(PhysicObj, IsVisible)
{
    // RVA 0x5F9A10
    auto* obj = (ai::PhysicObj*)context->asObject(0, "PhysicObj");
    context->pushBool(obj->IsVisible());
    return 1;
}

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(PhysicObj)
    RT_CLASS_EXPORT(PhysicObj, m3d::METHOD, SetPosition, "", "", "")
    RT_CLASS_EXPORT(PhysicObj, m3d::METHOD, GetPosition, "", "", "")
    RT_CLASS_EXPORT(PhysicObj, m3d::METHOD, SetRotation, "", "", "")
    RT_CLASS_EXPORT(PhysicObj, m3d::METHOD, GetRotation, "", "", "")
    RT_CLASS_EXPORT(PhysicObj, m3d::METHOD, SetDirection, "", "", "")
    RT_CLASS_EXPORT(PhysicObj, m3d::METHOD, GetDirection, "", "", "")
    RT_CLASS_EXPORT(PhysicObj, m3d::METHOD, GetLinearVelocity, "", "", "")
    RT_CLASS_EXPORT(PhysicObj, m3d::METHOD, SetLinearVelocity, "", "", "")
    RT_CLASS_EXPORT(PhysicObj, m3d::METHOD, GetAngularVelocity, "", "", "")
    RT_CLASS_EXPORT(PhysicObj, m3d::METHOD, SetAngularVelocity, "", "", "")
    RT_CLASS_EXPORT(PhysicObj, m3d::METHOD, SetUpdatingByODE, "", "", "")
    RT_CLASS_EXPORT(PhysicObj, m3d::METHOD, GetSkin, "", "", "")
    RT_CLASS_EXPORT(PhysicObj, m3d::METHOD, SetSkin, "", "", "")
    RT_CLASS_EXPORT(PhysicObj, m3d::METHOD, IsVisible, "", "", "")
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(PhysicObj);

    namespace
    {
        retruxx::set<m3d::Class*> standardTargetClasses;

        // v rotated by rot (row-vector convention), as the shipped build inlines it: the rotation matrix of the
        // quaternion, then v * M.
        // NOTE: the inlined products are summed in a slightly different order per call site, so the last bit of the
        // result can differ from the shipped code.
        CVector Rotate(Quaternion const& rot, CVector const& v)
        {
            CMatrix m;
            m.rotTranslate(rot, ZeroVector);
            return CVector(
                v.x * m._11 + v.y * m._21 + v.z * m._31,
                v.x * m._12 + v.y * m._22 + v.z * m._32,
                v.x * m._13 + v.y * m._23 + v.z * m._33);
        }
    }

    extern AIManager* theAIManager;

    PhysicObjPrototypeInfo::PhysicObjPrototypeInfo()
    {
        m_intersectionRadius = 0.0;
        m_lookRadius = 0.0;
    }

    bool PhysicObjPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = ai::PrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeFloatAttrib(m_intersectionRadius, xmlNode, "IntersectionRadius");
            m3d::SafeFloatAttrib(m_lookRadius, xmlNode, "LookRadius");
        }
        return result;
    }

    void PhysicObj::SetDirections(CVector const& forward, CVector const& up)
    {
        // RVA 0x5FA590 - the rotation whose basis rows are (up x forward, up, forward).
        CMatrix mat;
        mat.zero();
        mat._11 = up.y * forward.z - forward.y * up.z;
        mat._12 = forward.x * up.z - up.x * forward.z;
        mat._13 = forward.y * up.x - forward.x * up.y;
        mat._21 = up.x;
        mat._22 = up.y;
        mat._23 = up.z;
        mat._31 = forward.x;
        mat._32 = forward.y;
        mat._33 = forward.z;

        Quaternion rotation;
        rotation.FromMatrix(mat);
        SetRotation(rotation);
    }

    void PhysicObj::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x5FB900
        Obj::LoadRuntimeValues(xmlFile, xmlNode);
        CVector linearVelocity = GetLinearVelocity();
        m3d::SafeVectorAttrib(linearVelocity, xmlNode, "LinearVelocity");
        SetLinearVelocity(linearVelocity);
        CVector angularVelocity = GetAngularVelocity();
        m3d::SafeVectorAttrib(angularVelocity, xmlNode, "AngularVelocity");
        SetAngularVelocity(angularVelocity);

        bool isAutoDisabling = false;
        m3d::SafeBoolAttrib(isAutoDisabling, xmlNode, "IsAutoDisabling");
        if (isAutoDisabling)
        {
            float linearThreshold = 0.1f;
            m3d::SafeFloatAttrib(linearThreshold, xmlNode, "AutoDisableLinearThreshold");
            float angularThreshold = 0.1f;
            m3d::SafeFloatAttrib(angularThreshold, xmlNode, "AutoDisableAngularThreshold");
            int steps = 5;
            m3d::SafeIntAttrib(steps, xmlNode, "AutoDisableSteps");
            SetAutoDisabling(true, linearThreshold, angularThreshold, steps);
        }
        else
        {
            SetAutoDisabling(false, 0.0f, 0.0f, 0);
        }
        m3d::SafeIntAttrib(m_skinNumber, xmlNode, "SkinNumber");
    }

    bool PhysicObj::GetBodyEnabledBit() const
    {
        return m_physicState & 1;
    }

    Quaternion PhysicObj::GetRotation() const
    {
        dReal const* quat = dBodyGetQuaternion(m_body->id());

        Quaternion result;
        result.x = quat[1];
        result.y = quat[2];
        result.z = quat[3];
        result.w = quat[0];
        return result;
    }

    void PhysicObj::SetPostDisablePhysicsWithAutoEnable()
    {
        m_postActionFlags |= 8u;
        theObjects->AddObjToPostCollideList(this);
    }

    CVector PhysicObj::GetMassCenter() const
    {
        return m_massCenter;
    }

    bool PhysicObj::bIsUpdatingByODE() const
    {
        return m_bIsUpdatingByODE != 0;
    }

    void PhysicObj::RelinkToSpace(dxSpace* newSpace)
    {
        if (m_spaceId && m_bIsSpaceOwner)
        {
            if (dSpaceID space = dGeomGetSpace(m_spaceId))
            {
                dSpaceRemove(space, m_spaceId);
            }
            dSpaceAdd(newSpace, m_spaceId);
        }
        else
        {
            M3D_LOG_ERR("Error: attempt to relink " + GetDebugDescription() + " which is transferred to another space");
        }
    }

    bool PhysicObj::IsVisible()
    {
        // RVA 0x5FD680
        return false;
    }

    Quaternion PhysicObj::GetPostRotation() const
    {
        // RVA 0x5FA6C0
        if ((m_postActionFlags & 1) != 0)
        {
            return m_postRotation;
        }
        return GetRotation();
    }

    dxSpace* PhysicObj::GetSpaceId() const
    {
        return m_spaceId;
    }

    void PhysicObj::SetSkin(int skin)
    {
        m_skinNumber = skin;
    }

    m3d::Class* PhysicObj::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    void PhysicObj::UnlinkGeomsFromCollisionCells()
    {
        m_boundSphere->UnlinkFromCollisionCells(GetId());
    }

    bool PhysicObj::CanPhysicsBeEnabled() const
    {
        // RVA 0x5FAB10
        return dBodyIsEnabled(m_body->id()) || (dBodyGetAutoDisableFlag(m_body->id()) && (m_physicBehaviorFlags & 1) == 0);
    }

    float PhysicObj::GetMass() const
    {
        dMass mass;
        dMassSetZero(&mass);
        dBodyGetMass(m_body->id(), &mass);
        return mass.mass;
    }

    void PhysicObj::DisablePhysicsAndGeometry()
    {
        // RVA 0x5FA000
        DisablePhysics();
        DisableGeometry(true);
    }

    bool PhysicObj::SetPropertyById(int propertyId, m3d::AIParam const& newValue)
    {
        switch (propertyId)
        {
        case 4:
            SetPositionSelf(newValue.GetAsVector());
            return true;

        case 5:
            SetRotationSelf(newValue.GetAsQuaternion());
            return true;

        case 45:
            SetSkin(newValue.GetAsID());
            return true;

        default:
            return Obj::SetPropertyById(propertyId, newValue);
        }
    }

    void PhysicObj::SetPostEnablePhysicsIfPossible()
    {
        // RVA 0x5FDF10
        m_postActionFlags |= 0x20;
        theObjects->AddObjToPostCollideList(this);
    }

    CVector PhysicObj::GetMassCenterPosition() const
    {
        // RVA 0x5FA670
        dReal const* const pos = dBodyGetPosition(m_body->id());
        return CVector(pos[0], pos[1], pos[2]);
    }

    void PhysicObj::TransferToSpace(dxSpace* newSpace)
    {
        if (m_spaceId)
        {
            if (m_bIsSpaceOwner)
            {
                dSpaceDestroy(m_spaceId);
                m_spaceId = newSpace;
                m_bIsSpaceOwner = false;
            }
        }
    }

    void PhysicObj::SetVisible()
    {
        Obj::SetVisible();
        EnablePhysics();
    }

    void PhysicObj::AddTorque(CVector const& torque)
    {
        // RVA 0x5FAA00
        dBodyAddTorque(m_body->id(), torque.x, torque.y, torque.z);
    }

    void PhysicObj::CheckCollisionCells()
    {
        m_boundSphere->CheckCollisionCells();
    }

    void PhysicObj::Registration()
    {
        standardTargetClasses.insert(RT_CLASS_LOCAL(PhysicObj));
        theAIManager->RegisterFunc("AIGetCurPos", &PhysicObj::AIGetCurPos);

        m_propertiesMap["Pos"] = 4;
        m_propertiesMap["Rot"] = 5;
        m_propertiesMap["Skin"] = 45;

        m_countRelinksToCollisionCells =
            M3D_APP->GetDbgCounterStack().GetCounter(M3D_APP->GetDbgCounterStack().AddCounter("relinks to collision cells"));
        m_countRelinksToCollisionCells->SetI(0);
    }

    void PhysicObj::AddImpulseAtRelPos(CVector const& impulse, CVector const& relPos)
    {
        // RVA 0x5FA950 - applied as a force over the current AI tick.
        if (theAIManager->m_elapsedTime > 0.001)
        {
            float const invTime = 1.0 / theAIManager->m_elapsedTime;
            dBodyAddForceAtRelPos(
                m_body->id(), impulse.x * invTime, impulse.y * invTime, impulse.z * invTime, relPos.x, relPos.y, relPos.z);
        }
    }

    void PhysicObj::TransferToNewSpace()
    {
        // RVA 0x5FAC90 - moves the object's geoms into a space of its own.
        dxSpace* const parentSpace = ai::gGlobalSpace->lock_count ? ai::gTempSpace : ai::gGlobalSpace;
        m_spaceId = dSimpleSpaceCreate(parentSpace);
        dSpaceSetCleanup(m_spaceId, 0);
        TransferToSpace(m_spaceId);
        m_bIsSpaceOwner = true;
    }

    void PhysicObj::SetRotation(Quaternion const& rot)
    {
        SetRotationSelf(rot);
    }

    bool PhysicObj::CanCreateCollisionEffect() const
    {
        return m_timeFromLastCollisionEffect > 0.1 || !IsUpdating();
    }

    void PhysicObj::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>& props) const
    {
        // RVA 0x5ED2E0
        for (auto const& prop : m_propertiesMap)
        {
            props.insert(prop.first);
        }
        Obj::GetPropertiesNames(props);
    }

    void PhysicObj::AddImpulse(CVector const& impulse)
    {
        // RVA 0x5FA840 - applied as a force over the current AI tick.
        if (theAIManager->m_elapsedTime > 0.001)
        {
            float const invTime = 1.0 / theAIManager->m_elapsedTime;
            dBodyAddForce(m_body->id(), impulse.x * invTime, impulse.y * invTime, impulse.z * invTime);
        }
    }

    void PhysicObj::SetPositionSelf(CVector const& pos)
    {
        // RVA 0x5FC180 - the body sits at the mass center, so it is placed at pos plus the rotated mass-center offset.
        CVector const realPos = pos + Rotate(GetRotation(), m_massCenter);
        dBodySetPosition(m_body->id(), realPos.x, realPos.y, realPos.z);
        PhysicObj::SetCorrectEnabledCellsCounter();
    }

    void PhysicObj::SetPostRotation(Quaternion const& rot)
    {
        // RVA 0x5FDE70
        m_postRotation = rot;
        m_postActionFlags |= 1;
        theObjects->AddObjToPostCollideList(this);
    }

    void PhysicObj::DumpPhysicInfo(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x5FCF20
        xmlNode->SetAttribute("ClassName", CStr(GetClassNameA()).c_str());
        xmlNode->SetAttribute("Id", CStr(GetId()).c_str());
        if (PrototypeInfo const* const prototypeInfo = GetPrototypeInfo())
        {
            xmlNode->SetAttribute("Prototype", prototypeInfo->m_prototypeName.c_str());
        }
        xmlNode->SetAttribute("Name", m_name.c_str());
        xmlNode->SetAttribute("IsSpaceOwner", CStr(static_cast<int>(m_bIsSpaceOwner)).c_str());
        xmlNode->SetAttribute("Space", CStr(static_cast<unsigned>(reinterpret_cast<uintptr_t>(m_spaceId))).c_str());
        xmlNode->SetAttribute("SpaceEnabled", CStr(static_cast<int>(dGeomIsEnabled(m_spaceId) != 0)).c_str());
        if (!m_body)
        {
            return;
        }

        ref_ptr bodyNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Body");
        xmlNode->AddChild(bodyNode);
        bodyNode->SetAttribute("Mass", CStr(GetMass()).c_str());
        bodyNode->SetAttribute("Pos", CStr(GetPosition()).c_str());
        bodyNode->SetAttribute("Rot", CStr(GetRotation()).c_str());
        bodyNode->SetAttribute("LinearVelocity", CStr(GetLinearVelocity()).c_str());
        bodyNode->SetAttribute("AngularVelocity", CStr(GetAngularVelocity()).c_str());
        CVector const linearVelocity = GetLinearVelocity();
        bodyNode->SetAttribute(
            "LinearVelocityLengthSq",
            CStr(linearVelocity.x * linearVelocity.x + linearVelocity.y * linearVelocity.y + linearVelocity.z * linearVelocity.z)
                .c_str());
        dReal const* const angularVelocity = dBodyGetAngularVel(m_body->id());
        bodyNode->SetAttribute(
            "AngularVelocityLengthSq",
            CStr(angularVelocity[0] * angularVelocity[0] + angularVelocity[2] * angularVelocity[2] +
                 angularVelocity[1] * angularVelocity[1])
                .c_str());
        dxBody const* const body = m_body->id();
        bodyNode->SetAttribute("Flags", CStr(body->flags).c_str());
        bodyNode->SetAttribute("AutoDisabling", CStr(dBodyGetAutoDisableFlag(m_body->id())).c_str());
        bodyNode->SetAttribute("BodyEnabled", CStr(static_cast<int>(dBodyIsEnabled(m_body->id()) != 0)).c_str());
        if (dBodyGetAutoDisableFlag(m_body->id()) && dBodyIsEnabled(m_body->id()))
        {
            // m3d::XmlNodeSetAttribute<float> / <int>, inlined.
            bodyNode->SetAttribute("AutoDisableLinearThreshold", CStr(body->adis.linear_threshold).c_str());
            bodyNode->SetAttribute("AutoDisableAngularThreshold", CStr(body->adis.angular_threshold).c_str());
            bodyNode->SetAttribute("AutoDisableIdleTime", CStr(body->adis.idle_time).c_str());
            bodyNode->SetAttribute("AutoDisableIdleSteps", CStr(body->adis.idle_steps).c_str());
            bodyNode->SetAttribute("AutoDisableTimeLeft", CStr(body->adis_timeleft).c_str());
            bodyNode->SetAttribute("AutoDisableStepsLeft", CStr(body->adis_stepsleft).c_str());
        }
    }

    void PhysicObj::SetAutoDisabling(bool bIsAutoDisabling, float linearThreshold, float angularThreshold, int steps)
    {
        if (bIsAutoDisabling)
        {
            dBodySetAutoDisableFlag(m_body->id(), 1);
            dBodySetAutoDisableLinearThreshold(m_body->id(), linearThreshold);
            dBodySetAutoDisableAngularThreshold(m_body->id(), angularThreshold);
            dBodySetAutoDisableSteps(m_body->id(), steps);
            dBodySetAutoDisableTime(m_body->id(), 0.0);
        }
        else
        {
            dBodySetAutoDisableFlag(m_body->id(), 0);
        }
    }

    void PhysicObj::IncEnabledCellsCount()
    {
        if ((m_physicState & 1) == 0 && (m_physicState & 2) != 0 && !m_enabledCellsCount)
        {
            EnableGeometry(false);
        }
        ++m_enabledCellsCount;
    }

    void PhysicObj::SetTorque(CVector const& torque)
    {
        // RVA 0x5FA9E0
        dBodySetTorque(m_body->id(), torque.x, torque.y, torque.z);
    }

    unsigned PhysicObj::GetSkin() const
    {
        return m_skinNumber;
    }

    void PhysicObj::EnablePhysicsIfPossible()
    {
        // RVA 0x5FAB50
        if (CanPhysicsBeEnabled())
        {
            EnablePhysics();
        }
    }

    CVector PhysicObj::GetGeometricCenter() const
    {
        // RVA 0x5FC6B0
        return GetPosition();
    }

    void PhysicObj::SetPassedToAnotherMapStatus()
    {
        // RVA 0x5FBD70
        Obj::SetPassedToAnotherMapStatus();
        UnlinkGeomsFromCollisionCells();
        M3D_LOG_INFO(CStr(GetClassNameA()) + CStr(" '") + CStr(m_name) + CStr("' id=") + CStr(GetId()) + CStr(" passes to another map"));
    }

    void PhysicObj::SetCollisionEffectCreated()
    {
        m_timeFromLastCollisionEffect = 0.0;
    }

    void PhysicObj::DisableGeometry(bool changePhysicState)
    {
        if (m_spaceId && m_bIsSpaceOwner)
        {
            dGeomDisable(m_spaceId);
        }
        if (changePhysicState)
        {
            m_physicState &= ~2u;
        }
    }

    void PhysicObj::EnableGeometry(bool changePhysicState)
    {
        if (m_spaceId && m_bIsSpaceOwner)
        {
            dGeomEnable(m_spaceId);
        }
        if (changePhysicState)
        {
            m_physicState |= 2u;
        }
    }

    float PhysicObj::GetIntersectionRadius() const
    {
        if (m_intersectionObstacle)
        {
            return m_intersectionObstacle->GetIntersectionRadius();
        }
        return 0.0;
    }

    void PhysicObj::SetPostEnablePhysics()
    {
        // RVA 0x5FDEF0
        m_postActionFlags |= 4;
        theObjects->AddObjToPostCollideList(this);
    }

    dBody* PhysicObj::GetBody()
    {
        return m_body;
    }

    dBody const* PhysicObj::GetBody() const
    {
        return m_body;
    }

    void PhysicObj::Update(float elapsedTime, unsigned workTime)
    {
        Obj::Update(elapsedTime, workTime);
        if (!m_bIsUpdatingByODE)
        {
            _UpdateOwnPhysics(elapsedTime);
        }
        m_timeFromLastCollisionEffect = m_timeFromLastCollisionEffect + elapsedTime;
    }

    void PhysicObj::SetUpdatingByODE(bool byODE)
    {
        if (byODE)
        {
            if (!m_bIsUpdatingByODE)
            {
                dBodyAddIslandToWorld(m_body->id(), ai::gGlobalWorld);
                m_bIsUpdatingByODE = 1;
                SetLinearVelocity({0.0, 0.0, 0.0});
                dBodySetAngularVel(m_body->id(), 0.0, 0.0, 0.0);
                dBodySetForce(m_body->id(), 0.0, 0.0, 0.0);
                dBodySetTorque(m_body->id(), 0.0, 0.0, 0.0);
                CheckCollisionCells();
            }
        }
        else if (m_bIsUpdatingByODE)
        {
            dBodyRemoveIslandFromWorld(m_body->id());
            m_bIsUpdatingByODE = 0;
            CheckCollisionCells();
        }
    }

    void PhysicObj::SetMassCenterPosition(CVector const& pos)
    {
        // RVA 0x5FA6A0
        dBodySetPosition(m_body->id(), pos.x, pos.y, pos.z);
    }

    void PhysicObj::EnablePhysics()
    {
        if (m_body)
        {
            dBodyEnable(m_body->id());
        }
        _LinkBodyToGeoms();
        m_physicState |= 1u;
        SetCorrectEnabledCellsCounter();
        m_bBodyEnabledLastFrame = true;
        if ((m_physicState & 2) != 0)
        {
            EnableGeometry(false);
        }
    }

    void PhysicObj::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x5FBAF0 - velocities are written only when they are not negligible.
        Obj::SaveRuntimeValues(xmlFile, xmlNode);
        CVector const linearVelocity = GetLinearVelocity();
        if (linearVelocity.z * linearVelocity.z + linearVelocity.y * linearVelocity.y + linearVelocity.x * linearVelocity.x >
            0.0099999998f)
        {
            xmlNode->SetAttribute("LinearVelocity", CStr(linearVelocity).c_str());
        }
        CVector const angularVelocity = GetAngularVelocity();
        if (angularVelocity.z * angularVelocity.z + angularVelocity.y * angularVelocity.y + angularVelocity.x * angularVelocity.x >
            0.0099999998f)
        {
            xmlNode->SetAttribute("AngularVelocity", CStr(angularVelocity).c_str());
        }
        if (dBodyGetAutoDisableFlag(m_body->id()) != 0)
        {
            xmlNode->SetAttribute("IsAutoDisabling", CStr(1).c_str());
            xmlNode->SetAttribute("AutoDisableLinearThreshold", CStr(dBodyGetAutoDisableLinearThreshold(m_body->id())).c_str());
            xmlNode->SetAttribute("AutoDisableAngularThreshold", CStr(dBodyGetAutoDisableAngularThreshold(m_body->id())).c_str());
            xmlNode->SetAttribute("AutoDisableSteps", CStr(dBodyGetAutoDisableSteps(m_body->id())).c_str());
        }
        if (m_skinNumber)
        {
            xmlNode->SetAttribute("SkinNumber", CStr(m_skinNumber).c_str());
        }
    }

    void PhysicObj::AddForceAtPos(CVector const& force, CVector const& pos)
    {
        // RVA 0x5FA7E0
        dBodyAddForceAtPos(m_body->id(), force.x, force.y, force.z, pos.x, pos.y, pos.z);
    }

    void PhysicObj::DecEnabledCellsCount()
    {
        --m_enabledCellsCount;
        if ((m_physicState & 1) == 0 && (m_physicState & 2) != 0 && !m_enabledCellsCount)
        {
            DisableGeometry(false);
        }
    }

    CVector PhysicObj::GetPosition() const
    {
        // RVA 0x5FC410 - the inverse of SetPositionSelf: the body's position less the rotated mass-center offset.
        CVector const offset = Rotate(GetRotation(), m_massCenter);
        dReal const* bodyPos = dBodyGetPosition(m_body->id());
        return CVector(bodyPos[0] - offset.x, bodyPos[1] - offset.y, bodyPos[2] - offset.z);
    }

    void PhysicObj::PostCollide()
    {
        // RVA 0x5FCA70 - applies the actions deferred by the SetPost* calls.
        if ((m_postActionFlags & 1) != 0)
        {
            SetRotation(m_postRotation);
        }
        if ((m_postActionFlags & 2) != 0)
        {
            SetPosition(m_postPosition);
        }
        if ((m_postActionFlags & 4) != 0)
        {
            EnablePhysics();
        }
        if ((m_postActionFlags & 0x20) != 0 && CanPhysicsBeEnabled())
        {
            EnablePhysics();
        }
        if ((m_postActionFlags & 8) != 0)
        {
            DisablePhysicsWithAutoEnable();
        }
        if ((m_postActionFlags & 0x10) != 0)
        {
            DisablePhysicsAndGeometry();
        }
        m_postActionFlags = 0;
    }

    void PhysicObj::DisablePhysics()
    {
        if (m_body)
        {
            dBodyDisable(m_body->id());
        }

        m_physicState &= ~1u;

        ai::PhysicObj::SetCorrectEnabledCellsCounter();
        m_bBodyEnabledLastFrame = false;
        ai::PhysicObj::SetCorrectEnabledCellsCounter();

        dBodyDetachAllContactJoints(m_body->id());
        _UnlinkBodyFromGeoms();
    }

    void PhysicObj::SetDisablePhysicsWhenBodyDisabled()
    {
        // RVA 0x5F9AC0
        m_physicBehaviorFlags |= 1;
    }

    void PhysicObj::SetForce(CVector const& force)
    {
        // RVA 0x5FA7A0
        dBodySetForce(m_body->id(), force.x, force.y, force.z);
    }

    CVector PhysicObj::GetPostPosition() const
    {
        // RVA 0x5FC6D0
        if ((m_postActionFlags & 2) != 0)
        {
            return m_postPosition;
        }
        return GetPosition();
    }

    void PhysicObj::SetAngularVelocity(CVector const& angularVel)
    {
        dBodySetAngularVel(m_body->id(), angularVel.x, angularVel.y, angularVel.z);
    }

    int PhysicObj::GetPhysicState() const
    {
        return m_physicState;
    }

    void PhysicObj::RenderDebugInfo() const
    {
        if (IsAlive())
        {
            dMass mass;
            dMassSetZero(&mass);
            dBodyGetMass(m_body->id(), &mass);
            if (mass.mass > 0.001)
            {
                CVector pos;
                dReal const* posArray = dBodyGetPosition(m_body->id());
                pos.x = posArray[0];
                pos.y = posArray[1];
                pos.z = posArray[2];

                Quaternion const rotation = GetRotation();
                ai::pServer->GetWorld()->GetLandscape().DrawMassBox(&mass, pos, rotation);
            }
        }
    }

    void PhysicObj::AddRelTorque(CVector const& relTorque)
    {
        dBodyAddRelTorque(m_body->id(), relTorque.x, relTorque.y, relTorque.z);
    }

    void PhysicObj::SetPostDisablePhysics()
    {
        // RVA 0x5FDF30
        m_postActionFlags |= 0x10;
        theObjects->AddObjToPostCollideList(this);
    }

    void PhysicObj::Remove()
    {
        Obj::Remove();
        UnlinkGeomsFromCollisionCells();
    }

    void PhysicObj::SetDirection(CVector const& direction)
    {
        SetDirectionToObject<ai::PhysicObj>(*this, direction);
    }

    Geom::CellAabb PhysicObj::GetCollisionCellAabb() const
    {
        return m_boundSphere->CountCellAabb();
    }

    void PhysicObj::RenderObstacleDebugInfo() const
    {
        // RVA 0x5FACE0
        if (m_intersectionObstacle)
        {
            m_intersectionObstacle->RenderDebugInfo();
        }
    }

    bool PhysicObj::bIsStatic() const
    {
        // RVA 0x602640
        return (m_physicState & STATIC_BIT) != 0;
    }

    void PhysicObj::SetLinearVelocity(CVector const& linearVel)
    {
        dBodySetLinearVel(m_body->id(), linearVel.x, linearVel.y, linearVel.z);
    }

    void PhysicObj::AddForce(CVector const& force)
    {
        dBodyAddForce(m_body->id(), force.x, force.y, force.z);
    }

    void PhysicObj::AddForceAtRelPos(CVector const& force, CVector const& relPos)
    {
        // RVA 0x5FA810
        dBodyAddForceAtRelPos(m_body->id(), force.x, force.y, force.z, relPos.x, relPos.y, relPos.z);
    }

    void PhysicObj::AddImpulseAtPos(CVector const& impulse, CVector const& pos)
    {
        // RVA 0x5FA8C0 - applied as a force over the current AI tick.
        if (theAIManager->m_elapsedTime > 0.001)
        {
            float const invTime = 1.0 / theAIManager->m_elapsedTime;
            dBodyAddForceAtPos(m_body->id(), impulse.x * invTime, impulse.y * invTime, impulse.z * invTime, pos.x, pos.y, pos.z);
        }
    }

    void PhysicObj::SetInvisible()
    {
        // RVA 0x5F9BB0
        Obj::SetInvisible();
        DisablePhysics();
    }

    m3d::AIParam PhysicObj::AIGetCurPos(Obj* pObj)
    {
        // RVA 0x5FE710 - NOTE: pObj is taken to be a PhysicObj without any type check.
        return m3d::AIParam(static_cast<PhysicObj*>(pObj)->GetPosition());
    }

    CVector PhysicObj::GetPositionAtRelPoint(CVector point) const
    {
        // RVA 0x5FC710 - takes a point in the object's own frame into world
        // space. rotTranslate with a zero origin is exactly the quaternion to
        // matrix conversion the original inlines here.
        CMatrix rot;
        rot.rotTranslate(GetRotation(), CVector(0.0f, 0.0f, 0.0f));

        CVector const rotated = rot.vecRot(point);
        CVector const pos = GetPosition();

        CVector res;
        res.x = rotated.x + pos.x;
        res.y = pos.y + rotated.y;
        res.z = pos.z + rotated.z;
        return res;
    }

    void PhysicObj::SetPosition(CVector const& pos)
    {
        SetPositionSelf(pos);
    }

    PhysicObj::PhysicObj(PhysicObjPrototypeInfo const& prototypeInfo) : Obj(prototypeInfo)
    {
        m_intersectionObstacle = nullptr;
        m_body = 0;
        m_lookSphere = 0;
        m_postActionFlags = 0;
        m_postRotation.x = 0.0;
        m_postRotation.y = 0.0;
        m_postRotation.z = 0.0;
        m_postRotation.w = 1.0;
        m_postPosition.x = 0.0;
        m_postPosition.y = 0.0;
        m_postPosition.z = 0.0;
        m_physicBehaviorFlags = 0;
        m_massCenter.x = 0.0;
        m_massCenter.y = 0.0;
        m_massCenter.z = 0.0;

        m_body = new dBody(gGlobalWorld);

        m_body->setData(this);
        m_body->setChangeEnabledStateCallback(PhysicObj::_CommonBodyChangeEnabledStateCallback);
        m_spaceId = 0;
        m_bIsSpaceOwner = 1;
        if (prototypeInfo.m_lookRadius > 0.0099999998)
        {
            m_lookSphere = SphereForIntersection::CreateObject(prototypeInfo.m_lookRadius, SphereForIntersection::LOOKING, nullptr);
            dGeomSetBody(m_lookSphere->GetGeomId(), m_body->id());
            m_lookSphere->SetTargetClasses(standardTargetClasses);
        }
        m_boundSphere = ai::Sphere::CreateObject(0, 1.0, 0);
        dGeomSetBody(m_boundSphere->GetGeomId(), m_body->id());
        m_bIsUpdatingByODE = 1;
        m_enabledCellsCount = 0;
        m_bBodyEnabledLastFrame = 1;
        m_skinNumber = 0;
        m_physicState = 3;
        m_timeFromLastCollisionEffect = 1000.0;
    }

    void PhysicObj::EnablePhysicsAndGeometry()
    {
        // Not emitted in the shipped binary (never called); mirrors DisablePhysicsAndGeometry (RVA 0x5FA000).
        EnablePhysics();
        EnableGeometry(true);
    }

    ai::SphereForIntersection const* PhysicObj::GetIntersectionSphere() const
    {
        return m_lookSphere;
    }

    bool PhysicObj::bIsBodyDisabledGeomEnabled() const
    {
        // RVA 0x5FA020
        return (m_physicState & BODY_ENABLED_BIT) == 0 && (m_physicState & GEOM_ENABLED_BIT) != 0;
    }

    void PhysicObj::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>& props) const
    {
        // RVA 0x5ED3E0
        for (auto const& prop : m_propertiesMap)
        {
            props.insert(prop.second);
        }
        Obj::GetPropertiesIDs(props);
    }

    void PhysicObj::DisablePhysicsWithAutoEnable()
    {
        if (m_body)
        {
            dBodyDisable(m_body->id());
        }
        m_physicState &= ~1u;
        SetCorrectEnabledCellsCounter();
        m_bBodyEnabledLastFrame = 0;
        SetCorrectEnabledCellsCounter();
        dBodyDetachAllContactJoints(m_body->id());
    }

    void PhysicObj::SetRotationSelf(Quaternion const& rot)
    {
        CVector const pos = GetPosition();
        float dq[4];
        dq[0] = rot.w;
        dq[1] = rot.x;
        dq[2] = rot.y;
        dq[3] = rot.z;
        dBodySetQuaternion(m_body->id(), dq);
        PhysicObj::SetPositionSelf(pos);
    }

    void PhysicObj::SetPostPosition(CVector const& pos)
    {
        // RVA 0x5FDEB0
        m_postPosition = pos;
        m_postActionFlags |= 2;
        theObjects->AddObjToPostCollideList(this);
    }

    m3d::Class* PhysicObj::GetClass() const
    {
        // RVA 0x5F9A50
        return RT_CLASS_LOCAL(PhysicObj);
    }

    CVector PhysicObj::GetAngularVelocity() const
    {
        dReal const* angularVel = dBodyGetAngularVel(m_body->id());
        CVector result;
        result.x = angularVel[0];
        result.y = angularVel[1];
        result.z = angularVel[2];
        return result;
    }

    void PhysicObj::RelinkGeomsToCollisionCells()
    {
        m_boundSphere->RelinkToCollisionCells(GetId());
    }

    void PhysicObj::AddRelativeRotation(Quaternion const& relDeltaRot)
    {
        // RVA 0x5FA250 - the current rotation followed by relDeltaRot.
        Quaternion const rot = GetRotation();
        Quaternion newRot;
        newRot.x = relDeltaRot.z * rot.y + rot.x * relDeltaRot.w + rot.w * relDeltaRot.x - rot.z * relDeltaRot.y;
        newRot.y = rot.w * relDeltaRot.y + relDeltaRot.w * rot.y + rot.z * relDeltaRot.x - rot.x * relDeltaRot.z;
        newRot.z = rot.w * relDeltaRot.z + rot.x * relDeltaRot.y + rot.z * relDeltaRot.w - relDeltaRot.x * rot.y;
        newRot.w = rot.w * relDeltaRot.w - rot.x * relDeltaRot.x - rot.y * relDeltaRot.y - rot.z * relDeltaRot.z;
        SetRotation(newRot);
    }

    CVector PhysicObj::GetDirection() const
    {
        // RVA 0x5FA360 - the forward vector (0, 0, 1) in world space.
        return Rotate(GetRotation(), CVector(0.0f, 0.0f, 1.0f));
    }

    bool PhysicObj::GetGeomEnabledBit() const
    {
        // RVA 0x5FA040
        return (m_physicState & GEOM_ENABLED_BIT) != 0;
    }

    void PhysicObj::LinkGeomsToCollisionCells()
    {
        m_boundSphere->LinkToCollisionCells(GetId(), nullptr);
    }

    CStr PhysicObj::GetPropertyName(int id) const
    {
        // RVA 0x5ED4D0
        for (auto const& prop : m_propertiesMap)
        {
            if (prop.second == id)
            {
                return prop.first;
            }
        }
        return Obj::GetPropertyName(id);
    }

    eGObjPropertySaveStatus PhysicObj::GetPropertySaveStatus(int id) const
    {
        // RVA 0x5ED070
        auto const it = m_propertiesSaveStatesMap.find(id);
        if (it == m_propertiesSaveStatesMap.end())
        {
            return Obj::GetPropertySaveStatus(id);
        }
        return it->second;
    }

    void PhysicObj::ZeroEnabledCellsCount()
    {
        // RVA 0x5FAD70
        m_enabledCellsCount = 0;
        if ((m_physicState & BODY_ENABLED_BIT) == 0 && (m_physicState & GEOM_ENABLED_BIT) != 0)
        {
            DisableGeometry(false);
        }
    }

    void PhysicObj::SetCorrectEnabledCellsCounter()
    {
        // RVA 0x5FABD0 - an object with its physics off but its geometry on keeps that geometry only while it
        // touches a collision cell enabled last frame; every cell it touches is marked to be checked.
        m_enabledCellsCount = 0;
        if ((m_physicState & 1) != 0 || (m_physicState & 2) == 0)
        {
            return;
        }
        auto& landscape = ai::pServer->GetWorld()->GetLandscape();
        Geom::CellAabb const aabb = GetCollisionCellAabb();
        for (int x = aabb.x0; x <= aabb.x1; ++x)
        {
            for (int z = aabb.z0; z <= aabb.z1; ++z)
            {
                auto* const cell = landscape.GetCollisionCellItem(x, z);
                if (cell->m_wasEnabledLastFrame)
                {
                    ++m_enabledCellsCount;
                }
                cell->m_bMustCheck = 1;
            }
        }
        if (m_enabledCellsCount <= 0)
        {
            DisableGeometry(false);
        }
        else
        {
            EnableGeometry(false);
        }
    }

    PhysicObjPrototypeInfo const* PhysicObj::GetPrototypeInfo() const
    {
        return dynamic_cast<PhysicObjPrototypeInfo const*>(ai::thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    CVector PhysicObj::GetLinearVelocity() const
    {
        auto linearVel = dBodyGetLinearVel(m_body->id());

        CVector result;
        result.x = linearVel[0];
        result.y = linearVel[1];
        result.z = linearVel[2];

        return result;
    }

    m3d::DbgCounter* PhysicObj::GetRelinksToCollisionCounter()
    {
        return ai::PhysicObj::m_countRelinksToCollisionCells;
    }

    int PhysicObj::GetPropertyId(char const* propName) const
    {
        auto it = PhysicObj::m_propertiesMap.find(propName);
        if (it != PhysicObj::m_propertiesMap.end())
        {
            return it->second;
        }

        return ai::Obj::GetPropertyId(propName);
    }

    void PhysicObj::_SetSimpleCollision()
    {
        // RVA 0x5FB0A0
        for (dGeomID geom = dBodyGetFirstGeom(m_body->id()); geom; geom = dGeomGetBodyNext(geom))
        {
            if (dGeomGetClass(geom) == 6)
            {
                dGeomSetCategoryBits(geom, 0xFFFF);
                dGeomSetCollideBits(geom, 0xFFFF);
            }
        }
    }

    void PhysicObj::_AdjustMassCenter()
    {
        for (auto i = dBodyGetFirstGeom(m_body->id()); i; i = dGeomGetBodyNext(i))
        {
            if (dGeomGetClass(i) == 6)
            {
                auto geom = dGeomTransformGetGeom(i);
                if (geom)
                {
                    auto position = dGeomGetPosition(geom);
                    dGeomSetPosition(geom, position[0] - m_massCenter.x, position[1] - m_massCenter.y, position[2] - m_massCenter.z);
                }
            }
        }
    }

    SphereForIntersection* PhysicObj::_GetLookSphere() const
    {
        return m_lookSphere;
    }

    void PhysicObj::_SetRotationToGeoms(Quaternion const& rot)
    {
        // RVA 0x5FAFE0
        dQuaternion const dq = {rot.w, rot.x, rot.y, rot.z};
        for (dGeomID geom = dBodyGetFirstGeom(m_body->id()); geom; geom = dGeomGetBodyNext(geom))
        {
            dGeomSetQuaternion(geom, dq);
        }
    }

    void PhysicObj::_EnableIntersections(bool enable)
    {
        if (m_intersectionObstacle)
        {
            if (enable)
            {
                m_intersectionObstacle->Enable();
            }
            else
            {
                m_intersectionObstacle->Disable();
            }
        }
    }

    void PhysicObj::_SetBoundSphereRadius(float radius)
    {
        auto v2 = 1.0;
        if (radius < 1.0 || (v2 = 1.0e30, radius > 1.0e30))
            radius = v2;
        m_boundSphere->SetRadius(radius);
    }

    void PhysicObj::_SetBodyEnabledBit(bool enabled)
    {
        // RVA 0x5FB470
        if (enabled)
        {
            m_physicState |= BODY_ENABLED_BIT;
        }
        else
        {
            m_physicState &= ~BODY_ENABLED_BIT;
        }
        SetCorrectEnabledCellsCounter();
    }

    void PhysicObj::_CreateSpace(bool bForUntransfer)
    {
        if (!m_spaceId && m_bIsSpaceOwner || bForUntransfer)
        {
            auto v3 = ai::gGlobalSpace;
            if (ai::gGlobalSpace->lock_count)
                v3 = ai::gTempSpace;
            auto v4 = dSimpleSpaceCreate(v3);
            m_spaceId = v4;
            dSpaceSetCleanup(v4, 0);
        }
    }

    bool PhysicObj::_GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x5FE5E0
        switch (propertyId)
        {
        case 4:
            retVal = GetPosition();
            return true;
        case 5:
            retVal = GetRotation();
            return true;
        case 45:
            retVal = m_skinNumber;
            return true;
        default:
            return Obj::_GetPropertyInternal(propertyId, retVal);
        }
    }

    bool PhysicObj::_GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x5FE690
        switch (propertyId)
        {
        case 4:
            retVal = ZeroVector;
            return true;
        case 5:
            retVal = IdentityQuaternion;
            return true;
        case 45:
            retVal = 0;
            return true;
        default:
            return Obj::_GetPropertyDefaultInternal(propertyId, retVal);
        }
    }

    void PhysicObj::_UpdateOwnPhysics(float)
    {
    }

    void PhysicObj::_SetPositionToGeoms(CVector const& pos)
    {
        // RVA 0x5FAF90
        for (dGeomID geom = dBodyGetFirstGeom(m_body->id()); geom; geom = dGeomGetBodyNext(geom))
        {
            dGeomSetPosition(geom, pos.x, pos.y, pos.z);
        }
    }

    void PhysicObj::_UnlinkBodyFromGeoms()
    {
        for (auto i = dBodyGetFirstGeom(m_body->id()); i; i = dGeomGetBodyNext(i))
            dGeomUnlinkFromBody(i);
    }

    void PhysicObj::_SetStaticCollision()
    {
        for (auto* i = dBodyGetFirstGeom(m_body->id()); i; i = dGeomGetBodyNext(i))
        {
            if (dGeomGetClass(i) == 6)
            {
                dGeomSetCategoryBits(i, 1u);
                dGeomSetCollideBits(i, 0xFFFFFFFE);
            }
        }
    }

    void PhysicObj::_SetStatic()
    {
        m_physicState |= 4u;
        _SetStaticCollision();
    }

    void PhysicObj::_SetMassCenter(CVector const& massCenter)
    {
        auto const pos = GetPosition();
        m_massCenter.x = 0.0 - m_massCenter.x;
        m_massCenter.y = 0.0 - m_massCenter.y;
        m_massCenter.z = 0.0 - m_massCenter.z;

        _AdjustMassCenter();
        m_massCenter = massCenter;
        _AdjustMassCenter();

        SetPositionSelf(pos);
    }

    PhysicObj::~PhysicObj()
    {
        delete m_lookSphere;
        if (m_intersectionObstacle)
        {
            m_intersectionObstacle->UnlinkFromOwner();
        }
        delete m_boundSphere;
        delete m_body;
        if (m_spaceId && m_bIsSpaceOwner)
            dSpaceDestroy(m_spaceId);
    }

    void PhysicObj::_InternalPostLoad()
    {
        if (GetPrototypeInfo()->m_intersectionRadius > 0.0099999998)
        {
            m_intersectionObstacle = new Obstacle(this);
        }
    }

    void PhysicObj::_InternalCreateVisualPart()
    {
        LinkGeomsToCollisionCells();
    }

    void PhysicObj::_SetGeomEnabledBit(bool enabled)
    {
        if (enabled)
        {
            m_physicState |= 2u;
        }
        else
        {
            m_physicState &= ~2u;
        }
    }

    void PhysicObj::RegisterProperty(char const* name, int id, eGObjPropertySaveStatus saveStatus)
    {
        // RVA 0x5FE580
        m_propertiesMap[CStr(name)] = id;
        if (saveStatus)
        {
            m_propertiesSaveStatesMap[id] = saveStatus;
        }
    }

    bool PhysicObj::_UpdateMustBeRelinked()
    {
        // RVA 0x5F9CD0
        return true;
    }

    void PhysicObj::_LinkBodyToGeoms()
    {
        for (auto i = dBodyGetFirstGeom(m_body->id()); i; i = dGeomGetBodyNext(i))
            dGeomLinkToBody(i);
    }

    m3d::Object* PhysicObj::Clone()
    {
        // RVA 0x5FB580
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    m3d::Object* PhysicObj::CreateObject()
    {
        // RVA 0x5FB740
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    void PhysicObj::_CommonBodyChangeEnabledStateCallback(dxBody* bodyId)
    {
        auto Data = (ai::PhysicObj*)dBodyGetData(bodyId);
        if (Data)
        {
            auto IsEnabled = dBodyIsEnabled(bodyId);
            if (IsEnabled != Data->m_bBodyEnabledLastFrame)
            {
                if (IsEnabled)
                {
                    Data->m_physicState |= 1u;
                    Data->SetCorrectEnabledCellsCounter();
                    Data->m_bBodyEnabledLastFrame = IsEnabled;
                    return;
                }
                Data->m_physicState &= ~1u;
                Data->SetCorrectEnabledCellsCounter();
                if ((Data->m_physicBehaviorFlags & 1) != 0)
                    Data->DisablePhysics();
            }
            Data->m_bBodyEnabledLastFrame = IsEnabled;
        }
    }

    CVector getPhysicObjOrPhysicBodyGeometricCenter(ai::Obj const* obj)
    {
        if (IS_KIND_OF(obj, PhysicBody))
        {
            auto* body = RT_DYNCAST(obj, PhysicBody const);
            return body->GetPosition();
        }
        if (IS_KIND_OF(obj, PhysicObj))
        {
            auto* phys = RT_DYNCAST(obj, PhysicObj const);
            return phys->GetGeometricCenter();
        }
        return ZeroVector;
    }

    CVector getPhysicObjOrPhysicBodyPosition(ai::Obj const* obj)
    {
        if (IS_KIND_OF(obj, PhysicBody))
        {
            auto* body = RT_DYNCAST(obj, PhysicBody const);
            return body->GetPosition();
        }
        if (IS_KIND_OF(obj, PhysicObj))
        {
            auto* phys = RT_DYNCAST(obj, PhysicObj const);
            return phys->GetPosition();
        }
        return ZeroVector;
    }

    CVector getPhysicObjOrPhysicBodyLinearVelocity(ai::Obj const* obj)
    {
        // RVA 0x9FB250 - NOTE: the original also tests for a PhysicBody, but a body still reports no velocity.
        if (IS_KIND_OF(obj, PhysicObj))
        {
            return static_cast<PhysicObj const*>(obj)->GetLinearVelocity();
        }
        return ZeroVector;
    }

    CVector getPhysicObjOrPhysicBodyDirection(ai::Obj const* obj)
    {
        // RVA 0x9FB2B0 - anything that is neither faces the default +z direction.
        if (IS_KIND_OF(obj, PhysicBody))
        {
            return static_cast<PhysicBody const*>(obj)->GetDirection();
        }
        if (IS_KIND_OF(obj, PhysicObj))
        {
            return static_cast<PhysicObj const*>(obj)->GetDirection();
        }
        return CVector(0.0f, 0.0f, 1.0f);
    }
}  // namespace ai
