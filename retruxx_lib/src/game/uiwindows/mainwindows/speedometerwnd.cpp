#include "speedometerwnd.h"

RT_CLASS_EXPORTS_BEGIN(SpeedometerWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(SpeedometerWnd);

m3d::rend::TexHandle SpeedometerWnd::ArrowPointer::GetTexture() const
{
    throw std::logic_error("Not implemented");
}

void SpeedometerWnd::ArrowPointer::SetTexture(m3d::rend::TexHandle)
{
    throw std::logic_error("Not implemented");
}

SpeedometerWnd::ArrowPointer::~ArrowPointer()
{
    throw std::logic_error("Not implemented");
}

SpeedometerWnd::ArrowPointer::ArrowPointer()
{
    throw std::logic_error("Not implemented");
}

void SpeedometerWnd::ArrowPointer::Draw(m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}

SpeedometerWnd::AuxInfo::AuxInfo()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* SpeedometerWnd::GetClass() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* SpeedometerWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* SpeedometerWnd::CreateObject()
{
    throw std::logic_error("Not implemented");
}

SpeedometerWnd::~SpeedometerWnd()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* SpeedometerWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

void SpeedometerWnd::UpdateSpeedNumberColor(MotionDir)
{
    throw std::logic_error("Not implemented");
}

SpeedometerWnd::SpeedometerWnd(SpeedometerWnd const&)
{
    throw std::logic_error("Not implemented");
}

SpeedometerWnd::SpeedometerWnd()
{
    throw std::logic_error("Not implemented");
}

void SpeedometerWnd::UpdateSpeedNumber(float, MotionDir)
{
    throw std::logic_error("Not implemented");
}

void SpeedometerWnd::OnNewFrame()
{
    throw std::logic_error("Not implemented");
}

int SpeedometerWnd::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

void SpeedometerWnd::UpdateSpeedNumberBgColor(MotionDir)
{
    throw std::logic_error("Not implemented");
}

int SpeedometerWnd::OnPaint(m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}

void SpeedometerWnd::GetVelocity(float&, MotionDir&) const
{
    throw std::logic_error("Not implemented");
}

void SpeedometerWnd::UpdateSpeedPointer(float, MotionDir)
{
    throw std::logic_error("Not implemented");
}

int SpeedometerWnd::GameDataSetup()
{
    throw std::logic_error("Not implemented");
}

ai::Vehicle const* SpeedometerWnd::GetVehicle() const
{
    throw std::logic_error("Not implemented");
}
