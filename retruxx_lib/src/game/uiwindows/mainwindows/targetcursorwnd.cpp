#include "targetcursorwnd.h"

RT_CLASS_EXPORTS_BEGIN(TargetCursorWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(TargetCursorWnd);

m3d::Class* TargetCursorWnd::GetRtClass() const
{
    throw std::logic_error("Not implemented");
}

m3d::Class* TargetCursorWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(SmartCursorWnd);
}

TargetCursorWnd::~TargetCursorWnd()
{
    throw std::logic_error("Not implemented");
}

void TargetCursorWnd::OnNewFrame()
{
    throw std::logic_error("Not implemented");
}

TargetCursorWnd::TargetCursorWnd()
{
    throw std::logic_error("Not implemented");
}

int TargetCursorWnd::OnPaint(m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}

bool TargetCursorWnd::NeedShow() const
{
    throw std::logic_error("Not implemented");
}

void TargetCursorWnd::UpdatePosition()
{
    throw std::logic_error("Not implemented");
}

void TargetCursorWnd::Show(bool)
{
    throw std::logic_error("Not implemented");
}

void TargetCursorWnd::AccidentValidateBounds()
{
    throw std::logic_error("Not implemented");
}
