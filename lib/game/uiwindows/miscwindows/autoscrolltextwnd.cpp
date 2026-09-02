#include "autoscrolltextwnd.h"

#include "core/log.h"
#include "ui/ui_srv.h"

RT_CLASS_EXPORTS_BEGIN(AutoScrollTextWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(AutoScrollTextWnd);

int AutoScrollTextWnd::SetBackground(m3d::rend::TexHandle bgTex)
{
    auto res = Wnd::SetBackground(bgTex);
    PrecalcTextBounds();
    return res;
}

int AutoScrollTextWnd::SetBackground(CStr const& bgTextureName)
{
    auto res = Wnd::SetBackground(bgTextureName);
    PrecalcTextBounds();
    return res;
}

void AutoScrollTextWnd::StartScroll(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void AutoScrollTextWnd::SetBounds(const BoundsBase<float>& rect, bool bUpdateBaseOrigin)
{
    m_bounds = rect;
    if (bUpdateBaseOrigin)
    {
        m_baseOrigin.x = m_bounds.x0;
        m_baseOrigin.y = m_bounds.y0;
    }
    PrecalcTextBounds();
}

m3d::Object* AutoScrollTextWnd::CreateObject()
{
    return new AutoScrollTextWnd;
}

m3d::Class* AutoScrollTextWnd::GetClass() const
{
    return RT_CLASS_LOCAL(AutoScrollTextWnd);
}

int AutoScrollTextWnd::CreateFromPattern(m3d::ui::Wnd* patternWnd, bool deleteSrc)
{
    if (!patternWnd)
    {
        M3D_LOG_INFO("AutoScrollTextWnd::CreateFromPattern error - null patternWnd");
        return 0;
    }

    auto res = Wnd::Create(patternWnd->GetText(), patternWnd->GetStyle(), patternWnd->GetBounds(), patternWnd->GetId());
    if (res == 0)
    {
        M3D_LOG_INFO("AutoScrollTextWnd::CreateFromPattern error - cannot create window");
        return 0;
    }

    SetStyle(patternWnd->GetStyle());
    SetText(patternWnd->GetText());
    SetId(patternWnd->GetId());
    SetName(patternWnd->GetName());
    SetBounds(patternWnd->GetBounds(), true);
    SetDefaultFont(patternWnd->GetDefaultFont());
    SetWrapMode(patternWnd->GetWrapMode());

    PrecalcTextBounds();

    SetFormatMode(patternWnd->GetFormatMode());
    SetColor(patternWnd->GetColor());
    SetTextColor(patternWnd->GetColor());
    SetTextColorDisabled(patternWnd->GetTextColorDisabled());
    SetClientEdges(patternWnd->GetClientEdges());
    SetPane(patternWnd->GetPaneName());
    SetPaneFlags(patternWnd->GetPaneFlags());
    SetScrollPane(patternWnd->GetScrollPaneName());
    SetBackground(patternWnd->GetBackground());

    CStr tooltip;
    patternWnd->GetProperty(PROP_WND_TOOLTIP, &tooltip);
    SetProperty(PROP_WND_TOOLTIP, &tooltip);

    SetOnShowAnimation(patternWnd->GetOnShowAnimation());
    SetOnHideAnimation(patternWnd->GetOnHideAnimation());

    auto* parent = patternWnd->GetParent();
    if (!parent || !IS_KIND_OF(parent, Wnd))
    {
        M3D_LOG_INFO("AutoScrollTextWnd::CreateFromPattern error - null parent for paternWnd");
        return 0;
    }

    parent->AddChild(this);
    if (deleteSrc)
    {
        parent->RemoveChild(patternWnd);
        // TODO: check this obj delete
        patternWnd->DecRef();
    }

    m_gameDataFlags |= 1u;
    return 1;
}

void AutoScrollTextWnd::SetClientEdges(std::vector<float, std::allocator<float>> const& clientEdges)
{
    Wnd::SetClientEdges(clientEdges);
    PrecalcTextBounds();
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

void AutoScrollTextWnd::SetWrapMode(m3d::TextWrapFlags flags)
{
    Wnd::SetWrapMode(flags);
    PrecalcTextBounds();
}

int AutoScrollTextWnd::SetText(CStr const& caption)
{
    auto res = Wnd::SetText(caption);
    PrecalcTextBounds();
    return res;
}

void AutoScrollTextWnd::SetDefaultFont(CStr const&, float, m3d::ui::FontType, m3d::ui::FontParams)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void AutoScrollTextWnd::SetDefaultFont(int uiFont)
{
    Wnd::SetDefaultFont(uiFont);
    PrecalcTextBounds();
    PrecalcScrollPixelSpeed();
}

AutoScrollTextWnd::~AutoScrollTextWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool AutoScrollTextWnd::IsScrolling() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void AutoScrollTextWnd::SetPaneFlags(int flags)
{
    Wnd::SetPaneFlags(flags);
    PrecalcTextBounds();
}

void AutoScrollTextWnd::SetPane(const CStr& name)
{
    Wnd::SetPane(name);
    PrecalcTextBounds();
}

void AutoScrollTextWnd::SetScrollSpeed(float speed)
{
    m_scrollLineSpeed = speed;
    PrecalcScrollPixelSpeed();
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
    m_scrollLineSpeed = 2.0;
    m_textBounds.x0 = 0.0;
    m_textBounds.y0 = 0.0;
    m_textBounds.width = 0.0;
    m_bScrollingEnabled = true;
    m_textBounds.height = 0.0;
    PrecalcScrollPixelSpeed();
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
    const auto bounds = GetClientBounds();
    const auto textMeasure = GetGfxServer()->MeasureText(m_caption, m_defFont, m_textWrap, bounds.width);

    // TODO: check this
    m_textBounds.x0 = bounds.x0;
    m_textBounds.y0 = bounds.y0;
    m_textBounds.width = (bounds.width + bounds.x0) - bounds.x0;
    m_textBounds.height = (textMeasure.y + bounds.y0) - bounds.y0;
}

void AutoScrollTextWnd::ScrollTextOrigin()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void AutoScrollTextWnd::PrecalcScrollPixelSpeed()
{
    const auto averageLineH = GetGfxServer()->MeasureText("Ag", m_defFont, m3d::TW_NOWRAP, 1024.0).y;
    m_scrollPixelSpeed = m_scrollLineSpeed * averageLineH;
}

bool AutoScrollTextWnd::TextOrignReachedFinalPosition() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int AutoScrollTextWnd::OnPaint(m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}
