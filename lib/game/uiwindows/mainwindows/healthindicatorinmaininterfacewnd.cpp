#include "healthindicatorinmaininterfacewnd.h"

#include "core/log.h"
#include "server/dynamicquestmanager.h"
#include <ui/modelwnd.h>
#include <ui/progressbarwnd.h>
#include "twinklinglampwnd.h"
#include "electronicdigitalwnd.h"
#include "ui/ui_srv.h"
#include <game/m3dgame.h>

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
    RETRUXX_NOT_IMPLEMENTED;
}

void HealthIndicatorInMainInterfaceWnd::SetType(Type)
{
    RETRUXX_NOT_IMPLEMENTED;
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
    if (!parent || IS_KIND_OF(parent, Wnd))
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

    if (auto child = RT_DYNCAST(patternWnd->GetChildByName(m_aif.m_wndLowHpLampName), ImageWnd))
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

    if (auto child = RT_DYNCAST(patternWnd->GetChildByName(m_aif.m_wndProgressBarName), ProgressBarWnd))
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

    if (auto child = RT_DYNCAST(patternWnd->GetChildByName(m_aif.m_wndValueName), ImageWnd))
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
        M3D_LOG_INFO("Get control error: control " + m_aif.m_wndLowHpLampName + " is not found or incorrect type");
    }

    if (m_wndValue)
    {
        patternWnd->RemoveChild(m_wndValue);
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
    RETRUXX_NOT_IMPLEMENTED;
}

void HealthIndicatorInMainInterfaceWnd::SetVehicleId(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void HealthIndicatorInMainInterfaceWnd::UpdateTooltip(float, float)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Vehicle const* HealthIndicatorInMainInterfaceWnd::GetVehicle() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int HealthIndicatorInMainInterfaceWnd::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void HealthIndicatorInMainInterfaceWnd::UpdateLowHpLamp(float, float)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int HealthIndicatorInMainInterfaceWnd::GameDataUpdate(void*, int)
{
    // TODO: implement GameDataUpdate
    //  RETRUXX_NOT_IMPLEMENTED;
    return 0;
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

HealthIndicatorInMainInterfaceWnd::HealthIndicatorInMainInterfaceWnd(HealthIndicatorInMainInterfaceWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void HealthIndicatorInMainInterfaceWnd::GetHp(float&, float&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void HealthIndicatorInMainInterfaceWnd::OnNewFrame()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void HealthIndicatorInMainInterfaceWnd::UpdateProgressBar(float, float)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void HealthIndicatorInMainInterfaceWnd::UpdateValueWnd(float)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void HealthIndicatorInMainInterfaceWnd::FullUpdate(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}
