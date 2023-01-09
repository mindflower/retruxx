#include "mainmenu.h"
#include <m3dapp.h>
#include <core/log.h>

RT_CLASS_EXPORTS_BEGIN(MainMenuUI)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(MainMenuUI);

MainMenuUI::AuxInfo::AuxInfo()
{
}

m3d::Class* MainMenuUI::GetClass() const
{
    return RT_CLASS_LOCAL(MainMenuUI);
}

MainMenuUI::~MainMenuUI()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* MainMenuUI::CreateObject()
{
    return new MainMenuUI;
}

m3d::Class* MainMenuUI::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

void MainMenuUI::OnFinishVideoPlaying()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* MainMenuUI::Clone()
{
    throw std::logic_error("Not implemented");
}

int MainMenuUI::OnBeforeAddToWndStation()
{
    GetStation()->CaptureFocus(this);
    return Wnd::OnBeforeAddToWndStation();
}

MainMenuUI::MainMenuUI()
{
}

MainMenuUI::MainMenuUI(MainMenuUI const&)
{
    throw std::logic_error("Not implemented");
}

void MainMenuUI::OnNewGame()
{
    throw std::logic_error("Not implemented");
}

int MainMenuUI::OnAfterRemoveFromWndStation()
{
    throw std::logic_error("Not implemented");
}

int MainMenuUI::OnKey(unsigned short, unsigned char, unsigned)
{
    throw std::logic_error("Not implemented");
}

int MainMenuUI::GameDataSetup()
{
    if ((m_gameDataFlags & 2) != 0)
    {
        return 1;
    }
    auto res = 1;
    auto version = GetChildByName(m_aif.m_wndVersionName);
    if (version && version->IsKindOf(RT_CLASS_LOCAL(Wnd)))
    {
        m_wndVersion = dynamic_cast<Wnd*>(version);
    }
    else
    {
        M3D_LOG_INFO("Get control error: control " + m_aif.m_wndVersionName + " is not found or incorrect type");
        res = 0;
    }
    auto profile = GetChildByName(m_aif.m_wndProfileName);
    if (!profile || !IsKindOf(RT_CLASS_LOCAL(Wnd)))
    {
        M3D_LOG_INFO("Get control error: control " + m_aif.m_wndProfileName + " is not found or incorrect type");
        return 1;
    }
    m_wndProfile = dynamic_cast<Wnd*>(profile);
    if (!res)
    {
        return 1;
    }
    auto title = m3d::Application::g_pApp->GetWindowTitle();
    m_wndVersion->SetText(title);
    m_gameDataFlags |= 1;
    return 1;
}

void MainMenuUI::OnStartVideoPlaying()
{
    throw std::logic_error("Not implemented");
}

void MainMenuUI::QuitToWindows()
{
    throw std::logic_error("Not implemented");
}

int MainMenuUI::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

void MainMenuUI::OnCurProfileChanged()
{
    throw std::logic_error("Not implemented");
}

int MainMenuUI::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}
