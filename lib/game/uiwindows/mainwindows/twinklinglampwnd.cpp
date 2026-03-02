#include "twinklinglampwnd.h"
#include <core/log.h>

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
    RETRUXX_NOT_IMPLEMENTED;
}

void TwinklingLampWnd::SetTwinklePeriod(unsigned)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* TwinklingLampWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ImageWnd);
}

int TwinklingLampWnd::CreateFromPattern(m3d::ui::Wnd* patternWnd, bool deleteSrc)
{
    // TODO: check this!!
    using namespace m3d::ui;

    if (!patternWnd)
    {
        M3D_LOG_INFO("TwinklingLampWnd::CreateFromPattern error - null patternWnd");
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

void TwinklingLampWnd::SetThreshold(float)
{
    RETRUXX_NOT_IMPLEMENTED;
}

TwinklingLampWnd::~TwinklingLampWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* TwinklingLampWnd::GetClass() const
{
    return RT_CLASS_LOCAL(TwinklingLampWnd);
}

TwinklingLampWnd::TwinklingLampWnd(TwinklingLampWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
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
    RETRUXX_NOT_IMPLEMENTED;
}

void TwinklingLampWnd::Show(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void TwinklingLampWnd::Twinkle()
{
    RETRUXX_NOT_IMPLEMENTED;
}
