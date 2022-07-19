#pragma once
#include <core/aiparam.h>
#include <ui/button.h>
#include <ui/uilistboxwnd.h>
#include <ui/wnd.h>

namespace m3d
{
    namespace ui
    {
        class ImageWnd;
    }
}

class SaveButton :  public m3d::ui::ButtonWnd
{
    friend class SaveList;
public:
    class AuxInfo
    {
    public:
        AuxInfo();

    public:
        CStr m_wndPatternName = "SaveButton";
        CStr m_wndSaveNameName = "wndSaveName";
        CStr m_wndTimeName = "wndTime";
        m3d::ui::ButtonWnd* m_wndPattern = nullptr;
        m3d::ui::Wnd* m_wndPatternSaveName = nullptr;
        m3d::ui::Wnd* m_wndPatternTime = nullptr;
        unsigned int m_selectColor = 0x80FF0000;
    };

public:
    int SetupForSave(CStr const &);
    virtual m3d::Object * Clone();
    int SetupForNewSave(CStr const &,CStr const &, m3d::AIParam const &);
    void Select(enum SelectType);
    static m3d::Class * GetBaseClass();
    CStr const & GetLevelName() const ;
    static m3d::Object * CreateObject();
    virtual ~SaveButton();
    CStr const & GetSaveFolderName() const ;
    CStr const & GetSaveName() const ;
    virtual m3d::Class * GetClass() const ;
    m3d::AIParam const & GetGameTime() const ;
    _FILETIME const & GetSaveModifyTime() const ;

protected:
    static void __fastcall ClearPattern();
    SaveButton(SaveButton const &);
    SaveButton();
    CStr LocalTime2Str(_FILETIME) const ;
    int CreateFromPattern();
    static int __fastcall LoadPattern(m3d::ui::Wnd *);
    int CreateChildren();

public:
    RT_CLASS_DECLARE(SaveButton);

protected:
    static AuxInfo m_aif;

private:
    m3d::ui::Wnd *m_wndSaveName;
    m3d::ui::Wnd *m_wndTime;
    CStr m_saveFolderName;
    CStr m_saveName;
    _FILETIME m_saveModifyLocalTime;
    m3d::AIParam m_gameTime;
    CStr m_levelName;
};

class SaveList : public m3d::ui::ListBoxWnd<SaveButton*>
{
public:
    enum SortArg
    {
        ARG_NAME = 0x0,
        ARG_TIME = 0x1,
    };

    enum SortDir
    {
        DIR_INCREASE = 0x0,
        DIR_DECREASE = 0x1,
    };

    enum ListType
    {
        TYPE_SAVE = 0x0,
        TYPE_LOAD = 0x1,
    };

public:
    virtual ~SaveList();
    virtual m3d::Object* Clone();
    static m3d::Class* GetBaseClass();
    SortDir GetCurSortDir() const;
    static m3d::Object* CreateObject();
    int SortSaves(SortArg, SortDir);
    int CreateFromPattern(m3d::ui::Wnd*, bool);
    virtual void SetCurSel(int);
    ListType GetListType() const;
    SortArg GetCurSortArg() const;
    virtual m3d::Class* GetClass() const;
    int FullUpdate();
    void SetListType(ListType);

protected:
    int GetSaveFoldersList(std::vector<CStr, std::allocator<CStr> >&) const;
    SaveList();
    SaveList(SaveList const&);
    virtual int CompareItem(int, int);
    virtual int OnBeforeAddToWndStation();
    int AddButton(SaveButton*);
    virtual int RenderItem(int, PointBase<float> const&, m3d::ui::DrawInfo const&);
    virtual int MeasureItem(int, BoundsBase<float>&) const;
    int CreateItems();
    void SortSaves0(SortArg, SortDir, std::vector<SaveButton*, std::allocator<SaveButton*> >&);
    void Clear();
    virtual int DeleteItem(int);

public:
    RT_CLASS_DECLARE(SaveList);

private:
    SaveList::SortArg m_curSortArg;
    SaveList::SortDir m_curSortDir;
    SaveList::ListType m_listType;
};


class LSWnd :  public m3d::ui::ModalWnd
{
public:
    class AuxInfo
    {
    public:
        AuxInfo();

    public:
        CStr m_wndSaveListName = "wndSaveList";
        CStr m_wndScreenshotName = "wndScreenshot";
        CStr m_wndInfoName = "wndInfo";
        CStr m_btnSortByNameName = "btnSortName";
        CStr m_btnSortByTimeName = "btnSortTime";
        CStr m_wndSortByNameArrowName = "wndSortNameArrow";
        CStr m_wndSortByTimeArrowName = "wndSortTimeArrow";
        CStr m_texIdSortArrow = "SortArrow";
    };

public:
    virtual m3d::Class * GetClass() const ;
    static m3d::Object * CreateObject();
    virtual m3d::Object * Clone();
    virtual ~LSWnd();
    static m3d::Class * GetBaseClass();

protected:
    virtual void OnCurProfileChanged();
    virtual void OnEnter();
    virtual int OnKey(unsigned short,unsigned char,unsigned int);
    virtual int GameDataSetup();
    virtual int GameDataUpdate(void *,int);
    LSWnd();
    LSWnd(LSWnd const &);
    virtual void OnCurProfileParamChanged(void *);
    virtual int OnWndNotify(m3d::ui::Wnd *,unsigned int,unsigned int, m3d::AIParam const &);
    void UpdateSortButtonStates();
    void UpdateSortArrowsState();
    void OnScreenshotRelease(void *);
    virtual int SetScreenshotForSave(int);
    virtual void OnSaveSelectionChange();
    void OnSortByArg(SaveList::SortArg);
    virtual void ClearControls();
    void LaunchGameMenu();

public:
    RT_CLASS_DECLARE(LSWnd);

private:
    LSWnd::AuxInfo m_aif;
    m3d::ui::ImageWnd *m_wndScreenshot = nullptr;
    m3d::ui::Wnd *m_wndInfo = nullptr;
    SaveList *m_wndSaveList = nullptr;
    m3d::ui::ButtonWnd *m_btnSortByName = nullptr;
    m3d::ui::ButtonWnd *m_btnSortByTime = nullptr;
    m3d::ui::ImageWnd *m_wndSortByNameArrow = nullptr;
    m3d::ui::ImageWnd *m_wndSortByTimeArrow = nullptr;
};
