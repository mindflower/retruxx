#include "autoscrolltextwnd.h"

#include "core/kernel.h"
#include "core/log.h"
#include "core/timer.h"
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

void AutoScrollTextWnd::StartScroll(bool bFromCurrentPosition)
{
    // RVA 0x4A5490
    m_bScrollingEnabled = true;
    if (!bFromCurrentPosition)
    {
        SetZeroTextOrigin();
    }
}

void AutoScrollTextWnd::SetBounds(BoundsBase<float> const& rect, bool bUpdateBaseOrigin)
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
    // RVA 0x4A50E0 copies the pattern's text colour here, not its fill colour.
    SetTextColor(patternWnd->GetTextColor());
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
        // The binary runs patternWnd's scalar deleting destructor here; DecRef()
        // is the ref-counting-safe equivalent once the parent's reference is gone.
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

void AutoScrollTextWnd::SetClientEdges(float left, float top, float right, float bottom)
{
    // RVA 0x4A59A0 writes the four edges straight into m_clientEdges' storage
    // (the Wnd ctor sizes it to 4); Wnd::SetClientEdges does the same.
    Wnd::SetClientEdges(left, top, right, bottom);
    PrecalcTextBounds();
}

void AutoScrollTextWnd::StopScroll()
{
    // RVA 0x4A54D0
    m_bScrollingEnabled = false;
}

m3d::Object* AutoScrollTextWnd::Clone()
{
    // RVA 0x4A4FA0 allocates raw storage, runs the plain Wnd ctor and patches
    // the vtable - it copies nothing. new AutoScrollTextWnd(*this) routes through
    // the copy ctor below, which does carry the scroll state across.
    return new AutoScrollTextWnd(*this);
}

float AutoScrollTextWnd::GetScrollSpeed() const
{
    // RVA 0x4A5540 (Hex-Rays drops the xmm0 float return); mirror of SetScrollSpeed.
    return m_scrollLineSpeed;
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

void AutoScrollTextWnd::SetDefaultFont(
    CStr const& name,
    float height,
    m3d::ui::FontType type,
    m3d::ui::FontParams params)
{
    // RVA 0x4A5AF0
    Wnd::SetDefaultFont(name, height, type, params);
    PrecalcTextBounds();
    PrecalcScrollPixelSpeed();
}

void AutoScrollTextWnd::SetDefaultFont(int uiFont)
{
    Wnd::SetDefaultFont(uiFont);
    PrecalcTextBounds();
    PrecalcScrollPixelSpeed();
}

AutoScrollTextWnd::~AutoScrollTextWnd()
{
    // RVA 0x4A50D0: no owned resources; ~Wnd does the rest.
}

bool AutoScrollTextWnd::IsScrolling() const
{
    // RVA 0x4A54E0 - identical body to NeedScroll: scrolling is enabled and the
    // text origin has not yet reached its final position.
    return m_bScrollingEnabled && !TextOrignReachedFinalPosition();
}

void AutoScrollTextWnd::SetPaneFlags(int flags)
{
    Wnd::SetPaneFlags(flags);
    PrecalcTextBounds();
}

void AutoScrollTextWnd::SetPane(CStr const& name)
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

void AutoScrollTextWnd::DrawWndText(m3d::ui::DrawInfo const& di)
{
    // RVA 0x4A5570: build the text rect in screen space, intersect it with the
    // clip rect and, when the intersection is non-degenerate, draw the base text
    // clipped to a DrawInfo whose client rect is the full text rect.
    auto const screen = ToScreen(PointBase<float>{m_textBounds.x0, m_textBounds.y0});
    float const sx = screen.x;
    float const sy = screen.y;
    float const sw = m_textBounds.width;
    float const sh = m_textBounds.height;

    auto const& clip = di.m_clippedRect;
    float const clipRight = clip.x0 + clip.width;
    float const clipBottom = clip.y0 + clip.height;

    float iw = 0.0f;
    float ih = 0.0f;
    bool const disjoint = clip.x0 > sx + sw || sx > clipRight || clip.y0 > sy + sh || sy > clipBottom;
    if (!disjoint)
    {
        float const ix0 = clip.x0 > sx ? clip.x0 : sx;
        float ix1 = sx + sw;
        if (ix1 > clipRight)
        {
            ix1 = clipRight;
        }
        float iy0 = clip.y0;
        if (iy0 <= sy)
        {
            iy0 = sy;
        }
        float iy1 = sy + sh;
        if (iy1 > clipBottom)
        {
            iy1 = clipBottom;
        }
        iw = ix1 - ix0;
        ih = iy1 - iy0;
    }

    if (iw != 0.0f || ih != 0.0f)
    {
        m3d::ui::DrawInfo fakeDi(di);
        fakeDi.m_clientRect.x0 = sx;
        fakeDi.m_clientRect.y0 = sy;
        fakeDi.m_clientRect.width = sw;
        fakeDi.m_clientRect.height = sh;
        Wnd::DrawWndText(fakeDi);
    }
}

bool AutoScrollTextWnd::NeedScroll() const
{
    // RVA 0x4A5790
    return m_bScrollingEnabled && !TextOrignReachedFinalPosition();
}

void AutoScrollTextWnd::HandleScroll()
{
    // RVA 0x4A5720: advance the scroll origin one frame's worth while enabled and
    // not yet at the final position.
    if (m_bScrollingEnabled && !TextOrignReachedFinalPosition())
    {
        ScrollTextOrigin();
    }
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

AutoScrollTextWnd::AutoScrollTextWnd(AutoScrollTextWnd const& rhs) :
    Wnd(rhs),
    m_scrollLineSpeed(rhs.m_scrollLineSpeed),
    m_scrollPixelSpeed(rhs.m_scrollPixelSpeed),
    m_textOrigin(rhs.m_textOrigin),
    m_bScrollingEnabled(rhs.m_bScrollingEnabled),
    m_textBounds(rhs.m_textBounds)
{
    // NOTE: the shipped copy ctor (RVA 0x4A50C0) and Clone() (RVA 0x4A4FA0) only
    // default-construct the Wnd base and patch the vtable - they carry none of
    // the scroll state. We copy it so a clone is a usable widget.
}

void AutoScrollTextWnd::SetZeroTextOrigin()
{
    // RVA 0x4A5900: park the text origin at the bottom edge of the client area so
    // the text scrolls up into view.
    auto const clientB = GetClientBounds();
    m_textBounds.y0 = clientB.y0 + clientB.height;
}

void AutoScrollTextWnd::PrecalcTextBounds()
{
    // RVA 0x4A5810
    auto const bounds = GetClientBounds();
    auto const textMeasure = GetGfxServer()->MeasureText(m_caption, m_defFont, m_textWrap, bounds.width);

    // NOTE: m_textBounds.y0 is the live scrolling origin - the shipped code reads
    // it and writes the same value back, i.e. it must NOT be reset here.
    m_textBounds.x0 = bounds.x0;
    m_textBounds.width = bounds.width;
    m_textBounds.height = textMeasure.y;
}

void AutoScrollTextWnd::ScrollTextOrigin()
{
    // RVA 0x4A58C0: move the origin up by one unscaled frame time * pixel speed.
    double const dtSec = static_cast<double>(m3d::g_Kernel->GetTimer().GetLastFrameTimeUnscaled()) * 0.001;
    m_textBounds.y0 = static_cast<float>(m_textBounds.y0 - dtSec * m_scrollPixelSpeed);
}

void AutoScrollTextWnd::PrecalcScrollPixelSpeed()
{
    auto const averageLineH = GetGfxServer()->MeasureText("Ag", m_defFont, m3d::TW_NOWRAP, 1024.0).y;
    m_scrollPixelSpeed = m_scrollLineSpeed * averageLineH;
}

bool AutoScrollTextWnd::TextOrignReachedFinalPosition() const
{
    // RVA 0x4A57D0: the text's bottom (origin + height) has scrolled up past the
    // client top.
    auto const clientB = GetClientBounds();
    return clientB.y0 >= m_textBounds.height + m_textBounds.y0;
}

int AutoScrollTextWnd::OnPaint(m3d::ui::DrawInfo const& di)
{
    // RVA 0x4A5550
    HandleScroll();
    return Wnd::OnPaint(di);
}
