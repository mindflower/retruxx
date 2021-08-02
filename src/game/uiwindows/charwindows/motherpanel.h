#pragma once
#include <core/ref_ptr.h>
#include <ui/wnd.h>

namespace ai
{
    class Building;
    class Town;
}

class PlayerMoneyWnd;

namespace m3d
{
    namespace ui
    {
        class ButtonWnd;
        class ImageWnd;
    }
}

class MotherPanelTabButton;
class ChildPanel;

class MotherPanel :  public m3d::ui::ModalWnd
{
public:
    enum Tab
    {
        TAB_QUESTLOG = 0x0,
        TAB_MAP = 0x1,
        TAB_JOURNAL = 0x2,
        TAB_INVENTORY_VS_SHOP = 0x3,
        TAB_CHARACTERISTIC_VS_WORKSHOP = 0x4,
        TAB_BAR = 0x5,
        TAB_ADDITIONAL_BUILDING = 0x6,
        TAB_NUM_TABS = 0x7,
        TAB_INVALID = 0x7,
    };

    enum ChildPanelId
    {
        PANEL_LEFT = 0x0,
        PANEL_RIGHT = 0x1,
        PANEL_VIDEO = 0x2,
        PANEL_TRADE_RIGHT = 0x3,
        PANEL_TRADE_LEFT = 0x4,
        PANEL_TRADE_COMMON = 0x5,
        PANEL_FULLSCREEN = 0x6,
        PANEL_TOWN = 0x7,
        PANEL_PALM = 0x8,
        PANEL_CONVERSATION = 0x9,
        PANEL_INVALID = 0xA,
    };

    class AuxSuspendedShow
    {
    public:
        void Reset();
        AuxSuspendedShow();

    private:
        std::vector<ChildPanelId> m_previousPanelsToRemain;
        std::vector<std::pair<ChildPanelId, int>> m_suspendedPanels;
    };

    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        CStr m_tabBtnName;
        CStr m_wndDecorName;
        CStr m_wndDecorBarName;
        CStr m_wndDecorBgName;
        CStr m_btnExitName;
        CStr m_wndTopPanelName;
        CStr m_pickUpSoundName;
    };

public:
    static m3d::Class m_classMotherPanel;

public:
    void LeaveTown(bool);
    virtual m3d::Class * GetClass() const ;
    static m3d::Object * __fastcall CreateObject();
    virtual m3d::Object * Clone();
    static m3d::Class * __fastcall GetBaseClass();
    bool IsInTownRoot() const ;
    virtual ~MotherPanel();

protected:
    void UpdateTabButtonsOnEnterTown(ai::Town const *);
    virtual int OnBeforeAddToWndStation();
    void OnBtnExitClick(m3d::ui::Wnd *,int);
    void ClearPanels(std::vector<ChildPanelId,std::allocator<ChildPanelId> > const &);
    void OnEscape();
    virtual int RemoveChildForce(m3d::Object *);
    ChildPanelId GetCurrentPanelIdByGuiId(int) const ;
    void OnHidePanel(void *);
    void ToggleTab(Tab);
    void AdjustAnimationOnShowPanels(std::vector<std::pair<ChildPanelId,int>,std::allocator<std::pair<ChildPanelId,int> > > const &);
    void OnMap();
    void OnAdditionalBuilding();
    virtual int OnWndNotify(m3d::ui::Wnd *,unsigned int,unsigned int,m3d::AIParam const &);
    virtual int OnAfterRemoveFromWndStation();
    void OnShowPanel(void *);
    int AddChildPanel(ref_ptr<ChildPanel>,ChildPanelId);
    void OnShop();
    void OnPickUpAll();
    void OnLocalMap(void *);
    void OnLeaveTown(bool);
    bool PickUpItemsFromGround();
    void OnBuyVehicle();
    void ShowPanels(std::vector<std::pair<ChildPanelId,int>,std::allocator<std::pair<ChildPanelId,int> > >,std::vector<ChildPanelId,std::allocator<ChildPanelId> > const &);
    ai::Building * GetBuildingForTab(Tab) const ;
    virtual int RemoveChild(m3d::Object *);
    void ShowTabButton(Tab,bool);
    void OnFinishTrade();
    void OnEndWndAnimation();
    bool IsPanelPresent(int) const ;
    void OnJournal();
    bool CanChildPanelBeLaunchedNow(ChildPanelId) const ;
    Tab GetTabForBuilding(ai::Building const *) const ;
    void AdjustAnimationOnHidePanel(m3d::ui::Wnd *);
    void UpdateTabButtonsOnLeaveTown();
    void OnTalkWithNpc();
    void OnQuestLog();
    bool InTown() const ;
    void OnInventory();
    int RemoveChildPanelById(ChildPanelId);
    int GetGuiIdByCurrentPanelId(ChildPanelId) const ;
    Tab ValidateLastTab() const ;
    int RemoveChildPanel(ref_ptr<ChildPanel>);
    void OnGlobalMap();
    void OnCharacteristics();
    ai::Building const * GetOnlyBuilding() const ;
    void AdjustChildOrder();
    virtual int GameDataSetup();
    void OnTown();
    MotherPanel(MotherPanel const &);
    MotherPanel();
    virtual int GameDataUpdate(void *,int);
    void Hide(bool,bool);
    void DestroyHackedWorkshopVehicle();
    void Show();
    virtual int OnKey(unsigned short,unsigned char,unsigned int);
    void OnStartTrade(void *);
    void OnTabBtnClick(m3d::ui::Wnd *,int);
    int CreateHackedWorkshopVehicle();
    void OnWorkshop();
    void SetCurTab(Tab,bool);
    void OnEnterTown(ai::Town const *);
    void OnBar();
    void SelectTabButton(Tab);
    void AdjustDecor();

private:
    std::map<ChildPanelId,ref_ptr<ChildPanel>> m_panels;
    AuxSuspendedShow m_suspendedShow;
    std::vector<ChildPanelId> m_secondPanelLevel;
    int m_hackedWorkshopVehicleId;
    std::vector<MotherPanelTabButton *> m_tabButtons;
    Tab m_curTabId;
    Tab m_lastTabId;
    AuxInfo m_aif;
    m3d::ui::Wnd *m_wndDecor;
    m3d::ui::ImageWnd *m_wndDecorBar;
    m3d::ui::ButtonWnd *m_btnExit;
    ref_ptr<PlayerMoneyWnd> m_wndPlayerMoney;
    m3d::ui::Wnd *m_wndTopPanel;
    bool m_bCurMapLocal;
};
