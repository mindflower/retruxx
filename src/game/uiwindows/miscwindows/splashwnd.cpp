#include "splashwnd.h"
#include <core/log.h>
#include <ui/image.h>
#include <ui/progressbarwnd.h>

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

void SplashWnd::ShowSplash(int, CStr const&)
{
    throw std::logic_error("Not implemented");
}

SplashWnd::~SplashWnd()
{
    throw std::logic_error("Not implemented");
}

void SplashWnd::StartSplashing(int)
{
    throw std::logic_error("Not implemented");
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

std::vector<m3d::rend::TexHandle, std::allocator<m3d::rend::TexHandle>> SplashWnd::GetLevelSplashes(CStr const&) const
{
    throw std::logic_error("Not implemented");
}

SplashWnd::SplashWnd()
{
}

SplashWnd::SplashWnd(SplashWnd const&)
{
    throw std::logic_error("Not implemented");
}

m3d::rend::TexHandle SplashWnd::GetRandomImage(std::vector<m3d::rend::TexHandle, std::allocator<m3d::rend::TexHandle>> const&) const
{
    throw std::logic_error("Not implemented");
}
