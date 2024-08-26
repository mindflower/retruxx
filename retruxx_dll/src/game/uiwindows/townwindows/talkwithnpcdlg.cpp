#include "talkwithnpcdlg.h"
#include <game/uiwindows/charwindows/playermoneywnd.h>
#include "npcmodelwnd.h"
#include "conversationwnd.h"

RT_CLASS_EXPORT_METHOD_DEFINE(TalkWithNpcDlg, Show)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(TalkWithNpcDlg, Hide)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORTS_BEGIN(TalkWithNpcDlg)
	RT_CLASS_EXPORT(TalkWithNpcDlg, m3d::METHOD, Show, "", "", "")
	RT_CLASS_EXPORT(TalkWithNpcDlg, m3d::METHOD, Hide, "", "", "")
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(TalkWithNpcDlg);

TalkWithNpcDlg::AuxInfo::AuxInfo()
{
    throw std::logic_error("Not implemented");
}

TalkWithNpcDlg::~TalkWithNpcDlg()
{
    throw std::logic_error("Not implemented");
}

ai::Npc* TalkWithNpcDlg::GetNpc() const
{
    throw std::logic_error("Not implemented");
}

void TalkWithNpcDlg::Hide(bool)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* TalkWithNpcDlg::GetClass() const
{
    throw std::logic_error("Not implemented");
}

int TalkWithNpcDlg::SetupForNpc(int)
{
    throw std::logic_error("Not implemented");
}

int TalkWithNpcDlg::Show(int)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* TalkWithNpcDlg::Clone()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* TalkWithNpcDlg::GetBaseClass()
{
    return RT_CLASS_LOCAL(ChildPanel);
}

m3d::Object* TalkWithNpcDlg::CreateObject()
{
    throw std::logic_error("Not implemented");
}

bool TalkWithNpcDlg::IsValid() const
{
    throw std::logic_error("Not implemented");
}

bool TalkWithNpcDlg::IsTownDlgShowingNow() const
{
    throw std::logic_error("Not implemented");
}

int TalkWithNpcDlg::GetNpcToTalkWithFromLocation(int) const
{
    throw std::logic_error("Not implemented");
}

void TalkWithNpcDlg::OnBarNpc(void*)
{
    throw std::logic_error("Not implemented");
}

TalkWithNpcDlg::TalkWithNpcDlg()
{
    throw std::logic_error("Not implemented");
}

TalkWithNpcDlg::TalkWithNpcDlg(TalkWithNpcDlg const&)
{
    throw std::logic_error("Not implemented");
}

int TalkWithNpcDlg::OnActivate(bool)
{
    throw std::logic_error("Not implemented");
}

int TalkWithNpcDlg::GameDataClear(bool)
{
    throw std::logic_error("Not implemented");
}

void TalkWithNpcDlg::FillPanels()
{
    throw std::logic_error("Not implemented");
}

int TalkWithNpcDlg::OnAfterAddToWndStation()
{
    throw std::logic_error("Not implemented");
}

ref_ptr<TownDlg> TalkWithNpcDlg::GetTownDlg() const
{
    throw std::logic_error("Not implemented");
}

bool TalkWithNpcDlg::IsBarDlgShowingNow() const
{
    throw std::logic_error("Not implemented");
}

int TalkWithNpcDlg::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

ref_ptr<BarDlg> TalkWithNpcDlg::GetBarDlg() const
{
    throw std::logic_error("Not implemented");
}

void TalkWithNpcDlg::OnExit()
{
    throw std::logic_error("Not implemented");
}

void TalkWithNpcDlg::OnLocationNpc(void*)
{
    throw std::logic_error("Not implemented");
}

m3d::rend::TexHandle TalkWithNpcDlg::GetNpcBg(int) const
{
    throw std::logic_error("Not implemented");
}

int TalkWithNpcDlg::GameDataSetup()
{
    throw std::logic_error("Not implemented");
}

void TalkWithNpcDlg::UpdateOnStartConversation()
{
    throw std::logic_error("Not implemented");
}

ai::Bar const* TalkWithNpcDlg::GetBar() const
{
    throw std::logic_error("Not implemented");
}

int TalkWithNpcDlg::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}
