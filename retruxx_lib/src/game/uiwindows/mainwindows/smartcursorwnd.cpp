#include "smartcursorwnd.h"

#include "config.h"
#include "m3dapp.h"
#include "core/kernel.h"

RT_CLASS_EXPORTS_BEGIN(SmartCursorWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(SmartCursorWnd);

SmartCursorWnd::AuxDrawInfo::AuxDrawInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SmartCursorWnd::AuxDrawInfo::SetTexture(m3d::rend::TexHandle)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::rend::TexHandle SmartCursorWnd::AuxDrawInfo::GetTexture() const
{
    return m_tex;
}

SmartCursorWnd::AuxDrawInfo::~AuxDrawInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* SmartCursorWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* SmartCursorWnd::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* SmartCursorWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

SmartCursorWnd::~SmartCursorWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* SmartCursorWnd::GetClass() const
{
    return RT_CLASS_LOCAL(SmartCursorWnd);
}

SmartCursorWnd::SmartCursorWnd(SmartCursorWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

SmartCursorWnd::SmartCursorWnd() = default;

void SmartCursorWnd::OnNewFrame()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SmartCursorWnd::OnPaint(m3d::ui::DrawInfo const& di)
{
    Wnd::OnPaint(di);

    M3D_RENDERER->SetAlphaTest(M3D_ENGINE_CFG.m_alphaTestInterface.GetI());
    M3D_RENDERER->SetStageState(0, m3d::rend::BM_COLOR, m3d::rend::TS_MODULATE);
    M3D_RENDERER->SetStageState(0, m3d::rend::BM_ALPHA, m3d::rend::TS_MODULATE);
    M3D_RENDERER->SetStageState(1, m3d::rend::BM_COLOR, m3d::rend::TS_NONE);
    M3D_RENDERER->SetStageState(1, m3d::rend::BM_COLOR, m3d::rend::TS_NONE);
    M3D_RENDERER->PushBlend(m3d::rend::BM_ALPHA);
    M3D_RENDERER->PushZbState(m3d::rend::ZB_DISABLE);

    for (auto& drawInfo : m_drawInfo)
    {
        if (drawInfo)
        {
            DrawTexture(di, drawInfo);
        }
    }

    M3D_RENDERER->SetAlphaTest(0);
    M3D_RENDERER->PopBlend();
    M3D_RENDERER->PopZbState();
    return 1;
}

int SmartCursorWnd::GameDataUpdate(void*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SmartCursorWnd::ClearDrawInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SmartCursorWnd::DrawTexture(m3d::ui::DrawInfo const& di, AuxDrawInfo const* texInfo) const
{
    if (texInfo)
    {
        if (!texInfo->GetTexture().IsValid())
        {
            M3D_RENDERER->SetWhiteTexture(0);
        }
        else
        {
            M3D_RENDERER->SetTexture(0, texInfo->GetTexture(), -1.0);
        }
        M3D_APP->PutSpriteRelRot(
            texInfo->m_coord.x + (float)((float)((float)(di.m_originalRect.x0 * 2.0) + di.m_originalRect.width) * 0.5),
            texInfo->m_coord.y + (float)((float)((float)(di.m_originalRect.y0 * 2.0) + di.m_originalRect.height) * 0.5),
            texInfo->m_sz.x * 0.5,
            texInfo->m_sz.y * 0.5,
            texInfo->m_color,
            texInfo->m_angle,
            0.0,
            0.0,
            0.0,
            0.0,
            0.0);
    }
}
