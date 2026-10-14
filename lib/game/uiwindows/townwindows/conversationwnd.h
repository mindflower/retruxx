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
    virtual int GameDataClear(bool beforeContinuousLevel) override;
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
        virtual ~PlayerReplyButton() override;
        Reply const* GetReply() const;
        ai::DynamicQuest* GetDynamicQuest();
        void SetUpForReply(Reply const* reply, ai::DynamicQuest* dQuest);
        int GetIdx() const;

        struct AuxInfo
        {
            unsigned int m_selectColor;
            unsigned int m_unselectColor;
            AuxInfo();
        };

    private:
        virtual int OnMouseIn() override;
        virtual int OnMouseOut() override;
        void RecalcHeight();
        static inline ConversationWnd::PlayerReplyButton::AuxInfo m_aif;
        int m_idx;
        Reply const* m_reply;
        ai::DynamicQuest* m_dQuest;
    };

    class PlayerRepliesListBoxWnd : public m3d::ui::ListBoxWnd<ConversationWnd::PlayerReplyButton*>
    {
    public:
        PlayerRepliesListBoxWnd(ConversationWnd::PlayerRepliesListBoxWnd const&);
        PlayerRepliesListBoxWnd();
        virtual ~PlayerRepliesListBoxWnd() override;
        int SetUpForNpcReply(Reply const* npcReply, ai::Npc* npc, bool bDeletePrevItems);
        int SetUpForDynamicQuestThemes(ai::Npc* npc);

        struct AuxInfo
        {
            float m_space;
            AuxInfo();
        };

    private:
        virtual int MeasureItem(int itemIdx, BoundsBase<float>& bounds) const override;
        virtual int RenderItem(int itemIdx, PointBase<float> const& org, m3d::ui::DrawInfo const& di) override
           ;
        virtual int DeleteItem(int itemIdx) override;
        virtual int CompareItem(int itemIdx0, int itemIdx1) override;
        virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data)
            override;
        virtual int OnKey(unsigned short key, unsigned char scanCode, unsigned int state) override;
        ConversationWnd::PlayerRepliesListBoxWnd::AuxInfo m_aif;
    };

    struct AuxInfo
    {
        CStr m_txtNpcReplyName;
        CStr m_lstPlayerRepliesName;
        float m_space;
        AuxInfo(ConversationWnd::AuxInfo const&);
        AuxInfo();
    };

protected:
    virtual int GameDataUpdate(void* data, int dataType) override;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data) override
       ;
    virtual int OnAfterRemoveFromWndStation() override;
    void RecalcLayot();
    int UpdateOnSelectPlayerReply(Reply const* playerReply);
    int UpdateOnBeginConversation(int npcId);
    int UpdateOnEndConversation();
    int UpdateOnSetNpcReply(Reply const* npcReply);
    int ExecuteScript(CStr const& script);
    bool NeedConversationExit() const;

    ConversationWnd::AuxInfo m_aif;
    m3d::ui::TextBoxWnd* m_txtNpcReply;
    ConversationWnd::PlayerRepliesListBoxWnd* m_lstPlayerReplies;
    ai::DynamicQuest* m_curDynamicQuest;
    ai::Npc* m_npc;
    int m_scriptFlags;
    ConversationWnd();
    ConversationWnd(ConversationWnd const& rhs);

public:
    virtual ~ConversationWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classConversationWnd;
};
