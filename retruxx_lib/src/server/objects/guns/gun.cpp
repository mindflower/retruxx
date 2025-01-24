#include "gun.h"

#include <stdexcept>

#include "math/matrix.h"

RT_CLASS_EXPORT_METHOD_DEFINE(Gun, GetShellsInCurrentCharge)
{
    throw retruxx::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Gun, SetShellsInCurrentCharge)
{
    throw retruxx::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Gun, GetShellsInPool)
{
    throw retruxx::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Gun, SetShellsInPool)
{
    throw retruxx::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Gun, GetChargeState)
{
    throw retruxx::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Gun, SetChargeState)
{
    throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    CStr GunPrototypeInfo::DamageType2Str(DamageType)
    {
        throw retruxx::logic_error("Not implemented");
    }

    GunPrototypeInfo::GunPrototypeInfo()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void GunPrototypeInfo::CreateBlastWave(CVector const&, int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    CStr GunPrototypeInfo::FiringType2Str(FiringTypes)
    {
        throw retruxx::logic_error("Not implemented");
    }

    FiringTypes GunPrototypeInfo::Str2FiringType(CStr const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    DamageType GunPrototypeInfo::Str2DamageType(CStr const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool GunPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    float GunPrototypeInfo::GetDamageForOneShell() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void GunPrototypeInfo::PostLoad()
    {
        throw retruxx::logic_error("Not implemented");
    }

    short GunPrototypeInfo::GetExplosionType() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool GunPrototypeInfo::_bIsRapidFiring() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::SgNode* Gun::GetBarrelNode() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Gun::Fire(bool)
    {
        throw retruxx::logic_error("Not implemented");
    }

    GunPrototypeInfo const* Gun::GetPrototypeInfo() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Gun::SetShellsInPool(unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Gun::RenderGunDebugInfo() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    float Gun::EstimateDamage(CVector const&, retruxx::vector<int, retruxx::allocator<int>> const&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    float Gun::EstimateDamage() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Gun::bIs360DegreesHoriz() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    CStr Gun::GetPropertyName(int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Gun::CanLookAtTarget() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    float Gun::GetRechargingTime() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Class* Gun::GetClass() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Gun::isLookAtPoint(CVector const&, float) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Gun::SetInvisible()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Gun::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Gun::Update(float, unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }

    float Gun::GetDamageForOneShell() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Gun::CanShotToTarget(int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Gun::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Gun::PointIsReachableFromPosition(CVector const&, CVector const&, retruxx::vector<int, retruxx::allocator<int>>) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Gun::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    unsigned Gun::GetShellsInPool() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Gun::IsWithCharging() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    float Gun::GetFiringRate() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    CMatrix Gun::GetMatrixForShot(unsigned) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    float Gun::GetCurrentRechargingTime() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    unsigned Gun::GetShellsPoolSize() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Gun::OnEvent(Event const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Gun::SetTargetId(int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    unsigned Gun::GetChargeSize() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    float Gun::GetFiringRange() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    float Gun::EstimateDamageFromPosition(CVector const&, CVector const&, retruxx::vector<int, retruxx::allocator<int>> const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Gun::PointIsReachable(CVector const&, retruxx::vector<int, retruxx::allocator<int>>) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    unsigned Gun::GetShellsInCurrentCharge() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Gun::LookAtPoint(CVector const&, float)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Gun::Recharge()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Gun::GetPropertyId(char const*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    eGObjPropertySaveStatus Gun::GetPropertySaveStatus(int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Gun::Registration()
    {
        m_propertiesMap["Damage"] = 28;
        m_propertiesMap["FiringRate"] = 29;
        m_propertiesMap["FiringRange"] = 30;
        m_propertiesMap["ChargeSize"] = 34;
        m_propertiesMap["ReChargingTime"] = 35;
        m_propertiesMap["ShellsInPool"] = 36;
    }

    void Gun::SetPassedToAnotherMapStatus()
    {
        throw retruxx::logic_error("Not implemented");
    }

    float Gun::GetDamage() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Gun::SetInitialHorizAngle(float)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Gun::GetShellPrototypeId() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    Gun::ChargeState Gun::GetChargeState() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    float Gun::GetTurningSpeed() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Gun::SetPropertyById(int, m3d::AIParam const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Gun::SetShellsInCurrentCharge(unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }

    DamageType Gun::GetDamageType() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Gun::IsDurabilityEnoughForFiring() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Gun::IsWithShellsPoolLimit() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Gun::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Gun::CanFire() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    unsigned Gun::GetPrice(IPriceCoeffProvider const*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    Gun::Gun(GunPrototypeInfo const& prototype) : VehiclePart(prototype)
    {
        throw retruxx::logic_error("Not implemented");
    }

    float Gun::GetInitialHorizAngle() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Gun::SetChargeState(int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Gun::SetChargeState(ChargeState)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Gun::SetHorizontalStopAngles(float, float)
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Class* Gun::GetBaseClass()
    {
        return RT_CLASS_LOCAL(VehiclePart);
    }

    void Gun::DoRecoil()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Gun::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Gun::_DoFire()
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Gun::_IsDirVerticallyReachable(CVector const&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Gun::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Gun::_InternalCreateVisualPart()
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Gun::_bIsUsingVolley() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    unsigned Gun::GetBarrelsNum()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Gun::_LaunchShells()
    {
        throw retruxx::logic_error("Not implemented");
    }

    CVector Gun::_CalcDirForNextShot() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Gun::_InternalPostLoad()
    {
        throw retruxx::logic_error("Not implemented");
    }

    CVector Gun::_CalcRoughPosForNextShot() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Gun::_bIsVolleyFiring() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    Gun::~Gun()
    {
        throw retruxx::logic_error("Not implemented");
    }

    CVector Gun::_CalcPosForNextShot() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Gun::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Gun::_CreateBarrelNode()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Gun::_OnCinematic(Event const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Object* Gun::CreateObject()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Gun::_GetCurrentOffsetAngles(float&, float&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    CMatrix Gun::_CalcMatrixForNextShot() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Gun::BeginReCharge()
    {
        throw retruxx::logic_error("Not implemented");
    }

    unsigned Gun::getShellsForRecharge() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Gun::_GetOffsetAngles(CVector const&, float, float&, float&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Gun::_bIsRapidFiring() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Gun::_UpdateNodeFiringAction()
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Object* Gun::Clone()
    {
        throw retruxx::logic_error("Not implemented");
    }
}
