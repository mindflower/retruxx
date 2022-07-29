#include "salewnd.h"

RT_CLASS_EXPORTS_BEGIN(SaleWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(SaleWnd);

m3d::Class* SaleWnd::GetRtClass() const
{
    throw std::logic_error("Not implemented");
}

m3d::Class* SaleWnd::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

std::vector<int> SaleWnd::GetResourceIdsForTabItem(WorkshopTabType)
{
    throw std::logic_error("Not implemented");
}

int SaleWnd::SetUpForWorkshop(int)
{
    throw std::logic_error("Not implemented");
}

SaleWnd::~SaleWnd()
{
    throw std::logic_error("Not implemented");
}

ai::Workshop* SaleWnd::GetWorkshop() const
{
    throw std::logic_error("Not implemented");
}

ref_ptr<m3d::ui::Wnd> SaleWnd::GetWorkWndForTabItem(int) const
{
    throw std::logic_error("Not implemented");
}

int SaleWnd::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

void SaleWnd::AddTabItem(WorkshopTabType)
{
    throw std::logic_error("Not implemented");
}

int SaleWnd::OnBeforeAddToWndStation()
{
    throw std::logic_error("Not implemented");
}

SaleWnd::SaleWnd()
{
    throw std::logic_error("Not implemented");
}

int SaleWnd::LaunchTownDlg()
{
    throw std::logic_error("Not implemented");
}

int SaleWnd::SetupTabInfo(m3d::ui::Wnd*, WorkshopTabType)
{
    throw std::logic_error("Not implemented");
}

int SaleWnd::SetupTabItem(m3d::ui::Wnd*, WorkshopTabType)
{
    throw std::logic_error("Not implemented");
}

void SaleWnd::SetupListOfTabTypes()
{
    throw std::logic_error("Not implemented");
}

int SaleWnd::GetTabIdByTabType(WorkshopTabType) const
{
    throw std::logic_error("Not implemented");
}

void SaleWnd::ClearTabItems()
{
    throw std::logic_error("Not implemented");
}

bool SaleWnd::IsTabCreatedFromRepositorySource(WorkshopTabType) const
{
    throw std::logic_error("Not implemented");
}

m3d::ui::Wnd* SaleWnd::CreateTabItem(WorkshopTabType)
{
    throw std::logic_error("Not implemented");
}

void SaleWnd::CreateTabItems()
{
    throw std::logic_error("Not implemented");
}

SaleWnd::WorkshopTabType SaleWnd::GetTabTypeByTabId(int) const
{
    throw std::logic_error("Not implemented");
}

int SaleWnd::GameDataClear(bool)
{
    throw std::logic_error("Not implemented");
}

ai::WorkshopRepositoryType SaleWnd::GetWorkshopRepositoryType(WorkshopTabType)
{
    throw std::logic_error("Not implemented");
}

void SaleWnd::UpdateTownPicture()
{
    throw std::logic_error("Not implemented");
}

int SaleWnd::GameDataSetup()
{
    throw std::logic_error("Not implemented");
}

void SaleWnd::UpdateOnChangeTabSel(int, int, m3d::ui::TabWnd::SelectionType)
{
    throw std::logic_error("Not implemented");
}

int SaleWnd::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}
