#include "weaponslotwnd.h"
#include "weapongroupbuttonlist.h"

#include <i_event.h>
#include <game/m3dgame.h>
#include <ui/image.h>
#include <server/resourcemanager.h>
#include <server/objects/vehicle.h>
#include <server/objects/player.h>
#include <server/objects/base/objcontainer.h>

RT_CLASS_EXPORTS_BEGIN(WeaponSlotWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(WeaponSlotWnd);

WeaponSlotWnd::AuxInfo::AuxInfo()
{
    m_wndPatternName = "wndWeaponSlot";
    m_wndPatternGunPartIcoName = "wndGunPartIco";
    m_wndPatternGunNameName = "wndGunName";
    m_wndPatternGunIcoName = "wndGunIco";
    m_wndPatternInactiveBgName = "wndInactiveBg";
    m_texIdGunPart = "GunSlot_";
    m_texIdGunPartNone = "GunSlot_None";
    m_strPadding = "---";
    m_strIdGunNone = "NotInstalled";
    m_colorGunNone = static_cast<unsigned int>(-7323584);
}

WeaponSlotWnd::Pattern::Pattern()
{
    m_wndPattern = nullptr;
    m_wndPatternGunPartIco = nullptr;
    m_wndPatternGunName = nullptr;
    m_wndPatternGunIco = nullptr;
    m_wndPatternWeaponGroups = nullptr;
    m_wndPatternInactiveBg = nullptr;
    m_ref = 0;
}

WeaponSlotWnd::Pattern::~Pattern()
{
    Clear();
}

void WeaponSlotWnd::Pattern::Clear()
{
    if (m_wndPattern)
    {
        m_wndPattern->DecRef();
    }
    m_wndPattern = nullptr;
    if (m_wndPatternGunPartIco)
    {
        m_wndPatternGunPartIco->DecRef();
    }
    m_wndPatternGunPartIco = nullptr;
    if (m_wndPatternGunName)
    {
        m_wndPatternGunName->DecRef();
    }
    m_wndPatternGunName = nullptr;
    if (m_wndPatternGunIco)
    {
        m_wndPatternGunIco->DecRef();
    }
    m_wndPatternGunIco = nullptr;
    if (m_wndPatternWeaponGroups)
    {
        m_wndPatternWeaponGroups->DecRef();
    }
    m_wndPatternWeaponGroups = nullptr;
    if (m_wndPatternInactiveBg)
    {
        m_wndPatternInactiveBg->DecRef();
    }
    m_wndPatternInactiveBg = nullptr;
}

void WeaponSlotWnd::Pattern::IncRef()
{
    ++m_ref;
}

void WeaponSlotWnd::Pattern::DecRef()
{
    if (--m_ref <= 0)
    {
        Clear();
    }
}

int WeaponSlotWnd::Pattern::GetRef() const
{
    return m_ref;
}

CStr const& WeaponSlotWnd::GetGunPartName() const
{
    return m_gunPartName;
}

m3d::Class* WeaponSlotWnd::GetClass() const
{
    return RT_CLASS_LOCAL(WeaponSlotWnd);
}

m3d::Object* WeaponSlotWnd::CreateObject()
{
    return new WeaponSlotWnd;
}

m3d::Object* WeaponSlotWnd::Clone()
{
    return new WeaponSlotWnd(*this);
}

int WeaponSlotWnd::SetupForGunPart(CStr const&, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

WeaponSlotWnd::~WeaponSlotWnd()
{
    if (m_wndWeaponGroups)
    {
        M3D_APP->m_pInterfaceManager->RemoveWindow(m_wndWeaponGroups->GetGuiId());
    }
    // m_wndWeaponGroups (ref_ptr) and m_gunPartName (CStr) release themselves.
}

m3d::Class* WeaponSlotWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

int WeaponSlotWnd::GameDataUpdate(void* data, int dataType)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (dataType == 65)
    {
        OnVehiclePartChanged(data);
    }
    return 1;
}

bool WeaponSlotWnd::IsSetForGunPart() const
{
    return !m_gunPartName.empty() && m_vehicleId != -1;
}

int WeaponSlotWnd::CreateFromPattern()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool WeaponSlotWnd::NeedShowWeaponGroups() const
{
    if (m_gunPartName.empty() || m_vehicleId == -1)
    {
        return false;
    }
    ai::Vehicle const* playerVehicle = ai::thePlayer ? ai::thePlayer->GetVehicle() : nullptr;
    return GetVehicle() == playerVehicle;
}

void WeaponSlotWnd::UpdateActiveState()
{
    if ((m_gameDataFlags & 1) != 0 && m_wndInactiveBg)
    {
        m_wndInactiveBg->ShowWindow(!IsSetForGunPart());
    }
}

void WeaponSlotWnd::UpdateGunName(ai::VehiclePart const*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponSlotWnd::OnVehiclePartChanged(void* data)
{
    if (!IsChildOf(m3d::Application::g_pApp))
    {
        return;
    }
    auto const* evt = static_cast<m3d::Event const*>(data);
    if (evt && evt->m_intEv[0] == m_vehicleId && CStr::my_strcmp(evt->m_strEv.c_str(), m_gunPartName.c_str()) == 0)
    {
        UpdateOnGunChanged();
    }
}

int WeaponSlotWnd::CreateChildren()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponSlotWnd::UpdateGunPartIco(ai::VehiclePart const* gun)
{
    if ((m_gameDataFlags & 1) != 0)
    {
        m_wndGunPartIco->SetImage(GetIcoByGunPartName(m_gunPartName, gun != nullptr));
    }
}

void WeaponSlotWnd::UpdateOnGunChanged()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Vehicle const* WeaponSlotWnd::GetVehicle() const
{
    if (m_vehicleId == -1)
    {
        return nullptr;
    }
    ai::Obj* obj = ai::theObjects->GetEntityByObjId(m_vehicleId);
    if (obj && obj->IsKindOf(&ai::Vehicle::m_classVehicle))
    {
        return static_cast<ai::Vehicle const*>(obj);
    }
    return nullptr;
}

int WeaponSlotWnd::LoadPattern(m3d::ui::Wnd*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponSlotWnd::UpdateOnGunPartChanged()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::VehiclePart const* WeaponSlotWnd::GetGun() const
{
    if (m_gunPartName.empty() || m_vehicleId == -1)
    {
        return nullptr;
    }
    ai::Vehicle const* vehicle = GetVehicle();
    if (!vehicle)
    {
        return nullptr;
    }
    return vehicle->GetPartByName(m_gunPartName);
}

void WeaponSlotWnd::UpdateGunIco(ai::VehiclePart const*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

WeaponSlotWnd::WeaponSlotWnd(WeaponSlotWnd const&) : WeaponSlotWnd()
{
}

WeaponSlotWnd::WeaponSlotWnd()
{
    m_vehicleId = -1;
    m_wndGunPartIco = nullptr;
    m_wndGunName = nullptr;
    m_wndGunIco = nullptr;
    m_wndInactiveBg = nullptr;
}

m3d::rend::TexHandle WeaponSlotWnd::GetIcoByGunPartName(CStr const& gunPartName, bool bGunInstalled) const
{
    if (gunPartName.empty())
    {
        return M3D_APP->m_pInterfaceManager->GetIcoByName(m_aif.m_texIdGunPartNone, 0);
    }

    CStr const resourceName = ai::theResourceManager->GetResourceNameByVehiclePartName(gunPartName);
    if (resourceName.empty())
    {
        return m3d::rend::TexHandle{};
    }

    ai::Resource* resource = ai::theResourceManager->GetResource(ai::theResourceManager->GetResourceId(resourceName));
    if (!resource)
    {
        return m3d::rend::TexHandle{};
    }

    PointBase<int> const geomSize = resource->GetGeomSize();
    CStr const texName = m_aif.m_texIdGunPart + CStr(geomSize.x) + CStr("x") + CStr(geomSize.y);
    return M3D_APP->m_pInterfaceManager->GetIcoByName(texName, bGunInstalled ? 0 : 1);
}

void WeaponSlotWnd::ClearWeaponGroups()
{
    if (m_wndWeaponGroups)
    {
        M3D_APP->m_pInterfaceManager->RemoveWindow(m_wndWeaponGroups->GetGuiId());
    }
}
