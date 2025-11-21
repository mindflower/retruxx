#include "speedometerwnd.h"

#include "config.h"
#include "m3dapp.h"
#include "core/kernel.h"
#include "server/objects/player.h"

RT_CLASS_EXPORTS_BEGIN(SpeedometerWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(SpeedometerWnd);

m3d::rend::TexHandle SpeedometerWnd::ArrowPointer::GetTexture() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SpeedometerWnd::ArrowPointer::SetTexture(m3d::rend::TexHandle)
{
    RETRUXX_NOT_IMPLEMENTED;
}

SpeedometerWnd::ArrowPointer::~ArrowPointer()
{
    RETRUXX_NOT_IMPLEMENTED;
}

SpeedometerWnd::ArrowPointer::ArrowPointer()
{
    m_coords.x = 0.0;
    m_coords.y = 0.0;
    m_angle = 0.0;
    m_size.x = 0.0;
    m_size.y = 0.0;
}

void SpeedometerWnd::ArrowPointer::Draw(m3d::ui::DrawInfo const& di)
{
    auto halfW = m_size.x * 0.5;
    auto xEdge = di.m_originalRect.x0;
    auto halfH = m_size.y * 0.5;
    auto yEdge = di.m_originalRect.y0;

    if (!m_texture.IsValid())
    {
        M3D_RENDERER->SetWhiteTexture(0);
    }
    else
    {
        M3D_RENDERER->SetTexture(0, m_texture, -1.0);
    }

    M3D_APP->PutSpriteRelRot(m_coords.x + xEdge, m_coords.y + yEdge, halfW, halfH, 0xFFFFFFFF, this->m_angle, 0.0, 0.0, 0.0, 0.0, 0.0);
}

SpeedometerWnd::AuxInfo::AuxInfo()
{
    m_wndSpeedNumberName = ("wndSpeedNumber");
    m_speedPointerTextureName = ("SpeedPointer");

    m_speedPointerSize = { 8.0f, 64.0f };
    m_speedPointerCenter = { 62.0f, 65.0f };
    m_speedPointerMinAngle = -0.78539819f;
    m_speedPointerMaxAngle = 1.5707964f;
    m_speedPointerMaxAngleBackward = -1.5707964f;
    m_speedPointerRadius = 27.0f;

    m_colorForvard = 0xFF8CD618;  // Green color
    m_colorBackward = 0xFFFF0000; // Red color
}

m3d::Class* SpeedometerWnd::GetClass() const
{
    return RT_CLASS_LOCAL(SpeedometerWnd);
}

m3d::Object* SpeedometerWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* SpeedometerWnd::CreateObject()
{
    return new SpeedometerWnd;
}

SpeedometerWnd::~SpeedometerWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* SpeedometerWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

void SpeedometerWnd::UpdateSpeedNumberColor(MotionDir)
{
    RETRUXX_NOT_IMPLEMENTED;
}

SpeedometerWnd::SpeedometerWnd(SpeedometerWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

SpeedometerWnd::SpeedometerWnd()
{
    m_wndSpeedNumber = 0;
}

void SpeedometerWnd::UpdateSpeedNumber(float, MotionDir)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SpeedometerWnd::OnNewFrame()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SpeedometerWnd::GameDataUpdate(void*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SpeedometerWnd::UpdateSpeedNumberBgColor(MotionDir)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SpeedometerWnd::OnPaint(m3d::ui::DrawInfo const& di)
{
    m3d::ui::Wnd::OnPaint(di);
    if (ai::thePlayer && ai::thePlayer->GetVehicle())
    {
        M3D_RENDERER->SetAlphaTest(M3D_ENGINE_CFG.m_alphaTestInterface.GetI());
        M3D_RENDERER->SetStageState(0, m3d::rend::BM_COLOR, m3d::rend::TS_MODULATE);
        M3D_RENDERER->SetStageState(0, m3d::rend::BM_ALPHA, m3d::rend::TS_MODULATE);
        M3D_RENDERER->SetStageState(1, m3d::rend::BM_COLOR, m3d::rend::TS_NONE);
        M3D_RENDERER->SetStageState(1, m3d::rend::BM_COLOR, m3d::rend::TS_NONE);
        M3D_RENDERER->PushBlend(m3d::rend::BM_ALPHA);
        M3D_RENDERER->PushZbState(m3d::rend::ZB_DISABLE);

        m_speedPointer.Draw(di);

        M3D_RENDERER->SetAlphaTest(0);
        M3D_RENDERER->PopBlend();
        M3D_RENDERER->PopZbState();
    }
    return 1;
}

void SpeedometerWnd::GetVelocity(float&, MotionDir&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SpeedometerWnd::UpdateSpeedPointer(float, MotionDir)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SpeedometerWnd::GameDataSetup()
{
    // TODO: implement SpeedometerWnd::GameDataSetup
    // RETRUXX_NOT_IMPLEMENTED;
    return 1;
}

ai::Vehicle const* SpeedometerWnd::GetVehicle() const
{
    RETRUXX_NOT_IMPLEMENTED;
}
