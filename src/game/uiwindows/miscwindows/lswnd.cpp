#include "lswnd.h"
#include <core/log.h>
#include <ui/image.h>
#include <ui/listbox.h>

RT_CLASS_EXPORTS_BEGIN(SaveButton)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(SaveButton);

SaveButton::AuxInfo::AuxInfo()
{

}

SaveButton::AuxInfo SaveButton::m_aif;

int SaveButton::SetupForSave(CStr const&)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* SaveButton::Clone()
{
    throw std::logic_error("Not implemented");
}

int SaveButton::SetupForNewSave(CStr const&, CStr const&, m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

void SaveButton::Select(SelectType)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* SaveButton::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

CStr const& SaveButton::GetLevelName() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* SaveButton::CreateObject()
{
    throw std::logic_error("Not implemented");
}

SaveButton::~SaveButton()
{
    throw std::logic_error("Not implemented");
}

CStr const& SaveButton::GetSaveFolderName() const
{
    throw std::logic_error("Not implemented");
}

CStr const& SaveButton::GetSaveName() const
{
    throw std::logic_error("Not implemented");
}

m3d::Class* SaveButton::GetClass() const
{
    throw std::logic_error("Not implemented");
}

m3d::AIParam const& SaveButton::GetGameTime() const
{
    throw std::logic_error("Not implemented");
}

_FILETIME const& SaveButton::GetSaveModifyTime() const
{
    throw std::logic_error("Not implemented");
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
    throw std::logic_error("Not implemented");
}

SaveButton::SaveButton()
{
    throw std::logic_error("Not implemented");
}

CStr SaveButton::LocalTime2Str(_FILETIME) const
{
    throw std::logic_error("Not implemented");
}

int SaveButton::CreateFromPattern()
{
    throw std::logic_error("Not implemented");
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
                SaveButton::m_aif.m_wndPatternSaveName->m_bounds.y0 = SaveButton::m_aif.m_wndPatternSaveName->m_bounds.y0 - SaveButton::m_aif.m_wndPattern->m_bounds.y0;
                SaveButton::m_aif.m_wndPatternSaveName->m_bounds.x0 = SaveButton::m_aif.m_wndPatternSaveName->m_bounds.x0 - SaveButton::m_aif.m_wndPattern->m_bounds.x0;
            }
            else
            {
                M3D_LOG_INFO("GET_CHILD_PATTERN error - cannot find child wnd " + m_aif.m_wndSaveNameName);
                res = 0;
            }

            child = pattern->Object::GetChildByName(SaveButton::m_aif.m_wndTimeName);
            if (child && child->IsKindOf(RT_CLASS_LOCAL(Wnd)))
            {
                SaveButton::m_aif.m_wndPatternTime = dynamic_cast<Wnd*>(child);
                pattern->RemoveChild(child);
                SaveButton::m_aif.m_wndPatternTime->m_bounds.y0 = SaveButton::m_aif.m_wndPatternTime->m_bounds.y0 - SaveButton::m_aif.m_wndPattern->m_bounds.y0;
                SaveButton::m_aif.m_wndPatternTime->m_bounds.x0 = SaveButton::m_aif.m_wndPatternTime->m_bounds.x0
                    - SaveButton::m_aif.m_wndPattern->m_bounds.x0;
            }
            else
            {
                M3D_LOG_INFO("GET_CHILD_PATTERN error - cannot find child wnd " + m_aif.m_wndTimeName);
                res = 0;
            }
            SaveButton::m_aif.m_wndPattern->m_bounds.x0 = 0.0;
            SaveButton::m_aif.m_wndPattern->m_bounds.y0 = 0.0;
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
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORTS_BEGIN(SaveList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(SaveList);

SaveList::~SaveList()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* SaveList::Clone()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* SaveList::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

SaveList::SortDir SaveList::GetCurSortDir() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* SaveList::CreateObject()
{
    return new SaveList;
}

int SaveList::SortSaves(SortArg, SortDir)
{
    throw std::logic_error("Not implemented");
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
    throw std::logic_error("Not implemented");
}

SaveList::ListType SaveList::GetListType() const
{
    throw std::logic_error("Not implemented");
}

SaveList::SortArg SaveList::GetCurSortArg() const
{
    throw std::logic_error("Not implemented");
}

m3d::Class* SaveList::GetClass() const
{
    return RT_CLASS_LOCAL(SaveList);
}

int SaveList::FullUpdate()
{
    throw std::logic_error("Not implemented");
}

void SaveList::SetListType(ListType)
{
    throw std::logic_error("Not implemented");
}

int SaveList::GetSaveFoldersList(std::vector<CStr, std::allocator<CStr>>&) const
{
    throw std::logic_error("Not implemented");
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
    throw std::logic_error("Not implemented");
}

int SaveList::CompareItem(int, int)
{
    throw std::logic_error("Not implemented");
}

int SaveList::OnBeforeAddToWndStation()
{
    throw std::logic_error("Not implemented");
}

int SaveList::AddButton(SaveButton*)
{
    throw std::logic_error("Not implemented");
}

int SaveList::RenderItem(int, PointBase<float> const&, m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}

int SaveList::MeasureItem(int, BoundsBase<float>&) const
{
    throw std::logic_error("Not implemented");
}

int SaveList::CreateItems()
{
    throw std::logic_error("Not implemented");
}

void SaveList::SortSaves0(SortArg, SortDir, std::vector<SaveButton*, std::allocator<SaveButton*>>&)
{
    throw std::logic_error("Not implemented");
}

void SaveList::Clear()
{
    throw std::logic_error("Not implemented");
}

int SaveList::DeleteItem(int)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORTS_BEGIN(LSWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(LSWnd);

LSWnd::AuxInfo::AuxInfo()
{
}

m3d::Class* LSWnd::GetClass() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* LSWnd::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* LSWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

LSWnd::~LSWnd()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* LSWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ModalWnd);
}

void LSWnd::OnCurProfileChanged()
{
    throw std::logic_error("Not implemented");
}

void LSWnd::OnEnter()
{
    throw std::logic_error("Not implemented");
}

int LSWnd::OnKey(unsigned short, unsigned char, unsigned)
{
    throw std::logic_error("Not implemented");
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

int LSWnd::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

LSWnd::LSWnd()
{
}

LSWnd::LSWnd(LSWnd const&)
{
    throw std::logic_error("Not implemented");
}

void LSWnd::OnCurProfileParamChanged(void*)
{
    throw std::logic_error("Not implemented");
}

int LSWnd::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

void LSWnd::UpdateSortButtonStates()
{
    throw std::logic_error("Not implemented");
}

void LSWnd::UpdateSortArrowsState()
{
    throw std::logic_error("Not implemented");
}

void LSWnd::OnScreenshotRelease(void*)
{
    throw std::logic_error("Not implemented");
}

int LSWnd::SetScreenshotForSave(int)
{
    throw std::logic_error("Not implemented");
}

void LSWnd::OnSaveSelectionChange()
{
    throw std::logic_error("Not implemented");
}

void LSWnd::OnSortByArg(SaveList::SortArg)
{
    throw std::logic_error("Not implemented");
}

void LSWnd::ClearControls()
{
    throw std::logic_error("Not implemented");
}

void LSWnd::LaunchGameMenu()
{
    throw std::logic_error("Not implemented");
}
