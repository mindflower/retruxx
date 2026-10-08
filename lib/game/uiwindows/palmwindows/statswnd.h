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
        /* 0x0000 */ unsigned int m_textColor;
        /* 0x0004 */ float m_nameWRel;
        /* 0x0008 */ float m_spaceX;
        /* 0x000c */ float m_spaceY;
        AuxInfo();
    }; /* size: 0x0010 */

protected:
    virtual CStr GetStatsFullName() const /* 0x11c */;
    CStr GetHackedDefaultValueForStats(CStr const& statsName) const;
    /* 0x023c */ CStr m_statsName;
    /* 0x0248 */ m3d::ui::Wnd* m_lblName;
    /* 0x024c */ m3d::ui::Wnd* m_lblValue;
    static StatsButton::AuxInfo m_aif;
    /* 0x0250 */ BoundsBase<float> m_virtualBounds;
    StatsButton();
    StatsButton(StatsButton const& rhs);

public:
    virtual ~StatsButton() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x00 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x00 */;
    static m3d::Class m_classStatsButton;
}; /* size: 0x0260 */

class StatsList : public m3d::ui::ListBoxWnd<StatsButton*>
{
public:
    int CreateFromPattern(m3d::ui::Wnd* pw, bool deleteSrc);
    int AddButtonByStatsName(CStr const& statsName);
    int GetButtonIdByStatsName(CStr const& statsName) const;
    void UpdateStats();
    void ClearStats();

protected:
    virtual int MeasureItem(int itemIdx, BoundsBase<float>& bounds) const override /* 0x134 */;
    virtual int RenderItem(int itemIdx, PointBase<float> const& org, m3d::ui::DrawInfo const& di) override /* 0x138 */;
    virtual int DeleteItem(int itemIdx) override /* 0x13c */;
    virtual int CompareItem(int itemIdx0, int itemIdx1) override /* 0x140 */;
    int InitStats();
    StatsList();
    StatsList(StatsList const& rhs);

public:
    virtual ~StatsList() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x00 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x00 */;
    static m3d::Class m_classStatsList;
}; /* size: 0x0238 */

class StatsWnd : public ScreenWnd
{
public:
    struct AuxInfo
    {
        /* 0x0000 */ CStr m_statsListName;
        /* 0x000c */ CStr m_wndPlayerDizName;
        /* 0x0018 */ CStr m_wndPlayerPortraitName;
        AuxInfo(StatsWnd::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x0024 */
    ;

protected:
    virtual int GameDataSetup() override /* 0x00 */;
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x00 */;
    virtual int GameDataClear(bool beforeContinuousMap) override /* 0x00 */;
    virtual int OnAfterAddToWndStation() override /* 0x00 */;
    void OnNewFrame();
    void OnStartLevel();
    void UpdateStats();
    void UpdatePlayerPortrait();
    void UpdatePlayerPortraitAnmation();
    void UpdatePlayerDiz();
    /* 0x0224 */ StatsList* m_statsList;
    /* 0x0228 */ m3d::ui::TextBoxWnd* m_wndPlayerDiz;
    /* 0x022c */ ref_ptr<ItemModelWnd> m_wndPlayerPortrait;
    /* 0x0230 */ StatsWnd::AuxInfo m_aif;
    StatsWnd();
    StatsWnd(StatsWnd const& rhs);

public:
    virtual ~StatsWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classStatsWnd;
}; /* size: 0x0254 */
