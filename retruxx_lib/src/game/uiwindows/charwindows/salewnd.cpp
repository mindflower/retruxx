#include "salewnd.h"

RT_CLASS_EXPORTS_BEGIN(SaleWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(SaleWnd);

m3d::Class* SaleWnd::GetRtClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* SaleWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ChildPanel);
}

std::vector<int> SaleWnd::GetResourceIdsForTabItem(WorkshopTabType)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SaleWnd::SetUpForWorkshop(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

SaleWnd::~SaleWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Workshop* SaleWnd::GetWorkshop() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

ref_ptr<m3d::ui::Wnd> SaleWnd::GetWorkWndForTabItem(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SaleWnd::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SaleWnd::AddTabItem(WorkshopTabType)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SaleWnd::OnBeforeAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

SaleWnd::SaleWnd()
{
    m_wndTab = 0;
    m_wndTownPicture = 0;
    m_workshopId = -1;
    m_buildingType = ai::NUM_BUILDINGTYPES;
}

int SaleWnd::LaunchTownDlg()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SaleWnd::SetupTabInfo(m3d::ui::Wnd*, WorkshopTabType)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SaleWnd::SetupTabItem(m3d::ui::Wnd*, WorkshopTabType)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SaleWnd::SetupListOfTabTypes()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SaleWnd::GetTabIdByTabType(WorkshopTabType) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SaleWnd::ClearTabItems()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool SaleWnd::IsTabCreatedFromRepositorySource(WorkshopTabType) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::ui::Wnd* SaleWnd::CreateTabItem(WorkshopTabType)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SaleWnd::CreateTabItems()
{
    RETRUXX_NOT_IMPLEMENTED;
}

SaleWnd::WorkshopTabType SaleWnd::GetTabTypeByTabId(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SaleWnd::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::WorkshopRepositoryType SaleWnd::GetWorkshopRepositoryType(WorkshopTabType)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SaleWnd::UpdateTownPicture()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SaleWnd::GameDataSetup()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SaleWnd::UpdateOnChangeTabSel(int, int, m3d::ui::TabWnd::SelectionType)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SaleWnd::GameDataUpdate(void*, int)
{
    // TODO: implement GameDataUpdate
    //  RETRUXX_NOT_IMPLEMENTED;
    return 0;
}
