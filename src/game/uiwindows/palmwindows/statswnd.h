#pragma once

class StatsList :  public m3d::ui::ListBoxWnd<StatsButton *>
{
public:
    static struct m3d::Class * __fastcall GetBaseClass();
    int GetButtonIdByStatsName(class CStr const &) const ;
    virtual struct m3d::Class * GetClass() const ;
    static class m3d::Object * __fastcall CreateObject();
    void ClearStats();
    int CreateFromPattern(class m3d::ui::Wnd *,bool);
    void UpdateStats();
    virtual class m3d::Object * Clone();
    virtual ~StatsList();
    int AddButtonByStatsName(class CStr const &);
protected:
    StatsList(class StatsList const &);
    StatsList();
    virtual int MeasureItem(int,struct BoundsBase<float> &) const ;
    virtual int RenderItem(int,class PointBase<float> const &,struct m3d::ui::DrawInfo const &);
    virtual int CompareItem(int,int);
    int InitStats();
    virtual int DeleteItem(int);
private:
};

class StatsWnd :  public ScreenWnd
{
public:
    virtual struct m3d::Class * GetClass() const ;
    virtual class m3d::Object * Clone();
    virtual ~StatsWnd();
    static class m3d::Object * __fastcall CreateObject();
    static struct m3d::Class * __fastcall GetBaseClass();
protected:
    StatsWnd(class StatsWnd const &);
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
private:
    StatsList *m_statsList;
    m3d::ui::TextBoxWnd *m_wndPlayerDiz;
    ref_ptr<ItemModelWnd> m_wndPlayerPortrait;
    StatsWnd::AuxInfo m_aif;
};

class StatsButton :  public m3d::ui::ButtonWnd
{
public:
    int SetUpForStats(class CStr const &,class PointBase<float> const &,float);
    void ClearValue();
    static struct m3d::Class * __fastcall GetBaseClass();
    int UpdateValue();
    virtual class m3d::Object * Clone();
    void SetBounds(struct BoundsBase<float> const &,struct BoundsBase<float> const &);
    class CStr const & GetStatsName() const ;
    struct BoundsBase<float> const & GetVirtualBounds() const ;
    static class m3d::Object * __fastcall CreateObject();
    virtual ~StatsButton();
    virtual struct m3d::Class * GetClass() const ;
protected:
    virtual void SetBounds(struct BoundsBase<float> const &,bool);
    StatsButton(class StatsButton const &);
    StatsButton();
    virtual class CStr GetStatsFullName() const ;
    class CStr GetHackedDefaultValueForStats(class CStr const &) const ;
private:
    CStr m_statsName;
    m3d::ui::Wnd *m_lblName;
    m3d::ui::Wnd *m_lblValue;
    BoundsBase<float> m_virtualBounds;
};
