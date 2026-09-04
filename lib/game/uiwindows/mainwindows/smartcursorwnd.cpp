#include "smartcursorwnd.h"

#include "config.h"
#include "m3dapp.h"
#include "core/kernel.h"

RT_CLASS_EXPORTS_BEGIN(SmartCursorWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(SmartCursorWnd);

SmartCursorWnd::AuxDrawInfo::AuxDrawInfo()
{
    // RVA 0x132D80 - m_tex already defaults to an invalid handle via TexHandle's own ctor.
    m_sz = PointBase<float>{0.0f, 0.0f};
    m_angle = 0.0f;
    m_color = 0xFFFFFFFFu;
    m_coord = PointBase<float>{0.0f, 0.0f};
}

SmartCursorWnd::AuxDrawInfo::AuxDrawInfo(SmartCursorWnd::AuxDrawInfo const& rhs) :
    m_sz(rhs.m_sz),
    m_angle(rhs.m_angle),
    m_color(rhs.m_color),
    m_coord(rhs.m_coord)
{
    // NOTE: not present in the decompiled binary (never instantiated there);
    // reconstructed to mirror SetTexture's reference-counting semantics.
    SetTexture(rhs.m_tex);
}

void SmartCursorWnd::AuxDrawInfo::SetTexture(m3d::rend::TexHandle tex)
{
    // RVA 0x132DD0
    M3D_RENDERER->ReleaseTexture(m_tex);
    m_tex = tex;
    M3D_RENDERER->ReferenceTexture(m_tex);
}

m3d::rend::TexHandle SmartCursorWnd::AuxDrawInfo::GetTexture() const
{
    return m_tex;
}

SmartCursorWnd::AuxDrawInfo::~AuxDrawInfo()
{
    // RVA 0x132DB0
    M3D_RENDERER->ReleaseTexture(m_tex);
}

m3d::Object* SmartCursorWnd::Clone()
{
    // RVA 0x132870
    return new SmartCursorWnd(*this);
}

m3d::Object* SmartCursorWnd::CreateObject()
{
    // RVA 0x1328E0
    return new SmartCursorWnd;
}

m3d::Class* SmartCursorWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

SmartCursorWnd::~SmartCursorWnd()
{
    // RVA 0x1329C0
    ClearDrawInfo();
}

m3d::Class* SmartCursorWnd::GetClass() const
{
    return RT_CLASS_LOCAL(SmartCursorWnd);
}

SmartCursorWnd::SmartCursorWnd(SmartCursorWnd const&) : SmartCursorWnd()
{
    // Matches the original: the copy ctor default-constructs the base and an
    // empty draw-info vector; nothing is copied from the source.
}

SmartCursorWnd::SmartCursorWnd() = default;

void SmartCursorWnd::OnNewFrame()
{
    // RVA 0x132A30
    ClearDrawInfo();
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

int SmartCursorWnd::GameDataUpdate(void*, int dataType)
{
    // RVA 0x132A10
    if (dataType == 89)
    {
        OnNewFrame();
    }
    return 1;
}

void SmartCursorWnd::ClearDrawInfo()
{
    // RVA 0x132A40 - ~AuxDrawInfo() releases the texture reference.
    for (auto* info : m_drawInfo)
    {
        delete info;
    }
    m_drawInfo.clear();
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
