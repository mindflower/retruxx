#include "conversationwnd.h"

RT_CLASS_EXPORT_METHOD_DEFINE(ConversationWnd, EndConversation)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(ConversationWnd, GetCurrentDynamicQuest)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(ConversationWnd, ShowDynamicQuestsMenu)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORTS_BEGIN(ConversationWnd)
	RT_CLASS_EXPORT(ConversationWnd, m3d::METHOD, EndConversation, "", "", "")
	RT_CLASS_EXPORT(ConversationWnd, m3d::METHOD, GetCurrentDynamicQuest, "", "", "")
	RT_CLASS_EXPORT(ConversationWnd, m3d::METHOD, ShowDynamicQuestsMenu, "", "", "")
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(ConversationWnd);

Reply const* ConversationWnd::PlayerReplyButton::GetReply() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::DynamicQuest* ConversationWnd::PlayerReplyButton::GetDynamicQuest()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ConversationWnd::PlayerReplyButton::GetIdx() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

ConversationWnd::PlayerReplyButton::~PlayerReplyButton()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ConversationWnd::PlayerReplyButton::PlayerReplyButton(PointBase<float> const&, float, int, m3d::ui::Wnd*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ConversationWnd::PlayerReplyButton::SetUpForReply(Reply const*, ai::DynamicQuest*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ConversationWnd::PlayerReplyButton::RecalcHeight()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ConversationWnd::PlayerReplyButton::OnMouseOut()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ConversationWnd::PlayerReplyButton::OnMouseIn()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ConversationWnd::AuxInfo::AuxInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ConversationWnd::PlayerRepliesListBoxWnd::AuxInfo::AuxInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ConversationWnd::PlayerRepliesListBoxWnd::PlayerRepliesListBoxWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ConversationWnd::PlayerRepliesListBoxWnd::~PlayerRepliesListBoxWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ConversationWnd::PlayerRepliesListBoxWnd::SetUpForNpcReply(Reply const*, ai::Npc*, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ConversationWnd::PlayerRepliesListBoxWnd::SetUpForDynamicQuestThemes(ai::Npc*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ConversationWnd::PlayerRepliesListBoxWnd::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ConversationWnd::PlayerRepliesListBoxWnd::DeleteItem(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ConversationWnd::PlayerRepliesListBoxWnd::MeasureItem(int, BoundsBase<float>&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ConversationWnd::PlayerRepliesListBoxWnd::RenderItem(int, PointBase<float> const&, m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ConversationWnd::PlayerRepliesListBoxWnd::OnKey(unsigned short, unsigned char, unsigned)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ConversationWnd::PlayerRepliesListBoxWnd::CompareItem(int, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* ConversationWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::DynamicQuest* ConversationWnd::GetCurrentDynamicQuest()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* ConversationWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

int ConversationWnd::CreateFromPattern(m3d::ui::Wnd*, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ConversationWnd::ShowDynamicQuestsMenu()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ConversationWnd::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ConversationWnd::GetNpcId() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ConversationWnd::LeaveTown()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ConversationWnd::StartConversation(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* ConversationWnd::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

ConversationWnd::~ConversationWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* ConversationWnd::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ConversationWnd::EndConversation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ConversationWnd::ConversationWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ConversationWnd::ConversationWnd(ConversationWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ConversationWnd::GameDataUpdate(void*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ConversationWnd::ExecuteScript(CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ConversationWnd::UpdateOnEndConversation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ConversationWnd::UpdateOnSelectPlayerReply(Reply const*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ConversationWnd::OnAfterRemoveFromWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ConversationWnd::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ConversationWnd::UpdateOnBeginConversation(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ConversationWnd::UpdateOnSetNpcReply(Reply const*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool ConversationWnd::NeedConversationExit() const
{
    RETRUXX_NOT_IMPLEMENTED;
}
