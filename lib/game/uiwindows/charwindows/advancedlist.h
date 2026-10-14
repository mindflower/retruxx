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
    virtual int GetUnitsToBuy() const;
    virtual int GetPrice() const;
    virtual int SetupForObj(int objId, int workshopId);
    virtual int SetupForWorkshop(int workshopId);
    int GetObjId() const;
    void SetMode(AdvancedButton::Mode mode);

    struct Pattern
    {
        Pattern();
        ~Pattern();
        void IncRef();
        void DecRef();
        int GetRef() const;

        m3d::ui::Wnd* m_wndPattern;
        m3d::ui::ButtonWnd* m_btnPatternActiveZone;
        m3d::ui::ImageWnd* m_wndPatternIco;
        m3d::ui::Wnd* m_wndPatternName;
        m3d::ui::Wnd* m_wndPatternPrice;
        m3d::ui::ButtonWnd* m_btnPatternAdvance;
        m3d::ui::Wnd* m_wndPatternIdioticEmbossBtnAdvance;
        m3d::ui::Wnd* m_wndPatternSlider;
        m3d::ui::ButtonWnd* m_btnPatternLeft;
        m3d::ui::ButtonWnd* m_btnPatternRight;
        m3d::ui::Wnd* m_wndPatternUnits;
        m3d::ui::Wnd* m_wndPatternFrame;
        void Clear();
        int m_ref;
    };

    struct AuxInfo
    {
        CStr m_wndPatternName;
        CStr m_btnPatternActiveZoneName;
        CStr m_wndPatternIcoName;
        CStr m_wndPatternNameName;
        CStr m_wndPatternPriceName;
        CStr m_btnPatternAdvanceName;
        CStr m_wndPatternIdioticEmbossBtnAdvanceName;
        CStr m_wndPatternSliderName;
        CStr m_btnPatternLeftName;
        CStr m_btnPatternRightName;
        CStr m_wndPatternUnitsName;
        CStr m_wndPatternFrameName;
        CStr m_toAdvancePaneName;
        CStr m_toSimplePaneName;
        CStr m_strIdTooltipToSimple;
        CStr m_strIdTooltipToAdvanced;
        CStr m_strIdTooltipBuy;
        CStr m_strIdTooltipBuyNotNeed;
        CStr m_strIdTooltipBuyUnavailable;
        CStr m_strIdTooltipBuyPartial;
        CStr m_strIdTooltipBuyFull;
        CStr m_strIdTooltipOneUnitPrice;
        CStr m_strIdTooltipNumUnitsToBuy;
        CStr m_strIdTooltipSummaryPrice;
        CStr m_strIdState;
        unsigned int m_normalColor;
        unsigned int m_warnColor;
        unsigned int m_colorYellow;
        unsigned int m_colorGreen;
        unsigned int m_colorRed;
        unsigned int m_colorNormal;

        AuxInfo(AdvancedButton::AuxInfo const&);
        AuxInfo();
    };

protected:
    virtual int CreateFromPattern();
    virtual int CreateChildren();
    static int __fastcall LoadPattern(m3d::ui::Wnd* pattern);
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data) override;
    virtual int OnBeforeAddToWndStation() override;
    virtual void OnChangeMode();
    virtual void RecalcLayot();
    void UpdateAdvanceButtonState();
    virtual void OnBuy();
    virtual void UpdateValues();
    virtual void UpdatePrevValues();
    virtual void UpdateControlsOnNewFrame(bool bForce);
    virtual void UpdatePriceControls(bool bForce);
    virtual void UpdateUnitsControls(bool bForce);
    void UpdateSlider(bool bForce);
    void UpdateLeftRightButtonsState();
    void UpdateTooltip(bool bForce);
    virtual int GetMaxUnitsToBuy() const = 0;
    virtual int GetMaxPossibleUnitsToBuy() const;
    virtual int GetUnitsPrice(int units) const;
    virtual float GetPriceForOneUnit() const = 0;
    virtual void FullUpdate();
    virtual CStr GetServiceName() const = 0;
    virtual m3d::rend::TexHandle GetServiceIco() const = 0;
    virtual void UpdateServiceName();
    virtual void UpdateServiceIco();
    void OnLeft();
    void OnRight();
    ai::Workshop* GetWorkshop() const;
    virtual CStr const& GetStrIdTooltipBuy() const;
    virtual CStr const& GetStrIdTooltipBuyNotNeed() const;
    virtual CStr const& GetStrIdTooltipBuyUnavailable() const;
    virtual CStr const& GetStrIdTooltipBuyPartial() const;
    virtual CStr const& GetStrIdTooltipBuyFull() const;
    unsigned int EnumColor2Color(help::Color enumColor) const;
    void OnBtnAdvanceClick();

    static inline AdvancedButton::Pattern m_pattern;
    static inline AdvancedButton::AuxInfo m_aif;

    m3d::ui::ButtonWnd* m_btnActiveZone;
    m3d::ui::ImageWnd* m_wndIco;
    m3d::ui::Wnd* m_wndName;
    m3d::ui::Wnd* m_wndPrice;
    m3d::ui::ButtonWnd* m_btnAdvance;
    m3d::ui::Wnd* m_wndIdioticEmbossBtnAdvance;
    BuyServiceSlider* m_wndSlider;
    m3d::ui::ButtonWnd* m_btnLeft;
    m3d::ui::ButtonWnd* m_btnRight;
    m3d::ui::Wnd* m_wndUnits;
    m3d::ui::Wnd* m_wndFrame;
    AdvancedButton::Mode m_mode;
    int m_objId;
    int m_workshopId;
    int m_unitsToBuy;
    int m_maxUnitsToBuy;
    int m_maxPossibleUnitsToBuy;
    float m_oneUnitPrice;
    int m_summaryPrice;
    int m_prevUnitsToBuy;
    int m_prevMaxUnitsToBuy;
    int m_prevMaxPossibleUnitsToBuy;
    float m_prevOneUnitPrice;
    int m_prevSummaryPrice;

    AdvancedButton(AdvancedButton const&);
    AdvancedButton();

public:
    virtual ~AdvancedButton() override;
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetRtClass() const;
    static m3d::Class m_classAdvancedButton;
};

class AdvancedList : public m3d::ui::Wnd
{
    // GarageWnd::GameDataSetup moves the list into its own coordinates by writing m_bounds directly.
    friend class GarageWnd;

public:
    int SetupForWorkshop(int workshopId);
    void SetSwitchWndBounds(BoundsBase<float> const& b);

    using AdvancedButtonsVector = std::vector<AdvancedButton*, std::allocator<AdvancedButton*>>;

    struct AuxInfo
    {
        float m_space;
        AuxInfo();
    };

protected:
    virtual int GameDataSetup() override;
    virtual int GameDataClear(bool beforeContinuousLevel) override;
    virtual int GameDataUpdate(void* data, int dataType) override;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data) override;
    virtual int OnBeforeAddToWndStation() override;
    void ClearItems();
    virtual void RecalcLayot();
    virtual void BuyService(AdvancedButton const*) = 0;
    virtual int CreateItems();
    virtual int AddItem(int objId);
    virtual AdvancedButton* NewItem() const = 0;
    virtual void OnPlayerVehicleChanged();
    virtual void OnVehiclePartChanged(void* data);
    virtual void OnNewFrame();
    virtual std::vector<int, std::allocator<int>> GetObjIds() const = 0;
    ai::Workshop* GetWorkshop() const;
    void OnAdvancedModeChanged(m3d::ui::Wnd* srcWnd, m3d::AIParam const& data);
    void SetAdvancedModeForButton(AdvancedButton* btn);

    std::vector<AdvancedButton*, std::allocator<AdvancedButton*>> m_items;
    BoundsBase<float> m_switchB;
    AdvancedList::AuxInfo m_aif;
    int m_workshopId;

    AdvancedList(AdvancedList const&);
    AdvancedList();

public:
    virtual ~AdvancedList() override;
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetRtClass() const;
    static m3d::Class m_classAdvancedList;
};