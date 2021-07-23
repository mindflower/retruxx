#pragma once

class TalkWithNpcDlg :  public ChildPanel
{
public:
    virtual ~TalkWithNpcDlg();
    class ai::Npc * GetNpc() const ;
    void Hide(bool);
    virtual struct m3d::Class * GetClass() const ;
    int SetupForNpc(int);
    int Show(int);
    virtual class m3d::Object * Clone();
    static struct m3d::Class * __fastcall GetBaseClass();
    static class m3d::Object * __fastcall CreateObject();
protected:
    bool IsValid() const ;
    bool IsTownDlgShowingNow() const ;
    int GetNpcToTalkWithFromLocation(int) const ;
    void OnBarNpc(void *);
    TalkWithNpcDlg();
    TalkWithNpcDlg(class TalkWithNpcDlg const &);
    virtual int OnActivate(bool);
    virtual int GameDataClear(bool);
    void FillPanels();
    virtual int OnAfterAddToWndStation();
    class ref_ptr<class TownDlg> GetTownDlg() const ;
    bool IsBarDlgShowingNow() const ;
    virtual int GameDataUpdate(void *,int);
    class ref_ptr<class BarDlg> GetBarDlg() const ;
    virtual void OnExit();
    void OnLocationNpc(void *);
    class m3d::rend::TexHandle GetNpcBg(int) const ;
    virtual int GameDataSetup();
    void UpdateOnStartConversation();
    class ai::Bar const * GetBar() const ;
    virtual int OnWndNotify(class m3d::ui::Wnd *,unsigned int,unsigned int,class m3d::AIParam const &);
private:
    ref_ptr<ConversationWnd> m_wndConversation;
    ref_ptr<NpcModelWnd> m_wndNpcImage;
    ref_ptr<PlayerMoneyWnd> m_wndPlayerMoney;
    TalkWithNpcDlg::AuxInfo m_aif;
    TalkWithNpcDlg::ShowType m_showType;
    m3d::ui::Wnd *m_wndTopPanel;
    m3d::ui::Wnd *m_wndBottomPanel;
    m3d::ui::Wnd *m_lblNpcName;
    int m_npcId;
};
