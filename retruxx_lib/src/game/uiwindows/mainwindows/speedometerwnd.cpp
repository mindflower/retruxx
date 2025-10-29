#include "speedometerwnd.h"

RT_CLASS_EXPORTS_BEGIN(SpeedometerWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(SpeedometerWnd);

m3d::rend::TexHandle SpeedometerWnd::ArrowPointer::GetTexture() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SpeedometerWnd::ArrowPointer::SetTexture(m3d::rend::TexHandle)
{
    RETRUXX_NOT_IMPLEMENTED;
}

SpeedometerWnd::ArrowPointer::~ArrowPointer()
{
    RETRUXX_NOT_IMPLEMENTED;
}

SpeedometerWnd::ArrowPointer::ArrowPointer()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SpeedometerWnd::ArrowPointer::Draw(m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

SpeedometerWnd::AuxInfo::AuxInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* SpeedometerWnd::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* SpeedometerWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* SpeedometerWnd::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

SpeedometerWnd::~SpeedometerWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* SpeedometerWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

void SpeedometerWnd::UpdateSpeedNumberColor(MotionDir)
{
    RETRUXX_NOT_IMPLEMENTED;
}

SpeedometerWnd::SpeedometerWnd(SpeedometerWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

SpeedometerWnd::SpeedometerWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SpeedometerWnd::UpdateSpeedNumber(float, MotionDir)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SpeedometerWnd::OnNewFrame()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SpeedometerWnd::GameDataUpdate(void*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SpeedometerWnd::UpdateSpeedNumberBgColor(MotionDir)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SpeedometerWnd::OnPaint(m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SpeedometerWnd::GetVelocity(float&, MotionDir&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SpeedometerWnd::UpdateSpeedPointer(float, MotionDir)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SpeedometerWnd::GameDataSetup()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Vehicle const* SpeedometerWnd::GetVehicle() const
{
    RETRUXX_NOT_IMPLEMENTED;
}
