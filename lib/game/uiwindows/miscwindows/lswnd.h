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
        /* 0x0000 */ CStr m_wndPatternName;
        /* 0x000c */ CStr m_wndSaveNameName;
        /* 0x0018 */ CStr m_wndTimeName;
        /* 0x0024 */ m3d::ui::ButtonWnd* m_wndPattern;
        /* 0x0028 */ m3d::ui::Wnd* m_wndPatternSaveName;
        /* 0x002c */ m3d::ui::Wnd* m_wndPatternTime;
        /* 0x0030 */ unsigned int m_selectColor;
        AuxInfo(SaveButton::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x0034 */

protected:
    static int __fastcall LoadPattern(m3d::ui::Wnd* pattern);
    static void __fastcall ClearPattern();
    int CreateFromPattern();
    int CreateChildren();
    CStr LocalTime2Str(_FILETIME localTime) const;

    /* 0x023c */ m3d::ui::Wnd* m_wndSaveName;
    /* 0x0240 */ m3d::ui::Wnd* m_wndTime;
    static SaveButton::AuxInfo m_aif;
    /* 0x0244 */ CStr m_saveFolderName;
    /* 0x0250 */ CStr m_saveName;
    /* 0x025c */ _FILETIME m_saveModifyLocalTime;
    /* 0x0264 */ m3d::AIParam m_gameTime;
    /* 0x0280 */ CStr m_levelName;

    SaveButton();
    SaveButton(SaveButton const& rhs);

public:
    virtual ~SaveButton() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x00 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x00 */;
    RT_CLASS_DECLARE(SaveButton);
}; /* size: 0x028c */

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
    virtual void SetCurSel(int idx) override /* 0x11c */;
    using SaveButtonVector = retruxx::vector<SaveButton*>;

protected:
    virtual int MeasureItem(int itemIdx, BoundsBase<float>& bounds) const override /* 0x134 */;
    virtual int RenderItem(int itemIdx, PointBase<float> const& org, m3d::ui::DrawInfo const& di) override /* 0x138 */;
    virtual int DeleteItem(int itemIdx) override /* 0x13c */;
    virtual int CompareItem(int itemIdx0, int itemIdx1) override /* 0x140 */;
    virtual int OnBeforeAddToWndStation() override /* 0x00 */;
    void Clear();
    int CreateItems();
    int AddButton(SaveButton* btn);
    int GetSaveFoldersList(retruxx::vector<CStr>& saveFoldersList) const;
    void SortSaves0(SaveList::SortArg sortArg, SaveList::SortDir sortDir, retruxx::vector<SaveButton*>& btns);

    /* 0x0238 */ SaveList::SortArg m_curSortArg;
    /* 0x023c */ SaveList::SortDir m_curSortDir;
    /* 0x0240 */ SaveList::ListType m_listType;

    SaveList();
    SaveList(SaveList const& rhs);

public:
    virtual ~SaveList() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x00 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x00 */;
    RT_CLASS_DECLARE(SaveList);
}; /* size: 0x0244 */

class LSWnd : public m3d::ui::ModalWnd
{
public:
    struct AuxInfo
    {
        /* 0x0000 */ CStr m_wndSaveListName;
        /* 0x000c */ CStr m_wndScreenshotName;
        /* 0x0018 */ CStr m_wndInfoName;
        /* 0x0024 */ CStr m_btnSortByNameName;
        /* 0x0030 */ CStr m_btnSortByTimeName;
        /* 0x003c */ CStr m_wndSortByNameArrowName;
        /* 0x0048 */ CStr m_wndSortByTimeArrowName;
        /* 0x0054 */ CStr m_texIdSortArrow;
        AuxInfo(LSWnd::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x0060 */

protected:
    virtual int GameDataSetup() override /* 0x00 */;
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x00 */;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data) override /* 0x00 */;
    virtual int OnKey(unsigned short key, unsigned char scanCode, unsigned int state) override /* 0x00 */;
    void OnSortByArg(SaveList::SortArg arg);
    virtual void OnSaveSelectionChange() /* 0x130 */;
    virtual void OnCurProfileChanged() /* 0x134 */;
    virtual void OnCurProfileParamChanged(void* data) /* 0x138 */;
    virtual void ClearControls() /* 0x13c */;
    virtual int SetScreenshotForSave(int saveIdx) /* 0x140 */;
    void OnScreenshotRelease(void* data);
    void UpdateSortButtonStates();
    virtual void OnEnter() /* 0x144 */;
    void LaunchGameMenu();
    void UpdateSortArrowsState();

    /* 0x0224 */ LSWnd::AuxInfo m_aif;
    /* 0x0284 */ m3d::ui::ImageWnd* m_wndScreenshot;
    /* 0x0288 */ m3d::ui::Wnd* m_wndInfo;
    /* 0x028c */ SaveList* m_wndSaveList;
    /* 0x0290 */ m3d::ui::ButtonWnd* m_btnSortByName;
    /* 0x0294 */ m3d::ui::ButtonWnd* m_btnSortByTime;
    /* 0x0298 */ m3d::ui::ImageWnd* m_wndSortByNameArrow;
    /* 0x029c */ m3d::ui::ImageWnd* m_wndSortByTimeArrow;

    LSWnd();
    LSWnd(LSWnd const& rhs);

public:
    virtual ~LSWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x00 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x00 */;
    RT_CLASS_DECLARE(LSWnd);
}; /* size: 0x02a0 */
