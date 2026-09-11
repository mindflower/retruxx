#include "lswnd.h"

#include <windows.h>

#include <algorithm>

#include <core/kernel.h>
#include <core/log.h>
#include <game/m3dgame.h>
#include <game/profile.h>
#include <game/uimanager/truxxuimanager.h>
#include <game/uimisc/guihelper.h>
#include <game/uimisc/levelinfo.h>
#include <game/uimisc/savesmanager.h>
#include <m3dapp.h>
#include <server/objects/base/objcontainer.h>
#include <ui/image.h>
#include <ui/listbox.h>
#include <ui/wndstation.h>

namespace
{
    // Control ids the two windows and the list itself notify under.
    unsigned const ID_BTN_SORT_BY_NAME = 400000;
    unsigned const ID_BTN_SORT_BY_TIME = 400001;
    unsigned const ID_SAVE_LIST = 400002;
    unsigned const ID_BTN_CANCEL = 400003;

    // Copies every layout / text / style / pane / animation property from a
    // pattern window onto another window; the shipped CREATE_WND macro inlines
    // this block once per widget it builds.
    void CopyWndPropsFromPattern(m3d::ui::Wnd* dst, m3d::ui::Wnd const* pat)
    {
        dst->SetStyle(pat->GetStyle());
        dst->SetText(pat->GetText());
        dst->SetId(pat->GetId());
        dst->SetName(pat->GetName());
        dst->SetBounds(pat->GetBounds(), true);
        dst->SetDefaultFont(pat->GetDefaultFont());
        dst->SetWrapMode(pat->GetWrapMode());
        dst->SetFormatMode(pat->GetFormatMode());
        dst->SetColor(pat->GetColor());
        dst->SetTextColor(pat->GetTextColor());
        dst->SetTextColorDisabled(pat->GetTextColorDisabled());
        dst->SetClientEdges(pat->GetClientEdges());
        dst->SetPane(pat->GetPaneName());
        dst->SetPaneFlags(pat->GetPaneFlags());
        dst->SetScrollPane(pat->GetScrollPaneName());
        dst->SetBackground(pat->GetBackground());

        CStr tooltip;
        pat->GetProperty(PROP_WND_TOOLTIP, &tooltip);
        dst->SetProperty(PROP_WND_TOOLTIP, &tooltip);

        dst->SetOnShowAnimation(pat->GetOnShowAnimation());
        dst->SetOnHideAnimation(pat->GetOnHideAnimation());
    }

    // The shipped CREATE_WND macro: instantiate a plain Wnd, create it from the
    // pattern's style / bounds / id, parent it and copy the visuals across.
    m3d::ui::Wnd* CreateWndFromPattern(m3d::ui::Wnd* parent, m3d::ui::Wnd const* pattern)
    {
        if (!pattern)
        {
            M3D_LOG_INFO("CREATE error - NULL pattern");
            return nullptr;
        }

        auto* wnd = RT_DYNCAST(m3d::g_Kernel->New("Wnd"), m3d::ui::Wnd);
        if (!wnd)
        {
            M3D_LOG_INFO("CREATE error - cannot instantiate object");
            return nullptr;
        }

        if (!wnd->Create(CStr(), pattern->GetStyle(), pattern->GetBounds(), pattern->GetId()))
        {
            delete wnd;
            M3D_LOG_INFO("CREATE error - cannot create window");
            return nullptr;
        }

        parent->AddChild(wnd);
        parent->MoveChildToFirstPosition(wnd);
        CopyWndPropsFromPattern(wnd, pattern);
        return wnd;
    }

    // Sort predicates. Names compare case-insensitively through CStr's own
    // null-tolerant helper, times through the Win32 file-time comparison.
    bool LessByName(SaveButton* btn1, SaveButton* btn2)
    {
        return CStr::my_stricmp(btn1->GetSaveName().c_str(), btn2->GetSaveName().c_str()) < 0;
    }

    bool GreaterByName(SaveButton* btn1, SaveButton* btn2)
    {
        return CStr::my_stricmp(btn1->GetSaveName().c_str(), btn2->GetSaveName().c_str()) > 0;
    }

    // NOTE: both time predicates are non-strict (<= / >=), so neither is a valid
    // strict weak ordering; this is how the game shipped.
    bool LessByTime(SaveButton* btn1, SaveButton* btn2)
    {
        return CompareFileTime(&btn1->GetSaveModifyTime(), &btn2->GetSaveModifyTime()) <= 0;
    }

    bool GreaterByTime(SaveButton* btn1, SaveButton* btn2)
    {
        return CompareFileTime(&btn1->GetSaveModifyTime(), &btn2->GetSaveModifyTime()) >= 0;
    }
}  // namespace

// ===========================================================================
//  SaveButton
// ===========================================================================

RT_CLASS_EXPORTS_BEGIN(SaveButton)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(SaveButton);

SaveButton::AuxInfo::AuxInfo()
{
    // RVA 0x4BBDE0
    m_wndPatternName = "SaveButton";
    m_wndSaveNameName = "wndSaveName";
    m_wndTimeName = "wndTime";
    m_wndPattern = nullptr;
    m_wndPatternSaveName = nullptr;
    m_wndPatternTime = nullptr;
    m_selectColor = 0x80FF0000;
}

SaveButton::AuxInfo SaveButton::m_aif;

SaveButton::SaveButton()
{
    // RVA 0x4BBEC0
    m_wndSaveName = nullptr;
    m_wndTime = nullptr;
    m_saveModifyLocalTime.dwLowDateTime = 0;
    m_saveModifyLocalTime.dwHighDateTime = 0;
    m_gameTime = m3d::AIParam(0);
}

SaveButton::SaveButton(SaveButton const&)
{
    // RVA 0x4BBFB0 - the shipped copy constructor copies nothing from rhs and,
    // unlike the default one, leaves the two label pointers, the modify time and
    // the game time uninitialised.
}

SaveButton::~SaveButton()
{
    // RVA 0x4BC040 - only the strings, the AIParam and the ButtonWnd base need
    // unwinding; the two labels are ordinary children.
}

m3d::Object* SaveButton::Clone()
{
    // RVA 0x4BA170
    return new SaveButton(*this);
}

m3d::Object* SaveButton::CreateObject()
{
    return new SaveButton;
}

m3d::Class* SaveButton::GetBaseClass()
{
    return RT_CLASS_LOCAL(ButtonWnd);
}

m3d::Class* SaveButton::GetClass() const
{
    return RT_CLASS_LOCAL(SaveButton);
}

CStr const& SaveButton::GetSaveFolderName() const
{
    return m_saveFolderName;
}

CStr const& SaveButton::GetSaveName() const
{
    return m_saveName;
}

CStr const& SaveButton::GetLevelName() const
{
    return m_levelName;
}

m3d::AIParam const& SaveButton::GetGameTime() const
{
    return m_gameTime;
}

_FILETIME const& SaveButton::GetSaveModifyTime() const
{
    return m_saveModifyLocalTime;
}

int SaveButton::LoadPattern(m3d::ui::Wnd* pattern)
{
    // RVA 0x4BC0F0 - the pattern is shared by every button in every list, so it
    // is only harvested once.
    using namespace m3d::ui;
    if (m_aif.m_wndPattern)
    {
        return 1;
    }
    ClearPattern();

    if (!pattern)
    {
        M3D_LOG_INFO("SaveButton::LoadPattern - error to create - invalid pattern wnd");
        return 0;
    }

    auto res = 1;
    auto child = pattern->GetChildByName(m_aif.m_wndPatternName);
    if (child && child->IsKindOf(RT_CLASS_LOCAL(ButtonWnd)))
    {
        m_aif.m_wndPattern = RT_DYNCAST(child, ButtonWnd);
        pattern->RemoveChild(child);

        child = pattern->GetChildByName(m_aif.m_wndSaveNameName);
        if (child && child->IsKindOf(RT_CLASS_LOCAL(Wnd)))
        {
            m_aif.m_wndPatternSaveName = RT_DYNCAST(child, Wnd);
            pattern->RemoveChild(child);

            // The two labels are kept relative to the button they belong to.
            auto patternSaveName = m_aif.m_wndPatternSaveName->GetBounds();
            auto const patternBounds = m_aif.m_wndPattern->GetBounds();
            patternSaveName.y0 = patternSaveName.y0 - patternBounds.y0;
            patternSaveName.x0 = patternSaveName.x0 - patternBounds.x0;
            m_aif.m_wndPatternSaveName->SetBounds(patternSaveName, false);
        }
        else
        {
            M3D_LOG_INFO("GET_CHILD_PATTERN error - cannot find child wnd " + m_aif.m_wndSaveNameName);
            res = 0;
        }

        child = pattern->GetChildByName(m_aif.m_wndTimeName);
        if (child && child->IsKindOf(RT_CLASS_LOCAL(Wnd)))
        {
            m_aif.m_wndPatternTime = RT_DYNCAST(child, Wnd);
            pattern->RemoveChild(child);

            auto patternTime = m_aif.m_wndPatternTime->GetBounds();
            auto const patternBounds = m_aif.m_wndPattern->GetBounds();
            patternTime.y0 = patternTime.y0 - patternBounds.y0;
            patternTime.x0 = patternTime.x0 - patternBounds.x0;
            m_aif.m_wndPatternTime->SetBounds(patternTime, false);
        }
        else
        {
            M3D_LOG_INFO("GET_CHILD_PATTERN error - cannot find child wnd " + m_aif.m_wndTimeName);
            res = 0;
        }

        auto patternBounds = m_aif.m_wndPattern->GetBounds();
        patternBounds.x0 = 0.0;
        patternBounds.y0 = 0.0;
        m_aif.m_wndPattern->SetBounds(patternBounds, false);
    }
    else
    {
        M3D_LOG_INFO("SaveButton::LoadPattern - error to create - invalid pattern wnd");
        res = 0;
    }
    return res;
}

void SaveButton::ClearPattern()
{
    // RVA 0x4BC3D0
    if (m_aif.m_wndPattern)
    {
        delete m_aif.m_wndPattern;
        m_aif.m_wndPattern = nullptr;
    }
    if (m_aif.m_wndPatternSaveName)
    {
        delete m_aif.m_wndPatternSaveName;
        m_aif.m_wndPatternSaveName = nullptr;
    }
    if (m_aif.m_wndPatternTime)
    {
        delete m_aif.m_wndPatternTime;
        m_aif.m_wndPatternTime = nullptr;
    }
}

int SaveButton::CreateFromPattern()
{
    // RVA 0x4BC420
    if (Valid())
    {
        return 0;
    }

    if (!m_aif.m_wndPattern || !M3D_APP->IsWndAlive(m_aif.m_wndPattern, -1) || !m_aif.m_wndPattern->Valid())
    {
        M3D_LOG_INFO("SaveButton::CreateFromPattern error - null pattern");
        return 0;
    }

    if (!m3d::ui::Wnd::Create(
            CStr(), m_aif.m_wndPattern->GetStyle(), m_aif.m_wndPattern->GetBounds(), m_aif.m_wndPattern->GetId()))
    {
        M3D_LOG_INFO("SaveButton::CreateFromPattern error - cannot create window");
        return 0;
    }

    if (m_aif.m_wndPattern->IsImaged())
    {
        // NOTE: the disabled image is deliberately left invalid - the shipped
        // code passes a fresh handle rather than the pattern's own.
        m3d::rend::TexHandle disabled;
        disabled.SetInvalid();
        SetImaged(m_aif.m_wndPattern->GetImageRegular(),
                  m_aif.m_wndPattern->GetImageDown(),
                  m_aif.m_wndPattern->GetImageIn(),
                  disabled);
    }
    else
    {
        SetPane(m_aif.m_wndPattern->GetPaneName());
        SetPaneFlags(m_aif.m_wndPattern->GetPaneFlags());
    }

    SetColor(m_aif.m_wndPattern->GetColor());
    SetTextColor(m_aif.m_wndPattern->GetTextColor());
    SetTextColorDisabled(m_aif.m_wndPattern->GetTextColorDisabled());

    if (!CreateChildren())
    {
        M3D_LOG_INFO("SaveButton::CreateFromPattern error - cannot create children");
        return 0;
    }

    m_gameDataFlags |= 1u;
    return 1;
}

int SaveButton::CreateChildren()
{
    // RVA 0x4BC6C0
    if (!m_aif.m_wndPatternSaveName || !M3D_APP->IsWndAlive(m_aif.m_wndPatternSaveName, -1) ||
        !m_aif.m_wndPatternSaveName->Valid())
    {
        return 0;
    }
    if (!m_aif.m_wndPatternTime || !M3D_APP->IsWndAlive(m_aif.m_wndPatternTime, -1) ||
        !m_aif.m_wndPatternTime->Valid())
    {
        return 0;
    }

    if (m_wndSaveName || m_wndTime)
    {
        M3D_LOG_INFO("SaveButton::CreateChildren error - children already exist");
        return 0;
    }

    auto res = 1;

    m_wndSaveName = CreateWndFromPattern(this, m_aif.m_wndPatternSaveName);
    if (!m_wndSaveName)
    {
        M3D_LOG_INFO("CREATE_WND error creation window");
        res = 0;
    }

    m_wndTime = CreateWndFromPattern(this, m_aif.m_wndPatternTime);
    if (!m_wndTime)
    {
        M3D_LOG_INFO("CREATE_WND error creation window");
        res = 0;
    }

    return res;
}

int SaveButton::SetupForSave(CStr const& saveFolderName)
{
    // RVA 0x4BCFC0 - fills the button in from an existing save on disk.
    if (Valid())
    {
        return 0;
    }
    if (!CreateFromPattern())
    {
        return 0;
    }

    auto const* saveInfo = M3D_APP->m_pInterfaceManager->GetSavesManager()->GetSaveInfoByFolderName(saveFolderName);
    if (!saveInfo)
    {
        return 0;
    }

    m_saveFolderName = saveFolderName;
    m_saveName = saveInfo->m_saveName;
    m_gameTime = saveInfo->m_gameTime;
    m_levelName = saveInfo->m_levelName;
    m_saveModifyLocalTime = saveInfo->m_saveModifyLocalTime;

    m_wndSaveName->SetText(m_saveName);
    m_wndTime->SetText(LocalTime2Str(saveInfo->m_saveModifyLocalTime));

    // A save name that wraps onto a second line makes the whole row taller.
    auto const clientB = m_wndSaveName->GetClientBounds();
    auto const saveNameSz = GetGfxServer()->MeasureText(
        m_saveName, m_wndSaveName->GetDefaultFont(), m_wndSaveName->GetWrapMode(), clientB.width);
    auto const extraHeight = saveNameSz.y - clientB.height;
    if (extraHeight > 0.0)
    {
        auto newB = m_wndSaveName->GetBounds();
        newB.height = newB.height + extraHeight;
        m_wndSaveName->SetBounds(newB, true);
        m_bounds.height = extraHeight + m_bounds.height;
    }
    return 1;
}

int SaveButton::SetupForNewSave(CStr const& saveName, CStr const& levelName, m3d::AIParam const& gameTime)
{
    // RVA 0x4BD170 - the leading "create a new save" row of a SaveWnd list; it
    // has no folder of its own until it is actually written.
    if (Valid())
    {
        return 0;
    }
    if (!CreateFromPattern())
    {
        return 0;
    }

    m_saveName = saveName;
    m_levelName = levelName;
    m_gameTime = gameTime;
    if (m_saveName.empty())
    {
        return 0;
    }

    SYSTEMTIME sysTime;
    GetLocalTime(&sysTime);
    SystemTimeToFileTime(&sysTime, &m_saveModifyLocalTime);

    m_saveFolderName = CStr();
    m_wndSaveName->SetText(m_saveName);
    m_wndTime->SetText(LocalTime2Str(m_saveModifyLocalTime));
    return 1;
}

CStr SaveButton::LocalTime2Str(_FILETIME localTime) const
{
    // RVA 0x4BD290
    SYSTEMTIME sysTime;
    if (!FileTimeToSystemTime(&localTime, &sysTime))
    {
        return CStr();
    }

    CStr strTime;
    strTime.format(
        "%02d:%02d %02d/%02d/%d", sysTime.wHour, sysTime.wMinute, sysTime.wDay, sysTime.wMonth, sysTime.wYear);
    return strTime;
}

void SaveButton::Select(SaveButton::SelectType selectType)
{
    // RVA 0x4BD360
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    unsigned color = 0;
    switch (selectType)
    {
    case UNSELECTED:
        color = m_textColorDisabled;
        break;
    case SELECTED:
        color = m_textColor;
        break;
    case MARKED:
        color = m_aif.m_selectColor;
        break;
    }

    m_wndSaveName->SetTextColor(color);
    m_wndTime->SetTextColor(color);
}

// ===========================================================================
//  SaveList
// ===========================================================================

RT_CLASS_EXPORTS_BEGIN(SaveList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(SaveList);

SaveList::SaveList()
{
    // RVA 0x4BD400
    m_drawFlags = 0;
    m_clientEdges[2] = 2.0;
    m_curSel = -1;
    m_curSortArg = ARG_TIME;
    m_curSortDir = DIR_DECREASE;
    m_listType = TYPE_LOAD;
}

SaveList::SaveList(SaveList const&)
{
    // RVA 0x4BD490 - the shipped copy constructor copies nothing from rhs and,
    // unlike the default one, leaves the sort arg / dir and the list type
    // uninitialised.
    m_drawFlags = 0;
    m_clientEdges[2] = 2.0;
    m_curSel = -1;
}

SaveList::~SaveList()
{
    // RVA 0x4BD4E0 - the last list to go drops the shared button pattern too.
    RemoveAllItems();
    SaveButton::ClearPattern();
}

m3d::Object* SaveList::Clone()
{
    // RVA 0x4BA1A0
    return new SaveList(*this);
}

m3d::Object* SaveList::CreateObject()
{
    return new SaveList;
}

m3d::Class* SaveList::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Class* SaveList::GetClass() const
{
    return RT_CLASS_LOCAL(SaveList);
}

SaveList::SortArg SaveList::GetCurSortArg() const
{
    return m_curSortArg;
}

SaveList::SortDir SaveList::GetCurSortDir() const
{
    return m_curSortDir;
}

SaveList::ListType SaveList::GetListType() const
{
    return m_listType;
}

void SaveList::SetListType(SaveList::ListType type)
{
    m_listType = type;
}

int SaveList::MeasureItem(int itemIdx, BoundsBase<float>& bounds) const
{
    // RVA 0x4BD540 - an item is exactly as big as the button that draws it.
    auto const b = m_items[itemIdx].m_item->GetBounds();
    bounds.x0 = 0.0;
    bounds.y0 = 0.0;
    bounds.width = b.width;
    bounds.height = b.height;
    return 1;
}

int SaveList::RenderItem(int itemIdx, PointBase<float> const& org, m3d::ui::DrawInfo const& di)
{
    // RVA 0x4BD5B0 - the buttons are real child windows, so "rendering" an item
    // only means moving its button to where the row scrolled to.
    auto b = m_items[itemIdx].m_item->GetBounds();
    b.y0 = (di.m_clientRect.y0 - di.m_originalRect.y0) + org.y;
    m_items[itemIdx].m_item->SetBounds(b, true);
    return 1;
}

int SaveList::DeleteItem(int itemIdx)
{
    // RVA 0x4BD620
    delete m_items[itemIdx].m_item;
    m_items[itemIdx].m_item = nullptr;
    return 1;
}

int SaveList::CompareItem(int, int)
{
    // RVA 0x4BD660 - sorting goes through SortSaves0, never through the base
    // class hook.
    return 0;
}

void SaveList::Clear()
{
    // RVA 0x4BD670
    RemoveAllItems();
}

int SaveList::CreateFromPattern(m3d::ui::Wnd* patternWnd, bool deleteSrc)
{
    // RVA 0x4BD680 - the XML only carries a plain strings list box; the real
    // list replaces it in place.
    using namespace m3d::ui;
    if (!patternWnd || !patternWnd->IsKindOf(RT_CLASS_LOCAL(StringsListBoxWnd)))
    {
        M3D_LOG_INFO("SaveList::CreateFromPattern error - invalid patternWnd");
        return 0;
    }

    auto pattern = RT_DYNCAST(patternWnd, StringsListBoxWnd);
    if (!ListBoxWnd::Create(pattern->GetBounds(), pattern->GetStyle(), pattern->GetId()))
    {
        M3D_LOG_INFO("SaveList::CreateFromPattern error - cannot create window");
        return 0;
    }
    SetScrollPane(pattern->GetScrollPaneName());
    SetDrawFlags(pattern->GetDrawFlags());
    SetPane(pattern->GetPaneName());
    SetPaneFlags(pattern->GetPaneFlags());

    auto parent = pattern->GetParent();
    if (!parent || !parent->IsKindOf(RT_CLASS_LOCAL(Wnd)))
    {
        M3D_LOG_INFO("SaveList::CreateFromPattern error - null parent for paternWnd");
        return 0;
    }
    parent->AddChild(this);
    parent->MoveChildToFirstPosition(this);

    if (!SaveButton::LoadPattern(RT_DYNCAST(parent, Wnd)))
    {
        M3D_LOG_INFO("SaveList::CreateFromPattern error - cannot load pattern for SaveButton");
        return 0;
    }

    // Shrink to the width the button pattern actually needs.
    auto const clientWidth = GetClientBounds().width;
    auto const patternWidth = SaveButton::m_aif.m_wndPattern->GetBounds().width;
    auto bounds = GetBounds();
    bounds.width = bounds.width - (clientWidth - patternWidth);
    SetBounds(bounds, true);

    if (deleteSrc)
    {
        parent->RemoveChild(patternWnd);
        delete patternWnd;
    }
    m_gameDataFlags |= 1u;
    return 1;
}

int SaveList::OnBeforeAddToWndStation()
{
    // RVA 0x4BD930
    CreateItems();
    return Wnd::OnBeforeAddToWndStation();
}

int SaveList::CreateItems()
{
    // RVA 0x4BD950
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    RemoveAllItems();

    auto res = 1;
    if (m_listType == TYPE_SAVE)
    {
        // A save list leads with a "new save" row built from the current state.
        auto* newSaveBtn = RT_DYNCAST(m3d::g_Kernel->New("SaveButton"), SaveButton);
        if (newSaveBtn)
        {
            auto* savesManager = M3D_APP->m_pInterfaceManager->GetSavesManager();
            if (!newSaveBtn->SetupForNewSave(savesManager->GetNewSaveDefaultName(),
                                             help::GetCurrentLevelName(),
                                             ai::theObjects->GetGameTime()) ||
                !AddButton(newSaveBtn))
            {
                delete newSaveBtn;
                res = 0;
            }
        }
        else
        {
            res = 0;
        }
    }

    retruxx::vector<CStr> saveFoldersList;
    if (!M3D_APP->m_pInterfaceManager->GetSavesManager()->GetSaveFolderNames(saveFoldersList))
    {
        return 0;
    }

    retruxx::vector<SaveButton*> btns;
    for (auto i = 0; i < static_cast<int>(saveFoldersList.size()); ++i)
    {
        auto* btn = RT_DYNCAST(m3d::g_Kernel->New("SaveButton"), SaveButton);
        if (btn && btn->SetupForSave(saveFoldersList[i]))
        {
            btns.push_back(btn);
        }
        else
        {
            delete btn;
            res = 0;
        }
    }

    SortSaves0(m_curSortArg, m_curSortDir, btns);

    for (auto i = 0; i < static_cast<int>(btns.size()); ++i)
    {
        if (btns[i])
        {
            AddItem(btns[i]);
            AddChild(btns[i]);
        }
        else
        {
            res = 0;
        }
    }

    SetCurSel(GetCount() != 0 ? 0 : -1);
    return res;
}

int SaveList::AddButton(SaveButton* btn)
{
    // RVA 0x4BDC60
    if (!btn)
    {
        return 0;
    }
    AddItem(btn);
    AddChild(btn);
    return 1;
}

void SaveList::SortSaves0(SaveList::SortArg sortArg, SaveList::SortDir sortDir, retruxx::vector<SaveButton*>& btns)
{
    // RVA 0x4BDC90
    if (sortArg == ARG_NAME)
    {
        if (sortDir == DIR_INCREASE)
        {
            std::sort(btns.begin(), btns.end(), LessByName);
        }
        else if (sortDir == DIR_DECREASE)
        {
            std::sort(btns.begin(), btns.end(), GreaterByName);
        }
    }
    else if (sortArg == ARG_TIME)
    {
        if (sortDir == DIR_INCREASE)
        {
            std::sort(btns.begin(), btns.end(), LessByTime);
        }
        else if (sortDir == DIR_DECREASE)
        {
            std::sort(btns.begin(), btns.end(), GreaterByTime);
        }
    }
}

int SaveList::SortSaves(SaveList::SortArg sortArg, SaveList::SortDir sortDir)
{
    // RVA 0x4BDD20 - re-orders the existing buttons rather than rebuilding them.
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (sortArg == m_curSortArg && sortDir == m_curSortDir)
    {
        return 1;
    }
    m_curSortArg = sortArg;
    m_curSortDir = sortDir;
    if (!IsChildOf(M3D_APP))
    {
        return 1;
    }

    // The leading "new save" row of a save list never takes part in the sort.
    SaveButton* newSaveBtn = nullptr;
    retruxx::vector<SaveButton*> btns;
    for (auto i = 0; i < GetCount();)
    {
        if (m_listType == TYPE_LOAD || i != 0)
        {
            btns.push_back(m_items[i].m_item);
            ++i;
        }
        else
        {
            newSaveBtn = m_items[0].m_item;
            i = 1;
        }
    }
    m_items.clear();

    // Detach every button so the re-add below decides the new draw order.
    for (auto child = GetFirstChild(); child;)
    {
        auto next = child->GetNextSibling();
        if (child->IsKindOf(RT_CLASS_LOCAL(SaveButton)))
        {
            RemoveChild(child);
        }
        child = next;
    }

    SortSaves0(sortArg, sortDir, btns);

    auto res = 1;
    if (newSaveBtn)
    {
        AddItem(newSaveBtn);
        AddChild(newSaveBtn);
    }
    for (auto i = 0; i < static_cast<int>(btns.size()); ++i)
    {
        if (btns[i])
        {
            AddItem(btns[i]);
            AddChild(btns[i]);
        }
        else
        {
            res = 0;
        }
    }

    SetCurSel(GetCount() != 0 ? 0 : -1);
    return res;
}

int SaveList::FullUpdate()
{
    // RVA 0x4BDF50
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    RemoveAllItems();

    auto* profile = M3D_APP->GetProfileManager()->GetCurProfile();
    if (!profile)
    {
        return 0;
    }

    m3d::AIParam saveSortArg;
    m3d::AIParam saveSortDir;
    if (!profile->GetParam(PP_SAVE_SORT_ARG, saveSortArg) || !profile->GetParam(PP_SAVE_SORT_DIR, saveSortDir))
    {
        return 0;
    }

    m_curSortArg = static_cast<SortArg>(saveSortArg.GetAsID());
    m_curSortDir = static_cast<SortDir>(saveSortDir.GetAsID());

    if (IsChildOf(M3D_APP))
    {
        return CreateItems();
    }
    return 1;
}

int SaveList::GetSaveFoldersList(retruxx::vector<CStr>& saveFoldersList) const
{
    // RVA 0x4BE090 - kept for completeness; CreateItems reaches the saves
    // manager directly and nothing in the shipped game calls this.
    return M3D_APP->m_pInterfaceManager->GetSavesManager()->GetSaveFolderNames(saveFoldersList);
}

void SaveList::SetCurSel(int idx)
{
    // RVA 0x4BE0F0
    ListBoxWnd<SaveButton*>::SetCurSel(idx);

    for (auto i = 0; i < GetCount(); ++i)
    {
        auto* btn = m_items[i].m_item;
        if (!btn)
        {
            continue;
        }

        if (m_listType == TYPE_SAVE && i == 0)
        {
            // The "new save" row stays highlighted whatever else is selected.
            if ((btn->m_gameDataFlags & 1) != 0)
            {
                btn->m_wndSaveName->SetTextColor(SaveButton::m_aif.m_selectColor);
                btn->m_wndTime->SetTextColor(SaveButton::m_aif.m_selectColor);
            }
            continue;
        }

        btn->Select(i == idx ? SaveButton::SELECTED : SaveButton::UNSELECTED);
    }
}

// ===========================================================================
//  LSWnd
// ===========================================================================

RT_CLASS_EXPORTS_BEGIN(LSWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(LSWnd);

LSWnd::AuxInfo::AuxInfo()
{
    // RVA 0x4BA3B0
    m_wndSaveListName = "wndSaveList";
    m_wndScreenshotName = "wndScreenshot";
    m_wndInfoName = "wndInfo";
    m_btnSortByNameName = "btnSortName";
    m_btnSortByTimeName = "btnSortTime";
    m_wndSortByNameArrowName = "wndSortNameArrow";
    m_wndSortByTimeArrowName = "wndSortTimeArrow";
    m_texIdSortArrow = "SortArrow";
}

LSWnd::LSWnd()
{
    // RVA 0x4BA5B0
    m_wndScreenshot = nullptr;
    m_wndInfo = nullptr;
    m_wndSaveList = nullptr;
    m_btnSortByName = nullptr;
    m_btnSortByTime = nullptr;
    m_wndSortByNameArrow = nullptr;
    m_wndSortByTimeArrow = nullptr;
}

LSWnd::LSWnd(LSWnd const&)
{
    // RVA 0x4BA720 - the shipped copy constructor copies nothing from rhs and,
    // unlike the default one, leaves all seven control pointers uninitialised.
}

LSWnd::~LSWnd()
{
    // RVA 0x4BA740 - only the aux info and the ModalWnd base need unwinding.
}

m3d::Object* LSWnd::Clone()
{
    // RVA 0x4BA220
    return new LSWnd(*this);
}

m3d::Object* LSWnd::CreateObject()
{
    return new LSWnd;
}

m3d::Class* LSWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ModalWnd);
}

m3d::Class* LSWnd::GetClass() const
{
    return RT_CLASS_LOCAL(LSWnd);
}

int LSWnd::GameDataSetup()
{
    // RVA 0x4BA760
    using namespace m3d::ui;
    auto res = 1;
    if ((m_gameDataFlags & 2) == 0)
    {
        auto wndScreenshot = RT_DYNCAST(GetChildByName(m_aif.m_wndScreenshotName), ImageWnd);
        if (wndScreenshot && wndScreenshot->IsKindOf(RT_CLASS_LOCAL(ImageWnd)))
        {
            m_wndScreenshot = wndScreenshot;
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_wndScreenshotName + " is not found or incorrect type");
            res = 0;
        }

        auto wndInfo = RT_DYNCAST(GetChildByName(m_aif.m_wndInfoName), Wnd);
        if (wndInfo && wndInfo->IsKindOf(RT_CLASS_LOCAL(Wnd)))
        {
            m_wndInfo = wndInfo;
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_wndInfoName + " is not found or incorrect type");
            res = 0;
        }

        auto btnSortByName = RT_DYNCAST(GetChildByName(m_aif.m_btnSortByNameName), ButtonWnd);
        if (btnSortByName && btnSortByName->IsKindOf(RT_CLASS_LOCAL(ButtonWnd)))
        {
            m_btnSortByName = btnSortByName;
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_btnSortByNameName + " is not found or incorrect type");
            res = 0;
        }

        auto btnSortByTime = RT_DYNCAST(GetChildByName(m_aif.m_btnSortByTimeName), ButtonWnd);
        if (btnSortByTime && btnSortByTime->IsKindOf(RT_CLASS_LOCAL(ButtonWnd)))
        {
            m_btnSortByTime = btnSortByTime;
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_btnSortByTimeName + " is not found or incorrect type");
            res = 0;
        }

        auto wndSortByNameArrow = RT_DYNCAST(GetChildByName(m_aif.m_wndSortByNameArrowName), ImageWnd);
        if (wndSortByNameArrow && wndSortByNameArrow->IsKindOf(RT_CLASS_LOCAL(ImageWnd)))
        {
            m_wndSortByNameArrow = wndSortByNameArrow;
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_wndSortByNameArrowName +
                         " is not found or incorrect type");
            res = 0;
        }

        auto wndSortByTimeArrow = RT_DYNCAST(GetChildByName(m_aif.m_wndSortByTimeArrowName), ImageWnd);
        if (wndSortByTimeArrow && wndSortByTimeArrow->IsKindOf(RT_CLASS_LOCAL(ImageWnd)))
        {
            m_wndSortByTimeArrow = wndSortByTimeArrow;
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_wndSortByTimeArrowName +
                         " is not found or incorrect type");
            res = 0;
        }

        auto wndSaveList = RT_DYNCAST(GetChildByName(m_aif.m_wndSaveListName), StringsListBoxWnd);
        if (wndSaveList && wndSaveList->IsKindOf(RT_CLASS_LOCAL(StringsListBoxWnd)))
        {
            m_wndSaveList = RT_DYNCAST(m3d::g_Kernel->New("SaveList"), SaveList);
            if (m_wndSaveList)
            {
                if (m_wndSaveList->CreateFromPattern(wndSaveList, true))
                {
                    if (res)
                    {
                        m_gameDataFlags |= 1u;
                    }
                }
                else
                {
                    M3D_LOG_INFO("Make control error: cannot create " + m_aif.m_wndSaveListName +
                                 " from pattern class");
                }
            }
            else
            {
                M3D_LOG_INFO("Make control error: cannot create " + m_aif.m_wndSaveListName +
                             " - cannot find rtti class " + "SaveList");
            }
        }
        else
        {
            M3D_LOG_INFO("Make control error: control " + m_aif.m_wndSaveListName + " is not found or incorrect type");
        }
    }

    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }
    M3D_LOG_INFO("LSWnd: error - fail to init because of a bad resource");
    return 0;
}

int LSWnd::GameDataUpdate(void* data, int dataType)
{
    // RVA 0x4BB2C0
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    switch (dataType)
    {
    case 40:
        OnCurProfileChanged();
        break;
    case 42:
        OnCurProfileParamChanged(data);
        return 1;
    case 47:
        OnScreenshotRelease(data);
        return 1;
    }
    return 1;
}

int LSWnd::OnWndNotify(m3d::ui::Wnd* from, unsigned id, unsigned msg, m3d::AIParam const& data)
{
    // RVA 0x4BB130
    if (ModalWnd::OnWndNotify(from, id, msg, data))
    {
        return 1;
    }
    switch (id)
    {
    case ID_BTN_SORT_BY_NAME:
        if (msg != 1)
        {
            return 0;
        }
        OnSortByArg(SaveList::ARG_NAME);
        return 1;
    case ID_BTN_SORT_BY_TIME:
        if (msg != 1)
        {
            return 0;
        }
        OnSortByArg(SaveList::ARG_TIME);
        return 1;
    case ID_SAVE_LIST:
        if (msg != 5)
        {
            return 0;
        }
        OnSaveSelectionChange();
        return 1;
    case ID_BTN_CANCEL:
    {
        if (msg != 1)
        {
            return 0;
        }
        int hideMode = 3;
        M3D_APP->m_pInterfaceManager->ShowWindow(m_guiId, false, false, false, false, &hideMode);
        LaunchGameMenu();
        return 1;
    }
    default:
        break;
    }
    return 0;
}

int LSWnd::OnKey(unsigned short key, unsigned char scanCode, unsigned state)
{
    // RVA 0x4BBA70
    if (!state || (m_gameDataFlags & 1) == 0)
    {
        return ModalWnd::OnKey(key, scanCode, state);
    }

    switch (key & 0xff)
    {
    case m3d::KBD_ESC:
    {
        int hideMode = 3;
        M3D_APP->m_pInterfaceManager->ShowWindow(m_guiId, false, false, false, false, &hideMode);
        LaunchGameMenu();
        return 1;
    }
    case m3d::KBD_ENTER:
        OnEnter();
        return 1;
    case m3d::KBD_UP:
    case m3d::KBD_DOWN:
    {
        auto const curSel = m_wndSaveList->GetCurSel();
        if (curSel != -1)
        {
            auto newSel = (key & 0xff) == m3d::KBD_UP ? curSel - 1 : curSel + 1;
            auto const lastIdx = m_wndSaveList->GetCount() - 1;
            if (newSel < 0)
            {
                newSel = 0;
            }
            if (newSel > lastIdx)
            {
                newSel = lastIdx;
            }
            if (newSel != curSel)
            {
                m_wndSaveList->SetCurSel(newSel);
            }
        }
        return 1;
    }
    default:
        break;
    }
    return ModalWnd::OnKey(key, scanCode, state);
}

void LSWnd::OnSortByArg(SaveList::SortArg arg)
{
    // RVA 0x4BB210 - clicking the already active column flips the direction
    // instead of changing it. The list only reacts once the profile has written
    // the new value back and notified everyone.
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    auto* profile = M3D_APP->GetProfileManager()->GetCurProfile();
    if (!profile)
    {
        return;
    }

    ProfileParam paramId;
    int paramValue;
    if (m_wndSaveList->GetCurSortArg() == arg)
    {
        paramId = PP_SAVE_SORT_DIR;
        paramValue =
            m_wndSaveList->GetCurSortDir() == SaveList::DIR_INCREASE ? SaveList::DIR_DECREASE : SaveList::DIR_INCREASE;
    }
    else
    {
        paramId = PP_SAVE_SORT_ARG;
        paramValue = arg;
    }
    profile->SetParam(paramId, m3d::AIParam(paramValue));
}

void LSWnd::OnSaveSelectionChange()
{
    // RVA 0x4BB440
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    ClearControls();

    auto const curSel = m_wndSaveList->GetCurSel();
    if (curSel == -1)
    {
        return;
    }
    auto* btn = m_wndSaveList->GetItem(curSel);
    if (!btn)
    {
        return;
    }

    SetScreenshotForSave(curSel);

    CStr levelName;
    auto const* levelInfo =
        M3D_APP->m_pInterfaceManager->GetLevelInfoManager()->GetLevelInfoByName(btn->GetLevelName());
    if (levelInfo)
    {
        levelName = levelInfo->GetFullName();
    }
    else
    {
        levelName = btn->GetLevelName();
    }

    CStr gameTime;
    auto const time = btn->GetGameTime().GetAsIdList();
    if (time.size() == 5)
    {
        gameTime = CStr(time[2]) + "/" + CStr(time[3]) + "/" + CStr(time[4]);
    }

    m_wndInfo->SetText(levelName + "|" + gameTime);
}

void LSWnd::ClearControls()
{
    // RVA 0x4BB6F0
    if ((m_gameDataFlags & 1) != 0)
    {
        m_wndScreenshot->ShowWindow(false);
        m_wndInfo->SetText(CStr());
    }
}

int LSWnd::SetScreenshotForSave(int saveIdx)
{
    // RVA 0x4BB750
    auto* btn = m_wndSaveList->GetItem(saveIdx);
    if (!btn)
    {
        return 0;
    }

    auto* savesManager = M3D_APP->m_pInterfaceManager->GetSavesManager();
    CStr const screenshotFile = M3D_APP->GetStartupFolder() + "\\" +
                                savesManager->GetSaveFolderPathByFolderName(btn->GetSaveFolderName()) + "\\" +
                                savesManager->GetConstatntSaveInfo().m_screenshotFileName;

    auto const attribs = GetFileAttributesA(screenshotFile.c_str());
    if (attribs == INVALID_FILE_ATTRIBUTES || (attribs & FILE_ATTRIBUTE_DIRECTORY) != 0)
    {
        return 0;
    }

    m_wndScreenshot->ShowWindow(true);
    m_wndScreenshot->SetImage(screenshotFile);
    return 1;
}

void LSWnd::OnScreenshotRelease(void* data)
{
    // RVA 0x4BB960 - the texture the panel is showing has just been dropped, so
    // stop referring to it.
    if ((m_gameDataFlags & 1) == 0 || !data)
    {
        return;
    }

    auto const* ev = static_cast<m3d::Event const*>(data);
    CStr curScreenshotFilePath;
    M3D_RENDERER->GetTextureName(m_wndScreenshot->GetImage(), curScreenshotFilePath);
    if (!CStr::my_strcmp(ev->m_strEv.c_str(), curScreenshotFilePath.c_str()))
    {
        m3d::rend::TexHandle invalid;
        invalid.SetInvalid();
        m_wndScreenshot->SetImage(invalid);
    }
}

void LSWnd::UpdateSortButtonStates()
{
    // RVA 0x4BBA00 - the column the list is sorted by is drawn active.
    if ((m_gameDataFlags & 1) != 0)
    {
        auto textColor = m_textColor;
        if (m_wndSaveList->GetCurSortArg())
        {
            textColor = m_textColorDisabled;
        }
        m_btnSortByName->SetTextColor(textColor);

        textColor = m_textColorDisabled;
        if (m_wndSaveList->GetCurSortArg() == SaveList::ARG_TIME)
        {
            textColor = m_textColor;
        }
        m_btnSortByTime->SetTextColor(textColor);
        UpdateSortArrowsState();
    }
}

void LSWnd::UpdateSortArrowsState()
{
    // RVA 0x4BBC80
    if ((m_gameDataFlags & 1) != 0)
    {
        auto const curSortArg = m_wndSaveList->GetCurSortArg();
        auto const curSortDir = m_wndSaveList->GetCurSortDir();

        m_wndSortByNameArrow->ShowWindow(curSortArg == SaveList::ARG_NAME);
        m_wndSortByTimeArrow->ShowWindow(curSortArg == SaveList::ARG_TIME);

        auto wndSort = m_wndSortByNameArrow;
        if (curSortArg)
        {
            if (curSortArg != SaveList::ARG_TIME)
            {
                return;
            }
            wndSort = m_wndSortByTimeArrow;
        }

        if (wndSort)
        {
            auto const icoHandle = M3D_APP->m_pInterfaceManager->GetIcoByName(m_aif.m_texIdSortArrow,
                                                                             curSortDir != SaveList::DIR_INCREASE);
            wndSort->SetImage(icoHandle);
        }
    }
}

void LSWnd::OnCurProfileChanged()
{
    // RVA 0x4BB320
    if ((m_gameDataFlags & 1) != 0)
    {
        m_wndSaveList->FullUpdate();
        UpdateSortButtonStates();
    }
}

void LSWnd::OnCurProfileParamChanged(void* data)
{
    // RVA 0x4BB340 - only the two sort params matter here.
    if ((m_gameDataFlags & 1) != 0 && data)
    {
        auto const* ev = static_cast<m3d::Event const*>(data);
        if (ev->m_uintEv[0] <= 1u)
        {
            if (auto* profile = M3D_APP->GetProfileManager()->GetCurProfile())
            {
                m3d::AIParam sortArgParam;
                m3d::AIParam sortDirParam;
                if (profile->GetParam(PP_SAVE_SORT_ARG, sortArgParam) &&
                    profile->GetParam(PP_SAVE_SORT_DIR, sortDirParam))
                {
                    auto const sortArg = static_cast<SaveList::SortArg>(sortArgParam.GetAsID());
                    auto const sortDir = static_cast<SaveList::SortDir>(sortDirParam.GetAsID());
                    m_wndSaveList->SortSaves(sortArg, sortDir);
                    UpdateSortButtonStates();
                }
            }
        }
    }
}

void LSWnd::OnEnter()
{
    // RVA 0x4BA210 - the base window does nothing on Enter; LoadWnd and SaveWnd
    // override it.
}

void LSWnd::LaunchGameMenu()
{
    // RVA 0x4BBBE0
    if (!M3D_APP->GetCurGameMode())
    {
        M3D_APP->EnqueueMessage(UM_SHOWGAMEMENU, 0, 0, 0, 0, {}, {});
    }
}
