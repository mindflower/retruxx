#pragma once
#include <ui/button.h>
#include <ui/ui.h>
#include <ui/uilistboxwnd.h>

namespace ai
{
    class Npc;
    class DynamicQuest;
}  // namespace ai

class Reply;

namespace m3d
{
    namespace ui
    {
        class TextBoxWnd;
    }
}  // namespace m3d

class ConversationWnd : public m3d::ui::Wnd
{
public:
    int CreateFromPattern(m3d::ui::Wnd* patternWnd, bool deleteSrc);
    void StartConversation(int npcId);
    virtual int GameDataClear(bool beforeContinuousLevel) override /* 0x108 */;
    int GetNpcId() const;
    void EndConversation();
    int ShowDynamicQuestsMenu();
    ai::DynamicQuest* GetCurrentDynamicQuest();
    void LeaveTown();

    class PlayerReplyButton : public m3d::ui::ButtonWnd
    {
    public:
        PlayerReplyButton(ConversationWnd::PlayerReplyButton const&);
        PlayerReplyButton(PointBase<float> const& origin, float width, int idx, m3d::ui::Wnd* parent);
        virtual ~PlayerReplyButton() override /* 0x00 */;
        Reply const* GetReply() const;
        ai::DynamicQuest* GetDynamicQuest();
        void SetUpForReply(Reply const* reply, ai::DynamicQuest* dQuest);
        int GetIdx() const;

        struct AuxInfo
        {
            /* 0x0000 */ unsigned int m_selectColor;
            /* 0x0004 */ unsigned int m_unselectColor;
            AuxInfo();
        }; /* size: 0x0008 */

    private:
        virtual int OnMouseIn() override /* 0x00 */;
        virtual int OnMouseOut() override /* 0x00 */;
        void RecalcHeight();
        static inline ConversationWnd::PlayerReplyButton::AuxInfo m_aif;
        /* 0x023c */ int m_idx;
        /* 0x0240 */ Reply const* m_reply;
        /* 0x0244 */ ai::DynamicQuest* m_dQuest;
    }; /* size: 0x0248 */

    class PlayerRepliesListBoxWnd : public m3d::ui::ListBoxWnd<ConversationWnd::PlayerReplyButton*>
    {
    public:
        PlayerRepliesListBoxWnd(ConversationWnd::PlayerRepliesListBoxWnd const&);
        PlayerRepliesListBoxWnd();
        virtual ~PlayerRepliesListBoxWnd() override /* 0x00 */;
        int SetUpForNpcReply(Reply const* npcReply, ai::Npc* npc, bool bDeletePrevItems);
        int SetUpForDynamicQuestThemes(ai::Npc* npc);

        struct AuxInfo
        {
            /* 0x0000 */ float m_space;
            AuxInfo();
        }; /* size: 0x0004 */

    private:
        virtual int MeasureItem(int itemIdx, BoundsBase<float>& bounds) const override /* 0x134 */;
        virtual int RenderItem(int itemIdx, PointBase<float> const& org, m3d::ui::DrawInfo const& di) override
            /* 0x138 */;
        virtual int DeleteItem(int itemIdx) override /* 0x13c */;
        virtual int CompareItem(int itemIdx0, int itemIdx1) override /* 0x140 */;
        virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data)
            override /* 0x00 */;
        virtual int OnKey(unsigned short key, unsigned char scanCode, unsigned int state) override /* 0xbc */;
        /* 0x0238 */ ConversationWnd::PlayerRepliesListBoxWnd::AuxInfo m_aif;
    }; /* size: 0x023c */

    struct AuxInfo
    {
        /* 0x0000 */ CStr m_txtNpcReplyName;
        /* 0x000c */ CStr m_lstPlayerRepliesName;
        /* 0x0018 */ float m_space;
        AuxInfo(ConversationWnd::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x001c */

protected:
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x10c */;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data) override
        /* 0xc8 */;
    virtual int OnAfterRemoveFromWndStation() override /* 0x74 */;
    void RecalcLayot();
    int UpdateOnSelectPlayerReply(Reply const* playerReply);
    int UpdateOnBeginConversation(int npcId);
    int UpdateOnEndConversation();
    int UpdateOnSetNpcReply(Reply const* npcReply);
    int ExecuteScript(CStr const& script);
    bool NeedConversationExit() const;

    /* 0x0220 */ ConversationWnd::AuxInfo m_aif;
    /* 0x023c */ m3d::ui::TextBoxWnd* m_txtNpcReply;
    /* 0x0240 */ ConversationWnd::PlayerRepliesListBoxWnd* m_lstPlayerReplies;
    /* 0x0244 */ ai::DynamicQuest* m_curDynamicQuest;
    /* 0x0248 */ ai::Npc* m_npc;
    /* 0x024c */ int m_scriptFlags;
    ConversationWnd();
    ConversationWnd(ConversationWnd const& rhs);

public:
    virtual ~ConversationWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classConversationWnd;
}; /* size: 0x0250 */
