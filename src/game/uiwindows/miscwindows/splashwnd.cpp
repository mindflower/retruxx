#include "splashwnd.h"
#include <core/log.h>
#include <ui/image.h>
#include <ui/progressbarwnd.h>

#include "config.h"
#include "m3dapp.h"
#include "game/m3dgame.h"
#include "game/uimisc/guihelper.h"
#include "game/uimisc/levelinfo.h"

RT_CLASS_EXPORTS_BEGIN(SplashWnd)
    RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(SplashWnd);

SplashWnd::AuxInfo::AuxInfo()
{
}

m3d::Class* SplashWnd::GetClass() const
{
    return RT_CLASS_LOCAL(SplashWnd);
}

m3d::Object* SplashWnd::CreateObject()
{
    return new SplashWnd;
}

void SplashWnd::ShowSplash(int processStatus, CStr const& text)
{
    if ((m_gameDataFlags & 1) != 0 && m_numSplashes > 0)
    {
        if (CStr::my_strcmp(text.c_str(), m_text.c_str()))
        {
            if (!M3D_APP->m_bDoNotLoadMainmenuLevel && 
                !strcmp(M3D_KERNEL->GetEngineCfg().m_levFileName.GetS(), M3D_KERNEL->GetEngineCfg().m_mainMenuLevelName.GetS()))
            {
                m_text = M3D_APP->GetStringByStringId0("MainMenuLoading");
            }
            else
            {
                m_text = text;
            }
            m_lblText->SetText(m_text);
            m_curSplash++;
            if (m_curSplash >= m_numSplashes)
            {
                m_curSplash = m_numSplashes;
            }

        }
        m_progressBar->SetCurValue(m_curSplash * 100.0 + processStatus / m_numSplashes);
        auto const image = m_wndImage->GetImage();
        if (!image.IsValid())
        {
            auto const randImage = GetRandomImage(GetLevelSplashes(help::GetCurrentLevelName()));
            m_wndImage->SetImage(randImage);
        }
        M3D_APP->m_renderer->BeginScene();
        M3D_APP->m_renderer->ClearViewport(m3d::rend::M3DCLEAR_C, 0);
        M3D_APP->m_renderer->PushZbState(m3d::rend::ZB_DISABLE);
        
        
        GetStation()->DispatchPaint(this, { 1024.0, 768.0 });
        
        M3D_APP->m_renderer->PopZbState();
        M3D_APP->m_renderer->EndScene();
        M3D_APP->m_renderer->PresentScene();
    }
}

SplashWnd::~SplashWnd()
{
    throw std::logic_error("Not implemented");
}

void SplashWnd::StartSplashing(int numSplashes)
{
    m_curSplash = -1;
    m_numSplashes = numSplashes;
    if ((this->m_gameDataFlags & 1) != 0)
    {
        m_wndImage->SetImage(m3d::rend::TexHandle{});
        m_progressBar->SetCurValue(0.0);
    }
    m_lblText->SetText({});
    M3D_LOG_INFO("Start splashing " + CStr(m_numSplashes));
}

m3d::Class* SplashWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

int SplashWnd::GameDataSetup()
{
    using namespace m3d::ui;
    auto res = 1;
    if ((m_gameDataFlags & 2) == 0)
    {
        auto progressBar = dynamic_cast<ProgressBarWnd*>(GetChildByName(m_aif.m_progressBarName));
        if (progressBar && progressBar->IsKindOf(RT_CLASS_LOCAL(ProgressBarWnd)))
        {
            m_progressBar = progressBar;
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_progressBarName + " is not found or incorrect type");
            res = 0;
        }

        auto imageWnd = dynamic_cast<ImageWnd*>(GetChildByName(m_aif.m_wndImageName));
        if (imageWnd && imageWnd->IsKindOf(RT_CLASS_LOCAL(ImageWnd)))
        {
            m_wndImage = imageWnd;
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_wndImageName + " is not found or incorrect type");
            res = 0;
        }

        auto lblText = dynamic_cast<Wnd*>(GetChildByName(m_aif.m_lblTextName));
        if (lblText && lblText->IsKindOf(RT_CLASS_LOCAL(Wnd)))
        {
            m_lblText = lblText;
            if (res)
            {
                m_progressBar->SetMinValue(0.0);
                m_progressBar->SetMaxValue(100.0);
                m_progressBar->SetCurValue(0.0);
                m_gameDataFlags |= 1;
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

std::vector<m3d::rend::TexHandle> SplashWnd::GetLevelSplashes(CStr const& levelName) const
{
    auto app = dynamic_cast<CMiracle3d*>(m3d::Application::g_pApp);
    auto infoManager = app->m_pInterfaceManager->GetLevelInfoManager();
    auto levelInfo = infoManager->GetLevelInfoByName(levelName);
    if (levelInfo)
    {
        return levelInfo->GetSplashes();
    }
    return {};
}

SplashWnd::SplashWnd()
{
}

SplashWnd::SplashWnd(SplashWnd const&)
{
    throw std::logic_error("Not implemented");
}

m3d::rend::TexHandle SplashWnd::GetRandomImage(std::vector<m3d::rend::TexHandle> const& images) const
{
    if ((this->m_gameDataFlags & 1) != 0 && !images.empty())
    {
        auto randI = rand() % images.size();
        if (images[randI].IsValid() && m_wndImage->GetImage() == images[randI] && images.size() > 1)
        {
            return GetRandomImage(images);
        }
        return images[randI];
    }
    return {};
}
