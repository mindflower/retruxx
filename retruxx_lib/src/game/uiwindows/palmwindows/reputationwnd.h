#pragma once
#include "screenwnd.h"
#include <game/uimisc/guihelper.h>
#include <ui/button.h>

namespace m3d
{
    namespace ui
    {
        class ImageWnd;
    }
}


class ReputationButton :  public m3d::ui::ButtonWnd
{
public:
    virtual m3d::Object * Clone();
    static m3d::Class * GetBaseClass();
    int GetClanBelong() const ;
    static m3d::Object * CreateObject();
    int SetUpForClan(int);
    virtual m3d::Class * GetClass() const ;
    virtual ~ReputationButton();

protected:
    CStr BWTolerance2StrId2(help::BlackWightTolerance) const ;
    static CStr BWTolerance2StrId1(help::BlackWightTolerance);
    static void ClearPattern();
    ReputationButton(ReputationButton const &);
    ReputationButton();
    int CreateChildren();
    unsigned int BWTolerance2Color(help::BlackWightTolerance) const ;
    void UpdateTolerance();
    int CreateFromPattern();
    void UpdateToleranceIndicator(help::BlackWightTolerance);
    void UpdateTooltip(help::BlackWightTolerance);
    static int LoadPattern(m3d::ui::Wnd *);

public:
    RT_CLASS_DECLARE(ReputationButton);

private:
    int m_clanBelong;
    m3d::ui::Wnd *m_lblClanName;
    m3d::ui::ImageWnd *m_wndClanIco;
    m3d::ui::Wnd *m_wndToleranceName;
};

class AuxRPanel
{
public:
    ReputationButton* GetButtonBySlot(enum Slot) const;
    ReputationButton* AddButton(int, m3d::ui::Wnd*);
    AuxRPanel();
    ~AuxRPanel();
    bool HasEmptySlot(void) const;

private:
    Slot GetEmptySlotId(void) const;
    ReputationButton* m_btns[2];
};

class ReputationList : public m3d::ui::Wnd
{
public:
    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        float m_spaceX;
        float m_spaceY;
    };

public:
    virtual int GameDataClear(bool);
    virtual m3d::Class* GetClass() const;
    ReputationButton* AddButtonByClanBelong(int);
    void UpdateClans();
    virtual m3d::Object* Clone();
    virtual ~ReputationList();
    int CreateFromPattern(m3d::ui::Wnd*, bool);
    static m3d::Object* CreateObject();
    static m3d::Class* GetBaseClass();

protected:
    ReputationList();
    ReputationList(ReputationList const&);
    PointBase<int> GetButtonPanelCoord(ReputationButton const*) const;
    int GetPanelIdFitToPlaceButton();
    void PlaceButton(ReputationButton*);
    virtual int OnBeforeAddToWndStation();
    ReputationButton* GetButtonByClanBelong(int) const;
    void ClearClans();

public:
    RT_CLASS_DECLARE(ReputationList);

private:
    ReputationList::AuxInfo m_aif;
    std::vector<AuxRPanel*> m_items;
};

class ReputationWnd :  public ScreenWnd
{
public:
    class AuxInfo
    {
    public:
        AuxInfo(void);

    private:
        CStr m_reputationListName;
    };

public:
    virtual m3d::Object * Clone();
    virtual m3d::Class * GetClass() const ;
    static m3d::Object * CreateObject();
    static m3d::Class * GetBaseClass();
    virtual ~ReputationWnd();

protected:
    ReputationWnd(ReputationWnd const &);
    ReputationWnd();
    void OnKnownClansChanged();
    virtual int GameDataClear(bool);
    virtual int GameDataUpdate(void *,int);
    virtual int GameDataSetup();
    virtual int OnAfterAddToWndStation();

public:
    RT_CLASS_DECLARE(ReputationWnd);

private:
    ReputationList *m_reputationList;
    ReputationWnd::AuxInfo m_aif;
};

