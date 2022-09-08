#include "targetcapturingcursorwnd.h"

RT_CLASS_EXPORTS_BEGIN(TargetCapturingCursorWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(TargetCapturingCursorWnd);

m3d::Class* TargetCapturingCursorWnd::GetClass() const
{
    throw std::logic_error("Not implemented");
}

TargetCapturingCursorWnd::~TargetCapturingCursorWnd()
{
    throw std::logic_error("Not implemented");
}

CStr const& TargetCapturingCursorWnd::GetCaptureTexName()
{
    throw std::logic_error("Not implemented");
}

float TargetCapturingCursorWnd::GetCapturingTexAngle()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* TargetCapturingCursorWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(TargetCursorWnd);
}

m3d::Object* TargetCapturingCursorWnd::CreateObject()
{
    throw std::logic_error("Not implemented");
}

PointBase<float> const& TargetCapturingCursorWnd::GetCaptureTexSmallSz()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* TargetCapturingCursorWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

int TargetCapturingCursorWnd::GetTargetObjId() const
{
    throw std::logic_error("Not implemented");
}

TargetCapturingCursorWnd::TargetCapturingCursorWnd()
{
    throw std::logic_error("Not implemented");
}

TargetCapturingCursorWnd::TargetCapturingCursorWnd(TargetCapturingCursorWnd const&)
{
    throw std::logic_error("Not implemented");
}

void TargetCapturingCursorWnd::UpdateCaptureTexAngle()
{
    throw std::logic_error("Not implemented");
}

void TargetCapturingCursorWnd::OnNewFrame()
{
    throw std::logic_error("Not implemented");
}

void TargetCapturingCursorWnd::UpdateCaptureTex()
{
    throw std::logic_error("Not implemented");
}

int TargetCapturingCursorWnd::GameDataSetup()
{
    throw std::logic_error("Not implemented");
}

PointBase<float> TargetCapturingCursorWnd::GetCaptureTexSize() const
{
    throw std::logic_error("Not implemented");
}

unsigned char TargetCapturingCursorWnd::GetCaptureTexAlpha() const
{
    throw std::logic_error("Not implemented");
}
