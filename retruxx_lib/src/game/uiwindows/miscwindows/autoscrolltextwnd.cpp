#include "autoscrolltextwnd.h"

RT_CLASS_EXPORTS_BEGIN(AutoScrollTextWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(AutoScrollTextWnd);

int AutoScrollTextWnd::SetBackground(m3d::rend::TexHandle)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int AutoScrollTextWnd::SetBackground(CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void AutoScrollTextWnd::StartScroll(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void AutoScrollTextWnd::SetBounds(BoundsBase<float> const&, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* AutoScrollTextWnd::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* AutoScrollTextWnd::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int AutoScrollTextWnd::CreateFromPattern(m3d::ui::Wnd*, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void AutoScrollTextWnd::SetClientEdges(std::vector<float, std::allocator<float>> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void AutoScrollTextWnd::SetClientEdges(float, float, float, float)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void AutoScrollTextWnd::StopScroll()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* AutoScrollTextWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

float AutoScrollTextWnd::GetScrollSpeed() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void AutoScrollTextWnd::SetWrapMode(m3d::TextWrapFlags)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int AutoScrollTextWnd::SetText(CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void AutoScrollTextWnd::SetDefaultFont(CStr const&, float, m3d::ui::FontType, m3d::ui::FontParams)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void AutoScrollTextWnd::SetDefaultFont(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

AutoScrollTextWnd::~AutoScrollTextWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool AutoScrollTextWnd::IsScrolling() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void AutoScrollTextWnd::SetPaneFlags(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void AutoScrollTextWnd::SetPane(CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void AutoScrollTextWnd::SetScrollSpeed(float)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* AutoScrollTextWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

void AutoScrollTextWnd::DrawWndText(m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool AutoScrollTextWnd::NeedScroll() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void AutoScrollTextWnd::HandleScroll()
{
    RETRUXX_NOT_IMPLEMENTED;
}

AutoScrollTextWnd::AutoScrollTextWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

AutoScrollTextWnd::AutoScrollTextWnd(AutoScrollTextWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void AutoScrollTextWnd::SetZeroTextOrigin()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void AutoScrollTextWnd::PrecalcTextBounds()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void AutoScrollTextWnd::ScrollTextOrigin()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void AutoScrollTextWnd::PrecalcScrollPixelSpeed()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool AutoScrollTextWnd::TextOrignReachedFinalPosition() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int AutoScrollTextWnd::OnPaint(m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}
