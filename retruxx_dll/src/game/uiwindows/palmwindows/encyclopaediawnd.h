#pragma once
#include "checklist.h"
#include <map>
#include <ui/ui.h>

class ContextModelWnd;

namespace m3d
{
    namespace ui
    {
        class ComboBoxWnd;
        class TextBoxWnd;
    }
}

class SubjectList;

namespace ai
{
    class Vehicle;
}

class EncyclopaediaWnd :  public m3d::ui::Wnd
{
public:
    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        CStr m_subjectListName;
        CStr m_wndDizName;
        CStr m_wndModelName;
        CStr m_comboThemesName;
    };

    enum Theme
    {
        THEME_VEHICLES = 0x0,
        THEME_CB = 0x1,
        THEME_WEAPON = 0x2,
        THEME_GADGETS = 0x3,
        THEME_AFFIXES = 0x4,
        THEME_WARES = 0x5,
        THEME_CLANS = 0x6,
        THEME_QUEST_ITEMS = 0x7,
        THEME_NUM_THEMES = 0x8,
        THEME_INVALID = 0x8,
    };

public:
    virtual m3d::Class * GetClass() const ;
    std::vector<int> const * GetClans() const ;
    int AddClan(int);
    bool IsClanVisible(int) const ;
    void ShowAll();
    std::vector<int> const * GetPrototypesForTheme(Theme) const ;
    int AddPrototype(int);
    virtual ~EncyclopaediaWnd();
    static m3d::Class * GetBaseClass();
    static m3d::Object * CreateObject();
    bool IsPrototypeVisible(int) const ;
    virtual m3d::Object * Clone();

protected:
    m3d::rend::TexHandle GetImageForClan(CStr const &) const ;
    int AddVehicle(ai::Vehicle const *);
    void CheckAndAddTargetInfoVehicle();
    void OnObjMet(void *);
    int GetRelatedVisiblePrototypeId(int) const ;
    int AddPrototypeByThemeUnsafe(int,enum Theme);
    void CacheRelatedPrototypes();
    void OnNewFrameForce();
    virtual int OnWndNotify(m3d::ui::Wnd *,unsigned int,unsigned int,m3d::AIParam const &);
    EncyclopaediaWnd();
    EncyclopaediaWnd(EncyclopaediaWnd const &);
    void OnStartLevel(void *);
    int InitThemes();
    int UpdateModel();
    virtual int GameDataLoad(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *);
    void OnPrototypeMet(void *);
    int AddObj(int);
    CStr Theme2Str(Theme) const ;
    virtual int GameDataClear(bool);
    void OnSubjectChanged();
    int InitOnce();
    virtual int GameDataUpdate(void *,int);
    void OnPlayerVehicleDamaged(void *);
    void CheckAndAddTargetCapturingVehicle();
    Theme Str2Theme(CStr const &) const ;
    void CheckAndAddTargetCapturedVehicle();
    int UpdateDiz();
    void InitAlwaysVisiblePrototypes();
    void OnQuestItemsChanged();
    void OnBelongMet(void *);
    virtual int OnAfterAddToWndStation();
    virtual int GameDataSetup();
    void OnGadgetChanged(void *);
    m3d::rend::TexHandle GetImageForAffix(CStr const &) const ;
    void GetPrototypeIdsKindOfResource(int,std::vector<int> &) const ;
    Theme GetThemeByPrototypeId(int) const ;
    Theme GetCurTheme() const ;
    CStr GetCurSubjectName() const ;
    virtual int GameDataSave(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *);
    Theme GetThemeByResourceId(int) const ;
    void OnThemeChanged();
    void OnPlayerVehicleChanged();

public:
    RT_CLASS_DECLARE(EncyclopaediaWnd);

private:
    SubjectList *m_subjectList;
    m3d::ui::TextBoxWnd *m_wndDiz;
    m3d::ui::ComboBoxWnd *m_comboThemes;
    ContextModelWnd *m_wndModel;
    EncyclopaediaWnd::AuxInfo m_aif;
    std::map<EncyclopaediaWnd::Theme,std::vector<int>> m_prototypesByTheme;
    std::set<int> m_visiblePrototypeIds;
    std::map<int,int> m_relatedPrototypeIds;
    bool m_bInitedOnce;
    std::vector<int> m_clanBelongsVector;
    std::set<int> m_clanBelongsSet;
    int m_prevInfoObjId;
    int m_prevCapturingObjId;
    int m_prevCapturedObjId;
};

class SubjectButton :  public CheckButton
{
public:
    int SetUp(CStr const &,PointBase<float> const &,float,EncyclopaediaWnd::Theme,int);
    static m3d::Class * GetBaseClass();
    static m3d::Object * CreateObject();
    virtual m3d::Class * GetClass() const ;
    virtual ~SubjectButton();
    virtual m3d::Object * Clone();

protected:
    virtual int SetUp(CStr const &,PointBase<float> const &,float,CStr const &);
    SubjectButton();
    SubjectButton(SubjectButton const &);
    virtual CStr CalcFullName() const ;

public:
    RT_CLASS_DECLARE(SubjectButton);

private:
    EncyclopaediaWnd::Theme m_theme;
    int m_id;
};

class SubjectList :  public CheckList
{
public:
    static m3d::Object * CreateObject();
    int Update();
    virtual m3d::Class * GetClass() const ;
    static m3d::Class * GetBaseClass();
    virtual m3d::Object * Clone();
    CStr GetCurSubjectName() const ;
    virtual ~SubjectList();
    int SetUpForTheme(EncyclopaediaWnd::Theme);

protected:
    int CreateItemsForClans();
    virtual int AddButtonByNameAndId(CStr const &,int);
    virtual int OnBeforeAddToWndStation();
    int CreateItemsForPrototypes(EncyclopaediaWnd::Theme);
    int CreateItemsForAffixes();
    virtual int AddButtonByName(CStr const &,CStr const &);
    SubjectList();
    SubjectList(SubjectList const &);

public:
    RT_CLASS_DECLARE(SubjectList);

private:
    EncyclopaediaWnd::Theme m_theme;
};
