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
    m_coords.x = 0.0;
    m_coords.y = 0.0;
    m_angle = 0.0;
    m_size.x = 0.0;
    m_size.y = 0.0;
}

void SpeedometerWnd::ArrowPointer::Draw(m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

SpeedometerWnd::AuxInfo::AuxInfo()
{
    m_wndSpeedNumberName = ("wndSpeedNumber");
    m_speedPointerTextureName = ("SpeedPointer");

    m_speedPointerSize = { 8.0f, 64.0f };
    m_speedPointerCenter = { 62.0f, 65.0f };
    m_speedPointerMinAngle = -0.78539819f;
    m_speedPointerMaxAngle = 1.5707964f;
    m_speedPointerMaxAngleBackward = -1.5707964f;
    m_speedPointerRadius = 27.0f;

    m_colorForvard = 0xFF8CD618;  // Green color
    m_colorBackward = 0xFFFF0000; // Red color
}

m3d::Class* SpeedometerWnd::GetClass() const
{
    return RT_CLASS_LOCAL(SpeedometerWnd);
}

m3d::Object* SpeedometerWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* SpeedometerWnd::CreateObject()
{
    return new SpeedometerWnd;
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
    m_wndSpeedNumber = 0;
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
    // TODO: implement SpeedometerWnd::GameDataSetup
    // RETRUXX_NOT_IMPLEMENTED;
    return 1;
}

ai::Vehicle const* SpeedometerWnd::GetVehicle() const
{
    RETRUXX_NOT_IMPLEMENTED;
}
