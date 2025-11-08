#include "talkwithnpcdlg.h"
#include <game/uiwindows/charwindows/playermoneywnd.h>
#include "npcmodelwnd.h"
#include "conversationwnd.h"

RT_CLASS_EXPORT_METHOD_DEFINE(TalkWithNpcDlg, Show)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(TalkWithNpcDlg, Hide)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORTS_BEGIN(TalkWithNpcDlg)
	RT_CLASS_EXPORT(TalkWithNpcDlg, m3d::METHOD, Show, "", "", "")
	RT_CLASS_EXPORT(TalkWithNpcDlg, m3d::METHOD, Hide, "", "", "")
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(TalkWithNpcDlg);

TalkWithNpcDlg::AuxInfo::AuxInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

TalkWithNpcDlg::~TalkWithNpcDlg()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Npc* TalkWithNpcDlg::GetNpc() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void TalkWithNpcDlg::Hide(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* TalkWithNpcDlg::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int TalkWithNpcDlg::SetupForNpc(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int TalkWithNpcDlg::Show(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* TalkWithNpcDlg::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* TalkWithNpcDlg::GetBaseClass()
{
    return RT_CLASS_LOCAL(ChildPanel);
}

m3d::Object* TalkWithNpcDlg::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool TalkWithNpcDlg::IsValid() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool TalkWithNpcDlg::IsTownDlgShowingNow() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int TalkWithNpcDlg::GetNpcToTalkWithFromLocation(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void TalkWithNpcDlg::OnBarNpc(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

TalkWithNpcDlg::TalkWithNpcDlg()
{
    RETRUXX_NOT_IMPLEMENTED;
}

TalkWithNpcDlg::TalkWithNpcDlg(TalkWithNpcDlg const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int TalkWithNpcDlg::OnActivate(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int TalkWithNpcDlg::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void TalkWithNpcDlg::FillPanels()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int TalkWithNpcDlg::OnAfterAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ref_ptr<TownDlg> TalkWithNpcDlg::GetTownDlg() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool TalkWithNpcDlg::IsBarDlgShowingNow() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int TalkWithNpcDlg::GameDataUpdate(void*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ref_ptr<BarDlg> TalkWithNpcDlg::GetBarDlg() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void TalkWithNpcDlg::OnExit()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void TalkWithNpcDlg::OnLocationNpc(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::rend::TexHandle TalkWithNpcDlg::GetNpcBg(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int TalkWithNpcDlg::GameDataSetup()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void TalkWithNpcDlg::UpdateOnStartConversation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Bar const* TalkWithNpcDlg::GetBar() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int TalkWithNpcDlg::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}
