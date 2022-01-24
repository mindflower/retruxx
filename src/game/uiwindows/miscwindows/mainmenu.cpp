#include "mainmenu.h"

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
    throw std::logic_error("Not implemented");
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
    throw std::logic_error("Not implemented");
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
