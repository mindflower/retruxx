#include "fuelindicatorinmaininterfacewnd.h"
#include "twinklinglampwnd.h"
#include "electronicdigitalwnd.h"
#include <core/log.h>
#include <ui/modelwnd.h>
#include <ui/progressbarwnd.h>
#include <game/m3dgame.h>
#include "server/objects/vehicle.h"

RT_CLASS_EXPORTS_BEGIN(FuelIndicatorInMainInterfaceWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(FuelIndicatorInMainInterfaceWnd);

void FuelIndicatorInMainInterfaceWnd::SetType(Type t)
{
    m_type = t;
    FullUpdate(true);
}

m3d::Object* FuelIndicatorInMainInterfaceWnd::CreateObject()
{
    return new FuelIndicatorInMainInterfaceWnd;
}

m3d::Class* FuelIndicatorInMainInterfaceWnd::GetClass() const
{
    return RT_CLASS_LOCAL(FuelIndicatorInMainInterfaceWnd);
}

m3d::Class* FuelIndicatorInMainInterfaceWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Object* FuelIndicatorInMainInterfaceWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void FuelIndicatorInMainInterfaceWnd::SetVehicleId(int id)
{
    m_vehicleId = id;
    FullUpdate(true);
}

FuelIndicatorInMainInterfaceWnd::~FuelIndicatorInMainInterfaceWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int FuelIndicatorInMainInterfaceWnd::CreateFromPattern(m3d::ui::Wnd* patternWnd, bool deleteSrc)
{
    using namespace m3d::ui;

    if (!patternWnd)
    {
        M3D_LOG_INFO("HealthIndicatorInMainInterfaceWnd::CreateFromPattern error - null patternWnd");
        return 0;
    }

    auto* parent = patternWnd->GetParent();
    if (!parent || !IS_KIND_OF(parent, Wnd))
    {
        M3D_LOG_INFO("HealthIndicatorInMainInterfaceWnd::CreateFromPattern error - null parent");
        return 0;
    }

    auto res = Wnd::Create(patternWnd->GetText(), patternWnd->GetStyle(), patternWnd->GetBounds(), patternWnd->GetId());
    if (res == 0)
    {
        M3D_LOG_INFO("HealthIndicatorInMainInterfaceWnd::CreateFromPattern error - cannot create window");
        return 0;
    }

    SetStyle(patternWnd->GetStyle());
    SetText(patternWnd->GetText());
    SetId(patternWnd->GetId());
    SetName(patternWnd->GetName());
    SetBounds(patternWnd->GetBounds(), true);
    SetDefaultFont(patternWnd->GetDefaultFont());
    SetWrapMode(patternWnd->GetWrapMode());

    SetFormatMode(patternWnd->GetFormatMode());
    SetColor(patternWnd->GetColor());
    SetTextColor(patternWnd->GetColor());
    SetTextColorDisabled(patternWnd->GetTextColorDisabled());
    SetClientEdges(patternWnd->GetClientEdges());
    SetPane(patternWnd->GetPaneName());
    SetPaneFlags(patternWnd->GetPaneFlags());
    SetScrollPane(patternWnd->GetScrollPaneName());
    SetBackground(patternWnd->GetBackground());

    CStr tooltip;
    patternWnd->GetProperty(PROP_WND_TOOLTIP, &tooltip);
    SetProperty(PROP_WND_TOOLTIP, &tooltip);

    SetOnShowAnimation(patternWnd->GetOnShowAnimation());
    SetOnHideAnimation(patternWnd->GetOnHideAnimation());

    if (auto child = RT_DYNCAST(parent->GetChildByName(m_aif.m_wndOverlayName), Wnd))
    {
        parent->RemoveChild(child);
        AddChild(child);

        auto bounds = child->GetBounds();
        auto const parentBounds = GetBounds();
        bounds.x0 -= parentBounds.x0;
        bounds.y0 -= parentBounds.y0;
        child->SetBounds(bounds, false);
    }
    else
    {
        M3D_LOG_INFO("Get control error: control " + m_aif.m_wndOverlayName + " is not found or incorrect type");
    }

    if (auto child = RT_DYNCAST(parent->GetChildByName(m_aif.m_wndLowFuelLampName), ImageWnd))
    {
        m_wndLowFuelLamp = (TwinklingLampWnd*)M3D_KERNEL->New("TwinklingLampWnd");
        if (m_wndLowFuelLamp)
        {
            if (!m_wndLowFuelLamp->CreateFromPattern(child, true))
            {
                M3D_LOG_INFO("Make control error: cannot create " + m_aif.m_wndLowFuelLampName + " from pattern class");
            }
        }
        else
        {
            M3D_LOG_INFO(
                "Make control error: cannot create " + m_aif.m_wndLowFuelLampName +
                " - cannot find rtti class TwinklingLampWnd");
        }
    }
    else
    {
        M3D_LOG_INFO("Get control error: control " + m_aif.m_wndLowFuelLampName + " is not found or incorrect type");
    }

    if (m_wndLowFuelLamp)
    {
        parent->RemoveChild(m_wndLowFuelLamp);
        AddChild(m_wndLowFuelLamp);

        auto bounds = m_wndLowFuelLamp->GetBounds();
        auto const parentBounds = GetBounds();
        bounds.x0 -= parentBounds.x0;
        bounds.y0 -= parentBounds.y0;
        m_wndLowFuelLamp->SetBounds(bounds, false);
    }

    if (auto child = RT_DYNCAST(parent->GetChildByName(m_aif.m_wndProgressBarName), ProgressBarWnd))
    {
        m_wndProgressBar = child;
    }
    else
    {
        M3D_LOG_INFO("Get control error: control " + m_aif.m_wndProgressBarName + " is not found or incorrect type");
    }

    if (m_wndProgressBar)
    {
        parent->RemoveChild(m_wndProgressBar);
        AddChild(m_wndProgressBar);

        auto bounds = m_wndProgressBar->GetBounds();
        auto const parentBounds = GetBounds();
        bounds.x0 -= parentBounds.x0;
        bounds.y0 -= parentBounds.y0;
        m_wndProgressBar->SetBounds(bounds, false);
    }

    if (auto child = RT_DYNCAST(parent->GetChildByName(m_aif.m_wndValueName), Wnd))
    {
        m_wndValue = (ElectronicDigitalWnd*)M3D_KERNEL->New("ElectronicDigitalWnd");
        if (m_wndValue)
        {
            if (!m_wndValue->CreateFromPattern(child, true))
            {
                M3D_LOG_INFO("Make control error: cannot create " + m_aif.m_wndValueName + " from pattern class");
            }
        }
        else
        {
            M3D_LOG_INFO(
                "Make control error: cannot create " + m_aif.m_wndValueName +
                " - cannot find rtti class ElectronicDigitalWnd");
        }
    }
    else
    {
        M3D_LOG_INFO("Get control error: control " + m_aif.m_wndValueName + " is not found or incorrect type");
    }

    if (m_wndValue)
    {
        parent->RemoveChild(m_wndValue);
        AddChild(m_wndValue);

        auto bounds = m_wndValue->GetBounds();
        auto const parentBounds = GetBounds();
        bounds.x0 -= parentBounds.x0;
        bounds.y0 -= parentBounds.y0;
        m_wndValue->SetBounds(bounds, false);

        m_wndValue->SetDigitalSize(ElectronicDigitalWnd::DIGITAL_SIZE_LARGE);
    }

    parent->AddChild(this);
    if (deleteSrc)
    {
        parent->RemoveChild(patternWnd);
        // TODO: check this obj delete
        patternWnd->DecRef();
    }

    m_strFuel = M3D_APP->GetStringByStringId0(m_aif.m_strFuelId);
    FullUpdate(true);

    m_gameDataFlags |= 1u;
    return 1;
}

void FuelIndicatorInMainInterfaceWnd::UpdateProgressBar(float, float)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void FuelIndicatorInMainInterfaceWnd::GetFuel(float&, float&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void FuelIndicatorInMainInterfaceWnd::OnNewFrame()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void FuelIndicatorInMainInterfaceWnd::FullUpdate(bool bForce)
{
    float curFuel = 0.0;
    float maxFuel = 0.0;
    
    ai::Vehicle const* vehicle = GetVehicle();
    if (vehicle)
    {
        curFuel = vehicle->Fuel().value().get();
        maxFuel = vehicle->Fuel().maxValue().get();
    }
    if (bForce || curFuel != m_prevCurVal || maxFuel != m_prevMaxVal)
    {
        if ((m_gameDataFlags & 1) != 0)
        {
            m_wndProgressBar->SetMaxValue(maxFuel);
            m_wndProgressBar->SetCurValue(curFuel);
        }
        if ((m_gameDataFlags & 1) != 0)
        {
            m_wndValue->ShowNumber(static_cast<int>(curFuel), false, 4u, false);
            if ((m_gameDataFlags & 1) != 0)
            {
                m_wndLowFuelLamp->SetValue(curFuel, maxFuel);
            }
        }
        if (m_type == TYPE_IN_CHARACTERISTIC_WND)
            UpdateTooltip(curFuel, maxFuel);
    }
    m_prevCurVal = curFuel;
    m_prevMaxVal = maxFuel;

}

void FuelIndicatorInMainInterfaceWnd::UpdateValueWnd(float)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Vehicle const* FuelIndicatorInMainInterfaceWnd::GetVehicle() const
{
    return RT_DYNCAST(ai::theObjects->GetEntityByObjId(m_vehicleId), ai::Vehicle const);
}

void FuelIndicatorInMainInterfaceWnd::UpdateTooltip(float, float)
{
    // TODO: implement FuelIndicatorInMainInterfaceWnd::UpdateTooltip
}

FuelIndicatorInMainInterfaceWnd::FuelIndicatorInMainInterfaceWnd()
{
    m_vehicleId = -1;
    m_wndLowFuelLamp = 0;
    m_wndProgressBar = 0;
    m_wndValue = 0;
    m_type = TYPE_NUM_TYPES;
    m_prevCurVal = 0.0;
    m_prevMaxVal = 0.0;
}

FuelIndicatorInMainInterfaceWnd::FuelIndicatorInMainInterfaceWnd(FuelIndicatorInMainInterfaceWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int FuelIndicatorInMainInterfaceWnd::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int FuelIndicatorInMainInterfaceWnd::GameDataUpdate(void*, int dataType)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (dataType == 89 && m_vehicleId != -1)
    {
        FullUpdate(false);
    }
    return 1;
}

void FuelIndicatorInMainInterfaceWnd::UpdateLowFuelLamp(float, float)
{
    RETRUXX_NOT_IMPLEMENTED;
}
