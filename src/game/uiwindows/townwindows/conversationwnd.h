#pragma once
#include <ui/button.h>
#include <ui/ui.h>
#include <ui/uilistboxwnd.h>

namespace ai
{
    class Npc;
    class DynamicQuest;
}

class Reply;

namespace m3d
{
    namespace ui
    {
        class TextBoxWnd;
    }
}

class ConversationWnd : public m3d::ui::Wnd
{
public:
    class PlayerReplyButton : public m3d::ui::ButtonWnd
    {
    public:
        Reply const* GetReply() const;
        ai::DynamicQuest* GetDynamicQuest();
        int GetIdx() const;
        virtual ~PlayerReplyButton();
        PlayerReplyButton(PointBase<float> const&, float, int, m3d::ui::Wnd*);
        void SetUpForReply(Reply const*, ai::DynamicQuest*);

    private:
        void RecalcHeight();
        virtual int OnMouseOut();
        virtual int OnMouseIn();
        int m_idx;
        const Reply* m_reply;
        ai::DynamicQuest* m_dQuest;
    };

    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        CStr m_txtNpcReplyName;
        CStr m_lstPlayerRepliesName;
        float m_space;
    };

    class PlayerRepliesListBoxWnd : public m3d::ui::ListBoxWnd<PlayerReplyButton*>
    {
    public:
        class AuxInfo
        {
        public:
            AuxInfo();

        private:
            float m_space;
        };

    public:
        PlayerRepliesListBoxWnd();
        virtual ~PlayerRepliesListBoxWnd();
        int SetUpForNpcReply(Reply const*, ai::Npc*, bool);
        int SetUpForDynamicQuestThemes(ai::Npc*);

    private:
        virtual int OnWndNotify(m3d::ui::Wnd*, unsigned int, unsigned int, m3d::AIParam const&);
        virtual int DeleteItem(int);
        virtual int MeasureItem(int, BoundsBase<float>&) const;
        virtual int RenderItem(int, PointBase<float> const&, m3d::ui::DrawInfo const&);
        virtual int OnKey(unsigned short, unsigned char, unsigned int);
        virtual int CompareItem(int, int);

    private:
        AuxInfo m_aif;
    };

public:
    virtual m3d::Object * Clone();
    ai::DynamicQuest * GetCurrentDynamicQuest();
    static m3d::Class * GetBaseClass();
    int CreateFromPattern(m3d::ui::Wnd *,bool);
    int ShowDynamicQuestsMenu();
    virtual int GameDataClear(bool);
    int GetNpcId() const ;
    void LeaveTown();
    void StartConversation(int);
    virtual m3d::Class * GetClass() const ;
    virtual ~ConversationWnd();
    static m3d::Object * CreateObject();
    void EndConversation();

protected:
    ConversationWnd();
    ConversationWnd(ConversationWnd const &);
    virtual int GameDataUpdate(void *,int);
    int ExecuteScript(CStr const &);
    int UpdateOnEndConversation();
    int UpdateOnSelectPlayerReply(class Reply const *);
    virtual int OnAfterRemoveFromWndStation();
    virtual int OnWndNotify(m3d::ui::Wnd *,unsigned int,unsigned int,m3d::AIParam const &);
    int UpdateOnBeginConversation(int);
    int UpdateOnSetNpcReply(Reply const *);
    bool NeedConversationExit() const ;

public:
    RT_CLASS_DECLARE(ConversationWnd);

private:
    AuxInfo m_aif;
    m3d::ui::TextBoxWnd *m_txtNpcReply;
    ConversationWnd::PlayerRepliesListBoxWnd *m_lstPlayerReplies;
    ai::DynamicQuest *m_curDynamicQuest;
    ai::Npc *m_npc;
    int m_scriptFlags;
};
