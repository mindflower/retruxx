#include "cabinwnd.h"
#include "gadgetwnd.h"

#include "core/kernel.h"
#include "core/log.h"
#include "game/uimanager/uidefs.h"
#include <game/m3dgame.h>

#include <ui/image.h>

#include <server/resourcemanager.h>
#include <server/objects/vehicle.h>
#include <server/objects/cabin.h>
#include <server/objects/base/complexphysicobj.h>

RT_CLASS_EXPORTS_BEGIN(CabinWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(CabinWnd);

CabinWnd::AuxInfo::AuxInfo()
{
    m_cabinFirstGunOrigin.x = 368.0f;
    m_cabinFirstGunOrigin.y = 70.0f;
    m_cabinSecondGunOrigin.x = 144.0f;
    m_cabinSecondGunOrigin.y = 112.0f;
    m_gadgetCommonName = "wndGadgetCommonBg";
    m_gadgetWeaponName = "wndGadgetWeaponBg";
}

CabinWnd::CabinWnd()
{
    m_mainPartName = "CABIN";
    m_wndGadgets.resize(GADGET_NUM_GADGETS);
}

CabinWnd::CabinWnd(CabinWnd const&) : CabinWnd()
{
}

CabinWnd::~CabinWnd() = default;

m3d::Object* CabinWnd::Clone()
{
    return new CabinWnd(*this);
}

m3d::Object* CabinWnd::CreateObject()
{
    return new CabinWnd;
}

m3d::Class* CabinWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(CBWnd);
}

m3d::Class* CabinWnd::GetClass() const
{
    return RT_CLASS_LOCAL(CabinWnd);
}

InventoryWnd::VehicleType CabinWnd::GetVehicleTypeByGuiId(int guiId) const
{
    switch (guiId)
    {
    case IW_WND_PLAYER_CABIN:
        return InventoryWnd::VEHICLETYPE_PLAYER;
    case IW_WND_WORKSHOP_CABIN:
        return InventoryWnd::VEHICLETYPE_WORKSHOP;
    default:
        return InventoryWnd::VEHICLETYPE_INVALID;
    }
}

// ---------------------------------------------------------------------------

int CabinWnd::GameDataSetup()
{
    if (!CBWnd::GameDataSetup())
    {
        return 0;
    }

    int res = 1;
    if ((m_gameDataFlags & 2) == 0)
    {
        for (int i = 0; i < GADGET_NUM_GADGETS; ++i)
        {
            res &= CreateGadgetWnd(i);
        }
        if (!res)
        {
            m_gameDataFlags &= ~1u;
        }
    }

    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }
    M3D_LOG_INFO(CStr("CabinWnd: error - fail to init because of a bad resource"));
    return 0;
}

int CabinWnd::GameDataUpdate(void* data, int dataType)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (dataType == 31)
    {
        // TODO(RVA 0x438E80): gadget drag-drop reconciliation - looks up the
        // GadgetWnd by slot id (m_intEv[1]) for the matching vehicle
        // (m_intEv[0]), pulls its item off the vehicle and re-adds it either to
        // the vehicle or the player repository via ai::Vehicle::AddThing.
        // Blocked on the unported GadgetWnd/ItemWnd item pipeline.
        return 1;
    }
    CBWnd::GameDataUpdate(data, dataType);
    return 1;
}

void CabinWnd::UpdateOnMainPartChanged()
{
    SetupChildVehicleParts();
    SetupGadgets();
}

int CabinWnd::CreateGadgetWnd(int id)
{
    if (m_vehicleType == InventoryWnd::VEHICLETYPE_INVALID)
    {
        return 0;
    }

    CStr patternName;
    CStr resourceName;
    int idBase;
    if (id >= GADGET_COMMON_MIN && id <= GADGET_COMMON_MAX)
    {
        patternName = m_aif.m_gadgetCommonName;
        resourceName = "GADGET_COMMON";
        idBase = GADGET_COMMON_MIN;
    }
    else if (id >= GADGET_WEAPON_MIN && id <= GADGET_WEAPON_MAX)
    {
        patternName = m_aif.m_gadgetWeaponName;
        resourceName = "GADGET_WEAPON";
        idBase = GADGET_WEAPON_MIN;
    }
    else
    {
        return 0;
    }

    CStr strId;
    strId.format("%02d", id - idBase);
    patternName = patternName + strId;

    m3d::Object* child = GetChildByName(patternName);
    if (!child || !child->IsKindOf(&m3d::ui::Wnd::m_classWnd))
    {
        M3D_LOG_INFO(CStr("Make control error: control ") + patternName + " is not found or incorrect type");
        return 0;
    }

    m_wndGadgets[id] = static_cast<GadgetWnd*>(M3D_KERNEL->New("GadgetWnd"));
    if (!m_wndGadgets[id])
    {
        M3D_LOG_INFO(CStr("Make control error: cannot create ") + patternName + " - cannot find rtti class GadgetWnd");
        return 0;
    }
    if (!m_wndGadgets[id]->CreateFromPattern(static_cast<m3d::ui::Wnd*>(child), true))
    {
        M3D_LOG_INFO(CStr("Make control error: cannot create ") + patternName + " from pattern class");
        return 0;
    }

    m_wndGadgets[id]->SetResourceName(resourceName);

    int const guiBase = (m_vehicleType != InventoryWnd::VEHICLETYPE_PLAYER) ? 53 : 43;
    M3D_APP->m_pInterfaceManager->AddWindowById(m_wndGadgets[id].get(), id + guiBase, true, false);
    return 1;
}

ref_ptr<GadgetWnd> CabinWnd::GetGadgetWndByGadgetSlotId(int slotId) const
{
    for (auto const& gadget : m_wndGadgets)
    {
        if (gadget && gadget->GetSlotId() == slotId)
        {
            return gadget;
        }
    }
    return {};
}

void CabinWnd::SetupGadgets()
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    // Detach and reset every gadget window.
    for (auto& gadget : m_wndGadgets)
    {
        if (!gadget)
        {
            continue;
        }
        if (IsDirectChild(gadget.get()))
        {
            RemoveChild(gadget.get());
        }
        gadget->SetVehicleId(-1);
        gadget->SetSlotId(-1);
    }

    ai::Vehicle* vehicle = GetVehicle();
    if (!vehicle)
    {
        return;
    }
    ai::Cabin const* cabin = vehicle->GetCabin();
    if (!cabin)
    {
        return;
    }
    ai::CabinPrototypeInfo const* proto = cabin->GetPrototypeInfo();
    if (!proto)
    {
        return;
    }

    struct
    {
        char const* resName;
        int idxBase;
    } const groups[2] = {{"GADGET_COMMON", GADGET_COMMON_MIN}, {"GADGET_WEAPON", GADGET_WEAPON_MIN}};

    for (auto const& group : groups)
    {
        auto it = proto->m_gadgetSlots.find(CStr(group.resName));
        if (it == proto->m_gadgetSlots.end())
        {
            continue;
        }
        int const firstSlot = it->second.x;
        int const lastSlot = it->second.y;
        if (firstSlot == -1 || lastSlot == -1 || firstSlot > lastSlot)
        {
            continue;
        }

        int idx = group.idxBase;
        for (int slotId = firstSlot; slotId <= lastSlot && idx < static_cast<int>(m_wndGadgets.size()); ++slotId, ++idx)
        {
            auto& gadget = m_wndGadgets[idx];
            if (!gadget)
            {
                continue;
            }
            gadget->SetVehicleId(GetVehicleId());
            gadget->SetSlotId(slotId);
            AddChild(gadget.get());
            MoveChildToFirstPosition(gadget.get());
        }
    }
}

void CabinWnd::SetupChildVehicleParts()
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
    ai::VehiclePrototypeInfo const* proto = vehicle->GetPrototypeInfo();
    if (!proto)
    {
        return;
    }
    ai::ComplexPhysicObjPartDescription const* cabinDesc = proto->GetPartDescriptionByName(CStr("CABIN"));
    if (!cabinDesc)
    {
        return;
    }

    // Collect the (up to two) attachable gun parts hanging off the cabin.
    retruxx::vector<CStr> gunPartNames;
    for (m3d::Object* c = cabinDesc->GetFirstChild(); c; c = c->GetNextSibling())
    {
        CStr partName = c->GetName();
        if (vehicle->CanPartBeAttached(partName))
        {
            gunPartNames.push_back(partName);
            if (gunPartNames.size() > 2)
            {
                M3D_LOG_INFO(CStr("InventoryWnd: sorry but only two guns can be shown in interface"));
                break;
            }
        }
    }

    // The "big gun" (if any) takes the first slot origin, everything else the second.
    int firstWndIdx = 0;
    for (size_t i = 0; i < gunPartNames.size(); ++i)
    {
        CStr const rcName = ai::theResourceManager->GetResourceNameByVehiclePartName(gunPartNames[i]);
        int const rcId = ai::theResourceManager->GetResourceId(rcName);
        if (ai::theResourceManager->bResourceIsKindOf(rcId, ai::theResourceManager->GetResourceId(CStr("BIG_GUN"))))
        {
            firstWndIdx = static_cast<int>(i);
            break;
        }
    }

    for (size_t i = 0; i < gunPartNames.size(); ++i)
    {
        PointBase<float> const& origin =
            (static_cast<int>(i) != firstWndIdx) ? m_aif.m_cabinSecondGunOrigin : m_aif.m_cabinFirstGunOrigin;
        CreateChildVehiclePartWindow(gunPartNames[i], origin);
    }
}
