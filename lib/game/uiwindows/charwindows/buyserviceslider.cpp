#include "buyserviceslider.h"

#include <m3dapp.h>
#include <ui/ui_srv.h>

RT_CLASS_EXPORTS_BEGIN(BuyServiceSlider)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(BuyServiceSlider);

BuyServiceSlider::BuyServiceSlider()
{
    // RVA 0x4370B0
    m_maxPossible = 0;
}

BuyServiceSlider::BuyServiceSlider(BuyServiceSlider const&) : BuyServiceSlider()
{
    // NOTE: the shipped copy ctor (RVA 0x437100) default-constructs the SliderWnd
    // base without looking at rhs and leaves m_maxPossible uninitialised;
    // delegating to the default ctor avoids reading an uninitialised value while
    // matching the "nothing is actually copied" behaviour.
}

BuyServiceSlider::~BuyServiceSlider()
{
    // RVA 0x437110 - nothing of its own to release.
}

m3d::Object* BuyServiceSlider::Clone()
{
    return new BuyServiceSlider(*this);
}

m3d::Object* BuyServiceSlider::CreateObject()
{
    return new BuyServiceSlider;
}

m3d::Class* BuyServiceSlider::GetBaseClass()
{
    return RT_CLASS_LOCAL(SliderWnd);
}

m3d::Class* BuyServiceSlider::GetClass() const
{
    return RT_CLASS_LOCAL(BuyServiceSlider);
}

int BuyServiceSlider::GetMaxPossible() const
{
    return m_maxPossible;
}

void BuyServiceSlider::SetMaxPossible(int val)
{
    // RVA 0x437130 - the cap is clamped into the slider's own range, and the
    // current notch is pulled back if it now sits past the cap.
    m_maxPossible = val;
    if (m_maxPossible < GetMin())
    {
        m_maxPossible = GetMin();
    }
    if (m_maxPossible > GetMax())
    {
        m_maxPossible = GetMax();
    }
    if (GetNotch() > m_maxPossible)
    {
        SetNotch(m_maxPossible);
    }
}

void BuyServiceSlider::SetMinMax(int min, int max)
{
    // RVA 0x437180
    SliderWnd::SetMinMax(min, max);
    if (m_maxPossible > max)
    {
        SetMaxPossible(max);
    }
}

void BuyServiceSlider::SetNotch(int n)
{
    // RVA 0x4371B0 - the notch can never be dragged past what the player can
    // actually afford.
    if (n < GetMin())
    {
        n = GetMin();
    }
    if (n > m_maxPossible)
    {
        n = m_maxPossible;
    }
    SliderWnd::SetNotch(n);
}

int BuyServiceSlider::OnPaint(m3d::ui::DrawInfo const& di)
{
    // RVA 0x4371E0. Same three-piece slider as the base class, except the stretch
    // between m_maxPossible and m_max - the part the player cannot pay for - is
    // painted red and the right-hand cap is dimmed.
    auto const leftTex = GetGfxServer()->GetTexture(m3d::ui::TEX_SLIDER_LEFT);
    int w = 0;
    int h = 0;
    M3D_APP->m_renderer->GetDims(leftTex, w, h);
    float sideW = static_cast<float>(w);
    float sideH = static_cast<float>(h);
    M3D_APP->m_renderer->AbsToRel(sideW, sideH);

    auto const bounds = GetBounds();
    BoundsBase<float> const rc0{0.0f, 0.0f, bounds.width, bounds.height};

    unsigned const clr = ((m_style & 2) != 0 || (m_style & 0x80000) != 0) ? 3u : m_curClr;
    bool const capped = m_maxPossible < GetMax();

    BoundsBase<float> rc;
    rc.x0 = 0.0f;
    rc.y0 = 0.0f;
    rc.width = sideW;
    rc.height = rc0.height;
    GetGfxServer()->AddImagedRect(di, rc, clr, leftTex);

    rc.x0 = rc0.width - sideW;
    rc.y0 = rc0.y0;
    rc.width = sideW;
    rc.height = rc0.height;
    GetGfxServer()->AddImagedRect(
        di, rc, capped ? 0x81000000u : clr, GetGfxServer()->GetTexture(m3d::ui::TEX_SLIDER_RIGHT));

    rc.x0 = rc0.x0 + sideW;
    rc.y0 = rc0.y0;
    rc.width = rc0.width - (sideW * 2.0f);
    rc.height = rc0.height;

    auto const bodyTex = GetGfxServer()->GetTexture(m3d::ui::TEX_SLIDER_BODY);
    if (capped)
    {
        float const frac = static_cast<float>(m_maxPossible) / static_cast<float>(GetMax() - GetMin());
        auto const bodyRect = GetBodyRect();
        float const splitX = (frac == 0.0f) ? sideW : (bodyRect.x0 + bodyRect.width * frac);

        BoundsBase<float> inactiveRc;
        inactiveRc.x0 = splitX;
        inactiveRc.y0 = rc.y0;
        inactiveRc.width = (rc0.width - splitX) - sideW;
        inactiveRc.height = rc.height;
        GetGfxServer()->AddImagedRectGeneral(
            di, inactiveRc, 0x80FF0000, bodyTex, 0.0f, 0.0f, inactiveRc.width / rc.height, 1.0f);

        rc.width = inactiveRc.x0 - rc.x0;
    }
    GetGfxServer()->AddImagedRectGeneral(di, rc, clr, bodyTex, 0.0f, 0.0f, rc.width / rc.height, 1.0f);

    float const notchFrac = static_cast<float>(GetNotch() - GetMin()) / static_cast<float>(GetMax() - GetMin());
    auto const bodyRect = GetBodyRect();
    BoundsBase<float> notchRc;
    notchRc.x0 = (bodyRect.x0 + bodyRect.width * notchFrac) - (m_notchWidth * 0.5f);
    notchRc.y0 = (((rc.y0 * 2.0f) + rc.height) * 0.5f) - (rc.height * 0.5f);
    notchRc.width = m_notchWidth;
    notchRc.height = rc.height;
    GetGfxServer()->AddImagedRect(di, notchRc, clr, GetGfxServer()->GetTexture(m3d::ui::TEX_SLIDER_THUMB));
    return 1;
}
