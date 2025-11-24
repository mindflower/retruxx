#include "igrokamochatwnd.h"

RT_CLASS_EXPORTS_BEGIN(IgrokaMochatWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(IgrokaMochatWnd);

IgrokaMochatWnd::AuxInfo::AuxInfo()
{
    m_boderNames[0] = "borderLeft";
    m_boderNames[1] = "borderTop";
    m_boderNames[2] = "borderRight";
    m_boderNames[3] = "borderBottom";
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
    return new IgrokaMochatWnd;
}

m3d::Class* IgrokaMochatWnd::GetClass() const
{
    return RT_CLASS_LOCAL(IgrokaMochatWnd);
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
    // TODO: implement IgrokaMochatWnd::GameDataSetup
    // RETRUXX_NOT_IMPLEMENTED;
    return 1;
}

int IgrokaMochatWnd::GameDataUpdate(void*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int IgrokaMochatWnd::OnPaint(m3d::ui::DrawInfo const&)
{
    // TODO: implement IgrokaMochatWnd::OnPaint
    // RETRUXX_NOT_IMPLEMENTED;
    return 1;
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
    // Initialize integrated damage
    m_integratedDamage = 0.0f;

    // Initialize border arrays
    for (int i = 0; i < 4; ++i)
    {
        // Initialize border bounds
        m_borderBounds[i].x0 = 0.0f;
        m_borderBounds[i].y0 = 0.0f;
        m_borderBounds[i].width = 0.0f;
        m_borderBounds[i].height = 0.0f;

        // Initialize border alpha and timing
        m_borderAlpha[i] = -1;
        m_borderStartTime[i] = 0;
    }
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
    // TODO: implement IgrokaMochatWnd::OnBeforeAddToWndStation
    // RETRUXX_NOT_IMPLEMENTED;
    return 1;
}
