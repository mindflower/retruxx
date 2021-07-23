#pragma once

class SaveButton :  public m3d::ui::ButtonWnd
{
public:
    int SetupForSave(class CStr const &);
    virtual class m3d::Object * Clone();
    int SetupForNewSave(class CStr const &,class CStr const &,class m3d::AIParam const &);
    void Select(enum SelectType);
    static struct m3d::Class * __fastcall GetBaseClass();
    class CStr const & GetLevelName() const ;
    static class m3d::Object * __fastcall CreateObject();
    virtual ~SaveButton();
    class CStr const & GetSaveFolderName() const ;
    class CStr const & GetSaveName() const ;
    virtual struct m3d::Class * GetClass() const ;
    class m3d::AIParam const & GetGameTime() const ;
    struct _FILETIME const & GetSaveModifyTime() const ;
protected:
    static void __fastcall ClearPattern();
    SaveButton(class SaveButton const &);
    SaveButton();
    class CStr LocalTime2Str(struct _FILETIME) const ;
    int CreateFromPattern();
    static int __fastcall LoadPattern(class m3d::ui::Wnd *);
    int CreateChildren();
private:
    m3d::ui::Wnd *m_wndSaveName;
    m3d::ui::Wnd *m_wndTime;
    CStr m_saveFolderName;
    CStr m_saveName;
    _FILETIME m_saveModifyLocalTime;
    m3d::AIParam m_gameTime;
    CStr m_levelName;
};

class LSWnd :  public m3d::ui::ModalWnd
{
public:
    virtual struct m3d::Class * GetClass() const ;
    static class m3d::Object * __fastcall CreateObject();
    virtual class m3d::Object * Clone();
    virtual ~LSWnd();
    static struct m3d::Class * __fastcall GetBaseClass();
protected:
    virtual void OnCurProfileChanged();
    virtual void OnEnter();
    virtual int OnKey(unsigned short,unsigned char,unsigned int);
    virtual int GameDataSetup();
    virtual int GameDataUpdate(void *,int);
    LSWnd();
    LSWnd(class LSWnd const &);
    virtual void OnCurProfileParamChanged(void *);
    virtual int OnWndNotify(class m3d::ui::Wnd *,unsigned int,unsigned int,class m3d::AIParam const &);
    void UpdateSortButtonStates();
    void UpdateSortArrowsState();
    void OnScreenshotRelease(void *);
    virtual int SetScreenshotForSave(int);
    virtual void OnSaveSelectionChange();
    void OnSortByArg(enum SaveList::SortArg);
    virtual void ClearControls();
    void LaunchGameMenu();
private:
    LSWnd::AuxInfo m_aif;
    m3d::ui::ImageWnd *m_wndScreenshot;
    m3d::ui::Wnd *m_wndInfo;
    SaveList *m_wndSaveList;
    m3d::ui::ButtonWnd *m_btnSortByName;
    m3d::ui::ButtonWnd *m_btnSortByTime;
    m3d::ui::ImageWnd *m_wndSortByNameArrow;
    m3d::ui::ImageWnd *m_wndSortByTimeArrow;
};

class SaveList :  public m3d::ui::ListBoxWnd<SaveButton *>
{
public:
    virtual ~SaveList();
    virtual class m3d::Object * Clone();
    static struct m3d::Class * __fastcall GetBaseClass();
    enum SortDir GetCurSortDir() const ;
    static class m3d::Object * __fastcall CreateObject();
    int SortSaves(enum SortArg,enum SortDir);
    int CreateFromPattern(class m3d::ui::Wnd *,bool);
    virtual void SetCurSel(int);
    enum ListType GetListType() const ;
    enum SortArg GetCurSortArg() const ;
    virtual struct m3d::Class * GetClass() const ;
    int FullUpdate();
    void SetListType(enum ListType);
protected:
    int GetSaveFoldersList(class std::vector<class CStr,class std::allocator<class CStr> > &) const ;
    SaveList();
    SaveList(class SaveList const &);
    virtual int CompareItem(int,int);
    virtual int OnBeforeAddToWndStation();
    int AddButton(class SaveButton *);
    virtual int RenderItem(int,class PointBase<float> const &,struct m3d::ui::DrawInfo const &);
    virtual int MeasureItem(int,struct BoundsBase<float> &) const ;
    int CreateItems();
    void SortSaves0(enum SortArg,enum SortDir,class std::vector<class SaveButton *,class std::allocator<class SaveButton *> > &);
    void Clear();
    virtual int DeleteItem(int);
private:
    SaveList::SortArg m_curSortArg;
    SaveList::SortDir m_curSortDir;
    SaveList::ListType m_listType;
};
