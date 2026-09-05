#include "basketcharacteristicswnd.h"

#include "core/log.h"
#include "game/uimisc/guihelper.h"

#include "server/objects/base/objcontainer.h"
#include "server/objects/basket.h"
#include "server/objects/player.h"
#include "server/objects/vehicle.h"

RT_CLASS_EXPORTS_BEGIN(BasketCharacteristicsWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(BasketCharacteristicsWnd);

BasketCharacteristicsWnd::BasketAuxInfo::BasketAuxInfo()
{
    m_wndBasketCapacityValName = "wndBasketCapacityVal";
}

BasketCharacteristicsWnd::BasketCharacteristicsWnd()
{
    m_wndBasketCapacityVal = nullptr;
}

BasketCharacteristicsWnd::BasketCharacteristicsWnd(BasketCharacteristicsWnd const&) : BasketCharacteristicsWnd()
{
    // NOTE: the shipped copy ctor default-constructs the base and AuxInfo but
    // leaves m_wndBasketCapacityVal uninitialised; delegating to the default
    // ctor avoids reading an uninitialised pointer.
}

BasketCharacteristicsWnd::~BasketCharacteristicsWnd()
{
    // m_basketAif's CStr member and the CBCharacteristicsWnd base clean up
    // automatically.
}

m3d::Object* BasketCharacteristicsWnd::Clone()
{
    return new BasketCharacteristicsWnd(*this);
}

m3d::Object* BasketCharacteristicsWnd::CreateObject()
{
    return new BasketCharacteristicsWnd;
}

m3d::Class* BasketCharacteristicsWnd::GetClass() const
{
    return RT_CLASS_LOCAL(BasketCharacteristicsWnd);
}

m3d::Class* BasketCharacteristicsWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(CBCharacteristicsWnd);
}

int BasketCharacteristicsWnd::GameDataSetup()
{
    if (!CBCharacteristicsWnd::GameDataSetup())
    {
        return 0;
    }

    if ((m_gameDataFlags & 2) == 0)
    {
        auto* child = GetChildByName(m_basketAif.m_wndBasketCapacityValName);
        if (child && child->IsKindOf(&m3d::ui::Wnd::m_classWnd))
        {
            m_wndBasketCapacityVal = static_cast<m3d::ui::Wnd*>(child);
        }
        else
        {
            M3D_LOG_INFO(
                "Get control error: control " + m_basketAif.m_wndBasketCapacityValName +
                " is not found or incorrect type");
            m_gameDataFlags &= ~1u;
        }
    }

    if ((m_gameDataFlags & 1) == 0)
    {
        M3D_LOG_INFO("BasketCharacteristicsWnd: error - fail to init because of a bad resource");
        return 0;
    }
    return 1;
}

void BasketCharacteristicsWnd::SetupForCB(int cbId)
{
    CBCharacteristicsWnd::SetupForCB(cbId);
}

void BasketCharacteristicsWnd::SetupForBasket(int basketId)
{
    if (basketId != -1)
    {
        ai::Obj* obj = ai::theObjects->GetEntityByObjId(basketId);
        if (!obj || !obj->IsKindOf(&ai::Basket::m_classBasket))
        {
            basketId = -1;
        }
    }
    SetupForCB(basketId);
}

ai::Basket const* BasketCharacteristicsWnd::GetBasket() const
{
    if (m_cbId == -1)
    {
        return nullptr;
    }
    ai::Obj* obj = ai::theObjects->GetEntityByObjId(m_cbId);
    return (obj && obj->IsKindOf(&ai::Basket::m_classBasket)) ? static_cast<ai::Basket*>(obj) : nullptr;
}

void BasketCharacteristicsWnd::ClearCharacteristics()
{
    if ((m_gameDataFlags & 1) != 0)
    {
        CBCharacteristicsWnd::ClearCharacteristics();
        m_wndBasketCapacityVal->SetText(CStr());
    }
}

void BasketCharacteristicsWnd::UpdateCharacteristics()
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    CBCharacteristicsWnd::UpdateCharacteristics();

    ai::Basket const* basket = GetBasket();
    if (!basket)
    {
        return;
    }
    ai::Vehicle const* vehicle = GetVehicle();
    if (!vehicle)
    {
        return;
    }

    ai::Vehicle* baseVehicle = m_vehicleType != VehicleWnd::VEHICLE_NORMAL
                                   ? (ai::thePlayer ? ai::thePlayer->GetVehicle() : nullptr)
                                   : const_cast<ai::Vehicle*>(vehicle);
    if (!baseVehicle)
    {
        return;
    }

    ai::BasketPrototypeInfo const* basketPi = basket->GetPrototypeInfo();
    if (!basketPi)
    {
        return;
    }

    ai::VehiclePart* basePart = baseVehicle->GetPartByName(basket->GetPartName());
    ai::Basket const* baseBasket =
        (basePart && basePart->IsKindOf(&ai::Basket::m_classBasket)) ? static_cast<ai::Basket*>(basePart) : nullptr;
    ai::BasketPrototypeInfo const* basePi = baseBasket ? baseBasket->GetPrototypeInfo() : nullptr;

    int const capacity = basketPi->GetRepositoryCapacity();
    int const baseCapacity = basePi ? basePi->GetRepositoryCapacity() : 0;

    UpdateValue(m_wndBasketCapacityVal, capacity, baseCapacity, false);
}
