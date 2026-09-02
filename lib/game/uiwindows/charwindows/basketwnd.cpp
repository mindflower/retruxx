#include "basketwnd.h"
#include "izvratrepositorywnd.h"
#include "salewnd.h"

#include "core/kernel.h"
#include "core/log.h"
#include "game/uimanager/uidefs.h"
#include <game/m3dgame.h>

#include <server/geomrepository.h>
#include <server/objects/vehicle.h>
#include <server/objects/basket.h>

RT_CLASS_EXPORTS_BEGIN(BasketWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(BasketWnd);

BasketWnd::AuxInfo::AuxInfo()
{
    m_wndInventoryName = "wndInventory";
}

BasketWnd::BasketWnd()
{
    m_mainPartName = "BASKET";
}

BasketWnd::BasketWnd(BasketWnd const&) : BasketWnd()
{
}

BasketWnd::~BasketWnd()
{
    BasketWnd::GameDataClear(false);
}

m3d::Object* BasketWnd::Clone()
{
    return new BasketWnd(*this);
}

m3d::Object* BasketWnd::CreateObject()
{
    return new BasketWnd;
}

m3d::Class* BasketWnd::GetClass() const
{
    return RT_CLASS_LOCAL(BasketWnd);
}

m3d::Class* BasketWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(CBWnd);
}

InventoryWnd::VehicleType BasketWnd::GetVehicleTypeByGuiId(int guiId) const
{
    switch (guiId)
    {
    case IW_WND_PLAYER_BASKET:
        return InventoryWnd::VEHICLETYPE_PLAYER;
    case IW_WND_WORKSHOP_BASKET:
        return InventoryWnd::VEHICLETYPE_WORKSHOP;
    default:
        return InventoryWnd::VEHICLETYPE_INVALID;
    }
}

// ---------------------------------------------------------------------------

int BasketWnd::GameDataSetup()
{
    if (!CBWnd::GameDataSetup())
    {
        return 0;
    }

    int res = 1;
    if ((m_gameDataFlags & 2) == 0)
    {
        m3d::Object* child = GetChildByName(m_aif.m_wndInventoryName);
        if (!child || !child->IsKindOf(&m3d::ui::Wnd::m_classWnd))
        {
            M3D_LOG_INFO(CStr("Make control error: control ") + m_aif.m_wndInventoryName +
                         " is not found or incorrect type");
            res = 0;
        }
        else
        {
            m_wndInventory = static_cast<IzvratRepositoryWnd*>(M3D_KERNEL->New("IzvratRepositoryWnd"));
            if (!m_wndInventory)
            {
                M3D_LOG_INFO(CStr("Make control error: cannot create ") + m_aif.m_wndInventoryName +
                             " - cannot find rtti class IzvratRepositoryWnd");
                res = 0;
            }
            else if (!m_wndInventory->CreateFromPattern(static_cast<m3d::ui::Wnd*>(child), true))
            {
                M3D_LOG_INFO(CStr("Make control error: cannot create ") + m_aif.m_wndInventoryName +
                             " from pattern class");
                res = 0;
            }
        }

        int guiId = -1;
        if (m_vehicleType == InventoryWnd::VEHICLETYPE_PLAYER)
        {
            guiId = 8;
        }
        else if (m_vehicleType == InventoryWnd::VEHICLETYPE_WORKSHOP)
        {
            guiId = 9;
        }

        if (res && guiId != -1 && m_wndInventory)
        {
            res &= M3D_APP->m_pInterfaceManager->AddWindowById(m_wndInventory.get(), guiId, true, false);
        }
        else
        {
            res = 0;
        }

        if (res)
        {
            m_gameDataFlags |= 1u;
        }
        else
        {
            m_gameDataFlags &= ~1u;
        }
    }

    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }
    M3D_LOG_INFO(CStr("BasketWnd: error - fail to init because of a bad resource"));
    return 0;
}

int BasketWnd::GameDataClear(bool)
{
    SetVehicleId(-1);
    ClearChildVehicleParts();
    if ((m_gameDataFlags & 1) != 0)
    {
        m_wndInventory->SetIzvratRepository(nullptr, RepositoryWnd::REPOSITORYTYPE_PLAYER);
    }
    return 1;
}

int BasketWnd::GameDataUpdate(void* data, int dataType)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (dataType != 102)
    {
        return CBWnd::GameDataUpdate(data, dataType);
    }
    if (IsChildOf(M3D_APP))
    {
        UpdateOnSaleTabChanged(data);
    }
    return 1;
}

int BasketWnd::OnAfterRemoveFromWndStation()
{
    int const res = m3d::ui::Wnd::OnAfterRemoveFromWndStation();
    if (m_wndInventory)
    {
        if (ai::IzvratRepository* repo = m_wndInventory->GetIzvratRepository())
        {
            repo->DetachAllObjectsFromParents();
        }
    }
    return res;
}

void BasketWnd::UpdateOnMainPartChanged()
{
    SetupInventory();
    SetupChildVehicleParts();
}

void BasketWnd::SetupInventory()
{
    if ((m_gameDataFlags & 1) == 0 || m_vehicleType == InventoryWnd::VEHICLETYPE_INVALID)
    {
        return;
    }

    ai::Vehicle* vehicle = GetVehicle();
    ai::IzvratRepository* repository = vehicle ? vehicle->GetRepository() : nullptr;

    m_wndInventory->SetIzvratRepository(repository, m_vehicleType != InventoryWnd::VEHICLETYPE_PLAYER
                                                       ? RepositoryWnd::REPOSITORYTYPE_WORKSHOP_VEHICLE
                                                       : RepositoryWnd::REPOSITORYTYPE_PLAYER);

    if (repository)
    {
        if (!IsDirectChild(m_wndInventory.get()))
        {
            AddChild(m_wndInventory.get());
            MoveChildToFirstPosition(m_wndInventory.get());
        }
        m_wndInventory->SetItemDrawStyleByResourceId(4, -1);
    }
    else if (IsDirectChild(m_wndInventory.get()))
    {
        RemoveChild(m_wndInventory.get());
    }
}

void BasketWnd::SetupChildVehicleParts()
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    ClearChildVehicleParts();

    ai::Vehicle* vehicle = GetVehicle();
    if (!vehicle)
    {
        return;
    }
    const ai::Basket* basket = vehicle->GetBasket();
    if (!basket)
    {
        return;
    }
    const ai::BasketPrototypeInfo* proto = basket->GetPrototypeInfo();
    if (!proto)
    {
        return;
    }

    const BoundsBase<float> maxBounds = m_wndInventory->GetMaxRepositoryBounds();
    const PointBase<float> cellSize = m_wndInventory->GetCellSize();

    for (auto const& slot : proto->GetSlotPositions())
    {
        PointBase<float> origin;
        origin.x = static_cast<float>(slot.second.x) * cellSize.x + maxBounds.x0;
        origin.y = static_cast<float>(slot.second.y) * cellSize.y + maxBounds.y0;
        CreateChildVehiclePartWindow(slot.first, origin);
    }
}

void BasketWnd::UpdateOnSaleTabChanged(void* data)
{
    const auto tabType = static_cast<SaleWnd::WorkshopTabType>(reinterpret_cast<intptr_t>(data));
    if (tabType == 0)
    {
        return;
    }

    m_wndInventory->SetItemDrawStyleByResourceId(4, -1);
    const std::vector<int> resourceIds = SaleWnd::GetResourceIdsForTabItem(tabType);
    m_wndInventory->SetItemDrawStyleByResourceId(12, -1);
    for (int rcId : resourceIds)
    {
        m_wndInventory->SetItemDrawStyleByResourceId(4, rcId);
    }
}
