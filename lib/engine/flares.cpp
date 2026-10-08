#include <flares.h>
#include <m3dapp.h>
#include <stdexcept>
#include <core/stringm3d.h>
#include <client.h>
#include <world.h>
#include <cmath>
#include <set>
#include <Windows.h>
namespace m3d
{
    rend::TexHandle CFlare::m_tex[6];
    rend::TexHandle CFlare::m_texSunGlow;

    CFlare::~CFlare() = default;

    int CFlare::Init()
    {
        for (unsigned i = 0; i<6;++i)
        {
            auto fileName = "data\\fx\\flare" + CStr(i) + ".bmp";
            m_tex[i] = Application::g_pApp->m_renderer->AddTexture(fileName, 2);
        }
        m_texSunGlow = Application::g_pApp->m_renderer->AddTexture("data\\fx\\flare0.bmp", 2);
        return 1;
    }

    void CFlare::Release()
    {
        for (auto& tex : m_tex)
        {
            M3D_RENDERER->ReleaseTexture(tex);
        }
        M3D_RENDERER->ReleaseTexture(m_texSunGlow);
    }

    CFlare::CFlare()
    {
        this->m_percentageVisible = 0.0;
        this->m_event = 0;
        this->m_glowOnScreen = 0;
    }

    int CFlare::Render(FlareMode mode, CVector const& flareFrom, float size, float opacity)
    {
        // RVA 0x7ACA20 - FLARE_SUN draws a sprite spinning with the distance from the screen centre, FLARE_FLASH_NO_TEST
        // draws the glow and the lens flare chain; FLARE_TEST traces the scene for occlusion and FLARE_BLINDING adds
        // the glow and a full-screen whitening. Returns the number of triangles drawn.
        static float const glowScale = 4.0f;

        if (size < 0.1f)
        {
            size = 0.1f;
        }
        else if (size > 1.0f)
        {
            size = 1.0f;
        }
        int const width = M3D_RENDERER->GetViewport().m_width;
        int const height = M3D_RENDERER->GetViewport().m_height;
        float const fheight = static_cast<float>(height);
        float const w = fheight * size * 0.25f;
        int polys = 0;
        CVector const proj = M3D_RENDERER->Project(flareFrom);
        m_ssx = proj.x;
        m_ssy = proj.y;
        m_onScreen = false;
        m_glowOnScreen = 0;
        if (proj.z > 1.0f)
        {
            // NOTE: the visibility tests are only made for points the projection puts past z = 1.
            float half = w * 0.5f;
            if (!(0.0f > half + m_ssx) && !(m_ssx - half > static_cast<float>(width)) && !(0.0f > half + m_ssy)
                && !(m_ssy - half > fheight))
            {
                m_onScreen = true;
            }
            half = w * glowScale * 0.5f;
            if (!(0.0f > half + m_ssx) && !(m_ssx - half > static_cast<float>(width)) && !(0.0f > half + m_ssy)
                && !(m_ssy - half > fheight))
            {
                m_glowOnScreen = 1;
            }
        }

        bool const test = (mode & FLARE_TEST) != 0;
        bool const blinding = (mode & FLARE_BLINDING) != 0;
        M3D_RENDERER->SetStageState(0, rend::BM_COLOR, rend::TS_MODULATE);
        M3D_RENDERER->SetStageState(0, rend::BM_ALPHA, rend::TS_NONE);
        M3D_RENDERER->SetStageState(1, rend::BM_COLOR, rend::TS_NONE);
        M3D_RENDERER->SetStageState(1, rend::BM_ALPHA, rend::TS_NONE);
        M3D_RENDERER->PushBlend(rend::BM_1_1);
        M3D_RENDERER->PushZbState(rend::ZB_DISABLE);

        switch (mode & ~(FLARE_TEST | FLARE_BLINDING))
        {
        case FLARE_SUN:
            if (m_onScreen)
            {
                int const hw = width / 2;
                int const hh = height / 2;
                double const dx = m_ssx - static_cast<double>(hw);
                double const dy = m_ssy - static_cast<double>(hh);
                double const angle =
                    sqrt((dy * dy + dx * dx) / static_cast<double>(width * width + height * height) * 0.25) * 10.0;
                float const s = static_cast<float>(sin(angle));
                float const c = static_cast<float>(cos(angle));
                float const nw = 0.0f - w;
                M3D_RENDERER->SetTexture(0, m_tex[5], -1.0);
                M3D_APP->PutSpriteAbs(
                    nw * c - nw * s + m_ssx, nw * c + nw * s + m_ssy,
                    c * w - nw * s + m_ssx, nw * c + s * w + m_ssy,
                    nw * c - s * w + m_ssx, c * w + nw * s + m_ssy,
                    c * w - s * w + m_ssx, s * w + c * w + m_ssy,
                    0xFFFFFFFF);
                polys = 2;
            }
            break;

        case FLARE_FLASH_NO_TEST:
        {
            if (m_glowOnScreen && blinding)
            {
                M3D_RENDERER->SetTexture(0, m_texSunGlow, -1.0);
                M3D_APP->PutSpriteAbs(m_ssx, m_ssy, w * glowScale, 0xFF3F7F7F);
                polys = 2;
            }
            if (!m_onScreen)
            {
                break;
            }
            float const step = w * 0.5f;
            CVector const start = M3D_RENDERER->MatGetOrgInv();
            float visible = 0.0f;
            float const first = 0.0f - step;
            if (test)
            {
                // NOTE: the 2x2 samples start at -step and advance by step, so they cover the flare's top-left
                // quarter rather than being centred on it.
                float y = first;
                for (int row = 2; row; --row, y += step)
                {
                    float x = first;
                    for (int col = 2; col; --col, x += step)
                    {
                        CVector2 const pt(x + m_ssx, m_ssy + y);
                        CVector const dir = M3D_RENDERER->Unproject(pt);
                        // NOTE: the set of classes to trace holds a single null class.
                        retruxx::set<Class*> classes;
                        classes.insert(nullptr);
                        CVector const finish(dir.x * 100000.0f + start.x, start.y + dir.y * 100000.0f,
                                             start.z + dir.z * 100000.0f);
                        CVector hit;
                        if (!pClient->GetWorld().GetGraph().TraceLine(hit, start, finish, classes, 0))
                        {
                            visible = visible + 0.25f;
                        }
                    }
                }
            }
            else
            {
                visible = 1.0f;
            }

            if (visible == m_percentageVisible)
            {
                m_event = timeGetTime();
            }
            else
            {
                // Fades toward the new visibility over 500 ms.
                float const t = static_cast<float>(static_cast<double>(timeGetTime() - m_event) * 0.002f);
                if (t < 1.0f)
                {
                    m_percentageVisible = (visible - m_percentageVisible) * t + m_percentageVisible;
                }
                else
                {
                    m_percentageVisible = visible;
                }
            }
            if (m_percentageVisible == 0.0f)
            {
                break;
            }

            float const hw = static_cast<float>(width / 2);
            float const hh = static_cast<float>(height / 2);
            float const dx = m_ssx - hw;
            float const dy = m_ssy - hh;
            float const nx = dx / hw;
            float const ny = dy / hh;
            float dist = static_cast<float>(sqrt(static_cast<double>(ny) * ny + static_cast<double>(nx) * nx));
            if (!(1.0f > dist))
            {
                dist = 1.0f;
            }
            float intensity = (1.0f - dist) * (1.0f - dist) * (1.0f - dist) * m_percentageVisible * opacity;
            if (0.0039215689f > intensity)
            {
                break;
            }

            // The chain of flares along the line from the flare through the screen centre.
            static float const offsets[6] = {1.2f, -0.7f, -0.8f, -1.5f, -0.5f, -0.3f};
            static float const sizes[6] = {1.0f, 0.5f, 0.7f, 2.0f, 0.5f, 0.3f};
            static unsigned char const colors[6][3] = {
                {150, 150, 150}, {200, 200, 250}, {250, 140, 50}, {140, 200, 50}, {250, 140, 50}, {250, 140, 50}};
            polys += 12;
            for (int i = 0; i < 6; ++i)
            {
                M3D_RENDERER->SetTexture(0, m_tex[i], -1.0);
                unsigned int const r = static_cast<unsigned char>(static_cast<__int64>(static_cast<double>(colors[i][0]) * intensity));
                unsigned int const g = static_cast<unsigned char>(static_cast<__int64>(static_cast<double>(colors[i][1]) * intensity));
                unsigned int const b = static_cast<unsigned char>(static_cast<__int64>(static_cast<double>(colors[i][2]) * intensity));
                M3D_APP->PutSpriteAbs(offsets[i] * dx + hw, offsets[i] * dy + hh, sizes[i] * w,
                                      0xFF000000 | (r << 16) | (g << 8) | b);
            }

            if (blinding)
            {
                if (intensity > 0.5f)
                {
                    intensity = 0.5f;
                }
                unsigned int const a = static_cast<unsigned char>(static_cast<__int64>(static_cast<double>(intensity) * 255.0));
                rend::TexHandle const noTexture;
                M3D_RENDERER->SetTexture(0, noTexture, -1.0);
                M3D_APP->PutSpriteAbs(0.0f, 0.0f, static_cast<float>(width), fheight,
                                      0xFF000000 | (a << 16) | (a << 8) | a);
                polys += 2;
            }
            break;
        }

        default:
            break;
        }

        M3D_RENDERER->PopZbState();
        M3D_RENDERER->PopBlend();
        return polys;
    }
}
