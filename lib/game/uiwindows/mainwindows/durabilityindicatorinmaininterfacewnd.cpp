#include "durabilityindicatorinmaininterfacewnd.h"
#include "healthindicatorinmaininterfacewnd.h"
#include <core/log.h>
#include <game/m3dgame.h>

RT_CLASS_EXPORTS_BEGIN(DurabilityIndicatorInMainInterfaceWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(DurabilityIndicatorInMainInterfaceWnd);

m3d::Class* DurabilityIndicatorInMainInterfaceWnd::GetClass() const
{
    return RT_CLASS_LOCAL(DurabilityIndicatorInMainInterfaceWnd);
}

m3d::Object* DurabilityIndicatorInMainInterfaceWnd::CreateObject()
{
    return new DurabilityIndicatorInMainInterfaceWnd;
}

int DurabilityIndicatorInMainInterfaceWnd::CreateFromPattern(m3d::ui::Wnd* patternWnd, bool deleteSrc)
{
    // TODO: check this!!!
    using namespace m3d::ui;

    if (!patternWnd)
    {
        M3D_LOG_INFO("DurabilityIndicatorInMainInterfaceWnd::CreateFromPattern error - null patternWnd");
        return 0;
    }
     auto* parent = patternWnd->GetParent();
    if (!parent || !IS_KIND_OF(parent, Wnd))
    {
        M3D_LOG_INFO("DurabilityIndicatorInMainInterfaceWnd::CreateFromPattern error - null parent for paternWnd");
        return 0;
    }

    auto res = Wnd::Create(patternWnd->GetText(), patternWnd->GetStyle(), patternWnd->GetBounds(), patternWnd->GetId());
    if (res == 0)
    {
        M3D_LOG_INFO("DurabilityIndicatorInMainInterfaceWnd::CreateFromPattern error - cannot create window");
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

    auto* imageWnd = (ImageWnd*)patternWnd;
    SetImage(imageWnd->GetImage());

    parent->AddChild(this);
    if (deleteSrc)
    {
        parent->RemoveChild(patternWnd);
        // TODO: check this obj delete
        patternWnd->DecRef();
    }

    m_strCabinDurability = M3D_APP->GetStringByStringId0(DurabilityIndicatorInMainInterfaceWnd::m_aif.m_strCabinDurabilityId);
    m_strBasketDurability = M3D_APP->GetStringByStringId0(DurabilityIndicatorInMainInterfaceWnd::m_aif.m_strBasketDurabilityId);

    m_gameDataFlags |= 1u;
    return 1;
}

void DurabilityIndicatorInMainInterfaceWnd::SetType(Type, CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* DurabilityIndicatorInMainInterfaceWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void DurabilityIndicatorInMainInterfaceWnd::SetVehicleId(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

DurabilityIndicatorInMainInterfaceWnd::~DurabilityIndicatorInMainInterfaceWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* DurabilityIndicatorInMainInterfaceWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ImageWnd);
}

unsigned DurabilityIndicatorInMainInterfaceWnd::GetColorByValue(float, float) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int DurabilityIndicatorInMainInterfaceWnd::GameDataUpdate(void*, int)
{
    // TODO: implement GameDataUpdate
    //  RETRUXX_NOT_IMPLEMENTED;
    return 0;
}

int DurabilityIndicatorInMainInterfaceWnd::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Vehicle const* DurabilityIndicatorInMainInterfaceWnd::GetVehicle() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

DurabilityIndicatorInMainInterfaceWnd::DurabilityIndicatorInMainInterfaceWnd()
{
    m_vehicleId = -1;
    m_vehiclePartId = -1;
    m_type = TYPE_NUM_TYPES;
    m_prevCurVal = 0.0;
    m_prevMaxVal = 0.0;
}

DurabilityIndicatorInMainInterfaceWnd::DurabilityIndicatorInMainInterfaceWnd(
    DurabilityIndicatorInMainInterfaceWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void DurabilityIndicatorInMainInterfaceWnd::UpdateTooltip(float, float)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void DurabilityIndicatorInMainInterfaceWnd::OnVehiclePartChanged(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void DurabilityIndicatorInMainInterfaceWnd::GetValue(float&, float&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void DurabilityIndicatorInMainInterfaceWnd::FullUpdate(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void DurabilityIndicatorInMainInterfaceWnd::OnNewFrame()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void DurabilityIndicatorInMainInterfaceWnd::UpdateColor(float, float)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::VehiclePart const* DurabilityIndicatorInMainInterfaceWnd::GetVehiclePart() const
{
    RETRUXX_NOT_IMPLEMENTED;
}
