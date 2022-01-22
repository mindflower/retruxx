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
}

class ItemModelWnd;

class StatsButton : public m3d::ui::ButtonWnd
{
public:
    int SetUpForStats(CStr const&, PointBase<float> const&, float);
    void ClearValue();
    static m3d::Class* GetBaseClass();
    int UpdateValue();
    virtual m3d::Object* Clone();
    void SetBounds(BoundsBase<float> const&, BoundsBase<float> const&);
    CStr const& GetStatsName() const;
    BoundsBase<float> const& GetVirtualBounds() const;
    static m3d::Object* CreateObject();
    virtual ~StatsButton();
    virtual m3d::Class* GetClass() const;

protected:
    virtual void SetBounds(BoundsBase<float> const&, bool);
    StatsButton(StatsButton const&);
    StatsButton();
    virtual CStr GetStatsFullName() const;
    CStr GetHackedDefaultValueForStats(CStr const&) const;

public:
    RT_CLASS_DECLARE(StatsButton);

private:
    CStr m_statsName;
    m3d::ui::Wnd* m_lblName;
    m3d::ui::Wnd* m_lblValue;
    BoundsBase<float> m_virtualBounds;
};

class StatsList :  public m3d::ui::ListBoxWnd<StatsButton *>
{
public:
    static m3d::Class * GetBaseClass();
    int GetButtonIdByStatsName(CStr const &) const ;
    virtual m3d::Class * GetClass() const ;
    static m3d::Object * CreateObject();
    void ClearStats();
    int CreateFromPattern(m3d::ui::Wnd *,bool);
    void UpdateStats();
    virtual m3d::Object * Clone();
    virtual ~StatsList();
    int AddButtonByStatsName(CStr const &);

protected:
    StatsList(StatsList const &);
    StatsList();
    virtual int MeasureItem(int, BoundsBase<float> &) const ;
    virtual int RenderItem(int, PointBase<float> const &, m3d::ui::DrawInfo const &);
    virtual int CompareItem(int,int);
    int InitStats();
    virtual int DeleteItem(int);

public:
    RT_CLASS_DECLARE(StatsList);
};

class StatsWnd :  public ScreenWnd
{
public:
    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        CStr m_statsListName;
        CStr m_wndPlayerDizName;
        CStr m_wndPlayerPortraitName;
    };

public:
    virtual m3d::Class * GetClass() const ;
    virtual m3d::Object * Clone();
    virtual ~StatsWnd();
    static m3d::Object * CreateObject();
    static m3d::Class * GetBaseClass();

protected:
    StatsWnd(StatsWnd const &);
    StatsWnd();
    void OnNewFrame();
    virtual int GameDataClear(bool);
    virtual int GameDataUpdate(void *,int);
    void OnStartLevel();
    void UpdatePlayerPortrait();
    void UpdatePlayerDiz();
    void UpdatePlayerPortraitAnmation();
    virtual int OnAfterAddToWndStation();
    virtual int GameDataSetup();
    void UpdateStats();

public:
    RT_CLASS_DECLARE(StatsWnd);

private:
    StatsList *m_statsList;
    m3d::ui::TextBoxWnd *m_wndPlayerDiz;
    ref_ptr<ItemModelWnd> m_wndPlayerPortrait;
    StatsWnd::AuxInfo m_aif;
};
