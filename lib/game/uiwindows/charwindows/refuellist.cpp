#include "refuellist.h"

#include <core/kernel.h>
#include <game/m3dgame.h>
#include <game/uimanager/truxxuimanager.h>
#include <game/uimisc/guihelper.h>
#include <m3dapp.h>
#include <server/objects/base/objcontainer.h>
#include <server/objects/player.h>
#include <server/objects/town.h>
#include <server/objects/vehicle.h>
#include <ui/wndstation.h>

// ===========================================================================
//  RefuelButton
// ===========================================================================

RT_CLASS_EXPORTS_BEGIN(RefuelButton)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(RefuelButton);

RefuelButton::RflAuxInfo::RflAuxInfo() :
    m_strIdTooltipRefuel("Refuel"),
    m_strIdTooltipRefuelNotNeed("RefuelNotNeed"),
    m_strIdTooltipRefuelUnavailable("RefuelUnavailable"),
    m_strIdTooltipRefuelPartial("RefuelPartial"),
    m_strIdTooltipRefuelFull("RefuelFull")
{
    // RVA 0x468AA0
}

RefuelButton::RflAuxInfo::RflAuxInfo(RefuelButton::RflAuxInfo const& rhs) :
    m_strIdTooltipRefuel(rhs.m_strIdTooltipRefuel),
    m_strIdTooltipRefuelNotNeed(rhs.m_strIdTooltipRefuelNotNeed),
    m_strIdTooltipRefuelUnavailable(rhs.m_strIdTooltipRefuelUnavailable),
    m_strIdTooltipRefuelPartial(rhs.m_strIdTooltipRefuelPartial),
    m_strIdTooltipRefuelFull(rhs.m_strIdTooltipRefuelFull)
{
}

m3d::Class* RefuelButton::GetBaseClass()
{
    return RT_CLASS_LOCAL(AdvancedButton);
}

m3d::Class* RefuelButton::GetClass() const
{
    return RT_CLASS_LOCAL(RefuelButton);
}

m3d::Object* RefuelButton::CreateObject()
{
    return new RefuelButton;
}

m3d::Object* RefuelButton::Clone()
{
    // RVA 0x468960 - like every other window in this family, Clone hands back a
    // fresh default-constructed button rather than a copy of *this.
    return new RefuelButton;
}

RefuelButton::RefuelButton() = default;

RefuelButton::RefuelButton(RefuelButton const&)
{
    // RVA 0x468C40 - chains to the AdvancedButton default ctor and copies
    // nothing.
}

RefuelButton::~RefuelButton() = default;

CStr RefuelButton::GetServiceName() const
{
    // RVA 0x468C70
    return M3D_APP->GetStringByStringId0(CStr("Refuelling"));
}

m3d::rend::TexHandle RefuelButton::GetServiceIco() const
{
    // RVA 0x468CC0
    return M3D_APP->m_pInterfaceManager->GetIcoByName(CStr("Refuelling"), 0);
}

int RefuelButton::GetMaxUnitsToBuy() const
{
    // RVA 0x468D10 - how much fuel the tank is missing.
    ai::Obj* obj = m_objId == -1 ? nullptr : ai::theObjects->GetEntityByObjId(m_objId);
    auto* vehicle = RT_DYNCAST(obj, ai::Vehicle);
    if (!vehicle)
    {
        return 0;
    }
    return static_cast<int>(vehicle->Fuel().maxValue().get() - vehicle->Fuel().value().get());
}

float RefuelButton::GetPriceForOneUnit() const
{
    // RVA 0x468DA0
    ai::Town* town = M3D_APP->m_pInterfaceManager->GetCurrentTown();
    if (!town)
    {
        return 0.0f;
    }
    int const price = help::GetFuelPriceForOneUnit(m_objId, town->GetId());
    return price < 0 ? 0.0f : static_cast<float>(price);
}

CStr const& RefuelButton::GetStrIdTooltipBuy() const
{
    return m_rflAif.m_strIdTooltipRefuel;
}

CStr const& RefuelButton::GetStrIdTooltipBuyNotNeed() const
{
    return m_rflAif.m_strIdTooltipRefuelNotNeed;
}

CStr const& RefuelButton::GetStrIdTooltipBuyUnavailable() const
{
    return m_rflAif.m_strIdTooltipRefuelUnavailable;
}

CStr const& RefuelButton::GetStrIdTooltipBuyPartial() const
{
    return m_rflAif.m_strIdTooltipRefuelPartial;
}

CStr const& RefuelButton::GetStrIdTooltipBuyFull() const
{
    return m_rflAif.m_strIdTooltipRefuelFull;
}

// ===========================================================================
//  RefuelList
// ===========================================================================

RT_CLASS_EXPORTS_BEGIN(RefuelList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(RefuelList);

m3d::Class* RefuelList::GetBaseClass()
{
    return RT_CLASS_LOCAL(AdvancedList);
}

m3d::Class* RefuelList::GetClass() const
{
    return RT_CLASS_LOCAL(RefuelList);
}

m3d::Object* RefuelList::CreateObject()
{
    return new RefuelList;
}

m3d::Object* RefuelList::Clone()
{
    // RVA 0x468990
    return new RefuelList;
}

RefuelList::RefuelList() = default;

RefuelList::RefuelList(RefuelList const&)
{
    // RVA 0x468EE0 - copies nothing.
}

RefuelList::~RefuelList() = default;

AdvancedButton* RefuelList::NewItem() const
{
    // RVA 0x468FF0 - built through the kernel factory so the RT class registry
    // is what decides the concrete type.
    return static_cast<AdvancedButton*>(M3D_KERNEL->New("RefuelButton"));
}

std::vector<int, std::allocator<int>> RefuelList::GetObjIds() const
{
    // RVA 0x469010 - only ever one row: the player's own vehicle.
    std::vector<int> ids;
    if (!ai::thePlayer)
    {
        return ids;
    }
    ai::Vehicle* vehicle = ai::thePlayer->GetVehicle();
    if (!vehicle)
    {
        return ids;
    }
    ids.push_back(vehicle->GetId());
    return ids;
}

void RefuelList::BuyService(AdvancedButton const* btn)
{
    // RVA 0x468F10
    if (!btn)
    {
        return;
    }
    int const objId = btn->GetObjId();
    ai::Obj* obj = objId == -1 ? nullptr : ai::theObjects->GetEntityByObjId(objId);
    auto* vehicle = RT_DYNCAST(obj, ai::Vehicle);
    if (!vehicle)
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
    vehicle->Fuel().value().set(static_cast<float>(units) + vehicle->Fuel().value().get());
    ai::thePlayer->AddMoney(-price);
}
