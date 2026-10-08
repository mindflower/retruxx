#include "speedometerwnd.h"

#include <cmath>

#include "config.h"
#include "electronicdigitalwnd.h"
#include "m3dapp.h"
#include "core/kernel.h"
#include "core/log.h"
#include <game/m3dgame.h>
#include "server/objects/player.h"
#include "server/objects/vehicle.h"

RT_CLASS_EXPORTS_BEGIN(SpeedometerWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(SpeedometerWnd);

m3d::rend::TexHandle SpeedometerWnd::ArrowPointer::GetTexture() const
{
    return m_texture;
}

void SpeedometerWnd::ArrowPointer::SetTexture(m3d::rend::TexHandle tex)
{
    M3D_RENDERER->ReleaseTexture(m_texture);
    m_texture = tex;
    M3D_RENDERER->ReferenceTexture(m_texture);
}

SpeedometerWnd::ArrowPointer::~ArrowPointer()
{
    M3D_RENDERER->ReleaseTexture(m_texture);
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
    return new SpeedometerWnd(*this);
}

m3d::Object* SpeedometerWnd::CreateObject()
{
    return new SpeedometerWnd;
}

SpeedometerWnd::~SpeedometerWnd()
{
    // m_speedPointer (ArrowPointer) releases its texture, m_aif frees its strings and
    // ~Wnd runs, all via the compiler-chained member destructors.
}

m3d::Class* SpeedometerWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

void SpeedometerWnd::UpdateSpeedNumberColor(MotionDir dir)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    if (dir == DIR_FORWARD)
    {
        m_wndSpeedNumber->SetDigitalColor(m_aif.m_colorForvard);
    }
    else if (dir == DIR_BACKWARD)
    {
        m_wndSpeedNumber->SetDigitalColor(m_aif.m_colorBackward);
    }
}

SpeedometerWnd::SpeedometerWnd(SpeedometerWnd const&) : SpeedometerWnd()
{
}

SpeedometerWnd::SpeedometerWnd()
{
    m_wndSpeedNumber = 0;
}

void SpeedometerWnd::UpdateSpeedNumber(float velocity, MotionDir dir)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    if (dir == DIR_DEAD)
    {
        m_wndSpeedNumber->Clear();
    }
    else
    {
        m_wndSpeedNumber->ShowNumber(static_cast<int>(velocity * 3.5999999f), false, 3, false);
    }
}

void SpeedometerWnd::OnNewFrame()
{
    float velocity = 0.0f;
    MotionDir dir = DIR_DEAD;
    GetVelocity(velocity, dir);

    UpdateSpeedPointer(velocity, dir);
    UpdateSpeedNumber(velocity, dir);
    UpdateSpeedNumberColor(dir);
    UpdateSpeedNumberBgColor(dir);
}

int SpeedometerWnd::GameDataUpdate(void*, int dataType)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (dataType == 89)
    {
        OnNewFrame();
    }
    return 1;
}

void SpeedometerWnd::UpdateSpeedNumberBgColor(MotionDir dir)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    if (dir == DIR_DEAD)
    {
        m_wndSpeedNumber->SetStyle(m_wndSpeedNumber->GetStyle() | m3d::ui::WS_NOFRAME);
        return;
    }

    m_wndSpeedNumber->SetStyle(m_wndSpeedNumber->GetStyle() & ~static_cast<unsigned>(m3d::ui::WS_NOFRAME));
    m_wndSpeedNumber->SetColor(dir == DIR_BACKWARD ? m_aif.m_colorBackward : m_aif.m_colorForvard);
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

void SpeedometerWnd::GetVelocity(float& velocity, MotionDir& dir) const
{
    velocity = 0.0f;
    dir = DIR_DEAD;

    if (!ai::thePlayer)
    {
        return;
    }

    ai::Vehicle const* vehicle = ai::thePlayer->GetVehicle();
    if (!vehicle)
    {
        return;
    }

    CVector const linearVelocity = vehicle->GetLinearVelocity();
    CVector const direction = vehicle->GetDirection();

    if (std::fabs(linearVelocity.y) < 0.001f || std::fabs(linearVelocity.x) >= 0.001f ||
        std::fabs(linearVelocity.z) >= 0.001f)
    {
        velocity = std::sqrt(
            linearVelocity.z * linearVelocity.z + linearVelocity.x * linearVelocity.x +
            linearVelocity.y * linearVelocity.y);
    }
    else
    {
        velocity = 0.0f;
    }

    float const dot = direction.z * linearVelocity.z + direction.y * linearVelocity.y + direction.x * linearVelocity.x;
    dir = (dot < 0.0f && static_cast<int>(velocity * 3.5999999f) > 0) ? DIR_BACKWARD : DIR_FORWARD;
}

void SpeedometerWnd::UpdateSpeedPointer(float velocity, MotionDir dir)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    float maxVelocity = 0.0f;
    if (ai::thePlayer && ai::thePlayer->GetVehicle())
    {
        maxVelocity = ai::thePlayer->GetVehicle()->GetMaxSpeed();
    }

    float angle = 0.0f;
    if (maxVelocity != 0.0f)
    {
        float v = velocity;
        if (v < 0.0f)
        {
            v = 0.0f;
        }
        if (v > maxVelocity)
        {
            v = maxVelocity;
        }

        float const endAngle =
            (dir == DIR_BACKWARD) ? m_aif.m_speedPointerMaxAngleBackward : m_aif.m_speedPointerMaxAngle;
        angle = (endAngle - m_aif.m_speedPointerMinAngle) * (v / maxVelocity) + m_aif.m_speedPointerMinAngle;
    }

    float const radiusNeg = 0.0f - m_aif.m_speedPointerRadius;
    float const s = std::sin(angle);
    float const c = std::cos(angle);

    m_speedPointer.m_angle = angle;
    m_speedPointer.m_coords.x = m_aif.m_speedPointerCenter.x + (c * 0.0f - radiusNeg * s);
    m_speedPointer.m_coords.y = m_aif.m_speedPointerCenter.y + (radiusNeg * c + s * 0.0f);
}

int SpeedometerWnd::GameDataSetup()
{
    int result = 1;

    if ((m_gameDataFlags & 2) == 0)
    {
        m3d::Object* child = GetChildByName(m_aif.m_wndSpeedNumberName);
        if (child != nullptr && child->IsKindOf(&m3d::ui::Wnd::m_classWnd))
        {
            m_wndSpeedNumber = static_cast<ElectronicDigitalWnd*>(M3D_KERNEL->New("ElectronicDigitalWnd"));
            if (m_wndSpeedNumber)
            {
                if (m_wndSpeedNumber->CreateFromPattern(static_cast<m3d::ui::Wnd*>(child), true))
                {
                    m_wndSpeedNumber->SetDigitalSize(ElectronicDigitalWnd::DIGITAL_SIZE_SMALL);
                }
                else
                {
                    M3D_LOG_INFO(
                        "Make control error: cannot create " + m_aif.m_wndSpeedNumberName + " from pattern class");
                    result = 0;
                }
            }
            else
            {
                M3D_LOG_INFO(
                    "Make control error: cannot create " + m_aif.m_wndSpeedNumberName +
                    " - cannot find rtti class ElectronicDigitalWnd");
                result = 0;
            }
        }
        else
        {
            M3D_LOG_INFO(
                "Make control error: control " + m_aif.m_wndSpeedNumberName + " is not found or incorrect type");
            result = 0;
        }

        auto const tex = M3D_APP->m_pInterfaceManager->GetIcoByName(m_aif.m_speedPointerTextureName, 0);
        m_speedPointer.SetTexture(tex);
        m_speedPointer.m_size.x = m_aif.m_speedPointerSize.x;
        m_speedPointer.m_size.y = m_aif.m_speedPointerSize.y;

        if (result)
        {
            m_gameDataFlags |= 1u;
        }
    }

    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }

    M3D_LOG_INFO("SpeedometerWnd: error - fail to init because of a bad resource");
    return 0;
}

ai::Vehicle const* SpeedometerWnd::GetVehicle() const
{
    if (ai::thePlayer)
    {
        return ai::thePlayer->GetVehicle();
    }
    return nullptr;
}
