#include "rechargelist.h"

#include <core/kernel.h>
#include <game/m3dgame.h>
#include <game/uimanager/truxxuimanager.h>
#include <game/uimisc/guihelper.h>
#include <m3dapp.h>
#include <server/objects/base/objcontainer.h>
#include <server/objects/base/prototypeinfo.h>
#include <server/objects/base/prototypemanager.h>
#include <server/objects/guns/gun.h>
#include <server/objects/player.h>
#include <server/objects/vehicle.h>
#include <server/server.h>
#include <ui/wndstation.h>

// ===========================================================================
//  RechargeList
// ===========================================================================

RT_CLASS_EXPORTS_BEGIN(RechargeList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(RechargeList);

m3d::Class* RechargeList::GetBaseClass()
{
    return RT_CLASS_LOCAL(AdvancedList);
}

m3d::Class* RechargeList::GetClass() const
{
    return RT_CLASS_LOCAL(RechargeList);
}

m3d::Object* RechargeList::CreateObject()
{
    return new RechargeList;
}

m3d::Object* RechargeList::Clone()
{
    // RVA 0x467FD0 - a fresh default-constructed list, not a copy of *this.
    return new RechargeList;
}

RechargeList::RechargeList() = default;

RechargeList::RechargeList(RechargeList const&)
{
    // RVA 0x468710 - copies nothing.
}

RechargeList::~RechargeList() = default;

AdvancedButton* RechargeList::NewItem() const
{
    // RVA 0x468850
    return static_cast<AdvancedButton*>(M3D_KERNEL->New("RechargeButton"));
}

std::vector<int, std::allocator<int>> RechargeList::GetObjIds() const
{
    // RVA 0x468870 - one row per gun on the player's vehicle that takes ammo.
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

    std::vector<ai::Obj*> guns;
    help::GetGunsForVehicle(vehicle->GetId(), guns);
    for (int i = 0; i < static_cast<int>(guns.size()); ++i)
    {
        if (help::CanGunBeReloaded(guns[i]))
        {
            ids.push_back(guns[i]->GetId());
        }
    }
    return ids;
}

void RechargeList::BuyService(AdvancedButton const* btn)
{
    // RVA 0x468740
    if (!btn)
    {
        return;
    }
    int const objId = btn->GetObjId();
    ai::Obj* gun = objId == -1 ? nullptr : ai::theObjects->GetEntityByObjId(objId);
    if (!gun || !RT_DYNCAST(gun, ai::Obj) || !help::CanGunBeReloaded(gun))
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

    // Shells top up the loaded charge first, and only what is left over goes
    // into the reserve pool.
    int const missingFromCharge =
        static_cast<int>(help::GetGunChargeSize(gun)) - static_cast<int>(help::GetGunShellsInCurrentCharge(gun));
    int const toCharge = units < missingFromCharge ? units : missingFromCharge;
    help::SetGunShellsInCurrentCharge(gun, toCharge + static_cast<int>(help::GetGunShellsInCurrentCharge(gun)));

    int const toPool = units - toCharge;
    if (toPool)
    {
        help::SetGunShellsInPool(gun, toPool + static_cast<int>(help::GetGunShellsInPool(gun)));
    }
    ai::thePlayer->AddMoney(-price);
}

// ===========================================================================
//  RechargeButton
// ===========================================================================

RT_CLASS_EXPORTS_BEGIN(RechargeButton)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(RechargeButton);

RechargeButton::RchrAuxInfo::RchrAuxInfo() :
    m_strIdTooltipRecharge("Recharge"),
    m_strIdTooltipRechargeNotNeed("RechargeNotNeed"),
    m_strIdTooltipRechargeUnavailable("RechargeUnavailable"),
    m_strIdTooltipRechargePartial("RechargePartial"),
    m_strIdTooltipRechargeFull("RechargeFull")
{
    // RVA 0x4680E0
}

RechargeButton::RchrAuxInfo::RchrAuxInfo(RechargeButton::RchrAuxInfo const& rhs) :
    m_strIdTooltipRecharge(rhs.m_strIdTooltipRecharge),
    m_strIdTooltipRechargeNotNeed(rhs.m_strIdTooltipRechargeNotNeed),
    m_strIdTooltipRechargeUnavailable(rhs.m_strIdTooltipRechargeUnavailable),
    m_strIdTooltipRechargePartial(rhs.m_strIdTooltipRechargePartial),
    m_strIdTooltipRechargeFull(rhs.m_strIdTooltipRechargeFull)
{
}

m3d::Class* RechargeButton::GetBaseClass()
{
    return RT_CLASS_LOCAL(AdvancedButton);
}

m3d::Class* RechargeButton::GetClass() const
{
    return RT_CLASS_LOCAL(RechargeButton);
}

m3d::Object* RechargeButton::CreateObject()
{
    return new RechargeButton;
}

m3d::Object* RechargeButton::Clone()
{
    // RVA 0x467FA0
    return new RechargeButton;
}

RechargeButton::RechargeButton() = default;

RechargeButton::RechargeButton(RechargeButton const&)
{
    // RVA 0x468280 - copies nothing.
}

RechargeButton::~RechargeButton() = default;

CStr RechargeButton::GetServiceName() const
{
    // RVA 0x4682B0 - the row is named after the gun being reloaded.
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
    return ai::pServer->GetPrototypeFullName(proto->m_prototypeName);
}

m3d::rend::TexHandle RechargeButton::GetServiceIco() const
{
    // RVA 0x468340 - guns are iconified by firing type, not by prototype.
    ai::Obj* obj = m_objId == -1 ? nullptr : ai::theObjects->GetEntityByObjId(m_objId);
    if (!obj || !RT_DYNCAST(obj, ai::Obj))
    {
        return {};
    }
    return M3D_APP->m_pInterfaceManager->GetIcoByName(
        ai::GunPrototypeInfo::FiringType2Str(help::GetGunFiringType(obj)), 0);
}

int RechargeButton::GetMaxUnitsToBuy() const
{
    // RVA 0x468400 - what is missing from the loaded charge plus what is missing
    // from the reserve pool.
    ai::Obj* gun = m_objId == -1 ? nullptr : ai::theObjects->GetEntityByObjId(m_objId);
    if (!gun || !RT_DYNCAST(gun, ai::Obj) || !help::CanGunBeReloaded(gun))
    {
        return 0;
    }
    unsigned int const missingFromCharge = help::GetGunChargeSize(gun) - help::GetGunShellsInCurrentCharge(gun);
    return static_cast<int>(missingFromCharge + help::GetGunShellsPoolSize(gun) - help::GetGunShellsInPool(gun));
}

float RechargeButton::GetPriceForOneUnit() const
{
    // RVA 0x468550 - one shell costs whatever its own prototype is worth; note
    // this is the raw base price, with no town or workshop coefficient applied.
    ai::Obj* gun = m_objId == -1 ? nullptr : ai::theObjects->GetEntityByObjId(m_objId);
    if (!gun || !RT_DYNCAST(gun, ai::Obj))
    {
        return 0.0f;
    }
    int const shellPrototypeId = help::GetGunShellPrototypeId(gun);
    if (shellPrototypeId == -1)
    {
        return 0.0f;
    }
    ai::PrototypeInfo const* shellProto = ai::thePrototypeManager->GetPrototypeInfo(shellPrototypeId);
    if (!shellProto)
    {
        return 0.0f;
    }
    int const price = static_cast<int>(shellProto->GetBasePrice());
    return price < 0 ? 0.0f : static_cast<float>(price);
}

CStr const& RechargeButton::GetStrIdTooltipBuy() const
{
    return m_rchrAif.m_strIdTooltipRecharge;
}

CStr const& RechargeButton::GetStrIdTooltipBuyNotNeed() const
{
    return m_rchrAif.m_strIdTooltipRechargeNotNeed;
}

CStr const& RechargeButton::GetStrIdTooltipBuyUnavailable() const
{
    return m_rchrAif.m_strIdTooltipRechargeUnavailable;
}

CStr const& RechargeButton::GetStrIdTooltipBuyPartial() const
{
    return m_rchrAif.m_strIdTooltipRechargePartial;
}

CStr const& RechargeButton::GetStrIdTooltipBuyFull() const
{
    return m_rchrAif.m_strIdTooltipRechargeFull;
}
