#pragma once

class EncyclopaediaWnd :  public m3d::ui::Wnd
{
public:
    virtual struct m3d::Class * GetClass() const ;
    class std::vector<int,class std::allocator<int> > const * GetClans() const ;
    int AddClan(int);
    bool IsClanVisible(int) const ;
    void ShowAll();
    class std::vector<int,class std::allocator<int> > const * GetPrototypesForTheme(enum Theme) const ;
    int AddPrototype(int);
    virtual ~EncyclopaediaWnd();
    static struct m3d::Class * __fastcall GetBaseClass();
    static class m3d::Object * __fastcall CreateObject();
    bool IsPrototypeVisible(int) const ;
    virtual class m3d::Object * Clone();
protected:
    class m3d::rend::TexHandle GetImageForClan(CStr const &) const ;
    int AddVehicle(class ai::Vehicle const *);
    void CheckAndAddTargetInfoVehicle();
    void OnObjMet(void *);
    int GetRelatedVisiblePrototypeId(int) const ;
    int AddPrototypeByThemeUnsafe(int,enum Theme);
    void CacheRelatedPrototypes();
    void OnNewFrameForce();
    virtual int OnWndNotify(class m3d::ui::Wnd *,unsigned int,unsigned int,class m3d::AIParam const &);
    EncyclopaediaWnd();
    EncyclopaediaWnd(class EncyclopaediaWnd const &);
    void OnStartLevel(void *);
    int InitThemes();
    int UpdateModel();
    virtual int GameDataLoad(struct m3d::cmn::XmlFile *,struct m3d::cmn::XmlNode *);
    void OnPrototypeMet(void *);
    int AddObj(int);
    CStr Theme2Str(enum Theme) const ;
    virtual int GameDataClear(bool);
    void OnSubjectChanged();
    int InitOnce();
    virtual int GameDataUpdate(void *,int);
    void OnPlayerVehicleDamaged(void *);
    void CheckAndAddTargetCapturingVehicle();
    enum Theme Str2Theme(CStr const &) const ;
    void CheckAndAddTargetCapturedVehicle();
    int UpdateDiz();
    void InitAlwaysVisiblePrototypes();
    void OnQuestItemsChanged();
    void OnBelongMet(void *);
    virtual int OnAfterAddToWndStation();
    virtual int GameDataSetup();
    void OnGadgetChanged(void *);
    class m3d::rend::TexHandle GetImageForAffix(CStr const &) const ;
    void GetPrototypeIdsKindOfResource(int,class std::vector<int,class std::allocator<int> > &) const ;
    enum Theme GetThemeByPrototypeId(int) const ;
    enum Theme GetCurTheme() const ;
    CStr GetCurSubjectName() const ;
    virtual int GameDataSave(struct m3d::cmn::XmlFile *,struct m3d::cmn::XmlNode *);
    enum Theme GetThemeByResourceId(int) const ;
    void OnThemeChanged();
    void OnPlayerVehicleChanged();
private:
    SubjectList *m_subjectList;
    m3d::ui::TextBoxWnd *m_wndDiz;
    m3d::ui::ComboBoxWnd *m_comboThemes;
    ContextModelWnd *m_wndModel;
    EncyclopaediaWnd::AuxInfo m_aif;
    std::map<enum EncyclopaediaWnd::Theme,std::vector<int>> m_prototypesByTheme;
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
    int SetUp(CStr const &,class PointBase<float> const &,float,enum EncyclopaediaWnd::Theme,int);
    static struct m3d::Class * __fastcall GetBaseClass();
    static class m3d::Object * __fastcall CreateObject();
    virtual struct m3d::Class * GetClass() const ;
    virtual ~SubjectButton();
    virtual class m3d::Object * Clone();
protected:
    virtual int SetUp(CStr const &,class PointBase<float> const &,float,CStr const &);
    SubjectButton();
    SubjectButton(class SubjectButton const &);
    virtual CStr CalcFullName() const ;
private:
    EncyclopaediaWnd::Theme m_theme;
    int m_id;
};

class SubjectList :  public CheckList
{
public:
    static class m3d::Object * __fastcall CreateObject();
    int Update();
    virtual struct m3d::Class * GetClass() const ;
    static struct m3d::Class * __fastcall GetBaseClass();
    virtual class m3d::Object * Clone();
    CStr GetCurSubjectName() const ;
    virtual ~SubjectList();
    int SetUpForTheme(enum EncyclopaediaWnd::Theme);
protected:
    int CreateItemsForClans();
    virtual int AddButtonByNameAndId(CStr const &,int);
    virtual int OnBeforeAddToWndStation();
    int CreateItemsForPrototypes(enum EncyclopaediaWnd::Theme);
    int CreateItemsForAffixes();
    virtual int AddButtonByName(CStr const &,CStr const &);
    SubjectList();
    SubjectList(class SubjectList const &);
private:
    EncyclopaediaWnd::Theme m_theme;
};
