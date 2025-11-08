#include <flares.h>
#include <m3dapp.h>
#include <stdexcept>
#include <core/stringm3d.h>

namespace m3d
{
    rend::TexHandle CFlare::m_tex[6];
    rend::TexHandle CFlare::m_texSunGlow;

    CFlare::~CFlare()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

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
        RETRUXX_NOT_IMPLEMENTED;
    }

    CFlare::CFlare()
    {
        this->m_percentageVisible = 0.0;
        this->m_event = 0;
        this->m_glowOnScreen = 0;
    }

    int CFlare::Render(FlareMode, CVector const&, float, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}
