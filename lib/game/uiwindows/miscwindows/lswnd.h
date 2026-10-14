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
}  // namespace m3d

class SaveButton : public m3d::ui::ButtonWnd
{
    // The list owns its buttons and paints their two labels directly.
    friend class SaveList;

public:
    // NOTE: the shipped declaration keeps SelectType private even though the
    // public Select() takes one and SaveList calls it from outside; it is public
    // here so the call compiles. Layout is unaffected.
    enum SelectType
    {
        UNSELECTED = 0,
        SELECTED = 1,
        MARKED = 2,
    };

public:
    int SetupForSave(CStr const& saveFolderName);
    int SetupForNewSave(CStr const& saveName, CStr const& levelName, m3d::AIParam const& gameTime);
    CStr const& GetSaveFolderName() const;
    m3d::AIParam const& GetGameTime() const;
    CStr const& GetLevelName() const;
    void Select(SaveButton::SelectType selectType);
    CStr const& GetSaveName() const;
    _FILETIME const& GetSaveModifyTime() const;

    struct AuxInfo
    {
        CStr m_wndPatternName;
        CStr m_wndSaveNameName;
        CStr m_wndTimeName;
        m3d::ui::ButtonWnd* m_wndPattern;
        m3d::ui::Wnd* m_wndPatternSaveName;
        m3d::ui::Wnd* m_wndPatternTime;
        unsigned int m_selectColor;
        AuxInfo(SaveButton::AuxInfo const&);
        AuxInfo();
    };

protected:
    static int __fastcall LoadPattern(m3d::ui::Wnd* pattern);
    static void __fastcall ClearPattern();
    int CreateFromPattern();
    int CreateChildren();
    CStr LocalTime2Str(_FILETIME localTime) const;

    m3d::ui::Wnd* m_wndSaveName;
    m3d::ui::Wnd* m_wndTime;
    static SaveButton::AuxInfo m_aif;
    CStr m_saveFolderName;
    CStr m_saveName;
    _FILETIME m_saveModifyLocalTime;
    m3d::AIParam m_gameTime;
    CStr m_levelName;

    SaveButton();
    SaveButton(SaveButton const& rhs);

public:
    virtual ~SaveButton() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(SaveButton);
};

class SaveList : public m3d::ui::ListBoxWnd<SaveButton*>
{
public:
    // NOTE: private in the shipped declaration; public here because LSWnd and
    // SaveWnd name these types. Layout is unaffected.
    enum ListType
    {
        TYPE_SAVE = 0,
        TYPE_LOAD = 1,
    };

    enum SortDir
    {
        DIR_INCREASE = 0,
        DIR_DECREASE = 1,
    };

    enum SortArg
    {
        ARG_NAME = 0,
        ARG_TIME = 1,
    };

public:
    int CreateFromPattern(m3d::ui::Wnd* patternWnd, bool deleteSrc);
    int FullUpdate();
    int SortSaves(SaveList::SortArg sortArg, SaveList::SortDir sortDir);
    SaveList::SortArg GetCurSortArg() const;
    SaveList::SortDir GetCurSortDir() const;
    SaveList::ListType GetListType() const;
    void SetListType(SaveList::ListType type);
    virtual void SetCurSel(int idx) override;
    using SaveButtonVector = retruxx::vector<SaveButton*>;

protected:
    virtual int MeasureItem(int itemIdx, BoundsBase<float>& bounds) const override;
    virtual int RenderItem(int itemIdx, PointBase<float> const& org, m3d::ui::DrawInfo const& di) override;
    virtual int DeleteItem(int itemIdx) override;
    virtual int CompareItem(int itemIdx0, int itemIdx1) override;
    virtual int OnBeforeAddToWndStation() override;
    void Clear();
    int CreateItems();
    int AddButton(SaveButton* btn);
    int GetSaveFoldersList(retruxx::vector<CStr>& saveFoldersList) const;
    void SortSaves0(SaveList::SortArg sortArg, SaveList::SortDir sortDir, retruxx::vector<SaveButton*>& btns);

    SaveList::SortArg m_curSortArg;
    SaveList::SortDir m_curSortDir;
    SaveList::ListType m_listType;

    SaveList();
    SaveList(SaveList const& rhs);

public:
    virtual ~SaveList() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(SaveList);
};

class LSWnd : public m3d::ui::ModalWnd
{
public:
    struct AuxInfo
    {
        CStr m_wndSaveListName;
        CStr m_wndScreenshotName;
        CStr m_wndInfoName;
        CStr m_btnSortByNameName;
        CStr m_btnSortByTimeName;
        CStr m_wndSortByNameArrowName;
        CStr m_wndSortByTimeArrowName;
        CStr m_texIdSortArrow;
        AuxInfo(LSWnd::AuxInfo const&);
        AuxInfo();
    };

protected:
    virtual int GameDataSetup() override;
    virtual int GameDataUpdate(void* data, int dataType) override;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data) override;
    virtual int OnKey(unsigned short key, unsigned char scanCode, unsigned int state) override;
    void OnSortByArg(SaveList::SortArg arg);
    virtual void OnSaveSelectionChange();
    virtual void OnCurProfileChanged();
    virtual void OnCurProfileParamChanged(void* data);
    virtual void ClearControls();
    virtual int SetScreenshotForSave(int saveIdx);
    void OnScreenshotRelease(void* data);
    void UpdateSortButtonStates();
    virtual void OnEnter();
    void LaunchGameMenu();
    void UpdateSortArrowsState();

    LSWnd::AuxInfo m_aif;
    m3d::ui::ImageWnd* m_wndScreenshot;
    m3d::ui::Wnd* m_wndInfo;
    SaveList* m_wndSaveList;
    m3d::ui::ButtonWnd* m_btnSortByName;
    m3d::ui::ButtonWnd* m_btnSortByTime;
    m3d::ui::ImageWnd* m_wndSortByNameArrow;
    m3d::ui::ImageWnd* m_wndSortByTimeArrow;

    LSWnd();
    LSWnd(LSWnd const& rhs);

public:
    virtual ~LSWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(LSWnd);
};
