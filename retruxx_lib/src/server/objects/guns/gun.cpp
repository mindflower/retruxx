#include "gun.h"

#include <stdexcept>

#include "math/matrix.h"

RT_CLASS_EXPORT_METHOD_DEFINE(Gun, GetShellsInCurrentCharge)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Gun, SetShellsInCurrentCharge)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Gun, GetShellsInPool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Gun, SetShellsInPool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Gun, GetChargeState)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Gun, SetChargeState)
{
    RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr GunPrototypeInfo::DamageType2Str(DamageType)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    GunPrototypeInfo::GunPrototypeInfo()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void GunPrototypeInfo::CreateBlastWave(CVector const&, int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr GunPrototypeInfo::FiringType2Str(FiringTypes)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    FiringTypes GunPrototypeInfo::Str2FiringType(CStr const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    DamageType GunPrototypeInfo::Str2DamageType(CStr const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool GunPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float GunPrototypeInfo::GetDamageForOneShell() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void GunPrototypeInfo::PostLoad()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    short GunPrototypeInfo::GetExplosionType() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool GunPrototypeInfo::_bIsRapidFiring() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::SgNode* Gun::GetBarrelNode() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Gun::Fire(bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    GunPrototypeInfo const* Gun::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::SetShellsInPool(unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::RenderGunDebugInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Gun::EstimateDamage(CVector const&, retruxx::vector<int, retruxx::allocator<int>> const&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Gun::EstimateDamage() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Gun::bIs360DegreesHoriz() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr Gun::GetPropertyName(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Gun::CanLookAtTarget() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Gun::GetRechargingTime() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Gun::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Gun::isLookAtPoint(CVector const&, float) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::SetInvisible()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::Update(float, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Gun::GetDamageForOneShell() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Gun::CanShotToTarget(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Gun::PointIsReachableFromPosition(CVector const&, CVector const&, retruxx::vector<int, retruxx::allocator<int>>) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned Gun::GetShellsInPool() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Gun::IsWithCharging() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Gun::GetFiringRate() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CMatrix Gun::GetMatrixForShot(unsigned) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Gun::GetCurrentRechargingTime() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned Gun::GetShellsPoolSize() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Gun::OnEvent(Event const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::SetTargetId(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned Gun::GetChargeSize() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Gun::GetFiringRange() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Gun::EstimateDamageFromPosition(CVector const&, CVector const&, retruxx::vector<int, retruxx::allocator<int>> const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Gun::PointIsReachable(CVector const&, retruxx::vector<int, retruxx::allocator<int>>) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned Gun::GetShellsInCurrentCharge() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::LookAtPoint(CVector const&, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::Recharge()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Gun::GetPropertyId(char const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    eGObjPropertySaveStatus Gun::GetPropertySaveStatus(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Gun::GetDamage() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::SetInitialHorizAngle(float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Gun::GetShellPrototypeId() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Gun::ChargeState Gun::GetChargeState() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Gun::GetTurningSpeed() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Gun::SetPropertyById(int, m3d::AIParam const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::SetShellsInCurrentCharge(unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    DamageType Gun::GetDamageType() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Gun::IsDurabilityEnoughForFiring() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Gun::IsWithShellsPoolLimit() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Gun::CanFire() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned Gun::GetPrice(IPriceCoeffProvider const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Gun::Gun(GunPrototypeInfo const& prototype) : VehiclePart(prototype)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Gun::GetInitialHorizAngle() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::SetChargeState(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::SetChargeState(ChargeState)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::SetHorizontalStopAngles(float, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Gun::GetBaseClass()
    {
        return RT_CLASS_LOCAL(VehiclePart);
    }

    void Gun::DoRecoil()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Gun::_DoFire()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Gun::_IsDirVerticallyReachable(CVector const&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Gun::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::_InternalCreateVisualPart()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Gun::_bIsUsingVolley() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned Gun::GetBarrelsNum()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::_LaunchShells()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector Gun::_CalcDirForNextShot() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::_InternalPostLoad()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector Gun::_CalcRoughPosForNextShot() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Gun::_bIsVolleyFiring() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Gun::~Gun()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector Gun::_CalcPosForNextShot() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Gun::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::_CreateBarrelNode()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::_OnCinematic(Event const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Gun::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::_GetCurrentOffsetAngles(float&, float&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CMatrix Gun::_CalcMatrixForNextShot() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::BeginReCharge()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned Gun::getShellsForRecharge() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::_GetOffsetAngles(CVector const&, float, float&, float&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Gun::_bIsRapidFiring() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::_UpdateNodeFiringAction()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Gun::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}
