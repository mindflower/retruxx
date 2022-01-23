#include "autoscrolltextwnd.h"

RT_CLASS_DEFINE(AutoScrollTextWnd);

int AutoScrollTextWnd::SetBackground(m3d::rend::TexHandle)
{
    throw std::logic_error("Not implemented");
}

int AutoScrollTextWnd::SetBackground(CStr const&)
{
    throw std::logic_error("Not implemented");
}

void AutoScrollTextWnd::StartScroll(bool)
{
    throw std::logic_error("Not implemented");
}

void AutoScrollTextWnd::SetBounds(BoundsBase<float> const&, bool)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* AutoScrollTextWnd::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* AutoScrollTextWnd::GetClass() const
{
    throw std::logic_error("Not implemented");
}

int AutoScrollTextWnd::CreateFromPattern(m3d::ui::Wnd*, bool)
{
    throw std::logic_error("Not implemented");
}

void AutoScrollTextWnd::SetClientEdges(std::vector<float, std::allocator<float>> const&)
{
    throw std::logic_error("Not implemented");
}

void AutoScrollTextWnd::SetClientEdges(float, float, float, float)
{
    throw std::logic_error("Not implemented");
}

void AutoScrollTextWnd::StopScroll()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* AutoScrollTextWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

float AutoScrollTextWnd::GetScrollSpeed() const
{
    throw std::logic_error("Not implemented");
}

void AutoScrollTextWnd::SetWrapMode(m3d::TextWrapFlags)
{
    throw std::logic_error("Not implemented");
}

int AutoScrollTextWnd::SetText(CStr const&)
{
    throw std::logic_error("Not implemented");
}

void AutoScrollTextWnd::SetDefaultFont(CStr const&, float, m3d::ui::FontType, m3d::ui::FontParams)
{
    throw std::logic_error("Not implemented");
}

void AutoScrollTextWnd::SetDefaultFont(int)
{
    throw std::logic_error("Not implemented");
}

AutoScrollTextWnd::~AutoScrollTextWnd()
{
    throw std::logic_error("Not implemented");
}

bool AutoScrollTextWnd::IsScrolling() const
{
    throw std::logic_error("Not implemented");
}

void AutoScrollTextWnd::SetPaneFlags(int)
{
    throw std::logic_error("Not implemented");
}

void AutoScrollTextWnd::SetPane(CStr const&)
{
    throw std::logic_error("Not implemented");
}

void AutoScrollTextWnd::SetScrollSpeed(float)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* AutoScrollTextWnd::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

void AutoScrollTextWnd::DrawWndText(m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}

bool AutoScrollTextWnd::NeedScroll() const
{
    throw std::logic_error("Not implemented");
}

void AutoScrollTextWnd::HandleScroll()
{
    throw std::logic_error("Not implemented");
}

AutoScrollTextWnd::AutoScrollTextWnd()
{
    throw std::logic_error("Not implemented");
}

AutoScrollTextWnd::AutoScrollTextWnd(AutoScrollTextWnd const&)
{
    throw std::logic_error("Not implemented");
}

void AutoScrollTextWnd::SetZeroTextOrigin()
{
    throw std::logic_error("Not implemented");
}

void AutoScrollTextWnd::PrecalcTextBounds()
{
    throw std::logic_error("Not implemented");
}

void AutoScrollTextWnd::ScrollTextOrigin()
{
    throw std::logic_error("Not implemented");
}

void AutoScrollTextWnd::PrecalcScrollPixelSpeed()
{
    throw std::logic_error("Not implemented");
}

bool AutoScrollTextWnd::TextOrignReachedFinalPosition() const
{
    throw std::logic_error("Not implemented");
}

int AutoScrollTextWnd::OnPaint(m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}
