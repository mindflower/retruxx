#include "inventorywnd.h"
#include "basketwnd.h"
#include "cabinwnd.h"
#include "game/uimanager/uidefs.h"
#include <game/m3dgame.h>
#include <core/log.h>
#include <client.h>
#include "world.h"
#include "server/objects/vehicle.h"

RT_CLASS_EXPORTS_BEGIN(InventoryWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(InventoryWnd);

InventoryWnd::AuxInfo::AuxInfo()
{
    m_wndCabinName = "wndCabin";
    m_wndBasketName = "wndBasket";
}

InventoryWnd::ChildSaveInfo::ChildSaveInfo()
{
    m_bBasketOnShowAnimationEnabled = 0;
    m_bBasketOnHideAnimationEnabled = 0;
    m_bCabinOnShowAnimationEnabled = 0;
    m_bCabinOnHideAnimationEnabled = 0;
    m_basketB.x0 = 0.0;
    m_basketB.y0 = 0.0;
    m_basketB.width = 0.0;
    m_basketB.height = 0.0;
    m_cabinB.x0 = 0.0;
    m_cabinB.y0 = 0.0;
    m_cabinB.width = 0.0;
    m_cabinB.height = 0.0;
}

m3d::Class* InventoryWnd::GetClass() const
{
    return RT_CLASS_LOCAL(InventoryWnd);
}

InventoryWnd::~InventoryWnd() = default;

void InventoryWnd::SetVehicleId(int vehicleId)
{
    if ((m_gameDataFlags & 1) != 0)
    {
        ChildPanel::SetVehicleId(vehicleId);
        m_wndCabin->SetVehicleId(vehicleId);
        m_wndBasket->SetVehicleId(vehicleId);
    }
}

m3d::Class* InventoryWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ChildPanel);
}

m3d::Object* InventoryWnd::CreateObject()
{
    return new InventoryWnd;
}

void InventoryWnd::SetTradeVehicleId(int vehicleId, ZnayuKakProdatWnd::TradeType tradeType)
{
    if ((m_gameDataFlags & 1) == 0 || m_vehicleType != VEHICLETYPE_WORKSHOP || vehicleId == -1 ||
        tradeType == ZnayuKakProdatWnd::TRADETYPE_NUM_TRADETYPES)
    {
        return;
    }

    m_tradeType = tradeType;
    ChildPanel::SetVehicleId(vehicleId);

    switch (tradeType)
    {
    case ZnayuKakProdatWnd::TRADETYPE_VEHICLE:
        m_wndCabin->SetVehicleId(vehicleId);
        m_wndBasket->SetVehicleId(vehicleId);
        break;
    case ZnayuKakProdatWnd::TRADETYPE_CABIN:
        m_wndCabin->SetVehicleId(vehicleId);
        m_wndBasket->SetVehicleId(-1);
        break;
    case ZnayuKakProdatWnd::TRADETYPE_BASKET:
        m_wndCabin->SetVehicleId(-1);
        m_wndBasket->SetVehicleId(vehicleId);
        break;
    default:
        break;
    }
}

m3d::Object* InventoryWnd::Clone()
{
    return new InventoryWnd(*this);
}

InventoryWnd::InventoryWnd()
{
    m_vehicleType = VEHICLETYPE_INVALID;
    m_tradeType = ZnayuKakProdatWnd::TRADETYPE_NUM_TRADETYPES;
}

InventoryWnd::InventoryWnd(InventoryWnd const&) : InventoryWnd()
{
}

void InventoryWnd::RemoveCBWindows()
{
    // Only execute if game data flag 1 is set
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    // Add cabin window if not already a child
    if (m_wndCabin && m_wndCabin->IsChildOf(this))
    {
        // Disable animations
        m_wndCabin->EnableOnShowAnimation(m_childSaveInfo.m_bCabinOnShowAnimationEnabled);
        m_wndCabin->EnableOnHideAnimation(m_childSaveInfo.m_bCabinOnHideAnimationEnabled);

        // Set bounds from saved info
        m_wndCabin->SetBounds(m_childSaveInfo.m_cabinB, true);

        // Add as child
        RemoveChild(m_wndCabin.get());
    }

    // Add basket window if not already a child
    if (m_wndBasket && m_wndBasket->IsChildOf(this))
    {
        // Disable animations
        m_wndBasket->EnableOnShowAnimation(m_childSaveInfo.m_bBasketOnShowAnimationEnabled);
        m_wndBasket->EnableOnHideAnimation(m_childSaveInfo.m_bBasketOnHideAnimationEnabled);

        // Set bounds from saved info
        m_wndBasket->SetBounds(m_childSaveInfo.m_basketB, true);

        // Add as child
        RemoveChild(m_wndBasket.get());
    }
}

int InventoryWnd::GameDataUpdate(void*, int dataType)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (dataType == 64 && m_vehicleType == VEHICLETYPE_PLAYER)
    {
        auto* vehicleControlledByPlayer = m3d::pClient->GetWorld().GetVehicleControlledByPlayer();
        if (vehicleControlledByPlayer)
        {
            SetVehicleId(vehicleControlledByPlayer->GetId());
            return 1;
        }
        SetVehicleId(-1);
    }
    return 1;
}

InventoryWnd::VehicleType InventoryWnd::GetVehicleTypeByGuiId(int guiId) const
{
    switch (guiId - IW_WND_PLAYER_INVENTORY)
    {
    case 0:
        return VEHICLETYPE_PLAYER;
    case 1:
        return VEHICLETYPE_WORKSHOP;
    default:
        return VEHICLETYPE_INVALID;
    }
}

int InventoryWnd::GameDataSetup()
{
    // TODO: generated code InventoryWnd::GameDataSetup
    // Early return if already processed
    if ((m_gameDataFlags & 2) != 0)
    {
        // Game data already set up for this window
        return (m_gameDataFlags & 1) != 0 ? 1 : 0;
    }

    // Determine vehicle type based on GUI ID
    m_vehicleType = VEHICLETYPE_INVALID;
    if (m_guiId == IW_WND_PLAYER_INVENTORY)
    {
        m_vehicleType = VEHICLETYPE_PLAYER;
    }
    else if (m_guiId == IW_WND_WORKSHOP_INVENTORY)
    {
        m_vehicleType = VEHICLETYPE_WORKSHOP;
    }

    // Determine cabin and basket GUI IDs based on vehicle type
    int cabinGuiId = -1;
    int basketGuiId = -1;

    switch (m_vehicleType)
    {
    case VEHICLETYPE_PLAYER:
        cabinGuiId = IW_WND_PLAYER_CABIN;
        basketGuiId = IW_WND_PLAYER_BASKET;
        break;
    case VEHICLETYPE_WORKSHOP:
        cabinGuiId = IW_WND_WORKSHOP_CABIN;
        basketGuiId = IW_WND_WORKSHOP_BASKET;
        break;
    default:
        // Invalid vehicle type
        M3D_LOG_INFO("InventoryWnd: error - fail to init because of a bad resource");
        return 0;
    }

    // Get and setup cabin window
    ref_ptr<m3d::ui::Wnd> tempCabinWnd = M3D_APP->m_pInterfaceManager->GetWindow(cabinGuiId);

    if (tempCabinWnd && tempCabinWnd->IsKindOf(&CabinWnd::m_classCabinWnd))
    {
        // Store the cabin window reference
        m_wndCabin = static_cast<CabinWnd*>(tempCabinWnd.get());

        if (m_wndCabin)
        {
            // Save animation states
            m_childSaveInfo.m_bCabinOnShowAnimationEnabled = m_wndCabin->GetOnShowAnimation().m_bEnabled;
            m_childSaveInfo.m_bCabinOnHideAnimationEnabled = m_wndCabin->GetOnHideAnimation().m_bEnabled;

            // Save bounds
            m_childSaveInfo.m_cabinB = m_wndCabin->GetBounds();
        }
    }

    // Get and setup basket window
    ref_ptr<m3d::ui::Wnd> tempBasketWnd = M3D_APP->m_pInterfaceManager->GetWindow(basketGuiId);

    if (tempBasketWnd && tempBasketWnd->IsKindOf(&BasketWnd::m_classBasketWnd))
    {
        // Store the basket window reference
        m_wndBasket = static_cast<BasketWnd*>(tempBasketWnd.get());

        if (m_wndBasket)
        {
            // Save animation states
            m_childSaveInfo.m_bBasketOnShowAnimationEnabled = m_wndBasket->GetOnShowAnimation().m_bEnabled;
            m_childSaveInfo.m_bBasketOnHideAnimationEnabled = m_wndBasket->GetOnHideAnimation().m_bEnabled;

            // Save bounds
            m_childSaveInfo.m_basketB = m_wndBasket->GetBounds();

            // If we also have a cabin window, mark game data as valid
            if (m_wndCabin)
            {
                m_gameDataFlags |= 1;  // Set valid flag
            }
        }
    }

    // Mark as processed
    m_gameDataFlags |= 2;

    // Return success if we have valid game data
    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }

    // Log error if setup failed
    M3D_LOG_INFO("InventoryWnd: error - fail to init because of a bad resource");
    return 0;
}

int InventoryWnd::OnBeforeAddToWndStation()
{
    AddCBWindows();
    return Wnd::OnBeforeAddToWndStation();
}

int InventoryWnd::OnAfterRemoveFromWndStation()
{
    int const res = m3d::ui::Wnd::OnAfterRemoveFromWndStation();
    RemoveCBWindows();
    return res;
}

void InventoryWnd::OnPlayerVehicleChanged()
{
    if (m_vehicleType != VEHICLETYPE_PLAYER)
    {
        return;
    }
    auto* vehicleControlledByPlayer = m3d::pClient->GetWorld().GetVehicleControlledByPlayer();
    SetVehicleId(vehicleControlledByPlayer ? vehicleControlledByPlayer->GetId() : -1);
}

void InventoryWnd::AddCBWindows()
{
    // TOOD: generated code InventoryWnd::AddCBWindows
    // Only execute if game data flag 1 is set
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    // Add cabin window if not already a child
    if (m_wndCabin && !m_wndCabin->IsChildOf(this))
    {
        // Disable animations
        m_wndCabin->EnableOnShowAnimation(false);
        m_wndCabin->EnableOnHideAnimation(false);

        // Set bounds from saved info
        m_wndCabin->SetBounds(m_childSaveInfo.m_cabinB, true);

        // Add as child
        AddChild(m_wndCabin.get());
    }

    // Add basket window if not already a child
    if (m_wndBasket && !m_wndBasket->IsChildOf(this))
    {
        // Disable animations
        m_wndBasket->EnableOnShowAnimation(false);
        m_wndBasket->EnableOnHideAnimation(false);

        // Set bounds from saved info
        m_wndBasket->SetBounds(m_childSaveInfo.m_basketB, true);

        // Add as child
        AddChild(m_wndBasket.get());
    }
}
