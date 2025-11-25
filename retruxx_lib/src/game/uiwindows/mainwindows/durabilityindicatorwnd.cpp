#include "durabilityindicatorwnd.h"

RT_CLASS_EXPORTS_BEGIN(DurabilityIndicatorWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(DurabilityIndicatorWnd);

m3d::Class* DurabilityIndicatorWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Object* DurabilityIndicatorWnd::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int DurabilityIndicatorWnd::CreateFromPattern(m3d::ui::Wnd*, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* DurabilityIndicatorWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void DurabilityIndicatorWnd::SetType(Type, CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* DurabilityIndicatorWnd::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void DurabilityIndicatorWnd::SetVehicleId(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

DurabilityIndicatorWnd::~DurabilityIndicatorWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::rend::TexHandle DurabilityIndicatorWnd::GetImageByDurabilityRelValue(float) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int DurabilityIndicatorWnd::UpdateOnNewFrame()
{
    RETRUXX_NOT_IMPLEMENTED;
}

DurabilityIndicatorWnd::DurabilityIndicatorWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

DurabilityIndicatorWnd::DurabilityIndicatorWnd(DurabilityIndicatorWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

BoundsBase<float> DurabilityIndicatorWnd::GetRect() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Vehicle* DurabilityIndicatorWnd::GetVehicle() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void DurabilityIndicatorWnd::SetDurTextures(CStr const&, CStr const&, CStr const&, CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int DurabilityIndicatorWnd::OnPaint(m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int DurabilityIndicatorWnd::GameDataUpdate(void*, int)
{
    // TODO: implement GameDataUpdate
    //  RETRUXX_NOT_IMPLEMENTED;
    return 0;
}

int DurabilityIndicatorWnd::UpdateOnVehiclePartChanged()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void DurabilityIndicatorWnd::UpdateTooltip(float, float)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::VehiclePart* DurabilityIndicatorWnd::GetVehiclePart() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void DurabilityIndicatorWnd::UpdateVehiclePartImage()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void DurabilityIndicatorWnd::UpdateValue(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}
