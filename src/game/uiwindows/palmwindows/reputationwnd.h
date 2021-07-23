#pragma once

class ReputationButton :  public m3d::ui::ButtonWnd
{
public:
    virtual class m3d::Object * Clone();
    static struct m3d::Class * __fastcall GetBaseClass();
    int GetClanBelong() const ;
    static class m3d::Object * __fastcall CreateObject();
    int SetUpForClan(int);
    virtual struct m3d::Class * GetClass() const ;
    virtual ~ReputationButton();
protected:
    class CStr BWTolerance2StrId2(enum help::BlackWightTolerance) const ;
    static class CStr __fastcall BWTolerance2StrId1(enum help::BlackWightTolerance);
    static void __fastcall ClearPattern();
    ReputationButton(class ReputationButton const &);
    ReputationButton();
    int CreateChildren();
    unsigned int BWTolerance2Color(enum help::BlackWightTolerance) const ;
    void UpdateTolerance();
    int CreateFromPattern();
    void UpdateToleranceIndicator(enum help::BlackWightTolerance);
    void UpdateTooltip(enum help::BlackWightTolerance);
    static int __fastcall LoadPattern(class m3d::ui::Wnd *);
private:
    int m_clanBelong;
    m3d::ui::Wnd *m_lblClanName;
    m3d::ui::ImageWnd *m_wndClanIco;
    m3d::ui::Wnd *m_wndToleranceName;
};

class ReputationWnd :  public ScreenWnd
{
public:
    virtual class m3d::Object * Clone();
    virtual struct m3d::Class * GetClass() const ;
    static class m3d::Object * __fastcall CreateObject();
    static struct m3d::Class * __fastcall GetBaseClass();
    virtual ~ReputationWnd();
protected:
    ReputationWnd(class ReputationWnd const &);
    ReputationWnd();
    void OnKnownClansChanged();
    virtual int GameDataClear(bool);
    virtual int GameDataUpdate(void *,int);
    virtual int GameDataSetup();
    virtual int OnAfterAddToWndStation();
private:
    ReputationList *m_reputationList;
    ReputationWnd::AuxInfo m_aif;
};

class ReputationList :  public m3d::ui::Wnd
{
public:
    virtual int GameDataClear(bool);
    virtual struct m3d::Class * GetClass() const ;
    class ReputationButton * AddButtonByClanBelong(int);
    void UpdateClans();
    virtual class m3d::Object * Clone();
    virtual ~ReputationList();
    int CreateFromPattern(class m3d::ui::Wnd *,bool);
    static class m3d::Object * __fastcall CreateObject();
    static struct m3d::Class * __fastcall GetBaseClass();
protected:
    ReputationList();
    ReputationList(class ReputationList const &);
    class PointBase<int> GetButtonPanelCoord(class ReputationButton const *) const ;
    int GetPanelIdFitToPlaceButton();
    void PlaceButton(class ReputationButton *);
    virtual int OnBeforeAddToWndStation();
    class ReputationButton * GetButtonByClanBelong(int) const ;
    void ClearClans();
private:
    ReputationList::AuxInfo m_aif;
    std::vector<AuxRPanel *> m_items;
};
