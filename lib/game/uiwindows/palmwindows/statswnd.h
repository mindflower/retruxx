#pragma once
#include "screenwnd.h"
#include <core/ref_ptr.h>
#include <ui/button.h>
#include <ui/uilistboxwnd.h>

namespace m3d
{
    namespace ui
    {
        class TextBoxWnd;
    }
}  // namespace m3d

class ItemModelWnd;

class StatsButton : public m3d::ui::ButtonWnd
{
public:
    int SetUpForStats(CStr const& statsName, PointBase<float> const& origin, float width);
    CStr const& GetStatsName() const;
    BoundsBase<float> const& GetVirtualBounds() const;
    void SetBounds(BoundsBase<float> const& rect, bool bUpdateBaseOrigin);
    void SetBounds(BoundsBase<float> const& rect, BoundsBase<float> const& virtualRect);
    int UpdateValue();
    void ClearValue();

    struct AuxInfo
    {
        unsigned int m_textColor;
        float m_nameWRel;
        float m_spaceX;
        float m_spaceY;
        AuxInfo();
    };

protected:
    virtual CStr GetStatsFullName() const;
    CStr GetHackedDefaultValueForStats(CStr const& statsName) const;
    CStr m_statsName;
    m3d::ui::Wnd* m_lblName;
    m3d::ui::Wnd* m_lblValue;
    static StatsButton::AuxInfo m_aif;
    BoundsBase<float> m_virtualBounds;
    StatsButton();
    StatsButton(StatsButton const& rhs);

public:
    virtual ~StatsButton() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classStatsButton;
};

class StatsList : public m3d::ui::ListBoxWnd<StatsButton*>
{
public:
    int CreateFromPattern(m3d::ui::Wnd* pw, bool deleteSrc);
    int AddButtonByStatsName(CStr const& statsName);
    int GetButtonIdByStatsName(CStr const& statsName) const;
    void UpdateStats();
    void ClearStats();

protected:
    virtual int MeasureItem(int itemIdx, BoundsBase<float>& bounds) const override;
    virtual int RenderItem(int itemIdx, PointBase<float> const& org, m3d::ui::DrawInfo const& di) override;
    virtual int DeleteItem(int itemIdx) override;
    virtual int CompareItem(int itemIdx0, int itemIdx1) override;
    int InitStats();
    StatsList();
    StatsList(StatsList const& rhs);

public:
    virtual ~StatsList() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classStatsList;
};

class StatsWnd : public ScreenWnd
{
public:
    struct AuxInfo
    {
        CStr m_statsListName;
        CStr m_wndPlayerDizName;
        CStr m_wndPlayerPortraitName;
        AuxInfo(StatsWnd::AuxInfo const&);
        AuxInfo();
    };
    ;

protected:
    virtual int GameDataSetup() override;
    virtual int GameDataUpdate(void* data, int dataType) override;
    virtual int GameDataClear(bool beforeContinuousMap) override;
    virtual int OnAfterAddToWndStation() override;
    void OnNewFrame();
    void OnStartLevel();
    void UpdateStats();
    void UpdatePlayerPortrait();
    void UpdatePlayerPortraitAnmation();
    void UpdatePlayerDiz();
    StatsList* m_statsList;
    m3d::ui::TextBoxWnd* m_wndPlayerDiz;
    ref_ptr<ItemModelWnd> m_wndPlayerPortrait;
    StatsWnd::AuxInfo m_aif;
    StatsWnd();
    StatsWnd(StatsWnd const& rhs);

public:
    virtual ~StatsWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classStatsWnd;
};
