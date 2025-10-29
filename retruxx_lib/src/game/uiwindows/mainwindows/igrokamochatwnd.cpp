#include "igrokamochatwnd.h"

RT_CLASS_EXPORTS_BEGIN(IgrokaMochatWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(IgrokaMochatWnd);

IgrokaMochatWnd::AuxInfo::AuxInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* IgrokaMochatWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Object* IgrokaMochatWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* IgrokaMochatWnd::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* IgrokaMochatWnd::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

IgrokaMochatWnd::~IgrokaMochatWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int IgrokaMochatWnd::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int IgrokaMochatWnd::GameDataSetup()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int IgrokaMochatWnd::GameDataUpdate(void*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int IgrokaMochatWnd::OnPaint(m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void IgrokaMochatWnd::HideBorders()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void IgrokaMochatWnd::UpdateAlpha()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void IgrokaMochatWnd::UpdateStartTimes()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void IgrokaMochatWnd::OnPlayerVehicleDamaged(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

IgrokaMochatWnd::IgrokaMochatWnd(IgrokaMochatWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

IgrokaMochatWnd::IgrokaMochatWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void IgrokaMochatWnd::OnNewFrame()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void IgrokaMochatWnd::ShowBorder(int, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

std::vector<int, std::allocator<int>> IgrokaMochatWnd::GetBordersByAttackerId(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int IgrokaMochatWnd::OnBeforeAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}
