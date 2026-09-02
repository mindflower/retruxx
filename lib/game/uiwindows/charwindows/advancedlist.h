#pragma once
#include <game/uimisc/guihelper.h>
#include <ui/ui.h>

namespace ai
{
    class Workshop;
}

class BuyServiceSlider;

namespace m3d
{
    namespace ui
    {
        class ImageWnd;
        class ButtonWnd;
    }  // namespace ui
}  // namespace m3d

class AdvancedButton : public m3d::ui::Wnd
{
    // AdvancedList owns the shared pattern (m_pattern / LoadPattern) and drives
    // each button's mode (m_mode / OnChangeMode / FullUpdate).
    friend class AdvancedList;

public:
    enum Mode
    {
        MODE_SIMPLE = 0,
        MODE_ADVANCED = 1,
    };

public:
    virtual int GetUnitsToBuy() const /* 0x11c */;
    virtual int GetPrice() const /* 0x120 */;
    virtual int SetupForObj(int objId, int workshopId) /* 0x124 */;
    virtual int SetupForWorkshop(int workshopId) /* 0x128 */;
    int GetObjId() const;
    void SetMode(AdvancedButton::Mode mode);

    struct Pattern
    {
        Pattern();
        ~Pattern();
        void IncRef();
        void DecRef();
        int GetRef() const;

        /* 0x0000 */ m3d::ui::Wnd* m_wndPattern;
        /* 0x0004 */ m3d::ui::ButtonWnd* m_btnPatternActiveZone;
        /* 0x0008 */ m3d::ui::ImageWnd* m_wndPatternIco;
        /* 0x000c */ m3d::ui::Wnd* m_wndPatternName;
        /* 0x0010 */ m3d::ui::Wnd* m_wndPatternPrice;
        /* 0x0014 */ m3d::ui::ButtonWnd* m_btnPatternAdvance;
        /* 0x0018 */ m3d::ui::Wnd* m_wndPatternIdioticEmbossBtnAdvance;
        /* 0x001c */ m3d::ui::Wnd* m_wndPatternSlider;
        /* 0x0020 */ m3d::ui::ButtonWnd* m_btnPatternLeft;
        /* 0x0024 */ m3d::ui::ButtonWnd* m_btnPatternRight;
        /* 0x0028 */ m3d::ui::Wnd* m_wndPatternUnits;
        /* 0x002c */ m3d::ui::Wnd* m_wndPatternFrame;
        void Clear();
        /* 0x0030 */ int m_ref;
    }; /* size: 0x0034 */

    struct AuxInfo
    {
        /* 0x0000 */ CStr m_wndPatternName;
        /* 0x000c */ CStr m_btnPatternActiveZoneName;
        /* 0x0018 */ CStr m_wndPatternIcoName;
        /* 0x0024 */ CStr m_wndPatternNameName;
        /* 0x0030 */ CStr m_wndPatternPriceName;
        /* 0x003c */ CStr m_btnPatternAdvanceName;
        /* 0x0048 */ CStr m_wndPatternIdioticEmbossBtnAdvanceName;
        /* 0x0054 */ CStr m_wndPatternSliderName;
        /* 0x0060 */ CStr m_btnPatternLeftName;
        /* 0x006c */ CStr m_btnPatternRightName;
        /* 0x0078 */ CStr m_wndPatternUnitsName;
        /* 0x0084 */ CStr m_wndPatternFrameName;
        /* 0x0090 */ CStr m_toAdvancePaneName;
        /* 0x009c */ CStr m_toSimplePaneName;
        /* 0x00a8 */ CStr m_strIdTooltipToSimple;
        /* 0x00b4 */ CStr m_strIdTooltipToAdvanced;
        /* 0x00c0 */ CStr m_strIdTooltipBuy;
        /* 0x00cc */ CStr m_strIdTooltipBuyNotNeed;
        /* 0x00d8 */ CStr m_strIdTooltipBuyUnavailable;
        /* 0x00e4 */ CStr m_strIdTooltipBuyPartial;
        /* 0x00f0 */ CStr m_strIdTooltipBuyFull;
        /* 0x00fc */ CStr m_strIdTooltipOneUnitPrice;
        /* 0x0108 */ CStr m_strIdTooltipNumUnitsToBuy;
        /* 0x0114 */ CStr m_strIdTooltipSummaryPrice;
        /* 0x0120 */ CStr m_strIdState;
        /* 0x012c */ unsigned int m_normalColor;
        /* 0x0130 */ unsigned int m_warnColor;
        /* 0x0134 */ unsigned int m_colorYellow;
        /* 0x0138 */ unsigned int m_colorGreen;
        /* 0x013c */ unsigned int m_colorRed;
        /* 0x0140 */ unsigned int m_colorNormal;

        AuxInfo(AdvancedButton::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x0144 */

protected:
    virtual int CreateFromPattern() /* 0x12c */;
    virtual int CreateChildren() /* 0x130 */;
    static int __fastcall LoadPattern(m3d::ui::Wnd* pattern);
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data) override
        /* 0xc8 */;
    virtual int OnBeforeAddToWndStation() override /* 0x68 */;
    virtual void OnChangeMode() /* 0x134 */;
    virtual void RecalcLayot() /* 0x138 */;
    void UpdateAdvanceButtonState();
    virtual void OnBuy() /* 0x13c */;
    virtual void UpdateValues() /* 0x140 */;
    virtual void UpdatePrevValues() /* 0x144 */;
    virtual void UpdateControlsOnNewFrame(bool bForce) /* 0x148 */;
    virtual void UpdatePriceControls(bool bForce) /* 0x14c */;
    virtual void UpdateUnitsControls(bool bForce) /* 0x150 */;
    void UpdateSlider(bool bForce);
    void UpdateLeftRightButtonsState();
    void UpdateTooltip(bool bForce);
    virtual int GetMaxUnitsToBuy() const = 0 /* 0x154 */;
    virtual int GetMaxPossibleUnitsToBuy() const /* 0x158 */;
    virtual int GetUnitsPrice(int units) const /* 0x15c */;
    virtual float GetPriceForOneUnit() const = 0 /* 0x160 */;
    virtual void FullUpdate() /* 0x164 */;
    virtual CStr GetServiceName() const = 0 /* 0x168 */;
    virtual m3d::rend::TexHandle GetServiceIco() const = 0 /* 0x16c */;
    virtual void UpdateServiceName() /* 0x170 */;
    virtual void UpdateServiceIco() /* 0x174 */;
    void OnLeft();
    void OnRight();
    ai::Workshop* GetWorkshop() const;
    virtual CStr const& GetStrIdTooltipBuy() const /* 0x178 */;
    virtual CStr const& GetStrIdTooltipBuyNotNeed() const /* 0x17c */;
    virtual CStr const& GetStrIdTooltipBuyUnavailable() const /* 0x180 */;
    virtual CStr const& GetStrIdTooltipBuyPartial() const /* 0x184 */;
    virtual CStr const& GetStrIdTooltipBuyFull() const /* 0x188 */;
    unsigned int EnumColor2Color(help::Color enumColor) const;
    void OnBtnAdvanceClick();

    static inline AdvancedButton::Pattern m_pattern;
    static inline AdvancedButton::AuxInfo m_aif;

    /* 0x0220 */ m3d::ui::ButtonWnd* m_btnActiveZone;
    /* 0x0224 */ m3d::ui::ImageWnd* m_wndIco;
    /* 0x0228 */ m3d::ui::Wnd* m_wndName;
    /* 0x022c */ m3d::ui::Wnd* m_wndPrice;
    /* 0x0230 */ m3d::ui::ButtonWnd* m_btnAdvance;
    /* 0x0234 */ m3d::ui::Wnd* m_wndIdioticEmbossBtnAdvance;
    /* 0x0238 */ BuyServiceSlider* m_wndSlider;
    /* 0x023c */ m3d::ui::ButtonWnd* m_btnLeft;
    /* 0x0240 */ m3d::ui::ButtonWnd* m_btnRight;
    /* 0x0244 */ m3d::ui::Wnd* m_wndUnits;
    /* 0x0248 */ m3d::ui::Wnd* m_wndFrame;
    /* 0x024c */ AdvancedButton::Mode m_mode;
    /* 0x0250 */ int m_objId;
    /* 0x0254 */ int m_workshopId;
    /* 0x0258 */ int m_unitsToBuy;
    /* 0x025c */ int m_maxUnitsToBuy;
    /* 0x0260 */ int m_maxPossibleUnitsToBuy;
    /* 0x0264 */ float m_oneUnitPrice;
    /* 0x0268 */ int m_summaryPrice;
    /* 0x026c */ int m_prevUnitsToBuy;
    /* 0x0270 */ int m_prevMaxUnitsToBuy;
    /* 0x0274 */ int m_prevMaxPossibleUnitsToBuy;
    /* 0x0278 */ float m_prevOneUnitPrice;
    /* 0x027c */ int m_prevSummaryPrice;

    AdvancedButton(AdvancedButton const&);
    AdvancedButton();

public:
    virtual ~AdvancedButton() override /* 0x00 */;
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetRtClass() const /* 0x18c */;
    static m3d::Class m_classAdvancedButton;
}; /* size: 0x0280 */

class AdvancedList : public m3d::ui::Wnd
{
public:
    int SetupForWorkshop(int workshopId);
    void SetSwitchWndBounds(BoundsBase<float> const& b);

    using AdvancedButtonsVector = std::vector<AdvancedButton*, std::allocator<AdvancedButton*>>;

    struct AuxInfo
    {
        /* 0x0000 */ float m_space;
        AuxInfo();
    }; /* size: 0x0004 */

protected:
    virtual int GameDataSetup() override /* 0x104 */;
    virtual int GameDataClear(bool beforeContinuousLevel) override /* 0x108 */;
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x10c */;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data) override
        /* 0xc8 */;
    virtual int OnBeforeAddToWndStation() override /* 0x68 */;
    void ClearItems();
    virtual void RecalcLayot() /* 0x11c */;
    virtual void BuyService(AdvancedButton const*) = 0 /* 0x120 */;
    virtual int CreateItems() /* 0x124 */;
    virtual int AddItem(int objId) /* 0x128 */;
    virtual AdvancedButton* NewItem() const = 0 /* 0x12c */;
    virtual void OnPlayerVehicleChanged() /* 0x130 */;
    virtual void OnVehiclePartChanged(void* data) /* 0x134 */;
    virtual void OnNewFrame() /* 0x138 */;
    virtual std::vector<int, std::allocator<int>> GetObjIds() const = 0 /* 0x13c */;
    ai::Workshop* GetWorkshop() const;
    void OnAdvancedModeChanged(m3d::ui::Wnd* srcWnd, m3d::AIParam const& data);
    void SetAdvancedModeForButton(AdvancedButton* btn);

    /* 0x0220 */ std::vector<AdvancedButton*, std::allocator<AdvancedButton*>> m_items;
    /* 0x0230 */ BoundsBase<float> m_switchB;
    /* 0x0240 */ AdvancedList::AuxInfo m_aif;
    /* 0x0244 */ int m_workshopId;

    AdvancedList(AdvancedList const&);
    AdvancedList();

public:
    virtual ~AdvancedList() override /* 0x00 */;
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetRtClass() const /* 0x140 */;
    static m3d::Class m_classAdvancedList;
}; /* size: 0x0248 */