#pragma once

namespace ConversationWnd
{
    class PlayerReplyButton :  public m3d::ui::ButtonWnd
    {
    public:
        class Reply const * GetReply() const ;
        class ai::DynamicQuest * GetDynamicQuest();
        int GetIdx() const ;
        virtual ~PlayerReplyButton();
        PlayerReplyButton(class PointBase<float> const &,float,int,class m3d::ui::Wnd *);
        void SetUpForReply(class Reply const *,class ai::DynamicQuest *);
    protected:
    private:
        void RecalcHeight();
        virtual int OnMouseOut();
        virtual int OnMouseIn();
        int m_idx;
        const Reply *m_reply;
        ai::DynamicQuest *m_dQuest;
    };
}

class ConversationWnd :  public m3d::ui::Wnd
{
public:
    virtual class m3d::Object * Clone();
    class ai::DynamicQuest * GetCurrentDynamicQuest();
    static struct m3d::Class * __fastcall GetBaseClass();
    int CreateFromPattern(class m3d::ui::Wnd *,bool);
    int ShowDynamicQuestsMenu();
    virtual int GameDataClear(bool);
    int GetNpcId() const ;
    void LeaveTown();
    void StartConversation(int);
    virtual struct m3d::Class * GetClass() const ;
    virtual ~ConversationWnd();
    static class m3d::Object * __fastcall CreateObject();
    void EndConversation();
protected:
    ConversationWnd();
    ConversationWnd(class ConversationWnd const &);
    virtual int GameDataUpdate(void *,int);
    int ExecuteScript(class CStr const &);
    int UpdateOnEndConversation();
    int UpdateOnSelectPlayerReply(class Reply const *);
    virtual int OnAfterRemoveFromWndStation();
    virtual int OnWndNotify(class m3d::ui::Wnd *,unsigned int,unsigned int,class m3d::AIParam const &);
    int UpdateOnBeginConversation(int);
    int UpdateOnSetNpcReply(class Reply const *);
    bool NeedConversationExit() const ;
private:
    m3d::ui::ListBoxWnd<PlayerReplyButton *>::~ListBoxWnd<PlayerReplyButton *>();
    m3d::ui::ListBoxWnd<PlayerReplyButton *>::ListBoxWnd<PlayerReplyButton *>();
    ConversationWnd::AuxInfo m_aif;
    m3d::ui::TextBoxWnd *m_txtNpcReply;
    ConversationWnd::PlayerRepliesListBoxWnd *m_lstPlayerReplies;
    ai::DynamicQuest *m_curDynamicQuest;
    ai::Npc *m_npc;
    int m_scriptFlags;
};
