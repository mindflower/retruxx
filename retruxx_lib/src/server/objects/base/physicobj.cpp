#include "physicobj.h"
#include <stdexcept>
#include <core/aiparam.h>
#include <server/obstacle.h>

#include "core/ini.h"
#include "game/m3dgame.h"
#include "server/ai/aimanager.h"

#include "thirdparty/injecttools.h"

RT_CLASS_EXPORT_METHOD_DEFINE(PhysicObj, SetPosition)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(PhysicObj, GetPosition)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(PhysicObj, SetRotation)
{
    throw std::logic_error("Not implemented");
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
    throw std::logic_error("Not implemented");
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

    void PhysicObj::SetDirections(CVector const&, CVector const&)
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
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

    void PhysicObj::SetSkin(int)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* PhysicObj::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    void PhysicObj::UnlinkGeomsFromCollisionCells()
    {
        throw std::logic_error("Not implemented");
    }

    bool PhysicObj::CanPhysicsBeEnabled() const
    {
        throw std::logic_error("Not implemented");
    }

    float PhysicObj::GetMass() const
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::DisablePhysicsAndGeometry()
    {
        throw std::logic_error("Not implemented");
    }

    bool PhysicObj::SetPropertyById(int, m3d::AIParam const&)
    {
        throw std::logic_error("Not implemented");
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

    void PhysicObj::SetRotation(Quaternion const&)
    {
        throw std::logic_error("Not implemented");
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

    void PhysicObj::SetPositionSelf(CVector const&)
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
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

    void PhysicObj::DisableGeometry(bool)
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::EnableGeometry(bool)
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    dBody const* PhysicObj::GetBody() const
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::Update(float, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::SetUpdatingByODE(bool)
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::SetMassCenterPosition(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::EnablePhysics()
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::PostCollide()
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::DisablePhysics()
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::SetDirection(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    Geom::CellAabb PhysicObj::GetCollisionCellAabb() const
    {
        throw std::logic_error("Not implemented");
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

    void PhysicObj::SetPosition(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    PhysicObj::PhysicObj(PhysicObjPrototypeInfo const&) : m_intersectionObstacle(nullptr)
    {
        throw std::logic_error("Not implemented");
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

    void PhysicObj::SetRotationSelf(Quaternion const&)
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::AddRelativeRotation(Quaternion const&)
    {
        throw std::logic_error("Not implemented");
    }

    CVector PhysicObj::GetDirection() const
    {
        throw std::logic_error("Not implemented");
    }

    bool PhysicObj::GetGeomEnabledBit() const
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::LinkGeomsToCollisionCells()
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    PhysicObjPrototypeInfo const* PhysicObj::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    CVector PhysicObj::GetLinearVelocity() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::DbgCounter* PhysicObj::GetRelinksToCollisionCounter()
    {
        throw std::logic_error("Not implemented");
    }

    int PhysicObj::GetPropertyId(char const*) const
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::_SetSimpleCollision()
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::_AdjustMassCenter()
    {
        throw std::logic_error("Not implemented");
    }

    SphereForIntersection* PhysicObj::_GetLookSphere() const
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::_SetRotationToGeoms(Quaternion const&)
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::_EnableIntersections(bool)
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::_SetBoundSphereRadius(float)
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::_SetBodyEnabledBit(bool)
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::_CreateSpace(bool)
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::_SetStaticCollision()
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::_SetStatic()
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::_SetMassCenter(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    PhysicObj::~PhysicObj()
    {
    }

    void PhysicObj::_InternalPostLoad()
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicObj::_InternalCreateVisualPart()
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
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
