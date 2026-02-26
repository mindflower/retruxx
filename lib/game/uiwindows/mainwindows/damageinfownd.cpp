#include "damageinfownd.h"
#include "fuelindicatorinmaininterfacewnd.h"
#include "durabilityindicatorinmaininterfacewnd.h"
#include "healthindicatorinmaininterfacewnd.h"
#include "core/log.h"
#include "game/m3dgame.h"
#include "game/uimanager/uidefs.h"

RT_CLASS_EXPORTS_BEGIN(DamageInfoWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(DamageInfoWnd);

DamageInfoWnd::AuxInfo::AuxInfo()
{
    m_wndHealthName = "wndHealth";
    m_wndCabinDurabilityName = "wndCabinDurability";
    m_wndBasketDurabilityName = "wndBasketDurability";
    m_wndFuelName = "wndFuel";
}

m3d::Object* DamageInfoWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* DamageInfoWnd::GetClass() const
{
    return RT_CLASS_LOCAL(DamageInfoWnd);
}

m3d::Object* DamageInfoWnd::CreateObject()
{
    return new DamageInfoWnd;
}

m3d::Class* DamageInfoWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

DamageInfoWnd::~DamageInfoWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int DamageInfoWnd::UpdateOnPlayerVehicleChanged()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int DamageInfoWnd::GameDataUpdate(void*, int dataType)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (dataType == 64)
    {
        UpdateOnPlayerVehicleChanged();
    }
    return 1;
}

int DamageInfoWnd::GameDataSetup()
{
    using namespace m3d::ui;

    if ((m_gameDataFlags & 2) == 0)
    {
        int res = 1;

        if (auto child = RT_DYNCAST(GetChildByName(m_aif.m_wndHealthName), Wnd))
        {
            m_wndHealth =
                static_cast<HealthIndicatorInMainInterfaceWnd*>(M3D_KERNEL->New("HealthIndicatorInMainInterfaceWnd"));
            if (m_wndHealth)
            {
                if (!m_wndHealth->CreateFromPattern(child, true))
                {
                    M3D_LOG_INFO("Make control error: cannot create " + m_aif.m_wndHealthName + " from pattern class");
                    res = 0;
                }
            }
            else
            {
                M3D_LOG_INFO(
                    "Make control error: cannot create " + m_aif.m_wndHealthName +
                    " - cannot find rtti class HealthIndicatorInMainInterfaceWnd");
                res = 0;
            }
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_wndHealthName + " is not found or incorrect type");
            res = 0;
        }

        if (auto child = RT_DYNCAST(GetChildByName(m_aif.m_wndCabinDurabilityName), ImageWnd))
        {
            m_wndCabinDurability = static_cast<DurabilityIndicatorInMainInterfaceWnd*>(
                M3D_KERNEL->New("DurabilityIndicatorInMainInterfaceWnd"));
            if (m_wndCabinDurability)
            {
                if (!m_wndCabinDurability->CreateFromPattern(child, true))
                {
                    M3D_LOG_INFO(
                        "Make control error: cannot create " + m_aif.m_wndCabinDurabilityName + " from pattern class");
                    res = 0;
                }
            }
            else
            {
                M3D_LOG_INFO(
                    "Make control error: cannot create " + m_aif.m_wndCabinDurabilityName +
                    " - cannot find rtti class DurabilityIndicatorInMainInterfaceWnd");
                res = 0;
            }
        }
        else
        {
            M3D_LOG_INFO(
                "Get control error: control " + m_aif.m_wndCabinDurabilityName + " is not found or incorrect type");
            res = 0;
        }

        if (auto child = RT_DYNCAST(GetChildByName(m_aif.m_wndBasketDurabilityName), ImageWnd))
        {
            m_wndBasketDurability = static_cast<DurabilityIndicatorInMainInterfaceWnd*>(
                M3D_KERNEL->New("DurabilityIndicatorInMainInterfaceWnd"));
            if (m_wndBasketDurability)
            {
                if (!m_wndBasketDurability->CreateFromPattern(child, true))
                {
                    M3D_LOG_INFO(
                        "Make control error: cannot create " + m_aif.m_wndBasketDurabilityName + " from pattern class");
                    res = 0;
                }
            }
            else
            {
                M3D_LOG_INFO(
                    "Make control error: cannot create " + m_aif.m_wndBasketDurabilityName +
                    " - cannot find rtti class DurabilityIndicatorInMainInterfaceWnd");
                res = 0;
            }
        }
        else
        {
            M3D_LOG_INFO(
                "Get control error: control " + m_aif.m_wndBasketDurabilityName + " is not found or incorrect type");
            res = 0;
        }

        if (auto child = RT_DYNCAST(GetChildByName(m_aif.m_wndFuelName), Wnd))
        {
            m_wndFuel =
                static_cast<FuelIndicatorInMainInterfaceWnd*>(M3D_KERNEL->New("FuelIndicatorInMainInterfaceWnd"));
            if (m_wndFuel)
            {
                if (!m_wndFuel->CreateFromPattern(child, true))
                {
                    M3D_LOG_INFO("Make control error: cannot create " + m_aif.m_wndFuelName + " from pattern class");
                    res = 0;
                }
            }
            else
            {
                M3D_LOG_INFO(
                    "Make control error: cannot create " + m_aif.m_wndFuelName +
                    " - cannot find rtti class FuelIndicatorInMainInterfaceWnd");
                res = 0;
            }
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_wndFuelName + " is not found or incorrect type");
            res = 0;
        }

        if (res)
        {
            int healthId = -1;
            int cabinDurId = -1;
            int basketDurId = -1;
            int fuelId = -1;

            M3D_APP->m_pInterfaceManager->AddWindow(m_wndHealth.get(), healthId, true, false);
            M3D_APP->m_pInterfaceManager->AddWindow(m_wndCabinDurability.get(), cabinDurId, true, false);
            M3D_APP->m_pInterfaceManager->AddWindow(m_wndBasketDurability.get(), basketDurId, true, false);
            M3D_APP->m_pInterfaceManager->AddWindow(m_wndFuel.get(), fuelId, true, false);

            M3D_APP->m_pInterfaceManager->SetEventsForWindow(healthId, {89});
            M3D_APP->m_pInterfaceManager->SetEventsForWindow(cabinDurId, {89, 65});
            M3D_APP->m_pInterfaceManager->SetEventsForWindow(basketDurId, {89, 65});
            M3D_APP->m_pInterfaceManager->SetEventsForWindow(fuelId, {89});

            // TODO: check this
            m_wndHealth->SetType(
                m_guiId == IW_WND_DAMAGEINFO ? HealthIndicatorInMainInterfaceWnd::TYPE_IN_MAIN_INTERFACE :
                                               HealthIndicatorInMainInterfaceWnd::TYPE_IN_CHARACTERISTIC_WND);

            m_wndCabinDurability->SetType(
                m_guiId == IW_WND_DAMAGEINFO ? DurabilityIndicatorInMainInterfaceWnd::TYPE_IN_MAIN_INTERFACE :
                                               DurabilityIndicatorInMainInterfaceWnd::TYPE_IN_CHARACTERISTIC_WND,
                "CABIN");

            m_wndBasketDurability->SetType(
                m_guiId == IW_WND_DAMAGEINFO ? DurabilityIndicatorInMainInterfaceWnd::TYPE_IN_MAIN_INTERFACE :
                                               DurabilityIndicatorInMainInterfaceWnd::TYPE_IN_CHARACTERISTIC_WND,
                "BASKET");

            m_wndFuel->SetType(
                m_guiId == IW_WND_DAMAGEINFO ? FuelIndicatorInMainInterfaceWnd::TYPE_IN_MAIN_INTERFACE :
                                               FuelIndicatorInMainInterfaceWnd::TYPE_IN_CHARACTERISTIC_WND);

            MoveChildToFirstPosition(m_wndHealth);
            MoveChildToFirstPosition(m_wndFuel);
            MoveChildToFirstPosition(m_wndCabinDurability);
            MoveChildToFirstPosition(m_wndBasketDurability);

            m_gameDataFlags |= 1u;
        }
    }

    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }
    M3D_LOG_ERR("DamageInfoWnd: error - fail to init because of a bad resource");
    return 0;
}

DamageInfoWnd::DamageInfoWnd() = default;

DamageInfoWnd::DamageInfoWnd(DamageInfoWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}
