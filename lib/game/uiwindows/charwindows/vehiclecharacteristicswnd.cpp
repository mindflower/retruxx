#include "vehiclecharacteristicswnd.h"

#include <algorithm>

#include "core/log.h"
#include "game/uimisc/guihelper.h"

#include "server/objects/basket.h"
#include "server/objects/player.h"
#include "server/objects/vehicle.h"

RT_CLASS_EXPORTS_BEGIN(VehicleCharacteristicsWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(VehicleCharacteristicsWnd);

VehicleCharacteristicsWnd::AuxInfo::AuxInfo()
{
    m_wndMaxHealthValName = "wndMaxHealthVal";
    m_wndMaxFuelValName = "wndMaxFuelVal";
    m_wndPiercingValName = "wndPiercingVal";
    m_wndBlastValName = "wndBlastVal";
    m_wndEnergyValName = "wndEnergyVal";
    m_wndMaxDurabilityValName = "wndMaxDurabilityVal";
    m_wndBasketCapacityValName = "wndBasketCapacityVal";
    m_wndMaxSpeedValName = "wndMaxSpeedVal";
    m_wndTorqueValName = "wndTorqueVal";
    m_wndWeightValName = "wndWeightVal";
    m_wndControlValName = "wndControlVal";
}

VehicleCharacteristicsWnd::VehicleCharacteristicsWnd()
{
    m_wndMaxHealthVal = 0;
    m_wndMaxFuelVal = 0;
    m_wndPiercingVal = 0;
    m_wndBlastVal = 0;
    m_wndEnergyVal = 0;
    m_wndMaxDurabilityVal = 0;
    m_wndBasketCapacityVal = 0;
    m_wndMaxSpeedVal = 0;
    m_wndTorqueVal = 0;
    m_wndWeightVal = 0;
    m_wndControlVal = 0;
}

VehicleCharacteristicsWnd::VehicleCharacteristicsWnd(VehicleCharacteristicsWnd const&) : VehicleCharacteristicsWnd()
{
    // NOTE: the shipped copy ctor (RVA 0x74BA0) default-constructs the base and
    // AuxInfo, but leaves every Wnd* member uninitialised; delegating to the
    // default ctor here avoids reading uninitialised pointers.
}

VehicleCharacteristicsWnd::~VehicleCharacteristicsWnd()
{
    // RVA 0x74BF0 - m_aif's CStr members and the CharacteristicsWnd base clean
    // up automatically.
}

m3d::Object* VehicleCharacteristicsWnd::Clone()
{
    return new VehicleCharacteristicsWnd(*this);
}

m3d::Object* VehicleCharacteristicsWnd::CreateObject()
{
    return new VehicleCharacteristicsWnd;
}

m3d::Class* VehicleCharacteristicsWnd::GetClass() const
{
    return RT_CLASS_LOCAL(VehicleCharacteristicsWnd);
}

m3d::Class* VehicleCharacteristicsWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(CharacteristicsWnd);
}

int VehicleCharacteristicsWnd::GameDataSetup()
{
    // RVA 0x74C10
    int res = 1;
    if ((m_gameDataFlags & 2) == 0)
    {
        auto find = [this, &res](CStr const& name, m3d::ui::Wnd*& out)
        {
            auto* child = GetChildByName(name);
            if (child && child->IsKindOf(&m3d::ui::Wnd::m_classWnd))
            {
                out = static_cast<m3d::ui::Wnd*>(child);
            }
            else
            {
                M3D_LOG_INFO("Get control error: control " + name + " is not found or incorrect type");
                res = 0;
            }
        };

        find(m_aif.m_wndMaxHealthValName, m_wndMaxHealthVal);
        find(m_aif.m_wndMaxFuelValName, m_wndMaxFuelVal);
        find(m_aif.m_wndPiercingValName, m_wndPiercingVal);
        find(m_aif.m_wndBlastValName, m_wndBlastVal);
        find(m_aif.m_wndEnergyValName, m_wndEnergyVal);
        find(m_aif.m_wndMaxDurabilityValName, m_wndMaxDurabilityVal);
        find(m_aif.m_wndBasketCapacityValName, m_wndBasketCapacityVal);
        find(m_aif.m_wndMaxSpeedValName, m_wndMaxSpeedVal);
        find(m_aif.m_wndTorqueValName, m_wndTorqueVal);
        find(m_aif.m_wndWeightValName, m_wndWeightVal);
        find(m_aif.m_wndControlValName, m_wndControlVal);

        if (res)
        {
            m_gameDataFlags |= 1u;
        }
    }

    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }
    M3D_LOG_INFO("VehicleCharacteristicsWnd: error - fail to init because of a bad resource");
    return 0;
}

void VehicleCharacteristicsWnd::ClearCharacteristics()
{
    // RVA 0x75C90
    if ((m_gameDataFlags & 1) != 0)
    {
        m_wndMaxHealthVal->SetText(CStr());
        m_wndMaxFuelVal->SetText(CStr());
        m_wndPiercingVal->SetText(CStr());
        m_wndBlastVal->SetText(CStr());
        m_wndEnergyVal->SetText(CStr());
        m_wndMaxDurabilityVal->SetText(CStr());
        m_wndBasketCapacityVal->SetText(CStr());
        m_wndMaxSpeedVal->SetText(CStr());
        m_wndTorqueVal->SetText(CStr());
        m_wndWeightVal->SetText(CStr());
        m_wndControlVal->SetText(CStr());
    }
}

void VehicleCharacteristicsWnd::UpdateCharacteristics()
{
    // RVA 0x758B0
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    ai::Vehicle const* vehicle = GetVehicle();
    if (!vehicle)
    {
        return;
    }

    // In the "hacked" (workshop preview) vehicle type, characteristics are
    // shown for the player's own vehicle instead of the previewed one, so the
    // diff-coloring in UpdateValue can compare a pending purchase against it.
    ai::Vehicle* baseVehicle =
        m_vehicleType != VehicleWnd::VEHICLE_NORMAL ? (ai::thePlayer ? ai::thePlayer->GetVehicle() : nullptr)
                                                     : const_cast<ai::Vehicle*>(vehicle);
    if (!baseVehicle)
    {
        return;
    }

    ai::Basket const* basket = vehicle->GetBasket();
    ai::BasketPrototypeInfo const* basketPi = basket ? basket->GetPrototypeInfo() : nullptr;
    if (basket && !basketPi)
    {
        return;
    }
    ai::Basket const* baseBasket = baseVehicle->GetBasket();
    ai::BasketPrototypeInfo const* baseBasketPi = baseBasket ? baseBasket->GetPrototypeInfo() : nullptr;
    if (baseBasket && !baseBasketPi)
    {
        return;
    }

    int const maxHealth = static_cast<int>(vehicle->GetMaxHealth());
    int const maxFuel = static_cast<int>(vehicle->GetMaxFuel());
    int const piercing = static_cast<int>(vehicle->GetFullDurabilityCoeffForDamageType(ai::DAMAGE_PIERCING));
    int const blast = static_cast<int>(vehicle->GetFullDurabilityCoeffForDamageType(ai::DAMAGE_BLAST));
    int const energy = static_cast<int>(vehicle->GetFullDurabilityCoeffForDamageType(ai::DAMAGE_ENERGY));
    int const maxDurability = static_cast<int>(vehicle->GetMaxFullDurability());
    int const basketCapacity = basketPi ? basketPi->GetRepositoryCapacity() : 0;
    int const maxSpeed = static_cast<int>(vehicle->GetMaxSpeed() * 3.6f);
    int const torque = static_cast<int>(vehicle->GetMaxTorque());
    int const weight = static_cast<int>(vehicle->GetMass());
    int const control = std::clamp(static_cast<int>(vehicle->GetControl()), 0, 100);

    // Piercing/Blast/Energy/BasketCapacity/Weight compare directly against
    // baseVehicle's current stat; the rest are sourced through the property
    // system (help::GetBasePropertyValFromVehicle), which reverses every
    // installed gadget modification to recover baseVehicle's pre-upgrade
    // baseline. Property IDs below are as verified from the shipped binary.
    int const baseMaxHealth = static_cast<int>(help::GetBasePropertyValFromVehicle(baseVehicle, 27));
    int const baseMaxFuel = static_cast<int>(help::GetBasePropertyValFromVehicle(baseVehicle, 10));
    int const basePiercing = static_cast<int>(baseVehicle->GetFullDurabilityCoeffForDamageType(ai::DAMAGE_PIERCING));
    int const baseBlast = static_cast<int>(baseVehicle->GetFullDurabilityCoeffForDamageType(ai::DAMAGE_BLAST));
    int const baseEnergy = static_cast<int>(baseVehicle->GetFullDurabilityCoeffForDamageType(ai::DAMAGE_ENERGY));
    int const baseMaxDurability = static_cast<int>(help::GetBasePropertyValFromVehicle(baseVehicle, 20));
    int const baseBasketCapacity = baseBasketPi ? baseBasketPi->GetRepositoryCapacity() : 0;
    int const baseMaxSpeed = static_cast<int>(help::GetBasePropertyValFromVehicle(baseVehicle, 23) * 3.6f);
    int const baseTorque = static_cast<int>(help::GetBasePropertyValFromVehicle(baseVehicle, 22));
    int const baseWeight = static_cast<int>(baseVehicle->GetMass());
    int const baseControl =
        std::clamp(static_cast<int>(help::GetBasePropertyValFromVehicle(baseVehicle, 25)), 0, 100);

    UpdateValue(m_wndMaxHealthVal, maxHealth, baseMaxHealth, false);
    UpdateValue(m_wndMaxFuelVal, maxFuel, baseMaxFuel, false);
    UpdateValue(m_wndPiercingVal, piercing, basePiercing, false);
    UpdateValue(m_wndBlastVal, blast, baseBlast, false);
    UpdateValue(m_wndEnergyVal, energy, baseEnergy, false);
    UpdateValue(m_wndMaxDurabilityVal, maxDurability, baseMaxDurability, false);
    UpdateValue(m_wndBasketCapacityVal, basketCapacity, baseBasketCapacity, false);
    UpdateValue(m_wndMaxSpeedVal, maxSpeed, baseMaxSpeed, false);
    UpdateValue(m_wndTorqueVal, torque, baseTorque, false);
    UpdateValue(m_wndWeightVal, weight, baseWeight, true);
    UpdateValue(m_wndControlVal, control, baseControl, false);
}
