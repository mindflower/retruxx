#include "twinklinglampwnd.h"
#include <core/kernel.h>
#include <core/log.h>
#include <core/timer.h>

RT_CLASS_EXPORTS_BEGIN(TwinklingLampWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(TwinklingLampWnd);

void TwinklingLampWnd::SetValue(float value, float maxValue)
{
    m_value = value;
    m_maxValue = maxValue;
}

m3d::Object* TwinklingLampWnd::Clone()
{
    // RVA 0x137010
    return new TwinklingLampWnd(*this);
}

void TwinklingLampWnd::SetTwinklePeriod(unsigned period)
{
    // RVA 0x137170
    m_twinklePeriod = period;
}

m3d::Class* TwinklingLampWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ImageWnd);
}

int TwinklingLampWnd::CreateFromPattern(m3d::ui::Wnd* patternWnd, bool deleteSrc)
{
    // RVA 0x137190
    using namespace m3d::ui;

    if (!patternWnd || !patternWnd->IsKindOf(&ImageWnd::m_classImageWnd))
    {
        M3D_LOG_INFO("TwinklingLampWnd: error to create - invalid pattern wnd");
        return 0;
    }

    auto* parent = patternWnd->GetParent();
    if (!parent || !IS_KIND_OF(parent, Wnd))
    {
        M3D_LOG_INFO("TwinklingLampWnd::CreateFromPattern error - null parent");
        return 0;
    }

    auto res = Wnd::Create(patternWnd->GetText(), patternWnd->GetStyle(), patternWnd->GetBounds(), patternWnd->GetId());
    if (res == 0)
    {
        M3D_LOG_INFO("TwinklingLampWnd::CreateFromPattern error - cannot create window");
        return 0;
    }

    SetStyle(patternWnd->GetStyle());
    SetText(patternWnd->GetText());
    SetId(patternWnd->GetId());
    SetName(patternWnd->GetName());
    SetBounds(patternWnd->GetBounds(), true);
    SetDefaultFont(patternWnd->GetDefaultFont());
    SetWrapMode(patternWnd->GetWrapMode());

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

    auto* imageWnd = (ImageWnd*)patternWnd;
    SetImage(imageWnd->GetImage());

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

m3d::Object* TwinklingLampWnd::CreateObject()
{
    return new TwinklingLampWnd;
}

void TwinklingLampWnd::SetThreshold(float threshold)
{
    // RVA 0x137180
    m_threshold = threshold;
}

TwinklingLampWnd::~TwinklingLampWnd()
{
    // RVA 0x137140 - the ImageWnd base cleans up automatically.
}

m3d::Class* TwinklingLampWnd::GetClass() const
{
    return RT_CLASS_LOCAL(TwinklingLampWnd);
}

TwinklingLampWnd::TwinklingLampWnd(TwinklingLampWnd const&) : TwinklingLampWnd()
{
    // NOTE: the shipped copy ctor (RVA 0x137130) default-constructs the
    // ImageWnd base only, leaving m_threshold/m_value/m_maxValue/
    // m_twinklePeriod uninitialized; delegating to the default ctor here
    // reproduces "nothing copied from source" without relying on garbage
    // memory for those fields.
}

TwinklingLampWnd::TwinklingLampWnd()
{
    m_threshold = 0.25;
    m_value = 0.0;
    m_maxValue = 0.0;
    m_twinklePeriod = 300;
}

int TwinklingLampWnd::OnTick(int curTime, int deltaTime)
{
    UpdateLamp();
    return Wnd::OnTick(curTime, deltaTime);
}

void TwinklingLampWnd::UpdateLamp()
{
    if ((m_gameDataFlags & 1) != 0)
    {
        float value = 0.0;
        if (m_maxValue == 0.0)
        {
            value = 0.0;
        }
        else
        {
            value = m_value / m_maxValue;
        }
        if (value <= m_threshold)
        {
            if (value > 0.0)
            {
                Twinkle();
            }
            else
            {
                ShowWindow(true);
            }
        }
        else
        {
            ShowWindow(false);
        }
    }
}

bool TwinklingLampWnd::Visible() const
{
    // RVA 0x137560
    return (m_style & 0x200) != 0;
}

void TwinklingLampWnd::Show(bool bShow)
{
    // RVA 0x137540
    if ((m_gameDataFlags & 1) != 0)
    {
        ShowWindow(bShow);
    }
}

void TwinklingLampWnd::Twinkle()
{
    // RVA 0x137570
    unsigned int const t = M3D_KERNEL->GetTimer().GetCurTimeUnscaled();
    bool const show = (t / m_twinklePeriod) & 1;
    ShowWindow(show);
}
