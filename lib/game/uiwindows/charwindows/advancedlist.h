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
    }
}

class AdvancedButton : public m3d::ui::Wnd
{
public:
    enum Mode
    {
        MODE_SIMPLE = 0x0,
        MODE_ADVANCED = 0x1,
    };

public:
    int GetObjId(void) const;
    void SetMode(Mode);
    virtual m3d::Class* GetRtClass(void) const;
    virtual int SetupForObj(int, int);
    virtual ~AdvancedButton(void);
    static m3d::Class* GetBaseClass(void);
    virtual int GetPrice(void) const;
    virtual int GetUnitsToBuy(void) const;
    virtual int SetupForWorkshop(int);

protected:
    virtual CStr const& GetStrIdTooltipBuyUnavailable(void) const;
    virtual void UpdatePrevValues(void);
    virtual int CreateChildren(void);
    virtual CStr const& GetStrIdTooltipBuyPartial(void) const;
    virtual int OnBeforeAddToWndStation(void);
    virtual void UpdatePriceControls(bool);
    void OnLeft(void);
    void UpdateTooltip(bool);
    void OnRight(void);
    AdvancedButton(void);
    void UpdateSlider(bool);
    virtual CStr const& GetStrIdTooltipBuyNotNeed(void) const;
    virtual void UpdateServiceName(void);
    virtual void OnBuy(void);
    virtual int GetUnitsPrice(int) const;
    virtual CStr const& GetStrIdTooltipBuy(void) const;
    virtual void UpdateServiceIco(void);
    virtual CStr const& GetStrIdTooltipBuyFull(void) const;
    virtual int OnWndNotify(m3d::ui::Wnd*, unsigned int, unsigned int, m3d::AIParam const&);
    virtual void OnChangeMode(void);
    void UpdateLeftRightButtonsState(void);
    virtual void UpdateUnitsControls(bool);
    ai::Workshop* GetWorkshop(void) const;
    static int __fastcall LoadPattern(m3d::ui::Wnd*);
    virtual int GetMaxPossibleUnitsToBuy(void) const;
    virtual void FullUpdate(void);
    void UpdateAdvanceButtonState(void);
    virtual int CreateFromPattern(void);
    virtual void UpdateControlsOnNewFrame(bool);
    unsigned int EnumColor2Color(help::Color) const;
    virtual void UpdateValues(void);
    void OnBtnAdvanceClick(void);
    virtual void RecalcLayot(void);

public:
    RT_CLASS_DECLARE(AdvancedButton);

private:
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
};

class AdvancedList : public m3d::ui::Wnd
{
public:
    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        float m_space;
    };

public:
    void SetSwitchWndBounds(BoundsBase<float> const&);
    virtual m3d::Class* GetRtClass(void) const;
    static m3d::Class* GetBaseClass(void);
    int SetupForWorkshop(int);
    virtual ~AdvancedList(void);

protected:
    virtual void OnVehiclePartChanged(void*);
    AdvancedList(void);
    virtual int OnWndNotify(m3d::ui::Wnd*, unsigned int, unsigned int, m3d::AIParam const&);
    ai::Workshop* GetWorkshop(void) const;
    virtual int CreateItems(void);
    virtual void OnNewFrame(void);
    virtual int GameDataUpdate(void*, int);
    virtual void RecalcLayot(void);
    virtual int GameDataClear(bool);
    virtual int OnBeforeAddToWndStation(void);
    void OnAdvancedModeChanged(m3d::ui::Wnd*, m3d::AIParam const&);
    virtual int GameDataSetup(void);
    virtual int AddItem(int);
    virtual void OnPlayerVehicleChanged(void);
    virtual AdvancedButton* NewItem(void) const;
    void SetAdvancedModeForButton(AdvancedButton*);
    void ClearItems(void);

public:
    RT_CLASS_DECLARE(AdvancedList);

private:
    std::vector<AdvancedButton*> m_items;
    BoundsBase<float> m_switchB;
    AdvancedList::AuxInfo m_aif;
    int m_workshopId;
};