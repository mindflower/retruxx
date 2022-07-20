#include "creditswnd.h"

#include "game/m3dgame.h"

RT_CLASS_DEFINE(CreditsWnd);

CreditsWnd::PageInfo::PageInfo()
{
    throw std::logic_error("Not implemented");
}

CreditsWnd::Pointer::Pointer()
{
    auto app = dynamic_cast<CMiracle3d*>(m3d::Application::g_pApp);
    m_texture = app->m_pInterfaceManager->GetIcoByName(m_texId, 0);
    app->m_renderer->ReferenceTexture(m_texture);
}

void CreditsWnd::Pointer::Update(float, float, bool)
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::Pointer::Draw(m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}

CreditsWnd::Pointer::~Pointer()
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::Pointer::Clear()
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::Pointer::Init()
{
    throw std::logic_error("Not implemented");
}

CreditsWnd::AuxInfo::AuxInfo()
{
    throw std::logic_error("Not implemented");
}

CreditsWnd::~CreditsWnd()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* CreditsWnd::GetClass() const
{
    return RT_CLASS_LOCAL(CreditsWnd);
}

m3d::Object* CreditsWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* CreditsWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ModalWnd);
}

m3d::Object* CreditsWnd::CreateObject()
{
    return new CreditsWnd;
}

int CreditsWnd::OnAfterRemoveFromWndStation()
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::StartShowingPages()
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::UpdateStartTime()
{
    throw std::logic_error("Not implemented");
}

CreditsWnd::CreditsWnd()
{
    throw std::logic_error("Not implemented");
}

CreditsWnd::CreditsWnd(CreditsWnd const&)
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::UpdateBg()
{
    throw std::logic_error("Not implemented");
}

int CreditsWnd::OnKey(unsigned short, unsigned char, unsigned)
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::UpdatePortrait()
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::Exit()
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::DoShowPage()
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::PlaySwitchSound()
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::DoBreak()
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::StopSwitchSound()
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::CheckAndShowCurrentPage()
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::UpdateText()
{
    throw std::logic_error("Not implemented");
}

bool CreditsWnd::IsCurPageIdValid() const
{
    throw std::logic_error("Not implemented");
}

int CreditsWnd::PlaySoundA(int, bool)
{
    throw std::logic_error("Not implemented");
}

bool CreditsWnd::IsTimeEnded() const
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::StopSound(int)
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::LoadPageInfo()
{
    throw std::logic_error("Not implemented");
}

int CreditsWnd::OnBeforeAddToWndStation()
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::StopAllSoundsAndMusic()
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::UpdateControls()
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::ClearPages()
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::UpdatePointer()
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::PlayMusic()
{
    throw std::logic_error("Not implemented");
}

int CreditsWnd::OnPaint(m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}

int CreditsWnd::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::StopMusic()
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::Reset()
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::UpdatePortraitAnimation()
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::OnNewFrame()
{
    throw std::logic_error("Not implemented");
}

int CreditsWnd::GameDataSetup()
{
    throw std::logic_error("Not implemented");
}

bool CreditsWnd::NeedExit() const
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::CheckAndShowNextPage()
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::PlayAmbientSound()
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::StopAmbientSound()
{
    throw std::logic_error("Not implemented");
}
