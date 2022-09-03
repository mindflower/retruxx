#include "gun.h"

#include <stdexcept>

#include "math/matrix.h"

RT_CLASS_EXPORT_METHOD_DEFINE(Gun, GetShellsInCurrentCharge)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Gun, SetShellsInCurrentCharge)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Gun, GetShellsInPool)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Gun, SetShellsInPool)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Gun, GetChargeState)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Gun, SetChargeState)
{
    throw std::logic_error("Not implemented");
}

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Gun)
        RT_CLASS_EXPORT(Gun, m3d::METHOD, GetShellsInCurrentCharge, "", "", "")
        RT_CLASS_EXPORT(Gun, m3d::METHOD, SetShellsInCurrentCharge, "", "", "")
        RT_CLASS_EXPORT(Gun, m3d::METHOD, GetShellsInPool, "", "", "")
        RT_CLASS_EXPORT(Gun, m3d::METHOD, SetShellsInPool, "", "", "")
        RT_CLASS_EXPORT(Gun, m3d::METHOD, GetChargeState, "", "", "")
        RT_CLASS_EXPORT(Gun, m3d::METHOD, SetChargeState, "", "", "")
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Gun);

    void GunPrototypeInfo::RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    CStr GunPrototypeInfo::DamageType2Str(DamageType)
    {
        throw std::logic_error("Not implemented");
    }

    GunPrototypeInfo::GunPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    void GunPrototypeInfo::CreateBlastWave(CVector const&, int) const
    {
        throw std::logic_error("Not implemented");
    }

    CStr GunPrototypeInfo::FiringType2Str(FiringTypes)
    {
        throw std::logic_error("Not implemented");
    }

    FiringTypes GunPrototypeInfo::Str2FiringType(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    DamageType GunPrototypeInfo::Str2DamageType(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    bool GunPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    float GunPrototypeInfo::GetDamageForOneShell() const
    {
        throw std::logic_error("Not implemented");
    }

    void GunPrototypeInfo::PostLoad()
    {
        throw std::logic_error("Not implemented");
    }

    short GunPrototypeInfo::GetExplosionType() const
    {
        throw std::logic_error("Not implemented");
    }

    bool GunPrototypeInfo::_bIsRapidFiring() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::SgNode* Gun::GetBarrelNode() const
    {
        throw std::logic_error("Not implemented");
    }

    bool Gun::Fire(bool)
    {
        throw std::logic_error("Not implemented");
    }

    GunPrototypeInfo const* Gun::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    void Gun::SetShellsInPool(unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    void Gun::RenderGunDebugInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    float Gun::EstimateDamage(CVector const&, std::vector<int, std::allocator<int>> const&) const
    {
        throw std::logic_error("Not implemented");
    }

    float Gun::EstimateDamage() const
    {
        throw std::logic_error("Not implemented");
    }

    bool Gun::bIs360DegreesHoriz() const
    {
        throw std::logic_error("Not implemented");
    }

    CStr Gun::GetPropertyName(int) const
    {
        throw std::logic_error("Not implemented");
    }

    bool Gun::CanLookAtTarget() const
    {
        throw std::logic_error("Not implemented");
    }

    float Gun::GetRechargingTime() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* Gun::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    bool Gun::isLookAtPoint(CVector const&, float) const
    {
        throw std::logic_error("Not implemented");
    }

    void Gun::SetInvisible()
    {
        throw std::logic_error("Not implemented");
    }

    void Gun::GetPropertiesNames(std::set<CStr, std::less<CStr>, std::allocator<CStr>>&) const
    {
        throw std::logic_error("Not implemented");
    }

    void Gun::Update(float, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    float Gun::GetDamageForOneShell() const
    {
        throw std::logic_error("Not implemented");
    }

    bool Gun::CanShotToTarget(int) const
    {
        throw std::logic_error("Not implemented");
    }

    void Gun::GetPropertiesIDs(std::set<int, std::less<int>, std::allocator<int>>&) const
    {
        throw std::logic_error("Not implemented");
    }

    bool Gun::PointIsReachableFromPosition(CVector const&, CVector const&, std::vector<int, std::allocator<int>>) const
    {
        throw std::logic_error("Not implemented");
    }

    void Gun::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    unsigned Gun::GetShellsInPool() const
    {
        throw std::logic_error("Not implemented");
    }

    bool Gun::IsWithCharging() const
    {
        throw std::logic_error("Not implemented");
    }

    float Gun::GetFiringRate() const
    {
        throw std::logic_error("Not implemented");
    }

    CMatrix Gun::GetMatrixForShot(unsigned) const
    {
        throw std::logic_error("Not implemented");
    }

    float Gun::GetCurrentRechargingTime() const
    {
        throw std::logic_error("Not implemented");
    }

    unsigned Gun::GetShellsPoolSize() const
    {
        throw std::logic_error("Not implemented");
    }

    int Gun::OnEvent(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Gun::SetTargetId(int)
    {
        throw std::logic_error("Not implemented");
    }

    unsigned Gun::GetChargeSize() const
    {
        throw std::logic_error("Not implemented");
    }

    float Gun::GetFiringRange() const
    {
        throw std::logic_error("Not implemented");
    }

    float Gun::EstimateDamageFromPosition(CVector const&, CVector const&, std::vector<int, std::allocator<int>> const&)
    {
        throw std::logic_error("Not implemented");
    }

    bool Gun::PointIsReachable(CVector const&, std::vector<int, std::allocator<int>>) const
    {
        throw std::logic_error("Not implemented");
    }

    unsigned Gun::GetShellsInCurrentCharge() const
    {
        throw std::logic_error("Not implemented");
    }

    void Gun::LookAtPoint(CVector const&, float)
    {
        throw std::logic_error("Not implemented");
    }

    void Gun::Recharge()
    {
        throw std::logic_error("Not implemented");
    }

    int Gun::GetPropertyId(char const*) const
    {
        throw std::logic_error("Not implemented");
    }

    eGObjPropertySaveStatus Gun::GetPropertySaveStatus(int) const
    {
        throw std::logic_error("Not implemented");
    }

    void Gun::Registration()
    {
        throw std::logic_error("Not implemented");
    }

    void Gun::SetPassedToAnotherMapStatus()
    {
        throw std::logic_error("Not implemented");
    }

    float Gun::GetDamage() const
    {
        throw std::logic_error("Not implemented");
    }

    void Gun::SetInitialHorizAngle(float)
    {
        throw std::logic_error("Not implemented");
    }

    int Gun::GetShellPrototypeId() const
    {
        throw std::logic_error("Not implemented");
    }

    Gun::ChargeState Gun::GetChargeState() const
    {
        throw std::logic_error("Not implemented");
    }

    float Gun::GetTurningSpeed() const
    {
        throw std::logic_error("Not implemented");
    }

    bool Gun::SetPropertyById(int, m3d::AIParam const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Gun::SetShellsInCurrentCharge(unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    DamageType Gun::GetDamageType() const
    {
        throw std::logic_error("Not implemented");
    }

    bool Gun::IsDurabilityEnoughForFiring() const
    {
        throw std::logic_error("Not implemented");
    }

    bool Gun::IsWithShellsPoolLimit() const
    {
        throw std::logic_error("Not implemented");
    }

    void Gun::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    bool Gun::CanFire() const
    {
        throw std::logic_error("Not implemented");
    }

    unsigned Gun::GetPrice(IPriceCoeffProvider const*) const
    {
        throw std::logic_error("Not implemented");
    }

    Gun::Gun(GunPrototypeInfo const& prototype) : VehiclePart(prototype)
    {
        throw std::logic_error("Not implemented");
    }

    float Gun::GetInitialHorizAngle() const
    {
        throw std::logic_error("Not implemented");
    }

    void Gun::SetChargeState(int)
    {
        throw std::logic_error("Not implemented");
    }

    void Gun::SetChargeState(ChargeState)
    {
        throw std::logic_error("Not implemented");
    }

    void Gun::SetHorizontalStopAngles(float, float)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* Gun::GetBaseClass()
    {
        return RT_CLASS_LOCAL(VehiclePart);
    }

    void Gun::DoRecoil()
    {
        throw std::logic_error("Not implemented");
    }

    void Gun::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        throw std::logic_error("Not implemented");
    }

    bool Gun::_DoFire()
    {
        throw std::logic_error("Not implemented");
    }

    bool Gun::_IsDirVerticallyReachable(CVector const&) const
    {
        throw std::logic_error("Not implemented");
    }

    bool Gun::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        throw std::logic_error("Not implemented");
    }

    void Gun::_InternalCreateVisualPart()
    {
        throw std::logic_error("Not implemented");
    }

    bool Gun::_bIsUsingVolley() const
    {
        throw std::logic_error("Not implemented");
    }

    unsigned Gun::GetBarrelsNum()
    {
        throw std::logic_error("Not implemented");
    }

    void Gun::_LaunchShells()
    {
        throw std::logic_error("Not implemented");
    }

    CVector Gun::_CalcDirForNextShot() const
    {
        throw std::logic_error("Not implemented");
    }

    void Gun::_InternalPostLoad()
    {
        throw std::logic_error("Not implemented");
    }

    CVector Gun::_CalcRoughPosForNextShot() const
    {
        throw std::logic_error("Not implemented");
    }

    bool Gun::_bIsVolleyFiring() const
    {
        throw std::logic_error("Not implemented");
    }

    Gun::~Gun()
    {
        throw std::logic_error("Not implemented");
    }

    CVector Gun::_CalcPosForNextShot() const
    {
        throw std::logic_error("Not implemented");
    }

    bool Gun::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        throw std::logic_error("Not implemented");
    }

    void Gun::_CreateBarrelNode()
    {
        throw std::logic_error("Not implemented");
    }

    void Gun::_OnCinematic(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* Gun::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    void Gun::_GetCurrentOffsetAngles(float&, float&) const
    {
        throw std::logic_error("Not implemented");
    }

    CMatrix Gun::_CalcMatrixForNextShot() const
    {
        throw std::logic_error("Not implemented");
    }

    void Gun::BeginReCharge()
    {
        throw std::logic_error("Not implemented");
    }

    unsigned Gun::getShellsForRecharge() const
    {
        throw std::logic_error("Not implemented");
    }

    void Gun::_GetOffsetAngles(CVector const&, float, float&, float&)
    {
        throw std::logic_error("Not implemented");
    }

    bool Gun::_bIsRapidFiring() const
    {
        throw std::logic_error("Not implemented");
    }

    void Gun::_UpdateNodeFiringAction()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* Gun::Clone()
    {
        throw std::logic_error("Not implemented");
    }
}
