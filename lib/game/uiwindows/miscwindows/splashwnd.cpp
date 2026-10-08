#include "splashwnd.h"

#include <config.h>
#include <core/log.h>
#include <game/m3dgame.h>
#include <game/uimanager/truxxuimanager.h>
#include <game/uimisc/guihelper.h>
#include <game/uimisc/levelinfo.h>
#include <m3dapp.h>
#include <ui/image.h>
#include <ui/progressbarwnd.h>
#include <ui/wndstation.h>

namespace
{
    // The splash screen is always painted at the design resolution, whatever the
    // real back buffer happens to be.
    BoundsBase<float> const SPLASH_VIEWPORT{0.0f, 0.0f, 1024.0f, 768.0f};
}  // namespace

RT_CLASS_EXPORTS_BEGIN(SplashWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(SplashWnd);

// Data at 0x9C3D00 - also the default value of m_numSplashes.
int const SplashWnd::NUM_LEVEL_SPLASHES = 11;

SplashWnd::AuxInfo::AuxInfo()
{
    // RVA 0x4C8280
    m_progressBarName = "progressBar";
    m_wndImageName = "wndImage";
    m_lblTextName = "lblText";
}

SplashWnd::SplashWnd()
{
    // RVA 0x4C8360
    m_progressBar = nullptr;
    m_wndImage = nullptr;
    m_lblText = nullptr;
    m_curSplash = -1;
    m_numSplashes = NUM_LEVEL_SPLASHES;
}

SplashWnd::SplashWnd(SplashWnd const&)
{
    // RVA 0x4C8460 - the shipped copy constructor copies nothing from rhs and,
    // unlike the default one, leaves the three control pointers and both splash
    // counters uninitialised.
}

SplashWnd::~SplashWnd()
{
    // RVA 0x4C84A0 - only m_text, the aux info and the Wnd base need unwinding.
}

m3d::Object* SplashWnd::Clone()
{
    // RVA 0x4C81D0
    return new SplashWnd(*this);
}

m3d::Object* SplashWnd::CreateObject()
{
    return new SplashWnd;
}

m3d::Class* SplashWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Class* SplashWnd::GetClass() const
{
    return RT_CLASS_LOCAL(SplashWnd);
}

int SplashWnd::GameDataSetup()
{
    // RVA 0x4C84F0
    using namespace m3d::ui;
    auto res = 1;
    if ((m_gameDataFlags & 2) == 0)
    {
        auto progressBar = RT_DYNCAST(GetChildByName(m_aif.m_progressBarName), ProgressBarWnd);
        if (progressBar && progressBar->IsKindOf(RT_CLASS_LOCAL(ProgressBarWnd)))
        {
            m_progressBar = progressBar;
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_progressBarName + " is not found or incorrect type");
            res = 0;
        }

        auto imageWnd = RT_DYNCAST(GetChildByName(m_aif.m_wndImageName), ImageWnd);
        if (imageWnd && imageWnd->IsKindOf(RT_CLASS_LOCAL(ImageWnd)))
        {
            m_wndImage = imageWnd;
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_wndImageName + " is not found or incorrect type");
            res = 0;
        }

        auto lblText = RT_DYNCAST(GetChildByName(m_aif.m_lblTextName), Wnd);
        if (lblText && lblText->IsKindOf(RT_CLASS_LOCAL(Wnd)))
        {
            m_lblText = lblText;
            if (res)
            {
                m_progressBar->SetMinValue(0.0);
                m_progressBar->SetMaxValue(100.0);
                m_progressBar->SetCurValue(0.0);
                m_gameDataFlags |= 1u;
            }
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_lblTextName + " is not found or incorrect type");
        }
    }
    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }
    M3D_LOG_INFO("SplashWnd: error - fail to init because of a bad resource");
    return 0;
}

void SplashWnd::StartSplashing(int numSplashes)
{
    // RVA 0x4C8D10
    m_curSplash = -1;
    m_numSplashes = numSplashes;
    if ((m_gameDataFlags & 1) != 0)
    {
        m3d::rend::TexHandle invalid;
        invalid.SetInvalid();
        m_wndImage->SetImage(invalid);
        m_progressBar->SetCurValue(0.0);
    }
    m_text = CStr();
    // NOTE: the label is cleared outside the "controls are ready" guard above,
    // so an uninitialised window dereferences a null m_lblText here.
    m_lblText->SetText(CStr());
    M3D_LOG_INFO("Start splashing " + CStr(m_numSplashes));
}

void SplashWnd::ShowSplash(int processStatus, CStr const& text)
{
    // RVA 0x4C8910 - the loader calls this synchronously, so the window paints
    // and presents itself rather than waiting for the next frame.
    if ((m_gameDataFlags & 1) == 0 || m_numSplashes <= 0)
    {
        return;
    }

    if (CStr::my_strcmp(text.c_str(), m_text.c_str()))
    {
        // Loading the main menu level is never announced by its own name.
        CStr txt;
        if (!M3D_APP->m_bDoNotLoadMainmenuLevel &&
            !strcmp(M3D_ENGINE_CFG.m_levFileName.GetS(), M3D_ENGINE_CFG.m_mainMenuLevelName.GetS()))
        {
            txt = M3D_APP->GetStringByStringId0("MainMenuLoading");
        }
        else
        {
            txt = text;
        }

        // NOTE: m_text remembers the caller's text, not the substituted one, so
        // the comparison above stays keyed on what the loader passes in.
        m_text = text;
        m_lblText->SetText(txt);
        ++m_curSplash;
        if (m_curSplash >= m_numSplashes)
        {
            m_curSplash = m_numSplashes;
        }
    }

    m_progressBar->SetCurValue((m_curSplash * 100.0f + processStatus) / m_numSplashes);
    if (!m_wndImage->GetImage().IsValid())
    {
        m_wndImage->SetImage(GetRandomImage(GetLevelSplashes(help::GetCurrentLevelName())));
    }

    M3D_RENDERER->BeginScene();
    M3D_RENDERER->ClearViewport(m3d::rend::M3DCLEAR_C, 0);
    M3D_RENDERER->PushZbState(m3d::rend::ZB_DISABLE);
    GetStation()->DispatchPaint(this, SPLASH_VIEWPORT);
    M3D_RENDERER->PopZbState();
    M3D_RENDERER->EndScene();
    M3D_RENDERER->PresentScene();
}

m3d::rend::TexHandle SplashWnd::GetRandomImage(retruxx::vector<m3d::rend::TexHandle> const& images) const
{
    // RVA 0x4C8BD0 - picks a splash other than the one already on screen; with
    // a single image to choose from it simply returns that one.
    if ((m_gameDataFlags & 1) != 0 && !images.empty())
    {
        auto const randI = rand() % images.size();
        if (images[randI].IsValid() && m_wndImage->GetImage() == images[randI] && images.size() > 1)
        {
            return GetRandomImage(images);
        }
        return images[randI];
    }

    m3d::rend::TexHandle invalid;
    invalid.SetInvalid();
    return invalid;
}

retruxx::vector<m3d::rend::TexHandle> SplashWnd::GetLevelSplashes(CStr const& levelName) const
{
    // RVA 0x4C8C80
    auto* levelInfo = M3D_APP->m_pInterfaceManager->GetLevelInfoManager()->GetLevelInfoByName(levelName);
    if (levelInfo)
    {
        return levelInfo->GetSplashes();
    }
    return {};
}
