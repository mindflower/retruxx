#include "twinklinglampwnd.h"

RT_CLASS_EXPORTS_BEGIN(TwinklingLampWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(TwinklingLampWnd);

void TwinklingLampWnd::SetValue(float, float)
{
    RETRUXX_NOT_IMPLEMENTED;
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

int TwinklingLampWnd::CreateFromPattern(m3d::ui::Wnd*, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* TwinklingLampWnd::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
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
    RETRUXX_NOT_IMPLEMENTED;
}

TwinklingLampWnd::TwinklingLampWnd(TwinklingLampWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

TwinklingLampWnd::TwinklingLampWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int TwinklingLampWnd::OnTick(int, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void TwinklingLampWnd::UpdateLamp()
{
    RETRUXX_NOT_IMPLEMENTED;
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
