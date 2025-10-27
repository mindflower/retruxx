#include "physicobj.h"
#include <stdexcept>
#include <core/aiparam.h>
#include <server/obstacle.h>

#include "landscape.h"
#include "world.h"
#include "core/ini.h"
#include "game/m3dgame.h"
#include "server/dynamicscene.h"
#include "server/ai/aimanager.h"

#include "thirdparty/injecttools.h"

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
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(PhysicObj, SetDirection)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(PhysicObj, GetDirection)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(PhysicObj, GetLinearVelocity)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(PhysicObj, SetLinearVelocity)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(PhysicObj, GetAngularVelocity)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(PhysicObj, SetAngularVelocity)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(PhysicObj, SetUpdatingByODE)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(PhysicObj, GetSkin)
{
    throw std::logic_error("Not implemented");
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
    throw std::logic_error("Not implemented");
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
        this->m_intersectionRadius = 0.0;
        this->m_lookRadius = 0.0;
	}

	bool PhysicObjPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
	{
        auto result = ai::PrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeFloatAttrib(m_intersectionRadius, xmlNode, "IntersectionRadius");
            m3d::SafeFloatAttrib(m_lookRadius, xmlNode, "LookRadius");
            return 1;
        }
        return result;
	}

    void PhysicObj::SetDirections(CVector const& forward, CVector const& up)
    {
        // TODO: generated code
        // Create rotation matrix directly from forward and up vectors
        CMatrix mat;
        memset(&mat, 0, sizeof(mat));

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
        throw std::logic_error("Not implemented");
    }

    bool PhysicObj::GetBodyEnabledBit() const
    {
        throw std::logic_error("Not implemented");
    }

    Quaternion PhysicObj::GetRotation() const
    {
        auto quat = dBodyGetQuaternion(this->m_body->id());
        auto dq = *quat;
        auto v3 = quat[2];
        auto dq_4 = quat[1];
        auto v4 = quat[3];

        Quaternion result;
        result.x = dq_4;
        result.y = v3;
        result.z = v4;
        result.w = dq;
        return result;
    }

    void PhysicObj::SetPostDisablePhysicsWithAutoEnable()
    {
        throw std::logic_error("Not implemented");
    }

    CVector PhysicObj::GetMassCenter() const
    {
        throw std::logic_error("Not implemented");
    }

    bool PhysicObj::bIsUpdatingByODE() const
    {
        return this->m_bIsUpdatingByODE != 0;
    }

    void PhysicObj::RelinkToSpace(dxSpace*)
    {
        throw std::logic_error("Not implemented");
    }

    bool PhysicObj::IsVisible()
    {
        throw std::logic_error("Not implemented");
    }

    Quaternion PhysicObj::GetPostRotation() const
    {
        throw std::logic_error("Not implemented");
    }

    dxSpace* PhysicObj::GetSpaceId() const
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::SetSkin(int skin)
    {
        this->m_skinNumber = skin;
    }

    m3d::Class* PhysicObj::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    void PhysicObj::UnlinkGeomsFromCollisionCells()
    {
        this->m_boundSphere->UnlinkFromCollisionCells(GetId());
    }

    bool PhysicObj::CanPhysicsBeEnabled() const
    {
        throw std::logic_error("Not implemented");
    }

    float PhysicObj::GetMass() const
    {
        dMass mass;

        dMassSetZero(&mass);
        dBodyGetMass(this->m_body->id(), &mass);
        return mass.mass;
    }

    void PhysicObj::DisablePhysicsAndGeometry()
    {
        throw std::logic_error("Not implemented");
    }

    bool PhysicObj::SetPropertyById(int propertyId, m3d::AIParam const& newValue)
    {
        switch (propertyId)
        {
        case 4:
            SetPositionSelf(newValue.GetAsVector());
            return true;

        case 5:
            throw std::logic_error("Not implemented");
        case 45:
            throw std::logic_error("Not implemented");
        default:
            return ai::Obj::SetPropertyById(propertyId, newValue);
        }
    }

    void PhysicObj::SetPostEnablePhysicsIfPossible()
    {
        throw std::logic_error("Not implemented");
    }

    CVector PhysicObj::GetMassCenterPosition() const
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::TransferToSpace(dxSpace*)
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::SetVisible()
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::AddTorque(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::CheckCollisionCells()
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::Registration()
    {
        standardTargetClasses.insert(RT_CLASS_LOCAL(PhysicObj));
        theAIManager->RegisterFunc("AIGetCurPos", &PhysicObj::AIGetCurPos);
        m_propertiesMap["Pos"] = 4;
        m_propertiesMap["Rot"] = 5;
        m_propertiesMap["Skin"] = 45;
        m_countRelinksToCollisionCells = M3D_APP->GetDbgCounterStack().GetCounter(M3D_APP->GetDbgCounterStack().AddCounter("relinks to collision cells"));
        m_countRelinksToCollisionCells->SetI(0);
    }

    void PhysicObj::AddImpulseAtRelPos(CVector const&, CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::TransferToNewSpace()
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::SetRotation(Quaternion const& rot)
    {
        this->SetRotationSelf(rot);
    }

    bool PhysicObj::CanCreateCollisionEffect() const
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::AddImpulse(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::SetPositionSelf(CVector const& pos)
    {
        // TODO: generated code
        Quaternion rotation = GetRotation();

        float qx = rotation.x;
        float qy = rotation.y;
        float qz = rotation.z;
        float qw = rotation.w;

        // Calculate quaternion products
        float qx_qx = qx * qx;
        float qy_qy = qy * qy;
        float qz_qz = qz * qz;
        float qw_qz = qw * qz;
        float qx_qz = qx * qz;
        float qw_qx = qw * qx;
        float qx_qy = qx * qy;
        float qz_qy = qz * qy;
        float qw_qy = qw * qy;

        // Build rotation matrix from quaternion
        CMatrix rotationMatrix;

        rotationMatrix._11 = 1.0f - 2.0f * (qz_qz + qy_qy);
        rotationMatrix._12 = 2.0f * (qx_qy + qw_qz);
        rotationMatrix._13 = 2.0f * (qx_qz - qw_qy);
        rotationMatrix._14 = 0.0f;

        rotationMatrix._21 = 2.0f * (qx_qy - qw_qz);
        rotationMatrix._22 = 1.0f - 2.0f * (qz_qz + qx_qx);
        rotationMatrix._23 = 2.0f * (qz_qy + qw_qx);
        rotationMatrix._24 = 0.0f;

        rotationMatrix._31 = 2.0f * (qx_qz + qw_qy);
        rotationMatrix._32 = 2.0f * (qz_qy - qw_qx);
        rotationMatrix._33 = 1.0f - 2.0f * (qy_qy + qx_qx);
        rotationMatrix._34 = 0.0f;

        rotationMatrix._41 = 0.0f;
        rotationMatrix._42 = 0.0f;
        rotationMatrix._43 = 0.0f;
        rotationMatrix._44 = 1.0f;

        // Transform mass center by rotation matrix
        float transformedX = (this->m_massCenter.x * rotationMatrix._11) +
            (this->m_massCenter.y * rotationMatrix._21) +
            (this->m_massCenter.z * rotationMatrix._31);

        float transformedY = (this->m_massCenter.x * rotationMatrix._12) +
            (this->m_massCenter.y * rotationMatrix._22) +
            (this->m_massCenter.z * rotationMatrix._32);

        float transformedZ = (this->m_massCenter.x * rotationMatrix._13) +
            (this->m_massCenter.y * rotationMatrix._23) +
            (this->m_massCenter.z * rotationMatrix._33);

        // Calculate final position (position + transformed mass center)
        CVector realPos;
        realPos.x = pos.x + transformedX;
        realPos.y = pos.y + transformedY;
        realPos.z = pos.z + transformedZ;

        // Set body position
        dBodySetPosition(this->m_body->id(), realPos.x, realPos.y, realPos.z);

        // Update enabled cells counter
        ai::PhysicObj::SetCorrectEnabledCellsCounter();
    }

    void PhysicObj::SetPostRotation(Quaternion const&)
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::DumpPhysicInfo(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::SetAutoDisabling(bool, float, float, int)
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::IncEnabledCellsCount()
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::SetTorque(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    unsigned PhysicObj::GetSkin() const
    {
        return m_skinNumber;
    }

    void PhysicObj::EnablePhysicsIfPossible()
    {
        throw std::logic_error("Not implemented");
    }

    CVector PhysicObj::GetGeometricCenter() const
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::SetPassedToAnotherMapStatus()
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::SetCollisionEffectCreated()
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::DisableGeometry(bool changePhysicState)
    {
        if (m_spaceId && this->m_bIsSpaceOwner)
            dGeomDisable(m_spaceId);
        if (changePhysicState)
            this->m_physicState &= ~2u;
    }

    void PhysicObj::EnableGeometry(bool changePhysicState)
    {
        if (m_spaceId && this->m_bIsSpaceOwner)
            dGeomEnable(m_spaceId);
        if (changePhysicState)
            this->m_physicState |= 2u;
    }

    float PhysicObj::GetIntersectionRadius() const
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::SetPostEnablePhysics()
    {
        throw std::logic_error("Not implemented");
    }

    dBody* PhysicObj::GetBody()
    {
        return this->m_body;
    }

    dBody const* PhysicObj::GetBody() const
    {
        return this->m_body;
    }

    void PhysicObj::Update(float elapsedTime, unsigned workTime)
    {
        ai::Obj::Update(elapsedTime, workTime);
        if (!this->m_bIsUpdatingByODE)
            this->_UpdateOwnPhysics(elapsedTime);
        this->m_timeFromLastCollisionEffect = this->m_timeFromLastCollisionEffect + elapsedTime;
    }

    void PhysicObj::SetUpdatingByODE(bool byODE)
    {
        if (byODE)
        {
            if (!this->m_bIsUpdatingByODE)
            {
                dBodyAddIslandToWorld(this->m_body->id(), ai::gGlobalWorld);
                this->m_bIsUpdatingByODE = 1;
                SetLinearVelocity({0.0, 0.0, 0.0});
                dBodySetAngularVel(this->m_body->id(), 0.0, 0.0, 0.0);
                dBodySetForce(this->m_body->id(), 0.0, 0.0, 0.0);
                dBodySetTorque(this->m_body->id(), 0.0, 0.0, 0.0);
                this->CheckCollisionCells();
            }
        }
        else if (this->m_bIsUpdatingByODE)
        {
            dBodyRemoveIslandFromWorld(this->m_body->id());
            this->m_bIsUpdatingByODE = 0;
            CheckCollisionCells();
        }
    }

    void PhysicObj::SetMassCenterPosition(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::EnablePhysics()
    {
        if (m_body)
            dBodyEnable(m_body->id());
        this->_LinkBodyToGeoms();
        this->m_physicState |= 1u;
        SetCorrectEnabledCellsCounter();
        auto v3 = (this->m_physicState & 2) == 0;
        this->m_bBodyEnabledLastFrame = 1;
        if (!v3)
            this->EnableGeometry(0);
    }

    void PhysicObj::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::AddForceAtPos(CVector const&, CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::DecEnabledCellsCount()
    {
        throw std::logic_error("Not implemented");
    }

    RETRUXX_DLL_OVERWRITE_BY_ORIGINAL_FUNCTION(0x005FC410, PhysicObj::GetPosition)
    CVector PhysicObj::GetPosition() const
    {
        // TODO: generated code
        Quaternion rotation = GetRotation();

        // Extract quaternion components for clarity
        const float x = rotation.x;
        const float y = rotation.y;
        const float z = rotation.z;
        const float w = rotation.w;

        // Calculate quaternion products
        const float xx = x * x;
        const float xy = x * y;
        const float xz = x * z;
        const float xw = x * w;

        const float yy = y * y;
        const float yz = y * z;
        const float yw = y * w;

        const float zz = z * z;
        const float zw = z * w;

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
        const float transformedX = m_massCenter.x * rotationMatrix._11 +
            m_massCenter.y * rotationMatrix._21 +
            m_massCenter.z * rotationMatrix._31;

        const float transformedY = m_massCenter.x * rotationMatrix._12 +
            m_massCenter.y * rotationMatrix._22 +
            m_massCenter.z * rotationMatrix._32;

        const float transformedZ = m_massCenter.x * rotationMatrix._13 +
            m_massCenter.y * rotationMatrix._23 +
            m_massCenter.z * rotationMatrix._33;

        // Get body position (assuming dBodyGetPosition returns a pointer to 3 floats)
        const float* bodyPosition = dBodyGetPosition(m_body->id());

        // Calculate final position: body position - transformed mass center
        CVector result;
        result.x = bodyPosition[0] - transformedX;
        result.y = bodyPosition[1] - transformedY;
        result.z = bodyPosition[2] - transformedZ;

        return result;
    }

    void PhysicObj::PostCollide()
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::DisablePhysics()
    {
        if (m_body)
            dBodyDisable(m_body->id());
        this->m_physicState &= ~1u;
        ai::PhysicObj::SetCorrectEnabledCellsCounter();
        this->m_bBodyEnabledLastFrame = 0;
        ai::PhysicObj::SetCorrectEnabledCellsCounter();
        dBodyDetachAllContactJoints(this->m_body->id());
        this->_UnlinkBodyFromGeoms();
    }

    void PhysicObj::SetDisablePhysicsWhenBodyDisabled()
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::SetForce(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    CVector PhysicObj::GetPostPosition() const
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::SetAngularVelocity(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    int PhysicObj::GetPhysicState() const
    {
        return m_physicState;
    }

    void PhysicObj::RenderDebugInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::AddRelTorque(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::SetPostDisablePhysics()
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::Remove()
    {
        ai::Obj::Remove();
        this->UnlinkGeomsFromCollisionCells();
    }

    void PhysicObj::SetDirection(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    Geom::CellAabb PhysicObj::GetCollisionCellAabb() const
    {
        return m_boundSphere->CountCellAabb();
    }

    void PhysicObj::RenderObstacleDebugInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    bool PhysicObj::bIsStatic() const
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::SetLinearVelocity(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::AddForce(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::AddForceAtRelPos(CVector const&, CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::AddImpulseAtPos(CVector const&, CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::SetInvisible()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::AIParam PhysicObj::AIGetCurPos(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    CVector PhysicObj::GetPositionAtRelPoint(CVector) const
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::SetPosition(CVector const& pos)
    {
        this->SetPositionSelf(pos);
    }

    PhysicObj::PhysicObj(PhysicObjPrototypeInfo const& prototypeInfo)
    {
        this->m_intersectionObstacle = nullptr;
        this->m_body = 0;
        this->m_lookSphere = 0;
        this->m_postActionFlags = 0;
        this->m_postRotation.x = 0.0;
        this->m_postRotation.y = 0.0;
        this->m_postRotation.z = 0.0;
        this->m_postRotation.w = 1.0;
        this->m_postPosition.x = 0.0;
        this->m_postPosition.y = 0.0;
        this->m_postPosition.z = 0.0;
        this->m_physicBehaviorFlags = 0;
        this->m_massCenter.x = 0.0;
        this->m_massCenter.y = 0.0;
        this->m_massCenter.y = 0.0;

        m_body = new dBody(gGlobalWorld);

        m_body->setData(this);
        m_body->setChangeEnabledStateCallback(ai::PhysicObj::_CommonBodyChangeEnabledStateCallback);
        this->m_spaceId = 0;
        this->m_bIsSpaceOwner = 1;
        if (prototypeInfo.m_lookRadius > 0.0099999998)
        {
            m_lookSphere = SphereForIntersection::CreateObject(prototypeInfo.m_lookRadius, SphereForIntersection::LOOKING, nullptr);
            dGeomSetBody(m_lookSphere->GetGeomId(), m_body->id());
            m_lookSphere->SetTargetClasses(standardTargetClasses);
        }
        this->m_boundSphere = ai::Sphere::CreateObject(0, 1.0, 0);;
        dGeomSetBody(m_boundSphere->GetGeomId(), this->m_body->id());
        this->m_bIsUpdatingByODE = 1;
        this->m_enabledCellsCount = 0;
        this->m_bBodyEnabledLastFrame = 1;
        this->m_skinNumber = 0;
        this->m_physicState = 3;
        this->m_timeFromLastCollisionEffect = 1000.0;
    }

    void PhysicObj::EnablePhysicsAndGeometry()
    {
        throw std::logic_error("Not implemented");
    }

    const ai::SphereForIntersection* PhysicObj::GetIntersectionSphere() const
    {
        throw std::logic_error("Not implemented");
    }

    bool PhysicObj::bIsBodyDisabledGeomEnabled() const
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::DisablePhysicsWithAutoEnable()
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::SetRotationSelf(Quaternion const& rot)
    {
        CVector pos = GetPosition();
        m_body = this->m_body;

        float dq[4];
        dq[0] = rot.w;
        dq[1] = rot.x;
        dq[2] = rot.y;
        dq[3] = rot.z;
        dBodySetQuaternion(m_body->id(), dq);
        ai::PhysicObj::SetPositionSelf(pos);
    }

    void PhysicObj::SetPostPosition(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* PhysicObj::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    CVector PhysicObj::GetAngularVelocity() const
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::RelinkGeomsToCollisionCells()
    {
        m_boundSphere->RelinkToCollisionCells(GetId());
    }

    void PhysicObj::AddRelativeRotation(Quaternion const&)
    {
        throw std::logic_error("Not implemented");
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
        // This is the standard conversion: R = [1-2(y²+z²)  2(xy-zw)    2(xz+yw)   ]
        //                                     [2(xy+zw)     1-2(x²+z²)  2(yz-xw)   ]
        //                                     [2(xz-yw)     2(yz+xw)    1-2(x²+y²) ]
        CMatrix rotationMatrix;

        // First row
        rotationMatrix._11 = 1.0f - 2.0f * (y2 + z2);
        rotationMatrix._12 = 2.0f * (xy - zw);
        rotationMatrix._13 = 2.0f * (xz + yw);
        rotationMatrix._14 = 0.0f;

        // Second row
        rotationMatrix._21 = 2.0f * (xy + zw);
        rotationMatrix._22 = 1.0f - 2.0f * (x2 + z2);
        rotationMatrix._23 = 2.0f * (yz - xw);
        rotationMatrix._24 = 0.0f;

        // Third row
        rotationMatrix._31 = 2.0f * (xz - yw);
        rotationMatrix._32 = 2.0f * (yz + xw);
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
        result.x = rotationMatrix._11 * INITIAL_OBJECTS_DIRECTION_5.x +
            rotationMatrix._21 * INITIAL_OBJECTS_DIRECTION_5.y +
            rotationMatrix._31 * INITIAL_OBJECTS_DIRECTION_5.z;

        result.y = rotationMatrix._12 * INITIAL_OBJECTS_DIRECTION_5.x +
            rotationMatrix._22 * INITIAL_OBJECTS_DIRECTION_5.y +
            rotationMatrix._32 * INITIAL_OBJECTS_DIRECTION_5.z;

        result.z = rotationMatrix._13 * INITIAL_OBJECTS_DIRECTION_5.x +
            rotationMatrix._23 * INITIAL_OBJECTS_DIRECTION_5.y +
            rotationMatrix._33 * INITIAL_OBJECTS_DIRECTION_5.z;

        return result;
    }

    bool PhysicObj::GetGeomEnabledBit() const
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::LinkGeomsToCollisionCells()
    {
        m_boundSphere->LinkToCollisionCells(GetId(), nullptr);
    }

    CStr PhysicObj::GetPropertyName(int) const
    {
        throw std::logic_error("Not implemented");
    }

    eGObjPropertySaveStatus PhysicObj::GetPropertySaveStatus(int) const
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::ZeroEnabledCellsCount()
    {
        throw std::logic_error("Not implemented");
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
                                    ++this->m_enabledCellsCount;
                                CollisionCellItem->m_bMustCheck = 1;
                                v6 = retaddr;
                                ++x1;
                            } while (x1 <= retaddr);
                            x1 = aabb.x1;
                        }
                        ++z0;
                    } while (z0 <= aabb.z1);
                }
                if (this->m_enabledCellsCount <= 0)
                    this->DisableGeometry(0);
                else
                    this->EnableGeometry(0);
            }
        }
    }

    PhysicObjPrototypeInfo const* PhysicObj::GetPrototypeInfo() const
    {
        return dynamic_cast<PhysicObjPrototypeInfo const*>(ai::thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    CVector PhysicObj::GetLinearVelocity() const
    {
        auto linearVel = dBodyGetLinearVel(this->m_body->id());

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
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::_AdjustMassCenter()
    {
        for (auto i = dBodyGetFirstGeom(this->m_body->id()); i; i = dGeomGetBodyNext(i))
        {
            if (dGeomGetClass(i) == 6)
            {
                auto geom = dGeomTransformGetGeom(i);
                if (geom)
                {
                    auto position = dGeomGetPosition(geom);
                    dGeomSetPosition(
                        geom,
                        position[0] - this->m_massCenter.x,
                        position[1] - this->m_massCenter.y,
                        position[2] - this->m_massCenter.z);
                }
            }
        }
    }

    SphereForIntersection* PhysicObj::_GetLookSphere() const
    {
        return this->m_lookSphere;
    }

    void PhysicObj::_SetRotationToGeoms(Quaternion const&)
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::_EnableIntersections(bool)
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::_SetBoundSphereRadius(float radius)
    {
        auto v2 = 1.0;
        if (radius < 1.0 || (v2 = 1.0e30, radius > 1.0e30))
            radius = v2;
        this->m_boundSphere->SetRadius(radius);
    }

    void PhysicObj::_SetBodyEnabledBit(bool)
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::_CreateSpace(bool bForUntransfer)
    {
        if (!this->m_spaceId && this->m_bIsSpaceOwner || bForUntransfer)
        {
            auto v3 = ai::gGlobalSpace;
            if (ai::gGlobalSpace->lock_count)
                v3 = ai::gTempSpace;
            auto v4 = dSimpleSpaceCreate(v3);
            this->m_spaceId = v4;
            dSpaceSetCleanup(v4, 0);
        }
    }

    bool PhysicObj::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        throw std::logic_error("Not implemented");
    }

    bool PhysicObj::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::_UpdateOwnPhysics(float)
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::_SetPositionToGeoms(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::_UnlinkBodyFromGeoms()
    {
        for (auto i = dBodyGetFirstGeom(m_body->id()); i; i = dGeomGetBodyNext(i))
            dGeomUnlinkFromBody(i);
    }

    void PhysicObj::_SetStaticCollision()
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::_SetStatic()
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::_SetMassCenter(CVector const& massCenter)
    {
        const auto pos = GetPosition();
        this->m_massCenter.x = 0.0 - this->m_massCenter.x;
        this->m_massCenter.y = 0.0 - this->m_massCenter.y;
        this->m_massCenter.z = 0.0 - this->m_massCenter.z;
        _AdjustMassCenter();
        m_massCenter = massCenter;
        _AdjustMassCenter();
        SetPositionSelf(pos);
    }

    PhysicObj::~PhysicObj()
    {
        throw std::logic_error("Not implemented");
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
        this->LinkGeomsToCollisionCells();
    }

    void PhysicObj::_SetGeomEnabledBit(bool)
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        throw std::logic_error("Not implemented");
    }

    bool PhysicObj::_UpdateMustBeRelinked()
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::_LinkBodyToGeoms()
    {
        for (auto i = dBodyGetFirstGeom(this->m_body->id()); i; i = dGeomGetBodyNext(i))
            dGeomLinkToBody(i);
    }

    m3d::Object* PhysicObj::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* PhysicObj::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::_CommonBodyChangeEnabledStateCallback(dxBody*)
    {
        throw std::logic_error("Not implemented");
    }

    RETRUXX_DLL_OVERWRITE_BY_ORIGINAL_FUNCTION(0x005FB1E0, getPhysicObjOrPhysicBodyGeometricCenter)
    CVector getPhysicObjOrPhysicBodyGeometricCenter(ai::Obj const*)
    {
        throw std::logic_error("Not implemented");
        return CVector();
    }
}
