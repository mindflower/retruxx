#pragma once
#include <core/ref_ptr.h>
#include <ui/wnd.h>
#include <map>

namespace ai
{
    class Building;
    class Town;
}  // namespace ai

class PlayerMoneyWnd;

namespace m3d
{
    namespace ui
    {
        class ButtonWnd;
        class ImageWnd;
    }  // namespace ui
}  // namespace m3d

class MotherPanelTabButton;
class ChildPanel;

class MotherPanel : public m3d::ui::ModalWnd
{
public:
    enum Tab
    {
        TAB_QUESTLOG = 0,
        TAB_MAP = 1,
        TAB_JOURNAL = 2,
        TAB_INVENTORY_VS_SHOP = 3,
        TAB_CHARACTERISTIC_VS_WORKSHOP = 4,
        TAB_BAR = 5,
        TAB_ADDITIONAL_BUILDING = 6,
        TAB_NUM_TABS = 7,
        TAB_INVALID = 7,
    };

public:
    void LeaveTown(bool bQuick);
    static CStr __fastcall Tab2Str(MotherPanel::Tab tabId);
    bool IsInTownRoot() const;

    enum ChildPanelId
    {
        PANEL_LEFT = 0,
        PANEL_RIGHT = 1,
        PANEL_VIDEO = 2,
        PANEL_TRADE_RIGHT = 3,
        PANEL_TRADE_LEFT = 4,
        PANEL_TRADE_COMMON = 5,
        PANEL_FULLSCREEN = 6,
        PANEL_TOWN = 7,
        PANEL_PALM = 8,
        PANEL_CONVERSATION = 9,
        PANEL_INVALID = 10,
    };

    using ChildPanelMap = std::map<
        enum MotherPanel::ChildPanelId,
        ref_ptr<ChildPanel>,
        std::less<enum MotherPanel::ChildPanelId>,
        std::allocator<std::pair<enum MotherPanel::ChildPanelId const, ref_ptr<ChildPanel>>>>;
    using ChildPanelPair = std::pair<enum MotherPanel::ChildPanelId, ref_ptr<ChildPanel>>;
    using ChildPanelIdPair = std::pair<enum MotherPanel::ChildPanelId, int>;
    using ChildPanelIdGuiIdVector = std::vector<
        std::pair<enum MotherPanel::ChildPanelId, int>,
        std::allocator<std::pair<enum MotherPanel::ChildPanelId, int>>>;
    using StyleMap = std::map<int, int, std::less<int>, std::allocator<std::pair<int const, int>>>;
    using StylePair = std::pair<int, int>;
    using ChildPanelIdVector =
        std::vector<enum MotherPanel::ChildPanelId, std::allocator<enum MotherPanel::ChildPanelId>>;
    using TabButtonVector = std::vector<MotherPanelTabButton*, std::allocator<MotherPanelTabButton*>>;

    struct MotherPanel::AuxSuspendedShow
    {
        /* 0x0000 */ std::vector<enum MotherPanel::ChildPanelId, std::allocator<enum MotherPanel::ChildPanelId>>
            m_previousPanelsToRemain;
        /* 0x0010 */ std::vector<
            std::pair<enum MotherPanel::ChildPanelId, int>,
            std::allocator<std::pair<enum MotherPanel::ChildPanelId, int>>>
            m_suspendedPanels;
        AuxSuspendedShow(MotherPanel::AuxSuspendedShow const&);
        AuxSuspendedShow();
        void Reset();
    }; /* size: 0x0020 */

    struct MotherPanel::AuxInfo
    {
        /* 0x0000 */ CStr m_tabBtnName;
        /* 0x000c */ CStr m_wndDecorName;
        /* 0x0018 */ CStr m_wndDecorBarName;
        /* 0x0024 */ CStr m_wndDecorBgName;
        /* 0x0030 */ CStr m_btnExitName;
        /* 0x003c */ CStr m_wndTopPanelName;
        /* 0x0048 */ CStr m_pickUpSoundName;
        AuxInfo(MotherPanel::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x0054 */

protected:
    virtual int OnAfterRemoveFromWndStation() override /* 0x00 */;
    virtual int OnBeforeAddToWndStation() override /* 0x00 */;
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x00 */;
    virtual int GameDataSetup() override /* 0x00 */;
    virtual int RemoveChild(m3d::Object* w) override /* 0x00 */;
    virtual int RemoveChildForce(m3d::Object* w) override /* 0x00 */;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int idFrom, unsigned int message, m3d::AIParam const& data)
        override /* 0x00 */;
    int AddChildPanel(ref_ptr<ChildPanel> childPanel, MotherPanel::ChildPanelId panelId);
    int RemoveChildPanel(ref_ptr<ChildPanel> childPanel);
    int RemoveChildPanelById(MotherPanel::ChildPanelId panelId);
    void OnInventory();
    void OnShop();
    void OnWorkshop();
    void OnCharacteristics();
    void OnTown();
    void OnBar();
    void OnAdditionalBuilding();
    void OnQuestLog();
    void OnMap();
    void OnLocalMap(void* data);
    void OnGlobalMap();
    void OnJournal();
    void OnTalkWithNpc();
    void OnBuyVehicle();
    void OnStartTrade(void* data);
    void OnFinishTrade();
    void OnHidePanel(void* data);
    void OnShowPanel(void* data);
    void UpdateOnStartupImpulse();
    void ShowPanels(
        std::vector<
            std::pair<enum MotherPanel::ChildPanelId, int>,
            std::allocator<std::pair<enum MotherPanel::ChildPanelId, int>>> panels,
        std::vector<enum MotherPanel::ChildPanelId, std::allocator<enum MotherPanel::ChildPanelId>> const&
            previousPanelsToRemain);
    void Show();
    void Hide(bool bForce, bool bQuickLeaveTown);
    void ClearPanels(
        std::vector<enum MotherPanel::ChildPanelId, std::allocator<enum MotherPanel::ChildPanelId>> const&
            previousPanelsToRemain);
    virtual int OnKey(unsigned short key, unsigned char scanCode, unsigned int state) override /* 0x00 */;
    bool CanChildPanelBeLaunchedNow(MotherPanel::ChildPanelId panelId) const;
    void OnEndWndAnimation();
    bool IsPanelPresent(int guiId) const;
    MotherPanel::ChildPanelId GetCurrentPanelIdByGuiId(int guiId) const;
    int GetGuiIdByCurrentPanelId(MotherPanel::ChildPanelId panelId) const;
    int CreateHackedWorkshopVehicle();
    void DestroyHackedWorkshopVehicle();
    void SelectTabButton(MotherPanel::Tab tabId);
    void ShowTabButton(MotherPanel::Tab tabId, bool bShow);
    void OnTabBtnClick(m3d::ui::Wnd* wndFrom, int idFrom);
    void OnBtnExitClick(m3d::ui::Wnd* wndFrom, int idFrom);
    void OnEnterTown(ai::Town const* town);
    void OnLeaveTown(bool bQuick);
    void UpdateTabButtonsOnEnterTown(ai::Town const* town);
    void UpdateTabButtonsOnLeaveTown();
    bool InTown() const;
    void ToggleTab(MotherPanel::Tab tabId);
    void AdjustChildOrder();
    void AdjustDecor();
    ai::Building* GetBuildingForTab(MotherPanel::Tab tabId) const;
    MotherPanel::Tab GetTabForBuilding(ai::Building const* building) const;
    void OnEscape();
    MotherPanel::Tab ValidateLastTab() const;
    void SetCurTab(MotherPanel::Tab tabId, bool bUpdatePanels);
    void OnPickUpAll();
    ai::Building const* GetOnlyBuilding() const;
    void AdjustAnimationOnShowPanels(
        std::vector<
            std::pair<enum MotherPanel::ChildPanelId, int>,
            std::allocator<std::pair<enum MotherPanel::ChildPanelId, int>>> const& panels);
    void AdjustAnimationOnHidePanel(m3d::ui::Wnd* panel);
    bool PickUpItemsFromGround();
    /* 0x0224 */ std::map<
        enum MotherPanel::ChildPanelId,
        ref_ptr<ChildPanel>,
        std::less<enum MotherPanel::ChildPanelId>,
        std::allocator<std::pair<enum MotherPanel::ChildPanelId const, ref_ptr<ChildPanel>>>>
        m_panels;
    /* 0x0230 */ MotherPanel::AuxSuspendedShow m_suspendedShow;
    /* 0x0250 */ std::vector<enum MotherPanel::ChildPanelId, std::allocator<enum MotherPanel::ChildPanelId>>
        m_secondPanelLevel;
    /* 0x0260 */ int m_hackedWorkshopVehicleId;
    /* 0x0264 */ std::vector<MotherPanelTabButton*, std::allocator<MotherPanelTabButton*>> m_tabButtons;
    /* 0x0274 */ MotherPanel::Tab m_curTabId;
    /* 0x0278 */ MotherPanel::Tab m_lastTabId;
    /* 0x027c */ MotherPanel::AuxInfo m_aif;
    /* 0x02d0 */ m3d::ui::Wnd* m_wndDecor;
    /* 0x02d4 */ m3d::ui::ImageWnd* m_wndDecorBar;
    /* 0x02d8 */ m3d::ui::ButtonWnd* m_btnExit;
    /* 0x02dc */ ref_ptr<PlayerMoneyWnd> m_wndPlayerMoney;
    /* 0x02e0 */ m3d::ui::Wnd* m_wndTopPanel;
    /* 0x02e4 */ bool m_bCurMapLocal;
    MotherPanel();
    MotherPanel(MotherPanel const& rhs);

public:
    virtual ~MotherPanel() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x00 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x00 */;
    static m3d::Class m_classMotherPanel;
}; /* size: 0x02e8 */
