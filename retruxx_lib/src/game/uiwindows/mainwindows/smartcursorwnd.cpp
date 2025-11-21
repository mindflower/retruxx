#include "smartcursorwnd.h"

RT_CLASS_EXPORTS_BEGIN(SmartCursorWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(SmartCursorWnd);

SmartCursorWnd::AuxDrawInfo::AuxDrawInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SmartCursorWnd::AuxDrawInfo::SetTexture(m3d::rend::TexHandle)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::rend::TexHandle SmartCursorWnd::AuxDrawInfo::GetTexture() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

SmartCursorWnd::AuxDrawInfo::~AuxDrawInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* SmartCursorWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* SmartCursorWnd::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* SmartCursorWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

SmartCursorWnd::~SmartCursorWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* SmartCursorWnd::GetClass() const
{
    return RT_CLASS_LOCAL(SmartCursorWnd);
}

SmartCursorWnd::SmartCursorWnd(SmartCursorWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

SmartCursorWnd::SmartCursorWnd() = default;

void SmartCursorWnd::OnNewFrame()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SmartCursorWnd::OnPaint(m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SmartCursorWnd::GameDataUpdate(void*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SmartCursorWnd::ClearDrawInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SmartCursorWnd::DrawTexture(m3d::ui::DrawInfo const&, AuxDrawInfo const*) const
{
    RETRUXX_NOT_IMPLEMENTED;
}
