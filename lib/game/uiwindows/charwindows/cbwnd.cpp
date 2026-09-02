#include "cbwnd.h"
#include "vehiclepartwnd.h"
#include "core/log.h"
#include <game/m3dgame.h>
#include <i_event.h>
#include <ui/image.h>
#include <server/objects/base/objcontainer.h>
#include <server/objects/physicbodies/vehiclepart.h>

RT_CLASS_EXPORTS_BEGIN(CBWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(CBWnd);

CBWnd::CBAuxInfo::CBAuxInfo()
{
    m_vehiclePartPaneName = "PaneGun";
    m_wndHidePictureBgName = "wndHidePictureBg";
    m_wndDisabledBgName = "wndDisabledBg";
}

m3d::Class* CBWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ChildPanel);
}

m3d::Class* CBWnd::GetRtClass() const
{
    return RT_CLASS_LOCAL(CBWnd);
}

CBWnd::CBWnd()
{
    m_wndHidePictureBg = nullptr;
    m_wndDisabledBg = nullptr;
    m_vehicleType = InventoryWnd::VEHICLETYPE_INVALID;
    m_mainPartId = -1;
}

CBWnd::~CBWnd()
{
    CBWnd::SetVehicleId(-1);
    CBWnd::ClearChildVehicleParts();
    // m_wndChildVehicleParts, m_mainPartName, m_cbAif and the Wnd base are
    // destroyed by the compiler-chained destructors.
}

InventoryWnd::VehicleType CBWnd::GetVehicleTypeByGuiId(int) const
{
    return InventoryWnd::VEHICLETYPE_INVALID;
}

int CBWnd::GetPartId() const
{
    return m_mainPartId;
}

// ---------------------------------------------------------------------------

void CBWnd::SetVehicleId(int vehicleId)
{
    if ((m_gameDataFlags & 1) != 0)
    {
        ChildPanel::SetVehicleId(vehicleId);
        if (IsChildOf(M3D_APP))
        {
            FullUpdate();
        }
        if ((m_gameDataFlags & 1) != 0)
        {
            m_wndHidePictureBg->ShowWindow(vehicleId == -1);
            m_wndDisabledBg->ShowWindow(vehicleId == -1);
        }
    }
}

void CBWnd::SetPartId(int partId)
{
    if (partId != -1)
    {
        auto* obj = ai::theObjects->GetEntityByObjId(partId);
        if (!obj || !obj->IsKindOf(&ai::VehiclePart::m_classVehiclePart))
        {
            return;
        }
        // TODO(RVA 0x4407B0): the shipped build also requires the part's parent
        // part name to equal m_mainPartName. ai::VehiclePart exposes no clean
        // accessor for that in retruxx yet, so the gate is relaxed to "is a
        // VehiclePart".
    }
    m_mainPartId = partId;
    if (IsChildOf(M3D_APP))
    {
        FullUpdate();
    }
}

void CBWnd::UpdateOnMainPartChanged()
{
    SetupChildVehicleParts();
}

void CBWnd::SetupChildVehicleParts()
{
    // Empty in the shipped game (RVA 0x4408C0) - the per-part child window
    // subsystem (CreateChildVehiclePartWindow) is never actually driven.
}

void CBWnd::ClearChildVehicleParts()
{
    for (auto& entry : m_wndChildVehicleParts)
    {
        if (entry.second)
        {
            M3D_APP->m_pInterfaceManager->RemoveWindow(entry.second->GetGuiId());
        }
    }
    m_wndChildVehicleParts.clear();
}

void CBWnd::UpdateOnVehiclepartChanged(CStr const& partName)
{
    if ((m_gameDataFlags & 1) != 0 && IsChildOf(M3D_APP) &&
        !CStr::my_strcmp(partName.c_str(), m_mainPartName.c_str()))
    {
        UpdateOnMainPartChanged();
    }
}

void CBWnd::SetDisabledLook(bool bDisabled)
{
    if ((m_gameDataFlags & 1) != 0)
    {
        m_wndHidePictureBg->ShowWindow(bDisabled);
        m_wndDisabledBg->ShowWindow(bDisabled);
    }
}

void CBWnd::OnFinishTrade()
{
    if (m_vehicleType == InventoryWnd::VEHICLETYPE_WORKSHOP)
    {
        SetVehicleId(-1);
        GameDataClear(false);
    }
}

void CBWnd::FullUpdate()
{
    UpdateOnMainPartChanged();
}

int CBWnd::GameDataClear(bool)
{
    SetVehicleId(-1);
    ClearChildVehicleParts();
    return 1;
}

int CBWnd::GameDataSetup()
{
    if ((m_gameDataFlags & 2) == 0)
    {
        m_vehicleType = GetVehicleTypeByGuiId(m_guiId);
        bool ok = m_vehicleType != InventoryWnd::VEHICLETYPE_INVALID;

        auto* child = GetChildByName(m_cbAif.m_wndHidePictureBgName);
        if (child && child->IsKindOf(&m3d::ui::ImageWnd::m_classImageWnd))
        {
            m_wndHidePictureBg = static_cast<m3d::ui::ImageWnd*>(child);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_cbAif.m_wndHidePictureBgName +
                         " is not found or incorrect type");
            ok = false;
        }

        child = GetChildByName(m_cbAif.m_wndDisabledBgName);
        if (child && child->IsKindOf(&m3d::ui::ImageWnd::m_classImageWnd))
        {
            m_wndDisabledBg = static_cast<m3d::ui::ImageWnd*>(child);
            if (ok)
            {
                m_gameDataFlags |= 1u;
                m_wndHidePictureBg->ShowWindow(true);
                m_wndDisabledBg->ShowWindow(true);
            }
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_cbAif.m_wndDisabledBgName +
                         " is not found or incorrect type");
        }
    }

    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }
    M3D_LOG_INFO("CBWnd: error - fail to init because of a bad resource");
    return 0;
}

int CBWnd::GameDataUpdate(void* data, int dataType)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }

    switch (dataType)
    {
    case 32:
        // Trade part-removal notification. The child-part map is never populated
        // in this build, so there is nothing to reconcile here.
        break;
    case 34:
        OnFinishTrade();
        return 1;
    case 65:
        if (data)
        {
            const auto* evt = static_cast<const m3d::Event*>(data);
            if (evt->m_intEv[0] == GetVehicleId())
            {
                UpdateOnVehiclepartChanged(evt->m_strEv);
                return 1;
            }
        }
        break;
    default:
        break;
    }
    return 1;
}

int CBWnd::OnBeforeAddToWndStation()
{
    if ((m_gameDataFlags & 1) != 0)
    {
        FullUpdate();
    }
    return Wnd::OnBeforeAddToWndStation();
}

int CBWnd::OnAfterRemoveFromWndStation()
{
    return m3d::ui::Wnd::OnAfterRemoveFromWndStation();
}

int CBWnd::CreateChildVehiclePartWindow(CStr const&, PointBase<float> const&)
{
    // TODO(RVA 0x440990): spawns a VehiclePartWnd per child vehicle part via the
    // ResourceManager and m_pInterfaceManager. Dead code in the shipped game
    // (SetupChildVehicleParts, its only caller, is empty) and blocked on the
    // unported VehiclePartWnd; left as a stub.
    RETRUXX_NOT_IMPLEMENTED;
}
