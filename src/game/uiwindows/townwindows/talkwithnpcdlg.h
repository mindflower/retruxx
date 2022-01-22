#pragma once
#include <core/ref_ptr.h>
#include <game/uiwindows/charwindows/childpanel.h>

namespace ai
{
    class Npc;
    class Bar;
}

class PlayerMoneyWnd;
class NpcModelWnd;
class ConversationWnd;

class TalkWithNpcDlg :  public ChildPanel
{
public:
    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        CStr m_wndConversationName;
        CStr m_wndNpcImageName;
        CStr m_wndTopPanelName;
        CStr m_wndBottomPanelName;
        CStr m_lblNpcNameName;
    };

    enum ShowType
    {
        SHOWTYPE_IN_TOWN = 0x0,
        SHOWTYPE_IN_BAR = 0x1,
        SHOWTYPE_IN_FIELD = 0x2,
        SHOWTYPE_UNDEFINED = 0x3,
    };

public:
    virtual ~TalkWithNpcDlg();
    ai::Npc * GetNpc() const ;
    void Hide(bool);
    virtual m3d::Class * GetClass() const ;
    int SetupForNpc(int);
    int Show(int);
    virtual m3d::Object * Clone();
    static m3d::Class * GetBaseClass();
    static m3d::Object * CreateObject();

protected:
    bool IsValid() const ;
    bool IsTownDlgShowingNow() const ;
    int GetNpcToTalkWithFromLocation(int) const ;
    void OnBarNpc(void *);
    TalkWithNpcDlg();
    TalkWithNpcDlg(TalkWithNpcDlg const &);
    virtual int OnActivate(bool);
    virtual int GameDataClear(bool);
    void FillPanels();
    virtual int OnAfterAddToWndStation();
    ref_ptr<class TownDlg> GetTownDlg() const ;
    bool IsBarDlgShowingNow() const ;
    virtual int GameDataUpdate(void *,int);
    ref_ptr<class BarDlg> GetBarDlg() const ;
    virtual void OnExit();
    void OnLocationNpc(void *);
    m3d::rend::TexHandle GetNpcBg(int) const ;
    virtual int GameDataSetup();
    void UpdateOnStartConversation();
    ai::Bar const * GetBar() const ;
    virtual int OnWndNotify(m3d::ui::Wnd *,unsigned int,unsigned int, m3d::AIParam const &);

public:
    RT_CLASS_DECLARE(TalkWithNpcDlg);

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
