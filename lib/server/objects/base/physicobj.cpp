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
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(PhysicObj, SetDirection)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(PhysicObj, GetDirection)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(PhysicObj, GetLinearVelocity)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(PhysicObj, SetLinearVelocity)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(PhysicObj, GetAngularVelocity)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(PhysicObj, SetAngularVelocity)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(PhysicObj, SetUpdatingByODE)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(PhysicObj, GetSkin)
{
    RETRUXX_NOT_IMPLEMENTED;
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
    RETRUXX_NOT_IMPLEMENTED;
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
        // TODO: generated code
        // Create rotation matrix directly from forward and up vectors
        CMatrix mat;
        mat.zero();

        // Calculate the right vector (x-axis) as cross product of up and forward
        mat.m[0][0] = up.y * forward.z - up.z * forward.y;  // right.x
        mat.m[0][1] = up.z * forward.x - up.x * forward.z;  // right.y
        mat.m[0][2] = up.x * forward.y - up.y * forward.x;  // right.z

        // Use provided up vector for y-axis
        mat.m[1][0] = up.x;
        mat.m[1][1] = up.y;
        mat.m[1][2] = up.z;

        // Use provided forward vector for z-axis
        mat.m[2][0] = forward.x;
        mat.m[2][1] = forward.y;
        mat.m[2][2] = forward.z;

        // Convert to quaternion and set rotation
        Quaternion rotation;
        rotation.FromMatrix(mat);
        SetRotation(rotation);
    }

    void PhysicObj::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    Quaternion PhysicObj::GetPostRotation() const
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector PhysicObj::GetMassCenterPosition() const
    {
        RETRUXX_NOT_IMPLEMENTED;
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

    void PhysicObj::AddTorque(CVector const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
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

    void PhysicObj::AddImpulseAtRelPos(CVector const&, CVector const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicObj::TransferToNewSpace()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicObj::SetRotation(Quaternion const& rot)
    {
        SetRotationSelf(rot);
    }

    bool PhysicObj::CanCreateCollisionEffect() const
    {
        return m_timeFromLastCollisionEffect > 0.1 || !IsUpdating();
    }

    void PhysicObj::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicObj::AddImpulse(CVector const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicObj::SetPositionSelf(CVector const& pos)
    {
        // TODO: generated code
        Quaternion rotation = GetRotation();
        CMatrix rotationMatrix = rotation.ToMatrix();

        // Transform mass center by rotation matrix
        float transformedX =
            (m_massCenter.x * rotationMatrix._11) + (m_massCenter.y * rotationMatrix._21) + (m_massCenter.z * rotationMatrix._31);

        float transformedY =
            (m_massCenter.x * rotationMatrix._12) + (m_massCenter.y * rotationMatrix._22) + (m_massCenter.z * rotationMatrix._32);

        float transformedZ =
            (m_massCenter.x * rotationMatrix._13) + (m_massCenter.y * rotationMatrix._23) + (m_massCenter.z * rotationMatrix._33);

        // Calculate final position (position + transformed mass center)
        CVector realPos;
        realPos.x = pos.x + transformedX;
        realPos.y = pos.y + transformedY;
        realPos.z = pos.z + transformedZ;

        // Set body position
        dBodySetPosition(m_body->id(), realPos.x, realPos.y, realPos.z);

        // Update enabled cells counter
        PhysicObj::SetCorrectEnabledCellsCounter();
    }

    void PhysicObj::SetPostRotation(Quaternion const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicObj::DumpPhysicInfo(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
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

    void PhysicObj::SetTorque(CVector const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned PhysicObj::GetSkin() const
    {
        return m_skinNumber;
    }

    void PhysicObj::EnablePhysicsIfPossible()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector PhysicObj::GetGeometricCenter() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicObj::SetPassedToAnotherMapStatus()
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
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

    void PhysicObj::SetMassCenterPosition(CVector const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
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

    void PhysicObj::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicObj::AddForceAtPos(CVector const&, CVector const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        // TODO: generated code
        Quaternion rotation = GetRotation();

        // Extract quaternion components for clarity
        float const x = rotation.x;
        float const y = rotation.y;
        float const z = rotation.z;
        float const w = rotation.w;

        // Calculate quaternion products
        float const xx = x * x;
        float const xy = x * y;
        float const xz = x * z;
        float const xw = x * w;

        float const yy = y * y;
        float const yz = y * z;
        float const yw = y * w;

        float const zz = z * z;
        float const zw = z * w;

        // Build rotation matrix from quaternion
        CMatrix rotationMatrix;

        // First row
        rotationMatrix._11 = 1.0f - 2.0f * (yy + zz);
        rotationMatrix._12 = 2.0f * (xy + zw);
        rotationMatrix._13 = 2.0f * (xz - yw);
        rotationMatrix._14 = 0.0f;

        // Second row
        rotationMatrix._21 = 2.0f * (xy - zw);
        rotationMatrix._22 = 1.0f - 2.0f * (xx + zz);
        rotationMatrix._23 = 2.0f * (yz + xw);
        rotationMatrix._24 = 0.0f;

        // Third row
        rotationMatrix._31 = 2.0f * (xz + yw);
        rotationMatrix._32 = 2.0f * (yz - xw);
        rotationMatrix._33 = 1.0f - 2.0f * (xx + yy);
        rotationMatrix._34 = 0.0f;

        // Fourth row (translation/identity)
        rotationMatrix._41 = 0.0f;
        rotationMatrix._42 = 0.0f;
        rotationMatrix._43 = 0.0f;
        rotationMatrix._44 = 1.0f;

        // Transform mass center by rotation matrix
        float const transformedX =
            m_massCenter.x * rotationMatrix._11 + m_massCenter.y * rotationMatrix._21 + m_massCenter.z * rotationMatrix._31;

        float const transformedY =
            m_massCenter.x * rotationMatrix._12 + m_massCenter.y * rotationMatrix._22 + m_massCenter.z * rotationMatrix._32;

        float const transformedZ =
            m_massCenter.x * rotationMatrix._13 + m_massCenter.y * rotationMatrix._23 + m_massCenter.z * rotationMatrix._33;

        // Get body position (assuming dBodyGetPosition returns a pointer to 3 floats)
        float const* bodyPosition = dBodyGetPosition(m_body->id());

        // Calculate final position: body position - transformed mass center
        CVector result;
        result.x = bodyPosition[0] - transformedX;
        result.y = bodyPosition[1] - transformedY;
        result.z = bodyPosition[2] - transformedZ;

        return result;
    }

    void PhysicObj::PostCollide()
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicObj::SetForce(CVector const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector PhysicObj::GetPostPosition() const
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool PhysicObj::bIsStatic() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicObj::SetLinearVelocity(CVector const& linearVel)
    {
        dBodySetLinearVel(m_body->id(), linearVel.x, linearVel.y, linearVel.z);
    }

    void PhysicObj::AddForce(CVector const& force)
    {
        dBodyAddForce(m_body->id(), force.x, force.y, force.z);
    }

    void PhysicObj::AddForceAtRelPos(CVector const&, CVector const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicObj::AddImpulseAtPos(CVector const&, CVector const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicObj::SetInvisible()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::AIParam PhysicObj::AIGetCurPos(Obj*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector PhysicObj::GetPositionAtRelPoint(CVector) const
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        m_massCenter.y = 0.0;

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
        RETRUXX_NOT_IMPLEMENTED;
    }

    ai::SphereForIntersection const* PhysicObj::GetIntersectionSphere() const
    {
        return m_lookSphere;
    }

    bool PhysicObj::bIsBodyDisabledGeomEnabled() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicObj::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
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

    void PhysicObj::SetPostPosition(CVector const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* PhysicObj::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
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

    void PhysicObj::AddRelativeRotation(Quaternion const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector PhysicObj::GetDirection() const
    {
        // TODO: generated code
        // Get the object's current rotation as a quaternion
        Quaternion rotation = GetRotation();

        // Extract quaternion components for readability
        float x = rotation.x;
        float y = rotation.y;
        float z = rotation.z;
        float w = rotation.w;

        // Calculate squared components (used in matrix diagonal)
        float x2 = x * x;
        float y2 = y * y;
        float z2 = z * z;

        // Calculate cross terms (used in matrix off-diagonals)
        float xy = x * y;
        float xz = x * z;
        float xw = x * w;
        float yz = y * z;
        float yw = y * w;
        float zw = z * w;

        // Construct rotation matrix from quaternion
        CMatrix rotationMatrix;

        // First row
        rotationMatrix._11 = 1.0f - 2.0f * (y2 + z2);
        rotationMatrix._12 = 2.0f * (xy + zw);
        rotationMatrix._13 = 2.0f * (xz - yw);
        rotationMatrix._14 = 0.0f;

        // Second row
        rotationMatrix._21 = 2.0f * (xy - zw);
        rotationMatrix._22 = 1.0f - 2.0f * (x2 + z2);
        rotationMatrix._23 = 2.0f * (yz + xw);
        rotationMatrix._24 = 0.0f;

        // Third row
        rotationMatrix._31 = 2.0f * (xz + yw);
        rotationMatrix._32 = 2.0f * (yz - xw);
        rotationMatrix._33 = 1.0f - 2.0f * (x2 + y2);
        rotationMatrix._34 = 0.0f;

        // Fourth row (identity for homogeneous coordinates)
        rotationMatrix._41 = 0.0f;
        rotationMatrix._42 = 0.0f;
        rotationMatrix._43 = 0.0f;
        rotationMatrix._44 = 1.0f;

        CVector INITIAL_OBJECTS_DIRECTION_5(0.0, 0.0, 1.0);

        // Transform the initial forward direction by the rotation matrix
        // This gives us the object's current forward direction in world space
        CVector result;
        result.x = rotationMatrix._11 * INITIAL_OBJECTS_DIRECTION_5.x + rotationMatrix._21 * INITIAL_OBJECTS_DIRECTION_5.y +
            rotationMatrix._31 * INITIAL_OBJECTS_DIRECTION_5.z;

        result.y = rotationMatrix._12 * INITIAL_OBJECTS_DIRECTION_5.x + rotationMatrix._22 * INITIAL_OBJECTS_DIRECTION_5.y +
            rotationMatrix._32 * INITIAL_OBJECTS_DIRECTION_5.z;

        result.z = rotationMatrix._13 * INITIAL_OBJECTS_DIRECTION_5.x + rotationMatrix._23 * INITIAL_OBJECTS_DIRECTION_5.y +
            rotationMatrix._33 * INITIAL_OBJECTS_DIRECTION_5.z;

        return result;
    }

    bool PhysicObj::GetGeomEnabledBit() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicObj::LinkGeomsToCollisionCells()
    {
        m_boundSphere->LinkToCollisionCells(GetId(), nullptr);
    }

    CStr PhysicObj::GetPropertyName(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    eGObjPropertySaveStatus PhysicObj::GetPropertySaveStatus(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicObj::ZeroEnabledCellsCount()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicObj::SetCorrectEnabledCellsCounter()
    {
        m_enabledCellsCount = 0;
        if ((m_physicState & 1) == 0 && (m_physicState & 2) != 0)
        {
            auto aabb = GetCollisionCellAabb();
            if (aabb.z0 <= aabb.z1)
            {
                auto z0 = aabb.z0;
                if (aabb.z0 <= aabb.z1)
                {
                    int retaddr = 0;
                    auto v6 = retaddr;
                    auto x1 = aabb.x1;
                    do
                    {
                        if (x1 <= v6)
                        {
                            do
                            {
                                auto CollisionCellItem = ai::pServer->GetWorld()->GetLandscape().GetCollisionCellItem(z0, x1);
                                if (CollisionCellItem->m_wasEnabledLastFrame)
                                    ++m_enabledCellsCount;
                                CollisionCellItem->m_bMustCheck = 1;
                                v6 = retaddr;
                                ++x1;
                            } while (x1 <= retaddr);
                            x1 = aabb.x1;
                        }
                        ++z0;
                    } while (z0 <= aabb.z1);
                }
                if (m_enabledCellsCount <= 0)
                    DisableGeometry(0);
                else
                    EnableGeometry(0);
            }
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
        RETRUXX_NOT_IMPLEMENTED;
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

    void PhysicObj::_SetRotationToGeoms(Quaternion const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
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

    void PhysicObj::_SetBodyEnabledBit(bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
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

    bool PhysicObj::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool PhysicObj::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicObj::_UpdateOwnPhysics(float)
    {
    }

    void PhysicObj::_SetPositionToGeoms(CVector const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
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

    void PhysicObj::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool PhysicObj::_UpdateMustBeRelinked()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicObj::_LinkBodyToGeoms()
    {
        for (auto i = dBodyGetFirstGeom(m_body->id()); i; i = dGeomGetBodyNext(i))
            dGeomLinkToBody(i);
    }

    m3d::Object* PhysicObj::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* PhysicObj::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
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
}  // namespace ai
