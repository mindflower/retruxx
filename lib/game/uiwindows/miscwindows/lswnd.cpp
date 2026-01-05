#include "lswnd.h"
#include <core/log.h>
#include <ui/image.h>
#include <ui/listbox.h>

#include "game/m3dgame.h"
#include "game/profile.h"

RT_CLASS_EXPORTS_BEGIN(SaveButton)
    RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(SaveButton);

SaveButton::AuxInfo::AuxInfo()
{

}

SaveButton::AuxInfo SaveButton::m_aif;

int SaveButton::SetupForSave(CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* SaveButton::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SaveButton::SetupForNewSave(CStr const&, CStr const&, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SaveButton::Select(SelectType)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* SaveButton::GetBaseClass()
{
    return RT_CLASS_LOCAL(ButtonWnd);
}

CStr const& SaveButton::GetLevelName() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* SaveButton::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

SaveButton::~SaveButton()
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr const& SaveButton::GetSaveFolderName() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr const& SaveButton::GetSaveName() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* SaveButton::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::AIParam const& SaveButton::GetGameTime() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

_FILETIME const& SaveButton::GetSaveModifyTime() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SaveButton::ClearPattern()
{
    if (SaveButton::m_aif.m_wndPattern)
    {
        delete SaveButton::m_aif.m_wndPattern;
        SaveButton::m_aif.m_wndPattern = nullptr;
    }
    if (SaveButton::m_aif.m_wndPatternSaveName)
    {
        delete SaveButton::m_aif.m_wndPatternSaveName;
        SaveButton::m_aif.m_wndPatternSaveName = nullptr;
    }
    if (SaveButton::m_aif.m_wndPatternTime)
    {
        delete SaveButton::m_aif.m_wndPatternTime;
        SaveButton::m_aif.m_wndPatternTime = nullptr;
    }
}

SaveButton::SaveButton(SaveButton const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

SaveButton::SaveButton()
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr SaveButton::LocalTime2Str(_FILETIME) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SaveButton::CreateFromPattern()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SaveButton::LoadPattern(m3d::ui::Wnd* pattern)
{
    if (m_aif.m_wndPattern)
    {
        return 1;
    }
    ClearPattern();
    auto res = 1;
    if (pattern)
    {
        auto child = pattern->GetChildByName(m_aif.m_wndPatternName);
        if (child && child->IsKindOf(RT_CLASS_LOCAL(ButtonWnd)))
        {
            m_aif.m_wndPattern = dynamic_cast<ButtonWnd*>(child);
            pattern->RemoveChild(child);
            child = pattern->GetChildByName(m_aif.m_wndSaveNameName);
            if (child && child->IsKindOf(RT_CLASS_LOCAL(Wnd)))
            {
                m_aif.m_wndPatternSaveName = dynamic_cast<Wnd*>(child);;
                pattern->RemoveChild(child);

                auto patternSaveName = m_aif.m_wndPatternSaveName->GetBounds();
                const auto pattern = m_aif.m_wndPattern->GetBounds();

                patternSaveName.y0 = patternSaveName.y0 - pattern.y0;
                patternSaveName.x0 = patternSaveName.x0 - pattern.x0;

                m_aif.m_wndPatternSaveName->SetBounds(patternSaveName, false);

                //SaveButton::m_aif.m_wndPatternSaveName->m_bounds.y0 = SaveButton::m_aif.m_wndPatternSaveName->m_bounds.y0 - SaveButton::m_aif.m_wndPattern->m_bounds.y0;
                //SaveButton::m_aif.m_wndPatternSaveName->m_bounds.x0 = SaveButton::m_aif.m_wndPatternSaveName->m_bounds.x0 - SaveButton::m_aif.m_wndPattern->m_bounds.x0;
            }
            else
            {
                M3D_LOG_INFO("GET_CHILD_PATTERN error - cannot find child wnd " + m_aif.m_wndSaveNameName);
                res = 0;
            }

            child = pattern->Object::GetChildByName(SaveButton::m_aif.m_wndTimeName);
            if (child && child->IsKindOf(RT_CLASS_LOCAL(Wnd)))
            {
                m_aif.m_wndPatternTime = dynamic_cast<Wnd*>(child);
                pattern->RemoveChild(child);

                auto patternTime = m_aif.m_wndPatternTime->GetBounds();
                const auto pattern = m_aif.m_wndPattern->GetBounds();

                patternTime.y0 = patternTime.y0 - pattern.y0;
                patternTime.x0 = patternTime.x0 - pattern.x0;

                m_aif.m_wndPatternTime->SetBounds(patternTime, false);

                //m_aif.m_wndPatternTime->m_bounds.y0 = m_aif.m_wndPatternTime->m_bounds.y0 - m_aif.m_wndPattern->m_bounds.y0;
                //m_aif.m_wndPatternTime->m_bounds.x0 = m_aif.m_wndPatternTime->m_bounds.x0 - m_aif.m_wndPattern->m_bounds.x0;
            }
            else
            {
                M3D_LOG_INFO("GET_CHILD_PATTERN error - cannot find child wnd " + m_aif.m_wndTimeName);
                res = 0;
            }
            m_aif.m_wndPattern->SetBounds({ 0.0, 0.0 }, false);
            //m_aif.m_wndPattern->m_bounds.x0 = 0.0;
            //m_aif.m_wndPattern->m_bounds.y0 = 0.0;
        }
        else
        {

            M3D_LOG_INFO("SaveButton::LoadPattern - error to create - invalid pattern wnd");
            res = 0;
        }
    }
    else
    {
        M3D_LOG_INFO("SaveButton::LoadPattern - error to create - invalid pattern wnd");
        res = 0;
    }
    return res;
}

int SaveButton::CreateChildren()
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORTS_BEGIN(SaveList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(SaveList);

SaveList::~SaveList()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* SaveList::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* SaveList::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

SaveList::SortDir SaveList::GetCurSortDir() const
{
    return m_curSortDir;
}

m3d::Object* SaveList::CreateObject()
{
    return new SaveList;
}

int SaveList::SortSaves(SortArg, SortDir)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SaveList::CreateFromPattern(m3d::ui::Wnd* patternWnd, bool deleteSrc)
{
    using namespace m3d::ui;
    if (patternWnd && patternWnd->IsKindOf(RT_CLASS_LOCAL(StringsListBoxWnd)))
    {
        auto pattern = dynamic_cast<StringsListBoxWnd*>(patternWnd);
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
        if (!SaveButton::LoadPattern(dynamic_cast<Wnd*>(parent)))
        {
            M3D_LOG_INFO("SaveList::CreateFromPattern error - cannot load pattern for SaveButton");
            return 0;
        }
        auto clientWidth = GetClientBounds().width;
        auto patternWidth = SaveButton::m_aif.m_wndPattern->GetBounds().width;
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
    else
    {
        M3D_LOG_INFO("SaveList::CreateFromPattern error - invalid patternWnd");
        return 0;
    }
}

void SaveList::SetCurSel(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

SaveList::ListType SaveList::GetListType() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

SaveList::SortArg SaveList::GetCurSortArg() const
{
    return m_curSortArg;
}

m3d::Class* SaveList::GetClass() const
{
    return RT_CLASS_LOCAL(SaveList);
}

int SaveList::FullUpdate()
{
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

void SaveList::SetListType(ListType)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SaveList::GetSaveFoldersList(std::vector<CStr, std::allocator<CStr>>&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

SaveList::SaveList()
{
    m_drawFlags = 0;
    //TODO: check this
    m_clientEdges[2] = 2.0;
    m_curSel = -1;
    m_curSortArg = ARG_TIME;
    m_curSortDir = DIR_DECREASE;
    m_listType = TYPE_LOAD;
}

SaveList::SaveList(SaveList const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SaveList::CompareItem(int, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SaveList::OnBeforeAddToWndStation()
{
    CreateItems();
    return Wnd::OnBeforeAddToWndStation();
}

int SaveList::AddButton(SaveButton*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SaveList::RenderItem(int, PointBase<float> const&, m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SaveList::MeasureItem(int, BoundsBase<float>&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SaveList::CreateItems()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SaveList::SortSaves0(SortArg, SortDir, std::vector<SaveButton*, std::allocator<SaveButton*>>&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SaveList::Clear()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SaveList::DeleteItem(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORTS_BEGIN(LSWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(LSWnd);

LSWnd::AuxInfo::AuxInfo()
{
}

m3d::Class* LSWnd::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* LSWnd::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* LSWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

LSWnd::~LSWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* LSWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ModalWnd);
}

void LSWnd::OnCurProfileChanged()
{
    if ((m_gameDataFlags & 1) != 0)
    {
        m_wndSaveList->FullUpdate();
        UpdateSortButtonStates();
    }
}

void LSWnd::OnEnter()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int LSWnd::OnKey(unsigned short, unsigned char, unsigned)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int LSWnd::GameDataSetup()
{
    using namespace m3d::ui;
    auto res = 1;
    if ((m_gameDataFlags & 2) == 0)
    {
        auto wndScreenshot = dynamic_cast<ImageWnd*>(GetChildByName(m_aif.m_wndScreenshotName));
        if (wndScreenshot && wndScreenshot->IsKindOf(RT_CLASS_LOCAL(ImageWnd)))
        {
            m_wndScreenshot = wndScreenshot;
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_wndScreenshotName + " is not found or incorrect type");
            res = 0;
        }

        auto wndInfo = dynamic_cast<Wnd*>(GetChildByName(m_aif.m_wndInfoName));
        if (wndInfo && wndInfo->IsKindOf(RT_CLASS_LOCAL(Wnd)))
        {
            m_wndInfo = wndInfo;
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_wndInfoName + " is not found or incorrect type");
            res = 0;
        }

        auto btnSortByName = dynamic_cast<ButtonWnd*>(GetChildByName(m_aif.m_btnSortByNameName));
        if (btnSortByName && btnSortByName->IsKindOf(RT_CLASS_LOCAL(ButtonWnd)))
        {
            m_btnSortByName = btnSortByName;
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_btnSortByNameName + " is not found or incorrect type");
            res = 0;
        }

        auto btnSortByTime = dynamic_cast<ButtonWnd*>(GetChildByName(m_aif.m_btnSortByTimeName));
        if (btnSortByTime && btnSortByTime->IsKindOf(RT_CLASS_LOCAL(ButtonWnd)))
        {
            m_btnSortByTime = btnSortByTime;
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_btnSortByTimeName + " is not found or incorrect type");
            res = 0;
        }

        auto wndSortByNameArrow = dynamic_cast<ImageWnd*>(GetChildByName(m_aif.m_wndSortByNameArrowName));
        if (wndSortByNameArrow && wndSortByNameArrow->IsKindOf(RT_CLASS_LOCAL(ImageWnd)))
        {
            m_wndSortByNameArrow = wndSortByNameArrow;
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_wndSortByNameArrowName + " is not found or incorrect type");
            res = 0;
        }

        auto wndSortByTimeArrow = dynamic_cast<ImageWnd*>(GetChildByName(m_aif.m_wndSortByTimeArrowName));
        if (wndSortByTimeArrow && wndSortByTimeArrow->IsKindOf(RT_CLASS_LOCAL(ImageWnd)))
        {
            m_wndSortByTimeArrow = wndSortByTimeArrow;
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_wndSortByTimeArrowName + " is not found or incorrect type");
            res = 0;
        }

        auto wndSaveList = dynamic_cast<StringsListBoxWnd*>(GetChildByName(m_aif.m_wndSaveListName));
        if (wndSaveList && wndSaveList->IsKindOf(RT_CLASS_LOCAL(StringsListBoxWnd)))
        {
            m_wndSaveList = dynamic_cast<SaveList*>(m3d::g_Kernel->New("SaveList"));
            if (m_wndSaveList)
            {
                if (m_wndSaveList->CreateFromPattern(wndSaveList, true))
                {
                    if (res)
                    {
                        m_gameDataFlags |= 1;
                    }
                }
                else
                {
                    M3D_LOG_INFO("Make control error: cannot create " + m_aif.m_wndSaveListName + " from pattern class");
                }
            }
            else
            {
                M3D_LOG_INFO("Make control error: cannot create " + m_aif.m_wndSaveListName + " - cannot find rtti class SaveList");
            }
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_wndSortByNameArrowName + " is not found or incorrect type");
        }
    }
    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }
    M3D_LOG_INFO("LoadWnd: error - fail to init because of a bad resource");
    return 0;
}

int LSWnd::GameDataUpdate(void* data, int dataType)
{
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

LSWnd::LSWnd()
{
}

LSWnd::LSWnd(LSWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void LSWnd::OnCurProfileParamChanged(void* data)
{
    if ((m_gameDataFlags & 1) != 0 && data)
    {
        auto const ev = static_cast<m3d::Event*>(data);
        if (!ev->m_void[0] || ev->m_uintEv[0] == 1)
        {
            if (auto const profile = M3D_APP->GetProfileManager()->GetCurProfile())
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

int LSWnd::OnWndNotify(m3d::ui::Wnd* from, unsigned id, unsigned msg, m3d::AIParam const& data)
{
    if (ModalWnd::OnWndNotify(from, id, msg, data))
    {
        return 1;
    }
    switch (id)
    {
    case 0x61A80u:
    {
        if (msg != 1)
        {
            return 0;
        }
        OnSortByArg(SaveList::ARG_NAME);
        return 1;
    }
    case 0x61A81u:
    {
        if (msg != 1)
        {
            return 0;
        }
        OnSortByArg(SaveList::ARG_TIME);
        return 1;
    }
    case 0x61A82u:
    {
        if (msg != 5)
        {
            return 0;
        }
        OnSaveSelectionChange();
        return 1;
    }
    case 0x61A83u:
    {
        if (msg != 1)
        {
            return 0;
        }
        int data = 3;
        M3D_APP->m_pInterfaceManager->ShowWindow(m_guiId, false, false, false, false, &data);
        LaunchGameMenu();
        return 1;
    }
    default:
        break;
    }
    return 0;
}

void LSWnd::UpdateSortButtonStates()
{
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
    if ((m_gameDataFlags & 1) != 0)
    {
        auto curSortArg = m_wndSaveList->GetCurSortArg();
        auto curSortDir = m_wndSaveList->GetCurSortDir();

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
            auto const icoHandle = M3D_APP->m_pInterfaceManager->GetIcoByName(m_aif.m_texIdSortArrow, curSortDir != SaveList::DIR_INCREASE);
            wndSort->SetImage(icoHandle);
        }
    }
}

void LSWnd::OnScreenshotRelease(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int LSWnd::SetScreenshotForSave(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void LSWnd::OnSaveSelectionChange()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void LSWnd::OnSortByArg(SaveList::SortArg)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void LSWnd::ClearControls()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void LSWnd::LaunchGameMenu()
{
    if (!M3D_APP->GetCurGameMode())
    {
        M3D_APP->EnqueueMessage(65656, 0, 0, 0, 0, {}, {});
    }
}
