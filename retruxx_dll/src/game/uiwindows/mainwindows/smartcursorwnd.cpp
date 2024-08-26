#include "smartcursorwnd.h"

RT_CLASS_EXPORTS_BEGIN(SmartCursorWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(SmartCursorWnd);

SmartCursorWnd::AuxDrawInfo::AuxDrawInfo()
{
    throw std::logic_error("Not implemented");
}

void SmartCursorWnd::AuxDrawInfo::SetTexture(m3d::rend::TexHandle)
{
    throw std::logic_error("Not implemented");
}

m3d::rend::TexHandle SmartCursorWnd::AuxDrawInfo::GetTexture() const
{
    throw std::logic_error("Not implemented");
}

SmartCursorWnd::AuxDrawInfo::~AuxDrawInfo()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* SmartCursorWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* SmartCursorWnd::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* SmartCursorWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

SmartCursorWnd::~SmartCursorWnd()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* SmartCursorWnd::GetClass() const
{
    throw std::logic_error("Not implemented");
}

SmartCursorWnd::SmartCursorWnd(SmartCursorWnd const&)
{
    throw std::logic_error("Not implemented");
}

SmartCursorWnd::SmartCursorWnd()
{
    throw std::logic_error("Not implemented");
}

void SmartCursorWnd::OnNewFrame()
{
    throw std::logic_error("Not implemented");
}

int SmartCursorWnd::OnPaint(m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}

int SmartCursorWnd::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

void SmartCursorWnd::ClearDrawInfo()
{
    throw std::logic_error("Not implemented");
}

void SmartCursorWnd::DrawTexture(m3d::ui::DrawInfo const&, AuxDrawInfo const*) const
{
    throw std::logic_error("Not implemented");
}
