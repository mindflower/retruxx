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
        CStr m_wndConversationName;
        CStr m_wndNpcImageName;
        CStr m_wndTopPanelName;
        CStr m_wndBottomPanelName;
        CStr m_lblNpcNameName;
        AuxInfo(TalkWithNpcDlg::AuxInfo const&);
        AuxInfo();
    };

protected:
    virtual int GameDataSetup() override;
    virtual int GameDataUpdate(void* data, int dataType) override;
    virtual int GameDataClear(bool beforeContinuousLevel) override;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data) override;
    virtual int OnAfterAddToWndStation() override;
    virtual int OnActivate(bool on) override;
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
    virtual void OnExit() override;
    m3d::rend::TexHandle GetNpcBg(int npcId) const;
    ref_ptr<ConversationWnd> m_wndConversation;
    ref_ptr<NpcModelWnd> m_wndNpcImage;
    ref_ptr<PlayerMoneyWnd> m_wndPlayerMoney;
    TalkWithNpcDlg::AuxInfo m_aif;
    TalkWithNpcDlg::ShowType m_showType;
    m3d::ui::Wnd* m_wndTopPanel;
    m3d::ui::Wnd* m_wndBottomPanel;
    m3d::ui::Wnd* m_lblNpcName;
    int m_npcId;
    TalkWithNpcDlg();
    TalkWithNpcDlg(TalkWithNpcDlg const& rhs);

public:
    virtual ~TalkWithNpcDlg() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classTalkWithNpcDlg;
};
