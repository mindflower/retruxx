#include "iteminfownd.h"

#include <core/kernel.h>
#include <game/m3dgame.h>
#include <game/uimanager/truxxuimanager.h>
#include <game/uimisc/guihelper.h>
#include <m3dapp.h>
#include <server/geomrepository.h>
#include <server/objects/base/objcontainer.h>
#include <server/objects/base/prototypeinfo.h>
#include <server/objects/basket.h>
#include <server/objects/cabin.h>
#include <server/objects/gadget.h>
#include <server/objects/guns/compoundgun.h>
#include <server/objects/guns/gun.h>
#include <server/objects/physicbodies/vehiclepart.h>
#include <server/objects/player.h>
#include <server/objects/town.h>
#include <server/objects/ware.h>
#include <server/resourcemanager.h>
#include <server/server.h>

RT_CLASS_EXPORTS_BEGIN(ItemInfoWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(ItemInfoWnd);

ItemInfoWnd::AuxInfo ItemInfoWnd::m_aif;

namespace
{
    // Every row is a full-width line; the description gets extra space above it
    // and the four warning rows are tinted.
    int const ROW_FLAGS = INFOFLAG_FULL_ROW;
    int const ROW_FLAGS_SPACED = INFOFLAG_FULL_ROW | INFOFLAG_BIG_SPACE;
    int const ROW_FLAGS_WARN = INFOFLAG_FULL_ROW | INFOFLAG_WARN_COLOR;

    PointBase<float> const DEFAULT_ICO_SIZE{-1.0f, -1.0f};

    // "<name>: " when the localized property name resolved, otherwise nothing -
    // an unresolved string id produces a bare value rather than ": <value>".
    CStr NamePrefix(CStr const& propName)
    {
        return propName.empty() ? CStr() : propName + ": ";
    }

    CStr Localized(CStr const& id)
    {
        return M3D_APP->GetStringByStringId0(id);
    }

    // "<name> (<unitsId>)" - the unit label is itself a string id.
    CStr NameWithUnits(CStr const& propNameId, char const* unitsId)
    {
        return Localized(propNameId) + " (" + Localized(unitsId) + ")";
    }
}  // namespace

// ===========================================================================
//  ItemInfoWnd::AuxInfo
// ===========================================================================

ItemInfoWnd::AuxInfo::AuxInfo()
{
    // RVA 0x4531F0 - the five rows that have no localized caption of their own
    // (description, incompatible, too rich, broken, no ammo) are left empty.
    m_propStrIds[PROP_FULL_NAME] = "FullName";
    m_propStrIds[PROP_AMOUNT] = "Amount";
    m_propStrIds[PROP_ARMOR] = "Armor";
    m_propStrIds[PROP_SOLIDITY] = "Solidity";
    m_propStrIds[PROP_SPEED] = "MaxSpeed";
    m_propStrIds[PROP_TORQUE] = "Torque";
    m_propStrIds[PROP_POWER] = "EnginePower";
    m_propStrIds[PROP_DAMAGE] = "Damage";
    m_propStrIds[PROP_FIRING_RATE] = "FiringRate";
    m_propStrIds[PROP_FIRING_RANGE] = "FiringRange";
    m_propStrIds[PROP_PRICE] = "Price";
    m_propStrIds[PROP_RESISTANCE] = "Resistance";
    m_propStrIds[PROP_SHELLPOOL] = "ShellPool";
    m_propStrIds[PROP_CHARGE_SIZE] = "ChargeSize";
    m_propStrIds[PROP_RECHARGING_TIME] = "RechargingTime";
    m_propStrIds[PROP_REPOSITORY_CAPACITY] = "BasketCapacity";
    m_propStrIds[PROP_GADGET_TYPE] = "GadgetType";
    m_propStrIds[PROP_ACCURACY] = "Accuracy";
    m_propStrIds[PROP_FIRING_TYPE] = "GunType";
}

ItemInfoWnd::AuxInfo::AuxInfo(ItemInfoWnd::AuxInfo const& rhs)
{
    for (int i = 0; i < PROP_NUM_PROPS; ++i)
    {
        m_propStrIds[i] = rhs.m_propStrIds[i];
    }
}

// ===========================================================================
//  ItemInfoWnd
// ===========================================================================

ItemInfoWnd::ItemInfoWnd() : m_infoType(INFOTYPE_REPOSITORY_ITEM), m_objId(-1)
{
}

ItemInfoWnd::ItemInfoWnd(ItemInfoWnd const&) : ItemInfoWnd()
{
    // NOTE: the shipped copy ctor (RVA 0x4536A0) only builds the base, empties
    // m_auxFullName and nulls m_srcWnd; nothing is copied from rhs.
}

ItemInfoWnd::~ItemInfoWnd() = default;

m3d::Object* ItemInfoWnd::Clone()
{
    // RVA 0x453130
    return new ItemInfoWnd(*this);
}

m3d::Object* ItemInfoWnd::CreateObject()
{
    return new ItemInfoWnd;
}

m3d::Class* ItemInfoWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(InfoWnd);
}

m3d::Class* ItemInfoWnd::GetClass() const
{
    return RT_CLASS_LOCAL(ItemInfoWnd);
}

ai::GeomRepositoryItem const& ItemInfoWnd::GetItem() const
{
    return m_repositoryItem;
}

ref_ptr<m3d::ui::Wnd> ItemInfoWnd::GetSrcWnd() const
{
    return m_srcWnd;
}

ai::Obj const* ItemInfoWnd::GetObjectA() const
{
    // RVA 0x457180
    int objId;
    switch (m_infoType)
    {
    case INFOTYPE_REPOSITORY_ITEM:
        objId = m_repositoryItem.GetObjId();
        break;
    case INFOTYPE_OBJECT:
        objId = m_objId;
        break;
    default:
        return nullptr;
    }
    return ai::theObjects->GetEntityByObjId(objId);
}

bool ItemInfoWnd::IsValid() const
{
    // RVA 0x454020 - a repository item also needs a repository behind it.
    if (m_infoType == INFOTYPE_REPOSITORY_ITEM)
    {
        return m_repositoryItem.IsValid() && m_repositoryItem.m_parentRepository != nullptr;
    }
    return m_infoType == INFOTYPE_OBJECT && GetObjectA() != nullptr;
}

int ItemInfoWnd::GetResourceId() const
{
    // RVA 0x457100
    if (m_infoType == INFOTYPE_REPOSITORY_ITEM)
    {
        return m_repositoryItem.GetResourceId();
    }
    if (m_infoType == INFOTYPE_OBJECT)
    {
        if (ai::Obj const* obj = GetObjectA())
        {
            if (ai::PrototypeInfo const* prototypeInfo = obj->GetPrototypeInfo())
            {
                return prototypeInfo->m_resourceId;
            }
        }
    }
    return -1;
}

bool ItemInfoWnd::NeedShowPrice() const
{
    // RVA 0x457170 - the shipped body is an unconditional "yes".
    return true;
}

bool ItemInfoWnd::NeedShowChargeSize() const
{
    // RVA 0x4571E0 - mines, turbo and smoke have no meaningful charge size.
    ai::Obj const* gun = GetObjectA();
    if (!gun || (!gun->IsKindOf(&ai::Gun::m_classGun) && !gun->IsKindOf(&ai::CompoundGun::m_classCompoundGun)))
    {
        return false;
    }

    std::vector<ai::FiringTypes> firingTypesToNotShowChargeSize;
    firingTypesToNotShowChargeSize.push_back(ai::FT_MINE);
    firingTypesToNotShowChargeSize.push_back(ai::FT_TURBO);
    firingTypesToNotShowChargeSize.push_back(ai::FT_SMOKE);

    ai::FiringTypes const firingType = help::GetGunFiringType(gun);
    if (firingType == ai::FT_NUM_FIRING_TYPES)
    {
        return false;
    }
    return std::find(
               firingTypesToNotShowChargeSize.begin(), firingTypesToNotShowChargeSize.end(), firingType) ==
           firingTypesToNotShowChargeSize.end();
}

void ItemInfoWnd::SetUpForRepositoryItem(ai::GeomRepositoryItem const& item, ref_ptr<m3d::ui::Wnd> srcWnd)
{
    // RVA 0x453740 - the caption comes from the live object when there is one,
    // and from the prototype otherwise (an item that is only a stack count).
    GameDataClear(false);
    if (!item.IsValid())
    {
        return;
    }

    m_srcWnd = srcWnd;
    m_infoType = INFOTYPE_REPOSITORY_ITEM;
    m_repositoryItem = item;

    if (item.GetObjId() == -1)
    {
        m_auxFullName = ai::pServer->GetPrototypeFullName(item.GetIcoName());
    }
    else
    {
        ai::Obj const* obj = ai::theObjects->GetEntityByObjId(item.GetObjId());
        m_auxFullName = obj->GetFullDescriptionWithAffixes();
    }
    Update(true);
}

void ItemInfoWnd::SetUpForObject(int objId, ref_ptr<m3d::ui::Wnd> srcWnd)
{
    // RVA 0x4538D0
    GameDataClear(false);
    m_srcWnd = srcWnd;
    m_infoType = INFOTYPE_OBJECT;
    m_objId = objId;

    if (ai::Obj const* obj = GetObjectA())
    {
        m_auxFullName = obj->GetFullDescriptionWithAffixes();
        Update(true);
    }
}

int ItemInfoWnd::GameDataClear(bool beforeContinuousLevel)
{
    // RVA 0x454070
    InfoWnd::GameDataClear(beforeContinuousLevel);
    m_repositoryItem.Invalidate();
    m_objId = -1;
    m_auxFullName = CStr();
    m_infoType = INFOTYPE_OBJECT;
    m_srcWnd = nullptr;
    return 1;
}

int ItemInfoWnd::GameDataUpdate(void* data, int dataType)
{
    // RVA 0x4539C0 - repository changes only matter for repository items, and
    // per-frame refreshes only for live objects.
    if (!IsValid())
    {
        return 0;
    }

    if (dataType == 58)
    {
        if (data && m_infoType == INFOTYPE_REPOSITORY_ITEM &&
            reinterpret_cast<ai::GeomRepository*>(static_cast<int*>(data)[13]) ==
                m_repositoryItem.m_parentRepository)
        {
            Update(false);
        }
    }
    else if (dataType == 89 && m_infoType == INFOTYPE_OBJECT)
    {
        Update(false);
    }
    return 1;
}

int ItemInfoWnd::Update(bool firstUpdate)
{
    // RVA 0x453A50 - picks the row set by resource kind. Note the argument that
    // reaches ShowInfoFor* is `!firstUpdate`: the first pass builds the rows,
    // later ones only refresh their text.
    if (!IsValid())
    {
        return 0;
    }

    int const resourceId = GetResourceId();
    if (resourceId == -1)
    {
        return 0;
    }

    bool const updateOnly = !firstUpdate;
    auto const isKindOf = [&](char const* resourceName)
    {
        return ai::theResourceManager->bResourceIsKindOf(
            resourceId, ai::theResourceManager->GetResourceId(CStr(resourceName)));
    };

    if (isKindOf("CABIN"))
    {
        return ShowInfoForCabin(updateOnly);
    }
    if (isKindOf("BASKET"))
    {
        return ShowInfoForBasket(updateOnly);
    }
    if (isKindOf("GUN"))
    {
        return ShowInfoForGun(updateOnly);
    }
    if (isKindOf("GOODS"))
    {
        return ShowInfoForGoods(updateOnly);
    }
    if (isKindOf("GOLD"))
    {
        // NOTE: gold goes straight to the base rows rather than through
        // ShowInfoForGold(), which is only reachable from nowhere; as shipped.
        return ShowInfoBase(updateOnly) != 0;
    }
    if (isKindOf("GADGET"))
    {
        return ShowInfoForGadget(updateOnly);
    }
    return ShowInfoBase(updateOnly);
}

int ItemInfoWnd::ShowInfoBase(bool updateOnly)
{
    // RVA 0x453D10 - the rows every item gets: name, stack count, and the two
    // warnings that apply regardless of kind.
    if (!IsValid())
    {
        return 0;
    }

    if (!updateOnly)
    {
        m_items.clear();
    }

    int res = ShowPropertyInfo(PROP_FULL_NAME, updateOnly) & 1;
    if (m_infoType == INFOTYPE_REPOSITORY_ITEM)
    {
        res &= ShowPropertyInfo(PROP_AMOUNT, updateOnly);
    }
    res &= ShowPropertyInfo(PROP_UNCOMPATIBLE, updateOnly);
    return res & ShowPropertyInfo(PROP_TOO_RICH, updateOnly);
}

int ItemInfoWnd::ShowInfoForVehiclePart(bool updateOnly)
{
    // RVA 0x453DA0
    if (!ShowInfoBase(updateOnly))
    {
        return 0;
    }
    return ShowPropertyInfo(PROP_ARMOR, updateOnly) & 1;
}

int ItemInfoWnd::ShowInfoForCabin(bool updateOnly)
{
    // RVA 0x453DD0
    if (!ShowInfoBase(updateOnly))
    {
        return 0;
    }
    int res = ShowPropertyInfo(PROP_POWER, updateOnly) & 1;
    res &= ShowPropertyInfo(PROP_SPEED, updateOnly);
    res &= ShowPropertyInfo(PROP_ARMOR, updateOnly);
    res &= ShowPropertyInfo(PROP_RESISTANCE, updateOnly);
    return res & ShowPropertyInfo(PROP_PRICE, updateOnly);
}

int ItemInfoWnd::ShowInfoForBasket(bool updateOnly)
{
    // RVA 0x453E40
    if (!ShowInfoBase(updateOnly))
    {
        return 0;
    }
    int res = ShowPropertyInfo(PROP_REPOSITORY_CAPACITY, updateOnly) & 1;
    res &= ShowPropertyInfo(PROP_ARMOR, updateOnly);
    res &= ShowPropertyInfo(PROP_RESISTANCE, updateOnly);
    return res & ShowPropertyInfo(PROP_PRICE, updateOnly);
}

int ItemInfoWnd::ShowInfoForGun(bool updateOnly)
{
    // RVA 0x453EA0
    if (!ShowInfoBase(updateOnly))
    {
        return 0;
    }

    int res = ShowPropertyInfo(PROP_BROKEN, updateOnly) & 1;
    res &= ShowPropertyInfo(PROP_NO_AMMO, updateOnly);
    res &= ShowPropertyInfo(PROP_FIRING_TYPE, updateOnly);
    res &= ShowPropertyInfo(PROP_DAMAGE, updateOnly);
    res &= ShowPropertyInfo(PROP_FIRING_RATE, updateOnly);
    res &= ShowPropertyInfo(PROP_FIRING_RANGE, updateOnly);
    res &= ShowPropertyInfo(PROP_ACCURACY, updateOnly);
    if (NeedShowChargeSize())
    {
        res &= ShowPropertyInfo(PROP_CHARGE_SIZE, updateOnly);
    }
    res &= ShowPropertyInfo(PROP_SHELLPOOL, updateOnly);
    res &= ShowPropertyInfo(PROP_RECHARGING_TIME, updateOnly);
    res &= ShowPropertyInfo(PROP_SOLIDITY, updateOnly);
    return res & ShowPropertyInfo(PROP_PRICE, updateOnly);
}

int ItemInfoWnd::ShowInfoForGoods(bool updateOnly)
{
    // RVA 0x453F80
    if (!ShowInfoBase(updateOnly))
    {
        return 0;
    }
    int const res = ShowPropertyInfo(PROP_DESCRIPTION, updateOnly) & 1;
    return res & ShowPropertyInfo(PROP_PRICE, updateOnly);
}

int ItemInfoWnd::ShowInfoForGold(bool updateOnly)
{
    // RVA 0x453FC0
    return ShowInfoBase(updateOnly) != 0;
}

int ItemInfoWnd::ShowInfoForGadget(bool updateOnly)
{
    // RVA 0x453FD0 - note the three row results are discarded and success is
    // reported regardless; as shipped.
    if (!ShowInfoBase(updateOnly))
    {
        return 0;
    }
    ShowPropertyInfo(PROP_DESCRIPTION, updateOnly);
    ShowPropertyInfo(PROP_GADGET_TYPE, updateOnly);
    ShowPropertyInfo(PROP_PRICE, updateOnly);
    return 1;
}

int ItemInfoWnd::ShowPropertyInfo(ItemInfoWnd::Property prop, bool updateOnly)
{
    // RVA 0x454110 - builds one row's text and either adds it or, on a refresh
    // pass, re-sets the text of the row that is already there.
    if (!IsValid())
    {
        return 0;
    }

    ai::Obj const* obj = GetObjectA();

    // Adds the row on the first pass, refreshes it afterwards. A refresh for a
    // row that was never added is a failure.
    auto const emit = [&](int flags, CStr const& text) -> int
    {
        if (!updateOnly)
        {
            AddItemById(prop, InfoItem(flags, text, m3d::rend::TexHandle(), DEFAULT_ICO_SIZE));
            return 1;
        }
        InfoItem* item = GetItemById(prop);
        if (!item)
        {
            return 0;
        }
        item->SetString(text);
        return 1;
    };

    switch (prop)
    {
    case PROP_FULL_NAME:
        if (!updateOnly)
        {
            AddItemById(prop, InfoItem(INFOFLAG_TITLE, m_auxFullName, m3d::rend::TexHandle(), DEFAULT_ICO_SIZE));
        }
        return 1;

    case PROP_AMOUNT:
    {
        if (m_infoType != INFOTYPE_REPOSITORY_ITEM)
        {
            return 1;
        }
        unsigned const amount = m_repositoryItem.GetAmount();
        if (amount <= 1)
        {
            return 1;
        }
        return emit(ROW_FLAGS, NamePrefix(Localized(m_aif.m_propStrIds[prop])) + CStr(static_cast<int>(amount)));
    }

    case PROP_ARMOR:
    {
        if (!obj || !obj->IsKindOf(&ai::VehiclePart::m_classVehiclePart))
        {
            return 0;
        }
        auto const* part = static_cast<ai::VehiclePart const*>(obj);
        int const cur = static_cast<int>(part->Durability().value().get());
        int const max = static_cast<int>(part->Durability().maxValue().get());
        return emit(
            ROW_FLAGS,
            NamePrefix(NameWithUnits(m_aif.m_propStrIds[prop], "CurMax")) + CStr(cur) + "/" + CStr(max));
    }

    case PROP_SOLIDITY:
    {
        if (!obj)
        {
            return 0;
        }
        float cur;
        float max;
        if (obj->IsKindOf(&ai::Gun::m_classGun) || obj->IsKindOf(&ai::CompoundGun::m_classCompoundGun))
        {
            cur = help::GetGunDurability(obj);
            max = help::GetGunMaxDurability(obj);
        }
        else if (obj->IsKindOf(&ai::Ware::m_classWare))
        {
            auto const* ware = static_cast<ai::Ware const*>(obj);
            cur = ware->Durability().value().get();
            max = ware->Durability().maxValue().get();
        }
        else
        {
            return 0;
        }
        return emit(
            ROW_FLAGS,
            NamePrefix(NameWithUnits(m_aif.m_propStrIds[prop], "CurMax")) + CStr(static_cast<int>(cur)) + "/" +
                CStr(static_cast<int>(max)));
    }

    case PROP_SPEED:
    {
        if (!obj || !obj->IsKindOf(&ai::Cabin::m_classCabin))
        {
            return 0;
        }
        auto const* cabin = static_cast<ai::Cabin const*>(obj);
        return emit(
            ROW_FLAGS,
            NamePrefix(NameWithUnits(m_aif.m_propStrIds[prop], "KmInSec")) +
                CStr(static_cast<int>(cabin->GetMaxSpeed())));
    }

    case PROP_TORQUE:
    {
        if (!obj || !obj->IsKindOf(&ai::Cabin::m_classCabin))
        {
            return 0;
        }
        auto const* cabin = static_cast<ai::Cabin const*>(obj);
        return emit(ROW_FLAGS, NamePrefix(Localized(m_aif.m_propStrIds[prop])) + CStr(cabin->GetMaxTorque()));
    }

    case PROP_POWER:
    {
        if (!obj || !obj->IsKindOf(&ai::Cabin::m_classCabin))
        {
            return 0;
        }
        auto const* cabin = static_cast<ai::Cabin const*>(obj);
        return emit(
            ROW_FLAGS,
            NamePrefix(Localized(m_aif.m_propStrIds[prop])) + CStr(static_cast<int>(cabin->GetMaxPower())));
    }

    case PROP_DAMAGE:
    {
        if (!obj)
        {
            return 0;
        }
        float const damage = help::GetGunDamage(obj);
        if (damage == 0.0f)
        {
            return 1;
        }
        CStr const damageTypeName = Localized(help::DamageType2Str(help::GetGunDamageType(obj)));
        return emit(
            ROW_FLAGS,
            NamePrefix(Localized(m_aif.m_propStrIds[prop]) + " (" + damageTypeName + ")") +
                CStr(static_cast<int>(damage)));
    }

    case PROP_FIRING_RATE:
    {
        if (!obj)
        {
            return 0;
        }
        float firingRate;
        if (auto const* g = RT_DYNCAST(obj, ai::Gun const))
        {
            firingRate = g->GetFiringRate();
        }
        else if (auto const* cg = RT_DYNCAST(obj, ai::CompoundGun const))
        {
            firingRate = cg->GetFiringRate();
        }
        else
        {
            return 0;
        }
        return emit(
            ROW_FLAGS,
            NamePrefix(NameWithUnits(m_aif.m_propStrIds[prop], "ShotInMinute")) +
                CStr(static_cast<int>(firingRate)));
    }

    case PROP_FIRING_RANGE:
    {
        if (!obj)
        {
            return 0;
        }
        float firingRange;
        if (auto const* g = RT_DYNCAST(obj, ai::Gun const))
        {
            firingRange = g->GetFiringRange();
        }
        else if (auto const* cg = RT_DYNCAST(obj, ai::CompoundGun const))
        {
            firingRange = cg->GetFiringRange();
        }
        else
        {
            return 0;
        }
        return emit(
            ROW_FLAGS,
            NamePrefix(NameWithUnits(m_aif.m_propStrIds[prop], "Meter")) + CStr(static_cast<int>(firingRange)));
    }

    case PROP_PRICE:
    {
        if (!obj)
        {
            return 0;
        }
        int const price = help::GetPriceSmart(obj->GetId());
        CStr priceStr(price);
        if (price == -2)
        {
            priceStr = Localized("NotSellable");
        }
        else if (price == -3)
        {
            priceStr = Localized("NotBuyable");
        }
        else if (price < 0)
        {
            priceStr = CStr(0);
        }

        // Away from a town there is no trade price, only the list price.
        CStr const caption = M3D_APP->m_pInterfaceManager->GetCurrentTown()
                                 ? Localized(m_aif.m_propStrIds[prop])
                                 : Localized("BasePrice");
        return emit(ROW_FLAGS, NamePrefix(caption) + priceStr);
    }

    case PROP_RESISTANCE:
    {
        if (!obj || !obj->IsKindOf(&ai::VehiclePart::m_classVehiclePart))
        {
            return 0;
        }
        auto const* part = static_cast<ai::VehiclePart const*>(obj);

        CStr damageTypes;
        CStr values;
        for (int i = 0; i < 3; ++i)
        {
            damageTypes += Localized(help::DamageType2Str(static_cast<ai::DamageType>(i)));
            values += CStr(part->GetDurabilityCoeffForDamageType(static_cast<ai::DamageType>(i)));
            if (i < 2)
            {
                damageTypes += "/";
                values += "/";
            }
        }
        return emit(
            ROW_FLAGS,
            NamePrefix(Localized(m_aif.m_propStrIds[prop]) + " (" + damageTypes + ")") + values);
    }

    case PROP_SHELLPOOL:
    {
        if (!obj)
        {
            return 0;
        }
        if (!help::IsGunWithCharging(obj) || !help::IsGunWithShellsPoolLimit(obj))
        {
            return 1;
        }
        return emit(
            ROW_FLAGS,
            NamePrefix(NameWithUnits(m_aif.m_propStrIds[prop], "CurMax")) +
                CStr(static_cast<int>(help::GetGunShellsInPool(obj))) + "/" +
                CStr(static_cast<int>(help::GetGunShellsPoolSize(obj))));
    }

    case PROP_CHARGE_SIZE:
    {
        if (!obj)
        {
            return 0;
        }
        if (!help::IsGunWithCharging(obj))
        {
            return 1;
        }
        return emit(
            ROW_FLAGS,
            NamePrefix(NameWithUnits(m_aif.m_propStrIds[prop], "CurMax")) +
                CStr(static_cast<int>(help::GetGunShellsInCurrentCharge(obj))) + "/" +
                CStr(static_cast<int>(help::GetGunChargeSize(obj))));
    }

    case PROP_RECHARGING_TIME:
    {
        if (updateOnly)
        {
            return 1;
        }
        if (!obj)
        {
            return 0;
        }
        bool withCharging;
        float rechargingTime;
        if (auto const* g = RT_DYNCAST(obj, ai::Gun const))
        {
            withCharging = g->IsWithCharging();
            rechargingTime = g->GetRechargingTime();
        }
        else if (auto const* cg = RT_DYNCAST(obj, ai::CompoundGun const))
        {
            withCharging = cg->IsWithCharging();
            rechargingTime = cg->GetRechargingTime();
        }
        else
        {
            return 0;
        }
        if (!withCharging)
        {
            return 1;
        }
        return emit(
            ROW_FLAGS,
            NamePrefix(NameWithUnits(m_aif.m_propStrIds[prop], "Secunde")) + help::ftoa(rechargingTime, -1));
    }

    case PROP_REPOSITORY_CAPACITY:
    {
        if (updateOnly)
        {
            return 1;
        }
        if (!obj || !obj->IsKindOf(&ai::Basket::m_classBasket))
        {
            return 0;
        }
        auto const* prototypeInfo = static_cast<ai::BasketPrototypeInfo const*>(obj->GetPrototypeInfo());
        if (!prototypeInfo)
        {
            return 0;
        }
        return emit(
            ROW_FLAGS,
            NamePrefix(Localized(m_aif.m_propStrIds[prop])) + CStr(prototypeInfo->GetRepositoryCapacity()));
    }

    case PROP_DESCRIPTION:
    {
        if (updateOnly)
        {
            return 1;
        }
        if (!obj)
        {
            return 0;
        }
        ai::PrototypeInfo const* prototypeInfo = obj->GetPrototypeInfo();
        if (!prototypeInfo)
        {
            return 0;
        }
        return emit(ROW_FLAGS_SPACED, Localized(prototypeInfo->m_prototypeName + "_diz"));
    }

    case PROP_GADGET_TYPE:
    {
        if (updateOnly)
        {
            return 1;
        }
        if (!obj || !obj->IsKindOf(&ai::Gadget::m_classGadget))
        {
            return 0;
        }
        ai::PrototypeInfo const* prototypeInfo = obj->GetPrototypeInfo();
        if (!prototypeInfo)
        {
            return 0;
        }
        CStr const resourceName = ai::theResourceManager->GetResourceName(prototypeInfo->m_resourceId);
        if (resourceName.empty())
        {
            return 0;
        }
        return emit(
            ROW_FLAGS, NamePrefix(Localized(m_aif.m_propStrIds[prop])) + Localized(resourceName));
    }

    case PROP_UNCOMPATIBLE:
    {
        if (updateOnly)
        {
            return 1;
        }
        if (!obj)
        {
            return 0;
        }
        if (!obj->IsKindOf(&ai::VehiclePart::m_classVehiclePart) && !obj->IsKindOf(&ai::Gadget::m_classGadget))
        {
            return 1;
        }
        int const playerVehicleId = help::GetPlayerVehicleId();
        if (playerVehicleId == -1 || help::IsChildObjCompatibleWithVehicle(obj->GetId(), playerVehicleId))
        {
            return 1;
        }
        return emit(ROW_FLAGS_WARN, Localized("NotCompatible"));
    }

    case PROP_TOO_RICH:
    {
        if (updateOnly)
        {
            return 1;
        }
        if (!obj)
        {
            return 0;
        }
        if (help::GetObjectOwnerType(obj->GetId()) != help::OWNER_TOWN)
        {
            return 1;
        }
        ai::Town const* town = M3D_APP->m_pInterfaceManager->GetCurrentTown();
        if (!town)
        {
            return 0;
        }
        int const price = help::GetBuyPriceByObjId(obj->GetId(), town->GetId());
        if (price < 0 || price <= static_cast<int>(ai::thePlayer->GetMoney()))
        {
            return 1;
        }
        return emit(ROW_FLAGS_WARN, Localized("TooRich"));
    }

    case PROP_ACCURACY:
    {
        if (updateOnly)
        {
            return 1;
        }
        if (!obj)
        {
            return 0;
        }
        return emit(
            ROW_FLAGS,
            NamePrefix(NameWithUnits(m_aif.m_propStrIds[prop], "percent")) +
                CStr(static_cast<int>(help::GetGunAccuracy(obj))));
    }

    case PROP_FIRING_TYPE:
    {
        if (!obj)
        {
            return 0;
        }
        CStr const firingTypeName =
            Localized(ai::GunPrototypeInfo::FiringType2Str(help::GetGunFiringType(obj)));
        return emit(ROW_FLAGS, NamePrefix(Localized(m_aif.m_propStrIds[prop])) + firingTypeName);
    }

    case PROP_BROKEN:
    {
        if (updateOnly)
        {
            return 1;
        }
        if (!obj)
        {
            return 0;
        }
        if ((!obj->IsKindOf(&ai::Gun::m_classGun) && !obj->IsKindOf(&ai::CompoundGun::m_classCompoundGun)) ||
            help::IsGunDurabilityEnoughForFiring(obj))
        {
            return 1;
        }
        return emit(ROW_FLAGS_WARN, Localized("Broken"));
    }

    case PROP_NO_AMMO:
    {
        if (updateOnly)
        {
            return 1;
        }
        if (!obj)
        {
            return 0;
        }
        if (!obj->IsKindOf(&ai::Gun::m_classGun) && !obj->IsKindOf(&ai::CompoundGun::m_classCompoundGun))
        {
            return 1;
        }
        if (!help::IsGunWithCharging(obj) || !help::IsGunWithShellsPoolLimit(obj))
        {
            return 1;
        }
        if (help::GetGunShellsInPool(obj) + help::GetGunShellsInCurrentCharge(obj))
        {
            return 1;
        }
        return emit(ROW_FLAGS_WARN, Localized("NoAmmo"));
    }

    default:
        return 0;
    }
}
