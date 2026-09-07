#include "cbwnd.h"
#include "izvratrepositorywnd.h"
#include "vehiclepartwnd.h"
#include "core/kernel.h"
#include "core/log.h"
#include <game/m3dgame.h>
#include <game/uimanager/truxxuimanager.h>
#include <game/uimanager/uidefs.h>
#include <i_event.h>
#include <ui/image.h>
#include <server/objects/base/objcontainer.h>
#include <server/objects/physicbodies/vehiclepart.h>
#include <server/resourcemanager.h>

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
    // Empty in the shipped game (RVA 0x4408C0): the base class has no child
    // parts of its own. BasketWnd and CabinWnd override this and do drive
    // CreateChildVehiclePartWindow.
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

int CBWnd::CreateChildVehiclePartWindow(CStr const& partName, PointBase<float> const& origin)
{
    // RVA 0x440990 - builds the little VehiclePartWnd that sits on top of the
    // cabin/basket picture for one of its child parts (a gun, a cargo slot).
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }

    int const resourceId =
        ai::theResourceManager->GetResourceId(ai::theResourceManager->GetResourceNameByVehiclePartName(partName));
    if (resourceId == -1)
    {
        return 0;
    }

    auto* wnd = static_cast<VehiclePartWnd*>(M3D_KERNEL->New("VehiclePartWnd"));
    if (!wnd)
    {
        return 0;
    }

    // The window is sized in repository cells, so the cell size comes from the
    // player's inventory window rather than from anything local.
    PointBase<int> const geomSize = ai::theResourceManager->GetResource(resourceId)->GetGeomSize();

    ref_ptr<m3d::ui::Wnd> const wndPlayerRepository =
        M3D_APP->m_pInterfaceManager->GetWindow(IW_WND_PLAYERVEHICLE_INVENTORY);

    PointBase<float> cellSz{0.0f, 0.0f};
    if (wndPlayerRepository && wndPlayerRepository->IsKindOf(&IzvratRepositoryWnd::m_classIzvratRepositoryWnd))
    {
        cellSz = static_cast<RepositoryWnd const*>(wndPlayerRepository.get())->GetCellSize();
    }

    BoundsBase<float> b;
    b.x0 = origin.x;
    b.y0 = origin.y;
    b.width = static_cast<float>(geomSize.x) * cellSz.x;
    b.height = static_cast<float>(geomSize.y) * cellSz.y;

    if (!wnd->Create(CStr(), 0x200u, b, 0))
    {
        wnd->DecRef();
        return 0;
    }

    wnd->SetPane(m_cbAif.m_vehiclePartPaneName);
    wnd->SetPaneFlags(6);
    wnd->SetVehicleId(m_vehicleId);
    wnd->SetPartName(partName);

    AddChild(wnd);
    MoveChildToFirstPosition(wnd);
    m_wndChildVehicleParts.insert(VehiclePartPair(partName, wnd));

    int guiId = -1;
    M3D_APP->m_pInterfaceManager->AddWindow(wnd, guiId, false, false);

    // The part window listens for the vehicle/repository events the character
    // screen raises while parts are moved around.
    std::vector<int> events;
    for (int event = 91; event <= 98; ++event)
    {
        events.push_back(event);
    }
    events.push_back(65);
    M3D_APP->m_pInterfaceManager->SetEventsForWindow(guiId, events);
    return 1;
}
