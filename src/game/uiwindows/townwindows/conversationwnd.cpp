#include "conversationwnd.h"

RT_CLASS_EXPORT_METHOD_DEFINE(ConversationWnd, EndConversation)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(ConversationWnd, GetCurrentDynamicQuest)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(ConversationWnd, ShowDynamicQuestsMenu)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORTS_BEGIN(ConversationWnd)
	RT_CLASS_EXPORT(ConversationWnd, m3d::METHOD, EndConversation, "", "", "")
	RT_CLASS_EXPORT(ConversationWnd, m3d::METHOD, GetCurrentDynamicQuest, "", "", "")
	RT_CLASS_EXPORT(ConversationWnd, m3d::METHOD, ShowDynamicQuestsMenu, "", "", "")
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(ConversationWnd);

Reply const* ConversationWnd::PlayerReplyButton::GetReply() const
{
    throw std::logic_error("Not implemented");
}

ai::DynamicQuest* ConversationWnd::PlayerReplyButton::GetDynamicQuest()
{
    throw std::logic_error("Not implemented");
}

int ConversationWnd::PlayerReplyButton::GetIdx() const
{
    throw std::logic_error("Not implemented");
}

ConversationWnd::PlayerReplyButton::~PlayerReplyButton()
{
    throw std::logic_error("Not implemented");
}

ConversationWnd::PlayerReplyButton::PlayerReplyButton(PointBase<float> const&, float, int, m3d::ui::Wnd*)
{
    throw std::logic_error("Not implemented");
}

void ConversationWnd::PlayerReplyButton::SetUpForReply(Reply const*, ai::DynamicQuest*)
{
    throw std::logic_error("Not implemented");
}

void ConversationWnd::PlayerReplyButton::RecalcHeight()
{
    throw std::logic_error("Not implemented");
}

int ConversationWnd::PlayerReplyButton::OnMouseOut()
{
    throw std::logic_error("Not implemented");
}

int ConversationWnd::PlayerReplyButton::OnMouseIn()
{
    throw std::logic_error("Not implemented");
}

ConversationWnd::AuxInfo::AuxInfo()
{
    throw std::logic_error("Not implemented");
}

ConversationWnd::PlayerRepliesListBoxWnd::AuxInfo::AuxInfo()
{
    throw std::logic_error("Not implemented");
}

ConversationWnd::PlayerRepliesListBoxWnd::PlayerRepliesListBoxWnd()
{
    throw std::logic_error("Not implemented");
}

ConversationWnd::PlayerRepliesListBoxWnd::~PlayerRepliesListBoxWnd()
{
    throw std::logic_error("Not implemented");
}

int ConversationWnd::PlayerRepliesListBoxWnd::SetUpForNpcReply(Reply const*, ai::Npc*, bool)
{
    throw std::logic_error("Not implemented");
}

int ConversationWnd::PlayerRepliesListBoxWnd::SetUpForDynamicQuestThemes(ai::Npc*)
{
    throw std::logic_error("Not implemented");
}

int ConversationWnd::PlayerRepliesListBoxWnd::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

int ConversationWnd::PlayerRepliesListBoxWnd::DeleteItem(int)
{
    throw std::logic_error("Not implemented");
}

int ConversationWnd::PlayerRepliesListBoxWnd::MeasureItem(int, BoundsBase<float>&) const
{
    throw std::logic_error("Not implemented");
}

int ConversationWnd::PlayerRepliesListBoxWnd::RenderItem(int, PointBase<float> const&, m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}

int ConversationWnd::PlayerRepliesListBoxWnd::OnKey(unsigned short, unsigned char, unsigned)
{
    throw std::logic_error("Not implemented");
}

int ConversationWnd::PlayerRepliesListBoxWnd::CompareItem(int, int)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* ConversationWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

ai::DynamicQuest* ConversationWnd::GetCurrentDynamicQuest()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* ConversationWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

int ConversationWnd::CreateFromPattern(m3d::ui::Wnd*, bool)
{
    throw std::logic_error("Not implemented");
}

int ConversationWnd::ShowDynamicQuestsMenu()
{
    throw std::logic_error("Not implemented");
}

int ConversationWnd::GameDataClear(bool)
{
    throw std::logic_error("Not implemented");
}

int ConversationWnd::GetNpcId() const
{
    throw std::logic_error("Not implemented");
}

void ConversationWnd::LeaveTown()
{
    throw std::logic_error("Not implemented");
}

void ConversationWnd::StartConversation(int)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* ConversationWnd::GetClass() const
{
    throw std::logic_error("Not implemented");
}

ConversationWnd::~ConversationWnd()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* ConversationWnd::CreateObject()
{
    throw std::logic_error("Not implemented");
}

void ConversationWnd::EndConversation()
{
    throw std::logic_error("Not implemented");
}

ConversationWnd::ConversationWnd()
{
    throw std::logic_error("Not implemented");
}

ConversationWnd::ConversationWnd(ConversationWnd const&)
{
    throw std::logic_error("Not implemented");
}

int ConversationWnd::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

int ConversationWnd::ExecuteScript(CStr const&)
{
    throw std::logic_error("Not implemented");
}

int ConversationWnd::UpdateOnEndConversation()
{
    throw std::logic_error("Not implemented");
}

int ConversationWnd::UpdateOnSelectPlayerReply(Reply const*)
{
    throw std::logic_error("Not implemented");
}

int ConversationWnd::OnAfterRemoveFromWndStation()
{
    throw std::logic_error("Not implemented");
}

int ConversationWnd::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

int ConversationWnd::UpdateOnBeginConversation(int)
{
    throw std::logic_error("Not implemented");
}

int ConversationWnd::UpdateOnSetNpcReply(Reply const*)
{
    throw std::logic_error("Not implemented");
}

bool ConversationWnd::NeedConversationExit() const
{
    throw std::logic_error("Not implemented");
}
