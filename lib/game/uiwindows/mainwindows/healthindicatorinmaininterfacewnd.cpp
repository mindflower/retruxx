#include "healthindicatorinmaininterfacewnd.h"

#include "core/log.h"
#include "server/dynamicquestmanager.h"
#include <ui/modelwnd.h>
#include <ui/progressbarwnd.h>
#include "twinklinglampwnd.h"
#include "electronicdigitalwnd.h"
#include "ui/ui_srv.h"
#include <game/m3dgame.h>
#include "server/objects/vehicle.h"

RT_CLASS_EXPORTS_BEGIN(HealthIndicatorInMainInterfaceWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(HealthIndicatorInMainInterfaceWnd);

HealthIndicatorInMainInterfaceWnd::AuxInfo::AuxInfo()
{
    m_wndLowHpLampName = "wndLowHpLamp";
    m_wndProgressBarName = "wndHpProgressBar";
    m_wndValueName = "wndHpValue";
    m_wndOverlayName = "wndHpProgressBarOverlay";
    m_strHealthId = "Construction";
}

m3d::Object* HealthIndicatorInMainInterfaceWnd::Clone()
{
    // RVA 0x1256E0
    return new HealthIndicatorInMainInterfaceWnd(*this);
}

void HealthIndicatorInMainInterfaceWnd::SetType(Type newType)
{
    m_type = newType;
    FullUpdate(true);
}

int HealthIndicatorInMainInterfaceWnd::CreateFromPattern(m3d::ui::Wnd* patternWnd, bool deleteSrc)
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

    if (auto child = RT_DYNCAST(parent->GetChildByName(m_aif.m_wndLowHpLampName), ImageWnd))
    {
        m_wndLowHpLamp = (TwinklingLampWnd*)M3D_KERNEL->New("TwinklingLampWnd");
        if (m_wndLowHpLamp)
        {
            if (!m_wndLowHpLamp->CreateFromPattern(child, true))
            {
                M3D_LOG_INFO("Make control error: cannot create " + m_aif.m_wndLowHpLampName + " from pattern class");
            }
        }
        else
        {
            M3D_LOG_INFO(
                "Make control error: cannot create " + m_aif.m_wndLowHpLampName +
                " - cannot find rtti class TwinklingLampWnd");
        }
    }
    else
    {
        M3D_LOG_INFO("Get control error: control " + m_aif.m_wndLowHpLampName + " is not found or incorrect type");
    }

    if (m_wndLowHpLamp)
    {
        parent->RemoveChild(m_wndLowHpLamp);
        AddChild(m_wndLowHpLamp);

        auto bounds = m_wndLowHpLamp->GetBounds();
        auto const parentBounds = GetBounds();
        bounds.x0 -= parentBounds.x0;
        bounds.y0 -= parentBounds.y0;
        m_wndLowHpLamp->SetBounds(bounds, false);
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

    m_strHealth = M3D_APP->GetStringByStringId0(m_aif.m_strHealthId);
    FullUpdate(true);

    m_gameDataFlags |= 1u;
    return 1;
}

m3d::Object* HealthIndicatorInMainInterfaceWnd::CreateObject()
{
    return new HealthIndicatorInMainInterfaceWnd;
}

m3d::Class* HealthIndicatorInMainInterfaceWnd::GetClass() const
{
    return RT_CLASS_LOCAL(HealthIndicatorInMainInterfaceWnd);
}

m3d::Class* HealthIndicatorInMainInterfaceWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

HealthIndicatorInMainInterfaceWnd::~HealthIndicatorInMainInterfaceWnd()
{
    // RVA 0x125A50 - m_strHealth and the Wnd base (which owns/destroys the
    // child m_wndLowHpLamp/m_wndProgressBar/m_wndValue windows) clean up
    // automatically.
}

void HealthIndicatorInMainInterfaceWnd::SetVehicleId(int id)
{
    m_vehicleId = id;
    FullUpdate(true);
}

void HealthIndicatorInMainInterfaceWnd::UpdateTooltip(float curHp, float maxHp)
{
    // RVA 0x126900
    if ((m_gameDataFlags & 1) != 0)
    {
        CStr text = m_strHealth + ": " + CStr(static_cast<int>(curHp)) + "/" + CStr(static_cast<int>(maxHp));
        m_wndProgressBar->SetProperty(PROP_WND_TOOLTIP, &text);
        m_wndValue->SetProperty(PROP_WND_TOOLTIP, &text);
    }
}

ai::Vehicle const* HealthIndicatorInMainInterfaceWnd::GetVehicle() const
{
    return RT_DYNCAST(ai::theObjects->GetEntityByObjId(m_vehicleId), ai::Vehicle const);
}

int HealthIndicatorInMainInterfaceWnd::GameDataClear(bool)
{
    // RVA 0x126600
    m_vehicleId = -1;
    FullUpdate(true);
    return 1;
}

void HealthIndicatorInMainInterfaceWnd::UpdateLowHpLamp(float curHp, float maxHp)
{
    // RVA 0x1267E0
    if ((m_gameDataFlags & 1) != 0)
    {
        m_wndLowHpLamp->SetValue(curHp, maxHp);
    }
}

int HealthIndicatorInMainInterfaceWnd::GameDataUpdate(void*, int dataType)
{
    // RVA 0x126620
    if (dataType == 89 && m_vehicleId != -1)
    {
        FullUpdate(false);
    }
    return 1;
}

HealthIndicatorInMainInterfaceWnd::HealthIndicatorInMainInterfaceWnd()
{
    m_vehicleId = -1;
    m_wndLowHpLamp = 0;
    m_wndProgressBar = 0;
    m_wndValue = 0;
    m_type = TYPE_NUM_TYPES;
    m_prevCurVal = 0.0;
    m_prevMaxVal = 0.0;
}

HealthIndicatorInMainInterfaceWnd::HealthIndicatorInMainInterfaceWnd(HealthIndicatorInMainInterfaceWnd const&) :
    HealthIndicatorInMainInterfaceWnd()
{
    // NOTE: the shipped copy ctor (RVA 0x125A20) default-constructs the base
    // and resets m_strHealth to empty, but leaves m_type/m_wndLowHpLamp/
    // m_wndProgressBar/m_wndValue/m_vehicleId/m_prevCurVal/m_prevMaxVal
    // uninitialized; delegating to the default ctor here avoids reading
    // uninitialized pointers/ints while still copying nothing from the source.
}

void HealthIndicatorInMainInterfaceWnd::GetHp(float& curHp, float& maxHp) const
{
    // RVA 0x1268B0
    curHp = 0.0f;
    maxHp = 0.0f;
    if (ai::Vehicle const* vehicle = GetVehicle())
    {
        curHp = vehicle->Health().value().get();
        maxHp = vehicle->Health().maxValue().get();
    }
}

void HealthIndicatorInMainInterfaceWnd::OnNewFrame()
{
    // RVA 0x126650
    if (m_vehicleId != -1)
    {
        FullUpdate(false);
    }
}

void HealthIndicatorInMainInterfaceWnd::UpdateProgressBar(float curHp, float maxHp)
{
    // RVA 0x1267B0
    if ((m_gameDataFlags & 1) != 0)
    {
        m_wndProgressBar->SetMaxValue(maxHp);
        m_wndProgressBar->SetCurValue(curHp);
    }
}

void HealthIndicatorInMainInterfaceWnd::UpdateValueWnd(float curHp)
{
    // RVA 0x126810
    if ((m_gameDataFlags & 1) != 0)
    {
        m_wndValue->ShowNumber(static_cast<int>(curHp), false, 4u, false);
    }
}

void HealthIndicatorInMainInterfaceWnd::FullUpdate(bool bForce)
{
    float curHp = 0.0;
    float maxHp = 0.0;

    ai::Vehicle const* vehicle = GetVehicle();
    if (vehicle)
    {
        curHp = vehicle->Health().value().get();
        maxHp = vehicle->Health().maxValue().get();
    }
    if (bForce || curHp != m_prevCurVal || maxHp != m_prevMaxVal)
    {
        if ((m_gameDataFlags & 1) != 0)
        {
            m_wndProgressBar->SetMaxValue(maxHp);
            m_wndProgressBar->SetCurValue(curHp);
        }
        if ((m_gameDataFlags & 1) != 0)
        {
            m_wndValue->ShowNumber(static_cast<int>(curHp), false, 4u, false);
            if ((m_gameDataFlags & 1) != 0)
            {
                m_wndLowHpLamp->SetValue(curHp, maxHp);
            }
        }
        if (m_type == TYPE_IN_CHARACTERISTIC_WND)
            UpdateTooltip(curHp, maxHp);
    }
    m_prevCurVal = curHp;
    m_prevMaxVal = maxHp;
}
