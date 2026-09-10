#include "repairlist.h"

#include <core/kernel.h>
#include <game/m3dgame.h>
#include <game/uimanager/truxxuimanager.h>
#include <game/uimisc/guihelper.h>
#include <m3dapp.h>
#include <server/objects/base/objcontainer.h>
#include <server/objects/base/prototypeinfo.h>
#include <server/objects/guns/gun.h>
#include <server/objects/physicbodies/compoundvehiclepart.h>
#include <server/objects/physicbodies/vehiclepart.h>
#include <server/objects/player.h>
#include <server/objects/vehicle.h>
#include <server/objects/workshop.h>
#include <server/resourcemanager.h>
#include <server/server.h>
#include <ui/wndstation.h>

namespace
{
    // The four service branches of RepairButton::GetServiceName / GetServiceIco
    // both classify the object the same way, by walking the resource hierarchy.
    bool IsResourceKindOf(ai::PrototypeInfo const* proto, char const* resourceName)
    {
        return ai::theResourceManager->bResourceIsKindOf(
            proto->m_resourceId, ai::theResourceManager->GetResourceId(CStr(resourceName)));
    }
}  // namespace

// ===========================================================================
//  RepairButton
// ===========================================================================

RT_CLASS_EXPORTS_BEGIN(RepairButton)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(RepairButton);

RepairButton::RprAuxInfo::RprAuxInfo() :
    m_strIdTooltipRepair("Repair"),
    m_strIdTooltipRepairNotNeed("RepairNotNeed"),
    m_strIdTooltipRepairUnavailable("RepairUnavailable"),
    m_strIdTooltipRepairPartial("RepairPartial"),
    m_strIdTooltipRepairFull("RepairFull")
{
    // RVA 0x4691E0
}

RepairButton::RprAuxInfo::RprAuxInfo(RepairButton::RprAuxInfo const& rhs) :
    m_strIdTooltipRepair(rhs.m_strIdTooltipRepair),
    m_strIdTooltipRepairNotNeed(rhs.m_strIdTooltipRepairNotNeed),
    m_strIdTooltipRepairUnavailable(rhs.m_strIdTooltipRepairUnavailable),
    m_strIdTooltipRepairPartial(rhs.m_strIdTooltipRepairPartial),
    m_strIdTooltipRepairFull(rhs.m_strIdTooltipRepairFull)
{
}

m3d::Class* RepairButton::GetBaseClass()
{
    return RT_CLASS_LOCAL(AdvancedButton);
}

m3d::Class* RepairButton::GetClass() const
{
    return RT_CLASS_LOCAL(RepairButton);
}

m3d::Object* RepairButton::CreateObject()
{
    return new RepairButton;
}

m3d::Object* RepairButton::Clone()
{
    // RVA 0x4690A0 - a fresh default-constructed button, not a copy of *this.
    return new RepairButton;
}

RepairButton::RepairButton() = default;

RepairButton::RepairButton(RepairButton const&)
{
    // RVA 0x469380 - copies nothing.
}

RepairButton::~RepairButton() = default;

CStr RepairButton::GetServiceName() const
{
    // RVA 0x4693B0 - the whole vehicle is labelled "Construction"; a cabin or
    // basket gets its own generic label; a gun is named by its prototype.
    ai::Obj* obj = m_objId == -1 ? nullptr : ai::theObjects->GetEntityByObjId(m_objId);
    if (!obj || !RT_DYNCAST(obj, ai::Obj))
    {
        return {};
    }
    ai::PrototypeInfo const* proto = obj->GetPrototypeInfo();
    if (!proto)
    {
        return {};
    }

    if (IsResourceKindOf(proto, "VEHICLE"))
    {
        return M3D_APP->GetStringByStringId0(CStr("Construction"));
    }
    if (IsResourceKindOf(proto, "CABIN"))
    {
        return M3D_APP->GetStringByStringId0(CStr("Cabin"));
    }
    if (IsResourceKindOf(proto, "BASKET"))
    {
        return M3D_APP->GetStringByStringId0(CStr("Basket"));
    }
    if (IsResourceKindOf(proto, "GUN"))
    {
        return ai::pServer->GetPrototypeFullName(proto->m_prototypeName);
    }
    return {};
}

m3d::rend::TexHandle RepairButton::GetServiceIco() const
{
    // RVA 0x4695F0 - same classification as GetServiceName, but a gun uses the
    // icon of its firing type rather than its prototype icon.
    ai::Obj* obj = m_objId == -1 ? nullptr : ai::theObjects->GetEntityByObjId(m_objId);
    if (!obj || !RT_DYNCAST(obj, ai::Obj))
    {
        return {};
    }
    ai::PrototypeInfo const* proto = obj->GetPrototypeInfo();
    if (!proto)
    {
        return {};
    }

    if (IsResourceKindOf(proto, "VEHICLE"))
    {
        return M3D_APP->m_pInterfaceManager->GetIcoByName(CStr("Construction"), 0);
    }
    if (IsResourceKindOf(proto, "CABIN"))
    {
        return M3D_APP->m_pInterfaceManager->GetIcoByName(CStr("Cabin"), 0);
    }
    if (IsResourceKindOf(proto, "BASKET"))
    {
        return M3D_APP->m_pInterfaceManager->GetIcoByName(CStr("Basket"), 0);
    }
    if (IsResourceKindOf(proto, "GUN"))
    {
        return M3D_APP->m_pInterfaceManager->GetIcoByName(
            ai::GunPrototypeInfo::FiringType2Str(help::GetGunFiringType(obj)), 0);
    }
    return {};
}

int RepairButton::GetMaxUnitsToBuy() const
{
    // RVA 0x469880 - how much health / durability the object is missing.
    ai::Obj* obj = m_objId == -1 ? nullptr : ai::theObjects->GetEntityByObjId(m_objId);
    if (!obj || !RT_DYNCAST(obj, ai::Obj))
    {
        return 0;
    }
    if (auto* vehicle = RT_DYNCAST(obj, ai::Vehicle))
    {
        return static_cast<int>(vehicle->Health().maxValue().get() - vehicle->Health().value().get());
    }
    if (auto* compound = RT_DYNCAST(obj, ai::CompoundVehiclePart))
    {
        return static_cast<int>(compound->GetMaxDurability() - compound->GetDurability());
    }
    if (auto* part = RT_DYNCAST(obj, ai::VehiclePart))
    {
        return static_cast<int>(part->Durability().maxValue().get() - part->Durability().value().get());
    }
    return 0;
}

float RepairButton::GetPriceForOneUnit() const
{
    // RVA 0x4699B0 - the whole vehicle has a flat per-health-point rate, while a
    // part is quoted as a lump sum that gets spread over the damage it has taken.
    ai::Obj* obj = m_objId == -1 ? nullptr : ai::theObjects->GetEntityByObjId(m_objId);
    if (!obj || !RT_DYNCAST(obj, ai::Obj))
    {
        return 0.0f;
    }
    ai::Workshop* workshop = GetWorkshop();
    if (!workshop)
    {
        return 0.0f;
    }

    if (RT_DYNCAST(obj, ai::Vehicle))
    {
        float const price = workshop->GetHealthPriceForOneUnit();
        return price < 0.0f ? 0.0f : price;
    }
    if (RT_DYNCAST(obj, ai::VehiclePart))
    {
        int const maxDurToRepair = GetMaxUnitsToBuy();
        if (maxDurToRepair)
        {
            return static_cast<float>(
                static_cast<double>(workshop->GetObjectRepairPrice(obj)) / static_cast<double>(maxDurToRepair));
        }
    }
    return 0.0f;
}

CStr const& RepairButton::GetStrIdTooltipBuy() const
{
    return m_rprAif.m_strIdTooltipRepair;
}

CStr const& RepairButton::GetStrIdTooltipBuyNotNeed() const
{
    return m_rprAif.m_strIdTooltipRepairNotNeed;
}

CStr const& RepairButton::GetStrIdTooltipBuyUnavailable() const
{
    return m_rprAif.m_strIdTooltipRepairUnavailable;
}

CStr const& RepairButton::GetStrIdTooltipBuyPartial() const
{
    return m_rprAif.m_strIdTooltipRepairPartial;
}

CStr const& RepairButton::GetStrIdTooltipBuyFull() const
{
    return m_rprAif.m_strIdTooltipRepairFull;
}

// ===========================================================================
//  RepairList
// ===========================================================================

RT_CLASS_EXPORTS_BEGIN(RepairList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(RepairList);

m3d::Class* RepairList::GetBaseClass()
{
    return RT_CLASS_LOCAL(AdvancedList);
}

m3d::Class* RepairList::GetClass() const
{
    return RT_CLASS_LOCAL(RepairList);
}

m3d::Object* RepairList::CreateObject()
{
    return new RepairList;
}

m3d::Object* RepairList::Clone()
{
    // RVA 0x4690D0
    return new RepairList;
}

RepairList::RepairList() = default;

RepairList::RepairList(RepairList const&)
{
    // RVA 0x469BB0 - copies nothing.
}

RepairList::~RepairList() = default;

AdvancedButton* RepairList::NewItem() const
{
    // RVA 0x469D30
    return static_cast<AdvancedButton*>(M3D_KERNEL->New("RepairButton"));
}

std::vector<int, std::allocator<int>> RepairList::GetObjIds() const
{
    // RVA 0x469D50 - the vehicle itself, then its cabin and basket, then every
    // remaining part. The chassis is skipped because the vehicle's own health
    // already stands for it, and the cabin and basket are skipped in the loop
    // because they were already added by name above (which fixes their order at
    // the top of the list).
    std::vector<int> ids;
    if (!ai::thePlayer)
    {
        return ids;
    }
    ai::Vehicle* playerVehicle = ai::thePlayer->GetVehicle();
    if (!playerVehicle)
    {
        return ids;
    }

    ids.push_back(playerVehicle->GetId());
    if (ai::VehiclePart* cabin = playerVehicle->GetPartByName(CStr("CABIN")))
    {
        ids.push_back(cabin->GetId());
    }
    if (ai::VehiclePart* basket = playerVehicle->GetPartByName(CStr("BASKET")))
    {
        ids.push_back(basket->GetId());
    }

    for (auto vpIt = playerVehicle->begin(); vpIt != playerVehicle->end(); ++vpIt)
    {
        auto const& [partName, part] = *vpIt;
        if (partName == CStr("CHASSIS") || partName == CStr("CABIN") || partName == CStr("BASKET") || !part)
        {
            continue;
        }
        ids.push_back(part->GetId());
    }
    return ids;
}

void RepairList::BuyService(AdvancedButton const* btn)
{
    // RVA 0x469BE0
    if (!btn)
    {
        return;
    }
    int const objId = btn->GetObjId();
    ai::Obj* obj = objId == -1 ? nullptr : ai::theObjects->GetEntityByObjId(objId);
    if (!obj || !RT_DYNCAST(obj, ai::Obj))
    {
        return;
    }

    int const price = btn->GetPrice();
    if (price > ai::thePlayer->GetMoney())
    {
        return;
    }
    int const units = btn->GetUnitsToBuy();
    if (!units)
    {
        return;
    }

    if (auto* vehicle = RT_DYNCAST(obj, ai::Vehicle))
    {
        vehicle->Health().value().set(static_cast<float>(units) + vehicle->Health().value().get());
    }
    else if (auto* compound = RT_DYNCAST(obj, ai::CompoundVehiclePart))
    {
        compound->SetDurability(
            static_cast<float>(static_cast<double>(compound->GetDurability()) + static_cast<double>(units)));
    }
    else if (auto* part = RT_DYNCAST(obj, ai::VehiclePart))
    {
        part->Durability().value().set(static_cast<float>(units) + part->Durability().value().get());
    }
    else
    {
        // Nothing repairable - the money is deliberately left alone.
        return;
    }
    ai::thePlayer->AddMoney(-price);
}
