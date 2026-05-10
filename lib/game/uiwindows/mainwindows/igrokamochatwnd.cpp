#include "igrokamochatwnd.h"
#include "include/ui/image.h"
#include <core/log.h>
#include <core/timer.h>
#include <m3dapp.h>

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
    int res = 1;
    if ((m_gameDataFlags & 2) == 0)
    {
        for (size_t idx = 0; idx < 4; ++idx)
        {
            auto* child = RT_DYNCAST(GetChildByName(m_aif.m_boderNames[idx]), m3d::ui::ImageWnd);
            if (child == nullptr)
            {
                M3D_LOG_INFO(
                    "Get control error: control " + m_aif.m_boderNames[idx] + " is not found or incorrect type");
                res = 0;
                continue;
            }

            m_borderTextures[idx] = child->GetImage();
            M3D_RENDERER->ReferenceTexture(m_borderTextures[idx]);

            m_borderBounds[idx] = child->GetBounds();

            // TODO: check this!!!!
            RemoveChild(child);
        }

        if (res)
        {
            m_gameDataFlags |= 1u;
        }
    }

    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }

    M3D_LOG_ERR("IgrokaMochatWnd: error - fail to init because of a bad resource");

    return 0;
}

int IgrokaMochatWnd::GameDataUpdate(void* data, int dataType)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (dataType == 76)
    {
        OnPlayerVehicleDamaged(data);
    }
    else if (dataType == 89)
    {
        OnNewFrame();
        return 1;
    }
    return 1;
}

int IgrokaMochatWnd::OnPaint(m3d::ui::DrawInfo const& di)
{
    m3d::ui::Wnd::OnPaint(di);
    for (size_t i = 0; i < 4; ++i)
    {
        if (m_borderAlpha[i])
        {
            GetGfxServer()->AddImagedRect(
                di, m_borderBounds[i], (m_borderAlpha[i] << 24) | 0xFFFFFF, m_borderTextures[i]);
        }
    }
    return 1;
}

void IgrokaMochatWnd::HideBorders()
{
    for (size_t i = 0; i < 4; ++i)
    {
        m_borderAlpha[i] = 0;
        m_borderStartTime[i] = 0;
    }
}

void IgrokaMochatWnd::UpdateAlpha()
{
    auto const curTime = M3D_KERNEL->GetTimer().GetCurTime();
    for (size_t i = 0; i < 4; ++i)
    {
        if (m_borderStartTime[i])
        {
            if (curTime >= m_borderStartTime[i] + 10)
            {
                auto alpha = (int)((1.0 - (double)(curTime - m_borderStartTime[i] - 10) * 0.0033333334) * 255.0);
                if (alpha >= 0)
                {
                    if (alpha > 255)
                        alpha = -1;
                }
                else
                {
                    alpha = 0;
                }
                m_borderAlpha[i] = alpha;
            }
            else
            {
                m_borderAlpha[i] = -1;
            }
        }
        else
        {
            m_borderAlpha[i] = 0;
        }
    }
}

void IgrokaMochatWnd::UpdateStartTimes()
{
    auto const curTime = M3D_KERNEL->GetTimer().GetCurTime();

    auto v4 = m_borderStartTime[0];
    if (v4 && curTime >= v4 + 310)
        m_borderStartTime[0] = 0;

    auto v5 = m_borderStartTime[1];
    if (v5 && curTime >= v5 + 310)
        m_borderStartTime[1] = 0;

    auto v6 = m_borderStartTime[2];
    if (v6 && curTime >= v6 + 310)
        m_borderStartTime[2] = 0;

    auto v7 = m_borderStartTime[3];
    if (v7)
    {
        if (curTime >= v7 + 310)
            m_borderStartTime[3] = 0;
    }
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
    static thread_local int frameCount = 0;
    if (++frameCount == 10)
    {
        frameCount = 0;
        if (m_integratedDamage > 40.0)
        {
            M3D_APP->AddPostEffect("DamageIntegrated", m_integratedDamage);
        }
        m_integratedDamage = 0.0;
    }
    UpdateStartTimes();
    UpdateAlpha();
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
    HideBorders();
    return m3d::ui::Wnd::OnBeforeAddToWndStation();
}
