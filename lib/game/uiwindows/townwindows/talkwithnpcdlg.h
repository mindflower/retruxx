#pragma once
#include <core/ref_ptr.h>
#include <game/uiwindows/charwindows/childpanel.h>

namespace ai
{
    class Npc;
    class Bar;
}  // namespace ai

class PlayerMoneyWnd;
class NpcModelWnd;
class ConversationWnd;
class TownDlg;
class BarDlg;

class TalkWithNpcDlg : public ChildPanel
{
public:
    int Show(int npcId);
    void Hide(bool bLeaveTown);
    int SetupForNpc(int npcId);
    ai::Npc* GetNpc() const;

    enum ShowType
    {
        SHOWTYPE_IN_TOWN = 0,
        SHOWTYPE_IN_BAR = 1,
        SHOWTYPE_IN_FIELD = 2,
        SHOWTYPE_UNDEFINED = 3,
    };

    struct AuxInfo
    {
        /* 0x0000 */ CStr m_wndConversationName;
        /* 0x000c */ CStr m_wndNpcImageName;
        /* 0x0018 */ CStr m_wndTopPanelName;
        /* 0x0024 */ CStr m_wndBottomPanelName;
        /* 0x0030 */ CStr m_lblNpcNameName;
        AuxInfo(TalkWithNpcDlg::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x003c */

protected:
    virtual int GameDataSetup() override /* 0x00 */;
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x10c */;
    virtual int GameDataClear(bool beforeContinuousLevel) override /* 0x108 */;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data) override
        /* 0xc8 */;
    virtual int OnAfterAddToWndStation() override /* 0x00 */;
    virtual int OnActivate(bool on) override /* 0x00 */;
    bool IsValid() const;
    void OnLocationNpc(void* data);
    void OnBarNpc(void* data);
    ref_ptr<TownDlg> GetTownDlg() const;
    ref_ptr<BarDlg> GetBarDlg() const;
    ai::Bar const* GetBar() const;
    bool IsTownDlgShowingNow() const;
    bool IsBarDlgShowingNow() const;
    int GetNpcToTalkWithFromLocation(int locationId) const;
    void UpdateOnStartConversation();
    void FillPanels();
    virtual void OnExit() override /* 0x124 */;
    m3d::rend::TexHandle GetNpcBg(int npcId) const;
    /* 0x0224 */ ref_ptr<ConversationWnd> m_wndConversation;
    /* 0x0228 */ ref_ptr<NpcModelWnd> m_wndNpcImage;
    /* 0x022c */ ref_ptr<PlayerMoneyWnd> m_wndPlayerMoney;
    /* 0x0230 */ TalkWithNpcDlg::AuxInfo m_aif;
    /* 0x026c */ TalkWithNpcDlg::ShowType m_showType;
    /* 0x0270 */ m3d::ui::Wnd* m_wndTopPanel;
    /* 0x0274 */ m3d::ui::Wnd* m_wndBottomPanel;
    /* 0x0278 */ m3d::ui::Wnd* m_lblNpcName;
    /* 0x027c */ int m_npcId;
    TalkWithNpcDlg();
    TalkWithNpcDlg(TalkWithNpcDlg const& rhs);

public:
    virtual ~TalkWithNpcDlg() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classTalkWithNpcDlg;
}; /* size: 0x0280 */
