#include "lswnd.h"

RT_CLASS_DEFINE(SaveButton);

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
    throw std::logic_error("Not implemented");
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

int SaveButton::LoadPattern(m3d::ui::Wnd*)
{
    throw std::logic_error("Not implemented");
}

int SaveButton::CreateChildren()
{
    throw std::logic_error("Not implemented");
}

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
    throw std::logic_error("Not implemented");
}

SaveList::SortDir SaveList::GetCurSortDir() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* SaveList::CreateObject()
{
    throw std::logic_error("Not implemented");
}

int SaveList::SortSaves(SortArg, SortDir)
{
    throw std::logic_error("Not implemented");
}

int SaveList::CreateFromPattern(m3d::ui::Wnd*, bool)
{
    throw std::logic_error("Not implemented");
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
    throw std::logic_error("Not implemented");
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
    throw std::logic_error("Not implemented");
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

RT_CLASS_DEFINE(LSWnd);

LSWnd::AuxInfo::AuxInfo()
{
    throw std::logic_error("Not implemented");
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
    throw std::logic_error("Not implemented");
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
    throw std::logic_error("Not implemented");
}

int LSWnd::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

LSWnd::LSWnd()
{
    throw std::logic_error("Not implemented");
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
