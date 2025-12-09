#include "healthindicatorinmaininterfacewnd.h"

#include "core/log.h"
#include "server/dynamicquestmanager.h"

RT_CLASS_EXPORTS_BEGIN(HealthIndicatorInMainInterfaceWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(HealthIndicatorInMainInterfaceWnd);

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
    if (!patternWnd)
    {
        M3D_LOG_INFO("HealthIndicatorInMainInterfaceWnd::CreateFromPattern error - null patternWnd");
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

    RETRUXX_NOT_IMPLEMENTED;
    auto* parent = patternWnd->GetParent();
    if (!parent || !IS_KIND_OF(parent, Wnd))
    {
        M3D_LOG_INFO("HealthIndicatorInMainInterfaceWnd::CreateFromPattern error - null parent for paternWnd");
        return 0;
    }

    parent->AddChild(this);
    if (deleteSrc)
    {
        parent->RemoveChild(patternWnd);
        // TODO: check this obj delete
        patternWnd->DecRef();
    }

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
