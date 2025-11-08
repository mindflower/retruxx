#include "targetcursorwnd.h"

RT_CLASS_EXPORTS_BEGIN(TargetCursorWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(TargetCursorWnd);

m3d::Class* TargetCursorWnd::GetRtClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* TargetCursorWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(SmartCursorWnd);
}

TargetCursorWnd::~TargetCursorWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void TargetCursorWnd::OnNewFrame()
{
    RETRUXX_NOT_IMPLEMENTED;
}

TargetCursorWnd::TargetCursorWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int TargetCursorWnd::OnPaint(m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool TargetCursorWnd::NeedShow() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void TargetCursorWnd::UpdatePosition()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void TargetCursorWnd::Show(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void TargetCursorWnd::AccidentValidateBounds()
{
    RETRUXX_NOT_IMPLEMENTED;
}
