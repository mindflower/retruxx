#include "wheel.h"

#include <stdexcept>
#include <ode/objects.h>

#include "base/prototypemanager.h"
#include "core/log.h"
#include "ode/odecpp.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Wheel)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Wheel);

    WheelPrototypeInfo::WheelPrototypeInfo()
    {
        this->m_suspensionRange = 0.5;
        this->m_suspensionCFM = 0.1;
        this->m_suspensionERP = 0.80000001;
        this->m_mU = 1.0;
        this->m_typeName = "BIG";
        this->m_blowEffectName = "ET_PS_HARD_BLOW";
    }

    ai::Obj* WheelPrototypeInfo::CreateTargetObject() const
    {
        return new Wheel(*this);
    }

    bool WheelPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        auto result = ai::SimplePhysicObjPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            _SetGeomType(GEOM_TYPE_SPHERE);
            m3d::SafeFloatAttrib(this->m_suspensionRange, xmlNode, "SuspensionRange");
            m3d::SafeStrAttrib(this->m_suspensionModelName, xmlNode, "SuspensionModelFile");
            m3d::SafeFloatAttrib(this->m_suspensionCFM, xmlNode, "SuspensionCFM");
            m3d::SafeFloatAttrib(this->m_suspensionERP, xmlNode, "SuspensionERP");
            m3d::SafeFloatAttrib(this->m_mU, xmlNode, "mU");
            m3d::SafeStrAttrib(this->m_typeName, xmlNode, "EffectType");
            m3d::SafeStrAttrib(this->m_blowEffectName, xmlNode, "BlowEffect");
        }
        return result;
    }

    void Wheel::BreakModel()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Wheel::Wheel(WheelPrototypeInfo const& prototypeInfo) : SimplePhysicObj(prototypeInfo)
    {
        this->m_jointID = 0;
        this->m_driven = 1;
        this->m_steering = STEERING_NO;
        this->m_SplashEffect = 0;
        this->m_SplashType = 0;
        this->m_MakeSplash = 0;
        this->m_wheelType = ai::gDynamicScene->GetWheelTypeByName(prototypeInfo.m_typeName);
        this->m_bModelBroken = 0;
        this->m_suspensionNode = 0;
        this->m_curAngle = 0.0;
        this->m_initialRotation = {0.0, 0.0, 0.0, 1.0};
    }

    WheelPrototypeInfo const* Wheel::GetPrototypeInfo() const
    {
        return dynamic_cast<WheelPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    void Wheel::RelinkGeomsToCollisionCells()
    {
        SimplePhysicObj::RelinkGeomsToCollisionCells();
    }

    SphericBody const* Wheel::_SphericBody() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Wheel::GetClass() const
    {
        return RT_CLASS_LOCAL(Wheel);
    }

    void Wheel::LinkGeomsToCollisionCells()
    {
        SimplePhysicObj::LinkGeomsToCollisionCells();
    }

    bool Wheel::AttachToPhysicObj(PhysicObj const* physicObj)
    {
        // TODO: check and refactor this
        if (!physicObj)
            return 0;

        auto protoInfo = this->GetPrototypeInfo();
        LinkToParent(physicObj->GetId(), HIERARCHY_CHILD);
        auto hinge = dJointCreateHinge2(ai::gGlobalWorld, 0);

        this->m_jointID = hinge;
        dJointAttach(hinge, physicObj->GetBody()->id(), m_body->id());

        auto anchorPos = GetPosition();
        auto rot = physicObj->GetRotation();

        CMatrix vv;
        vv._11 = 1.0 - (float)((float)((float)(rot.z * rot.z) + (float)(rot.y * rot.y)) * 2.0);
        vv._21 = (float)((float)(rot.x * rot.y) - (float)(rot.w * rot.z)) * 2.0;
        vv._31 = (float)((float)(rot.w * rot.y) + (float)(rot.z * rot.x)) * 2.0;
        vv._12 = (float)((float)(rot.w * rot.z) + (float)(rot.x * rot.y)) * 2.0;
        vv._22 = 1.0 - (float)((float)((float)(rot.z * rot.z) + (float)(rot.x * rot.x)) * 2.0);
        vv._33 = 1.0 - (float)((float)((float)(rot.y * rot.y) + (float)(rot.x * rot.x)) * 2.0);
        vv._32 = (float)((float)(rot.z * rot.y) - (float)(rot.w * rot.x)) * 2.0;
        vv._13 = (float)((float)(rot.z * rot.x) - (float)(rot.w * rot.y)) * 2.0;
        vv._23 = (float)((float)(rot.w * rot.x) + (float)(rot.z * rot.y)) * 2.0;
        vv._14 = 0.0;
        vv._24 = 0.0;
        memset(&vv.m[2][3], 0, 16);
        vv._44 = 1.0;

        auto x = (vv._31 + vv._11) * 0.0 + vv._21;
        auto y = (vv._32 + vv._12) * 0.0 + vv._22;
        auto z = (vv._33 + vv._13) * 0.0 + vv._23;

        dJointSetHinge2Axis1(this->m_jointID, x, y, z);

        auto v6 = rot.z;
        vv._11 = 1.0 - (float)((float)((float)(v6 * v6) + (float)(rot.y * rot.y)) * 2.0);
        vv._21 = (float)((float)(rot.x * rot.y) - (float)(rot.w * rot.z)) * 2.0;
        vv._31 = (float)((float)(rot.w * rot.y) + (float)(rot.z * rot.x)) * 2.0;
        vv._12 = (float)((float)(rot.w * rot.z) + (float)(rot.x * rot.y)) * 2.0;
        vv._22 = 1.0 - (float)((float)((float)(v6 * v6) + (float)(rot.x * rot.x)) * 2.0);
        vv._33 = 1.0 - (float)((float)((float)(rot.y * rot.y) + (float)(rot.x * rot.x)) * 2.0);
        vv._32 = (float)((float)(rot.z * rot.y) - (float)(rot.w * rot.x)) * 2.0;
        vv._13 = (float)((float)(rot.z * rot.x) - (float)(rot.w * rot.y)) * 2.0;
        vv._23 = (float)((float)(rot.w * rot.x) + (float)(rot.z * rot.y)) * 2.0;
        vv._14 = 0.0;
        vv._24 = 0.0;
        memset(&vv.m[2][3], 0, 16);
        vv._44 = 1.0;

        auto xa = vv._31 * ai::Wheel::AXIS_FOR_WHEEL.z
            + vv._21 * ai::Wheel::AXIS_FOR_WHEEL.y
            + vv._11 * ai::Wheel::AXIS_FOR_WHEEL.x;
        auto ya = vv._32 * ai::Wheel::AXIS_FOR_WHEEL.z
            + vv._22 * ai::Wheel::AXIS_FOR_WHEEL.y
            + vv._12 * ai::Wheel::AXIS_FOR_WHEEL.x;
        auto za = vv._33 * ai::Wheel::AXIS_FOR_WHEEL.z
            + vv._23 * ai::Wheel::AXIS_FOR_WHEEL.y
            + vv._13 * ai::Wheel::AXIS_FOR_WHEEL.x;

        dJointSetHinge2Axis2(this->m_jointID, xa, ya, za);
        dJointSetHinge2Anchor(this->m_jointID, anchorPos.x, anchorPos.y, anchorPos.z);
        dJointSetHinge2Param(this->m_jointID, 10, protoInfo->m_suspensionCFM);
        dJointSetHinge2Param(this->m_jointID, 9, protoInfo->m_suspensionERP);
        dJointSetHinge2Param(this->m_jointID, 3, 1000000.0);

        if (this->m_steering)
        {
            dJointSetHinge2Param(m_jointID, 0, -3.1415927);
            dJointSetHinge2Param(this->m_jointID, 1, 3.1415927);
        }
        else
        {
            dJointSetHinge2Param(m_jointID, 0, 0.0);
            dJointSetHinge2Param(this->m_jointID, 1, 0.0);
        }
        return 1;

    }

    float Wheel::GetWidth() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector Wheel::GetDirection() const
    {
        // TODO: generated code
        // Get the inverse of the wheel's initial rotation
        Quaternion initialRotInv = m_initialRotation.getInversed();

        // Get the wheel's current rotation
        Quaternion currentRot = GetRotation();

        // Calculate the relative rotation: currentRot * initialRotInv
        // This gives the rotation from initial orientation to current orientation
        Quaternion relativeRot;
        relativeRot.x = (currentRot.x * initialRotInv.w) +
            (currentRot.y * initialRotInv.z) +
            (currentRot.w * initialRotInv.x) -
            (currentRot.z * initialRotInv.y);

        relativeRot.y = (initialRotInv.x * currentRot.z) +
            (currentRot.y * initialRotInv.w) +
            (currentRot.w * initialRotInv.y) -
            (currentRot.x * initialRotInv.z);

        relativeRot.z = (currentRot.w * initialRotInv.z) +
            (currentRot.z * initialRotInv.w) +
            (currentRot.x * initialRotInv.y) -
            (initialRotInv.x * currentRot.y);

        relativeRot.w = (currentRot.w * initialRotInv.w) -
            (currentRot.x * initialRotInv.x) -
            (currentRot.y * initialRotInv.y) -
            (currentRot.z * initialRotInv.z);

        // Convert the relative rotation quaternion to a rotation matrix
        float x = relativeRot.x;
        float y = relativeRot.y;
        float z = relativeRot.z;
        float w = relativeRot.w;

        // Precompute squared components for matrix calculation
        float x2 = x * x;
        float y2 = y * y;
        float z2 = z * z;
        float xy = x * y;
        float xz = x * z;
        float yz = y * z;
        float wx = w * x;
        float wy = w * y;
        float wz = w * z;

        // Build rotation matrix from quaternion
        CMatrix rotationMatrix;
        rotationMatrix._11 = 1.0f - 2.0f * (y2 + z2);
        rotationMatrix._12 = 2.0f * (xy + wz);
        rotationMatrix._13 = 2.0f * (xz - wy);
        rotationMatrix._14 = 0.0f;

        rotationMatrix._21 = 2.0f * (xy - wz);
        rotationMatrix._22 = 1.0f - 2.0f * (x2 + z2);
        rotationMatrix._23 = 2.0f * (yz + wx);
        rotationMatrix._24 = 0.0f;

        rotationMatrix._31 = 2.0f * (xz + wy);
        rotationMatrix._32 = 2.0f * (yz - wx);
        rotationMatrix._33 = 1.0f - 2.0f * (x2 + y2);
        rotationMatrix._34 = 0.0f;

        rotationMatrix._41 = 0.0f;
        rotationMatrix._42 = 0.0f;
        rotationMatrix._43 = 0.0f;
        rotationMatrix._44 = 1.0f;

        // Transform the wheel's forward axis by the rotation matrix
        // This gives the current direction vector in world space
        CVector wheelForwardAxis = ai::Wheel::AXIS_FOR_WHEEL; // Typically (1, 0, 0) or (0, 0, -1) depending on coordinate system

        CVector result;
        result.x = wheelForwardAxis.x * rotationMatrix._11 +
            wheelForwardAxis.y * rotationMatrix._21 +
            wheelForwardAxis.z * rotationMatrix._31;

        result.y = wheelForwardAxis.x * rotationMatrix._12 +
            wheelForwardAxis.y * rotationMatrix._22 +
            wheelForwardAxis.z * rotationMatrix._32;

        result.z = wheelForwardAxis.x * rotationMatrix._13 +
            wheelForwardAxis.y * rotationMatrix._23 +
            wheelForwardAxis.z * rotationMatrix._33;

        return result;
    }

    void Wheel::CreateSuspensionNode()
    {
        const auto* protoInfo = GetPrototypeInfo();
        if (!protoInfo->m_suspensionModelName.empty())
        {
            CVector scale;
            scale.x = 1.0;
            scale.y = 1.0;
            scale.z = 1.0;

            m_suspensionNode = ai::PhysicBody::CreateNode(protoInfo->m_suspensionModelName, 0, scale, 0, 0);

            int mac = 1;
            m_suspensionNode->SetProperty(8716u, &mac);
        }
        else
        {
            M3D_LOG_INFO("Suspension model name not specified");
            M3D_ASSERT(0);
        }
    }

    void Wheel::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Wheel::CanChildBeAdded(m3d::Class*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Vehicle* Wheel::GetVehicle() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Wheel::SetInitialRotation(Quaternion const& rot)
    {
        m_initialRotation = rot;
    }

    void Wheel::RenderDebugInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Wheel::Remove()
    {
        if (this->GetParentId() == -1)
        {
            ai::SimplePhysicObj::Remove();
        }
        else
        {
            M3D_LOG_INFO("Warning: attampt to remove " + GetDebugDescription() + ": it's attached to vehicle.");
        }
    }

    void Wheel::Update(float elapsedTime, unsigned workTime)
    {
        SimplePhysicObj::Update(elapsedTime, workTime);

        auto angualarVel = dBodyGetAngularVel(m_body->id());
        if (!m_MakeSplash && m_SplashEffect)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }
        m_MakeSplash = 0;
    }

    void Wheel::HealModel()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Wheel::DetachFromPhysicObj()
    {
        if (m_jointID)
        {
            dJointDestroy(m_jointID);
            this->m_jointID = 0;
            SetParentInvalid();
        }
    }

    void Wheel::SetPassedToAnotherMapStatus()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Wheel::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Wheel::UnlinkGeomsFromCollisionCells()
    {
        SimplePhysicObj::UnlinkGeomsFromCollisionCells();
    }

    m3d::Class* Wheel::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SimplePhysicObj);
    }

    float Wheel::GetRadius() const
    {
        auto* sphere = RT_DYNCAST(m_physicBody->m_pGeoms.front()->GetGeom(), Sphere);
        return sphere->GetRadius();
    }

    void Wheel::_InternalCreateVisualPart()
    {
        ai::SimplePhysicObj::_InternalCreateVisualPart();
        if (this->m_bModelBroken)
            ai::Wheel::BreakModel();
    }

    Wheel::~Wheel()
    {
        if (m_jointID)
        {
            dJointDestroy(m_jointID);
            m_jointID = nullptr;
        }
        if (m_SplashEffect)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }
    }

    m3d::Object* Wheel::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Wheel::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}
