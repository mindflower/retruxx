#include "conversationwnd.h"

#include <vector>

#include <core/aiparam.h>
#include <core/clazz.h>
#include <core/kernel.h>
#include <core/log.h>

#include <game/m3dgame.h>
#include <game/uimanager/truxxuimanager.h>
#include <game/uimisc/reply.h>
#include <game/uimisc/stringparser.h>

#include <ui/button.h>
#include <ui/textbox.h>
#include <ui/ui_srv.h>

#include <server/objects/dynamicquest.h>
#include <server/objects/npc.h>
#include <server/objects/town.h>
#include <server/objects/base/objcontainer.h>

namespace
{
    // The conversation-flow bits stored in ConversationWnd::m_scriptFlags. Any of
    // START / END / LEAVE_TOWN means the dialog is on its way out (mask 0xB); the
    // QUESTS bit just suppresses re-populating the reply list.
    enum
    {
        SCRIPT_FLAG_END = 1,
        SCRIPT_FLAG_START = 2,
        SCRIPT_FLAG_QUESTS = 4,
        SCRIPT_FLAG_LEAVE_TOWN = 8,
        SCRIPT_FLAG_EXIT_MASK = SCRIPT_FLAG_END | SCRIPT_FLAG_START | SCRIPT_FLAG_LEAVE_TOWN,
    };

    // Notify messages exchanged between ConversationWnd, its reply list and the
    // hosting TalkWithNpcDlg.
    enum
    {
        CN_END_CONVERSATION = 0xC364,      // 50020
        CN_BEGIN_CONVERSATION = 0xC365,    // 50021
        CN_LEAVE_TOWN = 0xC366,            // 50022
        CN_SELECT_PLAYER_REPLY = 0xC367,   // 50023
    };

    // Copies every layout / text / style / pane / animation property from a
    // pattern window onto another window (the shipped ConversationWnd pattern
    // copy inlines this block once per child control).
    void CopyWndPropsFromPattern(m3d::ui::Wnd* dst, m3d::ui::Wnd const* pat)
    {
        dst->SetStyle(pat->GetStyle());
        dst->SetText(pat->GetText());
        dst->SetId(pat->GetId());
        dst->SetName(pat->GetName());
        dst->SetBounds(pat->GetBounds(), true);
        dst->SetDefaultFont(pat->GetDefaultFont());
        dst->SetWrapMode(pat->GetWrapMode());
        dst->SetFormatMode(pat->GetFormatMode());
        dst->SetColor(pat->GetColor());
        dst->SetTextColor(pat->GetTextColor());
        dst->SetTextColorDisabled(pat->GetTextColorDisabled());
        dst->SetClientEdges(pat->GetClientEdges());
        dst->SetPane(pat->GetPaneName());
        dst->SetPaneFlags(pat->GetPaneFlags());
        dst->SetScrollPane(pat->GetScrollPaneName());
        dst->SetBackground(pat->GetBackground());

        CStr tooltip;
        pat->GetProperty(PROP_WND_TOOLTIP, &tooltip);
        dst->SetProperty(PROP_WND_TOOLTIP, &tooltip);

        dst->SetOnShowAnimation(pat->GetOnShowAnimation());
        dst->SetOnHideAnimation(pat->GetOnHideAnimation());
    }
}  // namespace

// ===========================================================================
//  script exports
// ===========================================================================

RT_CLASS_EXPORT_METHOD_DEFINE(ConversationWnd, EndConversation)
{
    auto* wnd = static_cast<ConversationWnd*>(context->asObject(0, "ConversationWnd"));
    wnd->EndConversation();
    return 0;
}

RT_CLASS_EXPORT_METHOD_DEFINE(ConversationWnd, GetCurrentDynamicQuest)
{
    auto* wnd = static_cast<ConversationWnd*>(context->asObject(0, "ConversationWnd"));
    context->pushObject(wnd->GetCurrentDynamicQuest());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(ConversationWnd, ShowDynamicQuestsMenu)
{
    auto* wnd = static_cast<ConversationWnd*>(context->asObject(0, "ConversationWnd"));
    context->pushInt(wnd->ShowDynamicQuestsMenu());
    return 1;
}

RT_CLASS_EXPORTS_BEGIN(ConversationWnd)
	RT_CLASS_EXPORT(ConversationWnd, m3d::METHOD, EndConversation, "", "", "")
	RT_CLASS_EXPORT(ConversationWnd, m3d::METHOD, GetCurrentDynamicQuest, "", "", "")
	RT_CLASS_EXPORT(ConversationWnd, m3d::METHOD, ShowDynamicQuestsMenu, "", "", "")
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(ConversationWnd);

// ===========================================================================
//  ConversationWnd::AuxInfo
// ===========================================================================

ConversationWnd::AuxInfo::AuxInfo()
{
    // RVA 0x977C0
    m_txtNpcReplyName = "wndNpcReplies";
    m_lstPlayerRepliesName = "wndPlayerReplies";
    m_space = 10.0f;
}

ConversationWnd::AuxInfo::AuxInfo(ConversationWnd::AuxInfo const& o)
{
    m_txtNpcReplyName = o.m_txtNpcReplyName;
    m_lstPlayerRepliesName = o.m_lstPlayerRepliesName;
    m_space = o.m_space;
}

// ===========================================================================
//  ConversationWnd
// ===========================================================================

ConversationWnd::ConversationWnd()
{
    // RVA 0x979B0
    m_txtNpcReply = nullptr;
    m_lstPlayerReplies = nullptr;
    m_curDynamicQuest = nullptr;
    m_npc = nullptr;
    m_scriptFlags = 0;
}

ConversationWnd::ConversationWnd(ConversationWnd const&) : ConversationWnd()
{
    // Matches the original: the copy ctor default-constructs the base and the
    // AuxInfo and copies nothing else. Reached only via Clone().
}

ConversationWnd::~ConversationWnd()
{
    // m_txtNpcReply / m_lstPlayerReplies are owned children, released by the
    // Wnd base destructor; the AuxInfo strings free themselves.
}

m3d::Object* ConversationWnd::Clone()
{
    // RVA 0x97740
    return new ConversationWnd(*this);
}

m3d::Object* ConversationWnd::CreateObject()
{
    // RVA 0x97950
    return new ConversationWnd;
}

m3d::Class* ConversationWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Class* ConversationWnd::GetClass() const
{
    // RVA 0x97940
    return RT_CLASS_LOCAL(ConversationWnd);
}

int ConversationWnd::GetNpcId() const
{
    // RVA 0x98E60
    return m_npc ? m_npc->GetId() : -1;
}

ai::DynamicQuest* ConversationWnd::GetCurrentDynamicQuest()
{
    // RVA 0x98E50
    return m_curDynamicQuest;
}

bool ConversationWnd::NeedConversationExit() const
{
    // RVA 0x98E80
    return (m_scriptFlags & SCRIPT_FLAG_EXIT_MASK) != 0;
}

void ConversationWnd::EndConversation()
{
    // RVA 0x98C70
    m_scriptFlags |= SCRIPT_FLAG_END;
    // NOTE: the shipped build packs the window pointer into message p0.
    M3D_APP->EnqueueMessage(65665, reinterpret_cast<int>(this), 0, 0, 0, {}, {});
}

void ConversationWnd::LeaveTown()
{
    // RVA 0x98D10
    m_scriptFlags |= SCRIPT_FLAG_LEAVE_TOWN;
    M3D_APP->EnqueueMessage(65665, reinterpret_cast<int>(this), 0, 0, 0, {}, {});
}

void ConversationWnd::StartConversation(int npcId)
{
    // RVA 0x98DB0
    m_scriptFlags |= SCRIPT_FLAG_START;
    M3D_APP->EnqueueMessage(65664, reinterpret_cast<int>(this), npcId, 0, 0, {}, {});
}

int ConversationWnd::ShowDynamicQuestsMenu()
{
    // RVA 0x98C40
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    m_scriptFlags |= SCRIPT_FLAG_QUESTS;
    return m_lstPlayerReplies->SetUpForDynamicQuestThemes(m_npc);
}

int ConversationWnd::ExecuteScript(CStr const& script)
{
    // RVA 0x98BF0
    m_scriptFlags = 0;
    if (script.empty())
    {
        return 1;
    }
    RepliesManager* replies = M3D_APP->m_pInterfaceManager->GetRepliesManager();
    return replies->ExecuteScript(script, m_npc);
}

int ConversationWnd::GameDataClear(bool)
{
    // RVA 0x98B80
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    m_npc = nullptr;
    m_txtNpcReply->SetText(CStr());
    m_lstPlayerReplies->RemoveAllItems();
    return 1;
}

int ConversationWnd::GameDataUpdate(void* data, int dataType)
{
    // RVA 0x98490
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    switch (dataType)
    {
    case 26:
        if (data && reinterpret_cast<ConversationWnd**>(data)[13] == this)
        {
            UpdateOnEndConversation();
        }
        break;
    case 27:
        if (data && reinterpret_cast<ConversationWnd**>(data)[13] == this)
        {
            UpdateOnBeginConversation(reinterpret_cast<int*>(data)[14]);
            return 1;
        }
        break;
    case 101:
        if (IsChildOf(static_cast<m3d::Object*>(M3D_APP)))
        {
            UpdateOnEndConversation();
            return 1;
        }
        break;
    }
    return 1;
}

int ConversationWnd::OnWndNotify(m3d::ui::Wnd* from, unsigned id, unsigned msg, m3d::AIParam const& data)
{
    // RVA 0x98520
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if ((m_style & 0x100000) != 0)
    {
        ReflectChildNotifyToParent(from, id, msg, data);
    }
    if (id != 500001)
    {
        return 0;
    }
    if (msg == CN_SELECT_PLAYER_REPLY)
    {
        int const idx = data.GetAsID();
        if (idx < m_lstPlayerReplies->GetCount())
        {
            PlayerReplyButton* btn = m_lstPlayerReplies->GetItem(idx);
            if (btn)
            {
                if (m_npc->GetNpcType() == ai::Npc::NPC_BARMAN)
                {
                    if (ai::DynamicQuest* dQuest = btn->GetDynamicQuest())
                    {
                        m_curDynamicQuest = dQuest;
                    }
                }
                UpdateOnSelectPlayerReply(btn->GetReply());
            }
        }
    }
    return 1;
}

int ConversationWnd::OnAfterRemoveFromWndStation()
{
    // RVA 0x98A00
    int const res = m3d::ui::Wnd::OnAfterRemoveFromWndStation();
    GameDataClear(false);
    m_scriptFlags = 0;
    return res;
}

int ConversationWnd::UpdateOnBeginConversation(int npcId)
{
    // RVA 0x98A30
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    GameDataClear(false);
    if (npcId < 0)
    {
        return 0;
    }

    ai::Obj* obj = ai::theObjects->GetEntityByObjId(npcId);
    if (!obj || !obj->IsKindOf(&ai::Npc::m_classNpc))
    {
        return 0;
    }
    auto* npc = static_cast<ai::Npc*>(obj);

    m_npc = npc;
    m_curDynamicQuest = nullptr;

    RepliesManager* replies = M3D_APP->m_pInterfaceManager->GetRepliesManager();
    Reply* firstNpcReply = replies->GetFirstNpcReply(npc);

    if (m_npc->GetNpcType() == ai::Npc::NPC_BARMAN && firstNpcReply)
    {
        std::vector<CStr> replyNames;
        replyNames.push_back(firstNpcReply->GetName());
        m_npc->SetHelloReplyNames(replyNames);
    }

    UpdateOnSetNpcReply(firstNpcReply);

    if ((m_scriptFlags & SCRIPT_FLAG_EXIT_MASK) == 0)
    {
        CallParentNotify(CN_BEGIN_CONVERSATION, m3d::AIParam(), true);
    }
    return 1;
}

int ConversationWnd::UpdateOnSetNpcReply(Reply const* npcReply)
{
    // RVA 0x985E0
    int res = 1;
    if ((m_gameDataFlags & 1) == 0 || !m_npc || !npcReply || npcReply->GetRole() != Reply::ROLE_NPC)
    {
        return 0;
    }

    CStr npcReplyText = npcReply->GetText();
    if (ai::DynamicQuest const* dQuest = m_curDynamicQuest)
    {
        StringParser const& parser = M3D_APP->m_pInterfaceManager->GetStringParser();
        parser.FormatStringForDynamicQuest(npcReplyText, dQuest);
    }
    m_txtNpcReply->SetText(npcReplyText);

    if (!ExecuteScript(npcReply->GetScriptResult()))
    {
        M3D_LOG_INFO(CStr("Fail to execute result script for player reply ") + npcReply->GetName());
        res = 0;
    }

    if ((m_scriptFlags & SCRIPT_FLAG_EXIT_MASK) != 0)
    {
        return 1;
    }

    if (m_lstPlayerReplies->SetUpForNpcReply(npcReply, m_npc, (m_scriptFlags & SCRIPT_FLAG_QUESTS) == 0))
    {
        M3D_APP->EnqueueMessage(65660, m_npc->GetId(), 0, 0, 0, {}, {});
        return res;
    }

    M3D_LOG_INFO(CStr("ConversationWnd: warning - abnormal end of conversation after reply ") + npcReply->GetName());
    UpdateOnEndConversation();
    return 0;
}

int ConversationWnd::UpdateOnSelectPlayerReply(Reply const* playerReply)
{
    // RVA 0x98820
    if ((m_gameDataFlags & 1) == 0 || !m_npc || !playerReply || playerReply->GetRole() != Reply::ROLE_PLAYER)
    {
        return 0;
    }

    m_scriptFlags = 0;
    if (!ExecuteScript(playerReply->GetScriptResult()))
    {
        M3D_LOG_INFO(CStr("Fail to execute result script for player reply ") + playerReply->GetName());
    }

    if ((m_scriptFlags & SCRIPT_FLAG_EXIT_MASK) != 0)
    {
        return 1;
    }

    RepliesManager* replies = M3D_APP->m_pInterfaceManager->GetRepliesManager();
    Reply* nextNpcReply = replies->GetNextNpcReply(playerReply, m_npc);
    if (nextNpcReply)
    {
        return UpdateOnSetNpcReply(nextNpcReply);
    }

    M3D_LOG_INFO(CStr("ConversationWnd: warning - abnormal end of conversation after reply ") + playerReply->GetName());
    UpdateOnEndConversation();
    return 0;
}

int ConversationWnd::UpdateOnEndConversation()
{
    // RVA 0x98980
    GameDataClear(false);

    if ((m_scriptFlags & SCRIPT_FLAG_LEAVE_TOWN) != 0)
    {
        m_scriptFlags &= ~(SCRIPT_FLAG_END | SCRIPT_FLAG_LEAVE_TOWN);
        CallParentNotify(CN_LEAVE_TOWN, m3d::AIParam(), true);
        return 1;
    }
    if ((m_scriptFlags & SCRIPT_FLAG_END) != 0)
    {
        m_scriptFlags &= ~(SCRIPT_FLAG_END | SCRIPT_FLAG_LEAVE_TOWN);
        CallParentNotify(CN_END_CONVERSATION, m3d::AIParam(), true);
        return 1;
    }
    return 1;
}

int ConversationWnd::CreateFromPattern(m3d::ui::Wnd* patternWnd, bool deleteSrc)
{
    // RVA 0x97B00
    if (!patternWnd)
    {
        M3D_LOG_INFO("ConversationWnd::CreateFromPattern error - bad pattern window");
        return 0;
    }

    auto* parent = RT_DYNCAST(patternWnd->GetParent(), m3d::ui::Wnd);
    if (!parent)
    {
        M3D_LOG_INFO("ConversationWnd::CreateFromPattern error - bad parent for pattern window");
        return 0;
    }

    if (!m3d::ui::Wnd::Create({}, patternWnd->GetStyle() | 0x20u, patternWnd->GetBounds(), patternWnd->GetId()))
    {
        M3D_LOG_INFO("ConversationWnd::CreateFromPattern error - cannot create self");
        return 0;
    }
    m_activationOrder = patternWnd->m_activationOrder;
    parent->AddChild(this);

    m_txtNpcReply = static_cast<m3d::ui::TextBoxWnd*>(M3D_KERNEL->New("TextBoxWnd"));
    m_lstPlayerReplies = new PlayerRepliesListBoxWnd();
    if (!m_txtNpcReply || !m_lstPlayerReplies)
    {
        M3D_LOG_INFO("ConversationWnd::CreateFromPattern error - cannot instantiate child controls");
        return 0;
    }

    // --- npc reply text box ---------------------------------------------
    m3d::Object* childReply = parent->GetChildByName(m_aif.m_txtNpcReplyName);
    if (!childReply || !childReply->IsKindOf(&m3d::ui::TextBoxWnd::m_classTextBoxWnd))
    {
        M3D_LOG_INFO(CStr("ConversationWnd::CreateFromPattern error - cannot find ") + m_aif.m_txtNpcReplyName);
        return 0;
    }
    auto* patternNpcReply = static_cast<m3d::ui::TextBoxWnd*>(childReply);

    if (!m_txtNpcReply->Create({}, patternNpcReply->GetStyle(), patternNpcReply->GetBounds(), patternNpcReply->GetId()))
    {
        M3D_LOG_INFO("ConversationWnd::CreateFromPattern error - cannot create m_txtNpcReply");
        return 0;
    }
    CopyWndPropsFromPattern(m_txtNpcReply, patternNpcReply);
    m_txtNpcReply->SetDrawFlags(m_txtNpcReply->GetDrawFlags() | 2u);

    // --- player replies list box ---------------------------------------
    m3d::Object* childReplies = parent->GetChildByName(m_aif.m_lstPlayerRepliesName);
    if (!childReplies || !childReplies->IsKindOf(&m3d::ui::TextBoxWnd::m_classTextBoxWnd))
    {
        M3D_LOG_INFO(CStr("ConversationWnd::CreateFromPattern error - cannot find ") + m_aif.m_lstPlayerRepliesName);
        return 0;
    }
    auto* patternPlayerReplies = static_cast<m3d::ui::Wnd*>(childReplies);

    if (!m_lstPlayerReplies->Create(patternPlayerReplies->GetBounds(), patternPlayerReplies->GetStyle(),
                                    patternPlayerReplies->GetId()))
    {
        M3D_LOG_INFO("ConversationWnd::CreateFromPattern error - cannot create m_lstPlayerReplies");
        return 0;
    }
    CopyWndPropsFromPattern(m_lstPlayerReplies, patternPlayerReplies);
    m_lstPlayerReplies->m_activationOrder = patternPlayerReplies->m_activationOrder;
    m_lstPlayerReplies->SetDrawFlags(7u);

    AddChild(m_txtNpcReply);
    AddChild(m_lstPlayerReplies);

    // Rebase both child origins so they sit relative to this window.
    BoundsBase<float> const patB = patternWnd->GetBounds();
    {
        BoundsBase<float> b = m_txtNpcReply->GetBounds();
        b.x0 -= patB.x0;
        b.y0 -= patB.y0;
        m_txtNpcReply->SetBounds(b, true);
    }
    {
        BoundsBase<float> b = m_lstPlayerReplies->GetBounds();
        b.x0 -= patB.x0;
        b.y0 -= patB.y0;
        m_lstPlayerReplies->SetBounds(b, true);
    }

    if (deleteSrc)
    {
        patternWnd->DecRef();
        patternNpcReply->DecRef();
        patternPlayerReplies->DecRef();
    }

    m_gameDataFlags |= 1u;
    return 1;
}

// ===========================================================================
//  ConversationWnd::PlayerReplyButton::AuxInfo
// ===========================================================================

ConversationWnd::PlayerReplyButton::AuxInfo::AuxInfo()
{
    // RVA 0x97860
    m_selectColor = 0xFF241D1Bu;  // -14452709
    m_unselectColor = 0;
}

// ===========================================================================
//  ConversationWnd::PlayerReplyButton
// ===========================================================================

ConversationWnd::PlayerReplyButton::PlayerReplyButton(
    PointBase<float> const& origin,
    float width,
    int idx,
    m3d::ui::Wnd* parent)
{
    // RVA 0x98E90
    m_idx = idx;
    m_reply = nullptr;
    m_dQuest = nullptr;

    BoundsBase<float> const rc{origin.x, origin.y, width, 0.0f};
    m3d::ui::ButtonWnd::Create({}, 0xA40u, rc, 500000u);

    if (parent)
    {
        SetWrapMode(parent->GetWrapMode());
        SetFormatMode(parent->GetFormatMode());
        SetDefaultFont(parent->GetDefaultFont());
        SetTextColor(parent->GetTextColor());
        m_aif.m_unselectColor = GetTextColor();
    }
}

ConversationWnd::PlayerReplyButton::PlayerReplyButton(ConversationWnd::PlayerReplyButton const& rhs) : ButtonWnd(rhs)
{
    m_idx = rhs.m_idx;
    m_reply = rhs.m_reply;
    m_dQuest = rhs.m_dQuest;
}

ConversationWnd::PlayerReplyButton::~PlayerReplyButton()
{
    // RVA 0x97730 - nothing owned beyond the ButtonWnd base.
}

Reply const* ConversationWnd::PlayerReplyButton::GetReply() const
{
    // RVA 0x98F80
    return m_reply;
}

ai::DynamicQuest* ConversationWnd::PlayerReplyButton::GetDynamicQuest()
{
    // RVA 0x98F90
    return m_dQuest;
}

int ConversationWnd::PlayerReplyButton::GetIdx() const
{
    // RVA 0x99230
    return m_idx;
}

void ConversationWnd::PlayerReplyButton::SetUpForReply(Reply const* reply, ai::DynamicQuest* dQuest)
{
    // RVA 0x98FA0
    m_reply = reply;
    m_dQuest = dQuest;

    CStr const replyText = reply ? reply->GetText() : CStr();
    CStr text = (CStr(m_idx + 1) + ". ") + replyText;

    if (m_dQuest)
    {
        StringParser const& parser = M3D_APP->m_pInterfaceManager->GetStringParser();
        parser.FormatStringForDynamicQuest(text, m_dQuest);
    }

    SetText(text);
    RecalcHeight();
}

void ConversationWnd::PlayerReplyButton::RecalcHeight()
{
    // RVA 0x991B0
    m_bounds.height =
        GetGfxServer()->MeasureText(GetText(), GetDefaultFont(), GetWrapMode(), m_bounds.width).y;
}

int ConversationWnd::PlayerReplyButton::OnMouseIn()
{
    // RVA 0x991F0
    SetTextColor(m_aif.m_selectColor);
    return m3d::ui::ButtonWnd::OnMouseIn();
}

int ConversationWnd::PlayerReplyButton::OnMouseOut()
{
    // RVA 0x99210
    SetTextColor(m_aif.m_unselectColor);
    return m3d::ui::ButtonWnd::OnMouseOut();
}

// ===========================================================================
//  ConversationWnd::PlayerRepliesListBoxWnd::AuxInfo
// ===========================================================================

ConversationWnd::PlayerRepliesListBoxWnd::AuxInfo::AuxInfo()
{
    // RVA 0x97870
    m_space = 7.0f;
}

// ===========================================================================
//  ConversationWnd::PlayerRepliesListBoxWnd
// ===========================================================================

ConversationWnd::PlayerRepliesListBoxWnd::PlayerRepliesListBoxWnd()
{
    // RVA 0x99240
    m_drawFlags = 0;
    m_curSel = -1;
    if (m_clientEdges.size() > 2)
    {
        m_clientEdges[2] = 2.0f;
    }
    m_aif.m_space = 7.0f;
}

ConversationWnd::PlayerRepliesListBoxWnd::PlayerRepliesListBoxWnd(ConversationWnd::PlayerRepliesListBoxWnd const& rhs)
    : ListBoxWnd<ConversationWnd::PlayerReplyButton*>(rhs)
{
    // RVA 0x99240 twin: fresh, empty list.
    m_drawFlags = 0;
    m_curSel = -1;
    if (m_clientEdges.size() > 2)
    {
        m_clientEdges[2] = 2.0f;
    }
    m_aif.m_space = 7.0f;
}

ConversationWnd::PlayerRepliesListBoxWnd::~PlayerRepliesListBoxWnd()
{
    // RVA 0x99300 - items / storage released by the ListBoxWnd base.
}

int ConversationWnd::PlayerRepliesListBoxWnd::MeasureItem(int itemIdx, BoundsBase<float>& bounds) const
{
    // RVA 0x994A0
    PlayerReplyButton* btn = m_items[itemIdx].m_item;
    BoundsBase<float> const b = btn->GetBounds();
    bounds.x0 = 0.0f;
    bounds.y0 = 0.0f;
    bounds.width = b.width;
    bounds.height = b.height + m_aif.m_space;
    return 1;
}

int ConversationWnd::PlayerRepliesListBoxWnd::RenderItem(int itemIdx, PointBase<float> const& org, m3d::ui::DrawInfo const& di)
{
    // RVA 0x993F0
    PlayerReplyButton* btn = m_items[itemIdx].m_item;
    BoundsBase<float> b = btn->GetBounds();
    b.y0 = (di.m_clientRect.y0 - di.m_originalRect.y0) + org.y;
    btn->SetBounds(b, true);
    return 1;
}

int ConversationWnd::PlayerRepliesListBoxWnd::DeleteItem(int itemIdx)
{
    // RVA 0x99460
    if (PlayerReplyButton* btn = m_items[itemIdx].m_item)
    {
        btn->DecRef();
    }
    m_items[itemIdx].m_item = nullptr;
    return 1;
}

int ConversationWnd::PlayerRepliesListBoxWnd::CompareItem(int, int)
{
    // RVA 0x99530
    return 1;
}

int ConversationWnd::PlayerRepliesListBoxWnd::OnWndNotify(
    m3d::ui::Wnd* from,
    unsigned id,
    unsigned msg,
    m3d::AIParam const& data)
{
    // RVA 0x99350
    if ((m_style & 0x100000) != 0)
    {
        ReflectChildNotifyToParent(from, id, msg, data);
    }
    if (id != 500000 || msg != 1)
    {
        return 0;
    }

    auto* btn = static_cast<PlayerReplyButton*>(from);
    CallParentNotify(CN_SELECT_PLAYER_REPLY, m3d::AIParam(btn->GetIdx()), true);
    return 1;
}

int ConversationWnd::PlayerRepliesListBoxWnd::OnKey(unsigned short key, unsigned char scanCode, unsigned int state)
{
    // RVA 0x998A0 - number keys 1..0 (scancodes 2..0xB) pick a reply directly,
    // page keys 27 / 28 scroll the list.
    if (scanCode >= 2 && scanCode <= 0xB)
    {
        if (!state)
        {
            int const idx = scanCode - 2;
            if (idx >= 0 && idx < static_cast<int>(m_items.size()))
            {
                CallParentNotify(CN_SELECT_PLAYER_REPLY, m3d::AIParam(idx), false);
            }
            return 1;
        }
        // key held down: fall through to the page-scroll handling
    }
    else if (key == 27 || key == 28)
    {
        return 1;
    }

    if (key != 27 && key != 28)
    {
        return 0;
    }
    if (state)
    {
        Scroll(key == 27);
    }
    return 1;
}

int ConversationWnd::PlayerRepliesListBoxWnd::SetUpForNpcReply(Reply const* npcReply, ai::Npc* npc, bool bDeletePrevItems)
{
    // RVA 0x99540
    if (bDeletePrevItems)
    {
        RemoveAllItems();
    }
    if (!npcReply || npcReply->GetRole() != Reply::ROLE_NPC)
    {
        return 0;
    }

    RepliesManager* replies = M3D_APP->m_pInterfaceManager->GetRepliesManager();
    std::vector<Reply*> nextPlayerReplies = replies->GetNextPlayerReplies(npcReply, npc);
    if (nextPlayerReplies.empty())
    {
        return 0;
    }

    BoundsBase<float> const clientB = GetClientBounds();
    int const baseIdx = static_cast<int>(m_items.size());

    for (int i = 0; i < static_cast<int>(nextPlayerReplies.size()); ++i)
    {
        Reply* reply = nextPlayerReplies[i];
        if (!reply || reply->GetRole() != Reply::ROLE_PLAYER)
        {
            continue;
        }
        auto* btn = new PlayerReplyButton(
            PointBase<float>{clientB.x0, clientB.y0}, clientB.width, baseIdx + i, this);
        btn->SetUpForReply(reply, nullptr);
        AddItem(btn);
        AddChild(btn);
    }
    return 1;
}

int ConversationWnd::PlayerRepliesListBoxWnd::SetUpForDynamicQuestThemes(ai::Npc* npc)
{
    // RVA 0x996B0
    RemoveAllItems();
    if (!npc)
    {
        return 0;
    }
    if (npc->GetNpcType() != ai::Npc::NPC_BARMAN)
    {
        return 0;
    }

    ai::Obj* town = npc->GetParent();
    while (town && !town->IsKindOf(&ai::Town::m_classTown))
    {
        town = town->GetParent();
    }
    if (!town)
    {
        return 0;
    }

    auto const dynamicQuestIds = static_cast<ai::Town*>(town)->GetDynamicQuestIds();
    BoundsBase<float> const clientB = GetClientBounds();
    int idx = 0;

    for (int qId : dynamicQuestIds)
    {
        ai::Obj* obj = ai::theObjects->GetEntityByObjId(qId);
        if (!obj || !obj->IsKindOf(&ai::DynamicQuest::m_classDynamicQuest))
        {
            continue;
        }
        auto* dQuest = static_cast<ai::DynamicQuest*>(obj);

        // TODO(RVA 0x996B0): the shipped build reads a per-quest status word and
        // skips quests that are no longer offerable before this point. That
        // field (ai::DynamicQuest::QuestStatus) is only forward declared in the
        // ported headers, so for now every live dynamic quest is listed.
        dQuest->UpdateBeforeShowInConversation();

        auto* btn = new PlayerReplyButton(
            PointBase<float>{clientB.x0, clientB.y0}, clientB.width, idx, this);

        RepliesManager* replies = M3D_APP->m_pInterfaceManager->GetRepliesManager();
        Reply const* request = replies->GetPlayerRequestForDynamicQuest(npc, dQuest);
        if (!request)
        {
            btn->DecRef();
            continue;
        }

        btn->SetUpForReply(request, dQuest);
        AddItem(btn);
        AddChild(btn);
        ++idx;
    }
    return 1;
}
