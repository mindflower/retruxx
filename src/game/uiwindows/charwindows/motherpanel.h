#pragma once

namespace MotherPanel
{
    class AuxSuspendedShow
    {
    public:
        void Reset();
        AuxSuspendedShow();
    protected:
    private:
        std::vector<ChildPanelId> m_previousPanelsToRemain;
        std::vector<std::pair<ChildPanelId,int>> m_suspendedPanels;
    };
}

class MotherPanel :  public m3d::ui::ModalWnd
{
public:
    void LeaveTown(bool);
    std::pair<enum ChildPanelId,int>::pair<enum ChildPanelId,int>(enum ChildPanelId const &,int const &);
    virtual struct m3d::Class * GetClass() const ;
    static class m3d::Object * __fastcall CreateObject();
    std::allocator<enum ChildPanelId>::allocator<enum ChildPanelId>(class std::allocator<enum ChildPanelId> const &);
    std::allocator<enum ChildPanelId>::allocator<enum ChildPanelId>();
    virtual class m3d::Object * Clone();
    static struct m3d::Class * __fastcall GetBaseClass();
    `public: static CStr __fastcall Tab2Str(enum Tab)'::`2'::_Tab2Str::~_Tab2Str();
    bool IsInTownRoot() const ;
    virtual ~MotherPanel();
protected:
    void UpdateTabButtonsOnEnterTown(class ai::Town const *);
    virtual int OnBeforeAddToWndStation();
    void OnBtnExitClick(class m3d::ui::Wnd *,int);
    void ClearPanels(class std::vector<enum ChildPanelId,class std::allocator<enum ChildPanelId> > const &);
    void OnEscape();
    virtual int RemoveChildForce(class m3d::Object *);
    enum ChildPanelId GetCurrentPanelIdByGuiId(int) const ;
    void OnHidePanel(void *);
    void ToggleTab(enum Tab);
    void AdjustAnimationOnShowPanels(class std::vector<struct std::pair<enum ChildPanelId,int>,class std::allocator<struct std::pair<enum ChildPanelId,int> > > const &);
    void OnMap();
    void OnAdditionalBuilding();
    virtual int OnWndNotify(class m3d::ui::Wnd *,unsigned int,unsigned int,class m3d::AIParam const &);
    virtual int OnAfterRemoveFromWndStation();
    void OnShowPanel(void *);
    int AddChildPanel(class ref_ptr<class ChildPanel>,enum ChildPanelId);
    void OnShop();
    void OnPickUpAll();
    void OnLocalMap(void *);
    void OnLeaveTown(bool);
    bool PickUpItemsFromGround();
    void OnBuyVehicle();
    void ShowPanels(class std::vector<struct std::pair<enum ChildPanelId,int>,class std::allocator<struct std::pair<enum ChildPanelId,int> > >,class std::vector<enum ChildPanelId,class std::allocator<enum ChildPanelId> > const &);
    class ai::Building * GetBuildingForTab(enum Tab) const ;
    virtual int RemoveChild(class m3d::Object *);
    void ShowTabButton(enum Tab,bool);
    void OnFinishTrade();
    void OnEndWndAnimation();
    bool IsPanelPresent(int) const ;
    void OnJournal();
    bool CanChildPanelBeLaunchedNow(enum ChildPanelId) const ;
    enum Tab GetTabForBuilding(class ai::Building const *) const ;
    void AdjustAnimationOnHidePanel(class m3d::ui::Wnd *);
    void UpdateTabButtonsOnLeaveTown();
    void OnTalkWithNpc();
    void OnQuestLog();
    bool InTown() const ;
    void OnInventory();
    int RemoveChildPanelById(enum ChildPanelId);
    int GetGuiIdByCurrentPanelId(enum ChildPanelId) const ;
    enum Tab ValidateLastTab() const ;
    int RemoveChildPanel(class ref_ptr<class ChildPanel>);
    void OnGlobalMap();
    void OnCharacteristics();
    class ai::Building const * GetOnlyBuilding() const ;
    void AdjustChildOrder();
    virtual int GameDataSetup();
    void OnTown();
    MotherPanel(class MotherPanel const &);
    MotherPanel();
    virtual int GameDataUpdate(void *,int);
    void Hide(bool,bool);
    void DestroyHackedWorkshopVehicle();
    void Show();
    virtual int OnKey(unsigned short,unsigned char,unsigned int);
    void OnStartTrade(void *);
    void OnTabBtnClick(class m3d::ui::Wnd *,int);
    int CreateHackedWorkshopVehicle();
    void OnWorkshop();
    void SetCurTab(enum Tab,bool);
    void OnEnterTown(class ai::Town const *);
    void OnBar();
    void SelectTabButton(enum Tab);
    void AdjustDecor();
private:
    enum ChildPanelId * __fastcall std::_Copy_backward_opt<enum ChildPanelId *,enum ChildPanelId *>(enum ChildPanelId *,enum ChildPanelId *,enum ChildPanelId *,struct std::_Nonscalar_ptr_iterator_tag);
    void __fastcall std::fill<enum ChildPanelId *,enum ChildPanelId>(enum ChildPanelId *,enum ChildPanelId *,enum ChildPanelId const &);
    void __fastcall std::_Destroy<enum ChildPanelId>(enum ChildPanelId *);
    enum ChildPanelId * __fastcall std::_Copy_opt<enum ChildPanelId *,enum ChildPanelId *>(enum ChildPanelId *,enum ChildPanelId *,enum ChildPanelId *,struct std::_Nonscalar_ptr_iterator_tag);
    struct std::_Nonscalar_ptr_iterator_tag __fastcall std::_Ptr_cat<enum ChildPanelId *,enum ChildPanelId *>(enum ChildPanelId * &,enum ChildPanelId * &);
    void __fastcall std::_Construct<enum ChildPanelId,enum ChildPanelId>(enum ChildPanelId *,enum ChildPanelId const &);
    enum ChildPanelId * __fastcall std::_Allocate<enum ChildPanelId>(unsigned int,enum ChildPanelId *);
    enum ChildPanelId * __fastcall std::copy<enum ChildPanelId *,enum ChildPanelId *>(enum ChildPanelId *,enum ChildPanelId *,enum ChildPanelId *);
    enum ChildPanelId * __fastcall std::copy_backward<enum ChildPanelId *,enum ChildPanelId *>(enum ChildPanelId *,enum ChildPanelId *,enum ChildPanelId *);
    std::map<enum MotherPanel::ChildPanelId,ref_ptr<ChildPanel>> m_panels;
    MotherPanel::AuxSuspendedShow m_suspendedShow;
    std::vector<enum MotherPanel::ChildPanelId> m_secondPanelLevel;
    int m_hackedWorkshopVehicleId;
    std::vector<MotherPanelTabButton *> m_tabButtons;
    MotherPanel::Tab m_curTabId;
    MotherPanel::Tab m_lastTabId;
    MotherPanel::AuxInfo m_aif;
    m3d::ui::Wnd *m_wndDecor;
    m3d::ui::ImageWnd *m_wndDecorBar;
    m3d::ui::ButtonWnd *m_btnExit;
    ref_ptr<PlayerMoneyWnd> m_wndPlayerMoney;
    m3d::ui::Wnd *m_wndTopPanel;
    bool m_bCurMapLocal;
};
