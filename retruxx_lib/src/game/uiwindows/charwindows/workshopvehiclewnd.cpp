#include "workshopvehiclewnd.h"

RT_CLASS_EXPORTS_BEGIN(WorkshopVehicleWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(WorkshopVehicleWnd);

WorkshopVehicleWnd::WAuxInfo::WAuxInfo()
{
    m_btnPrevName = "btnPrev";
    m_btnNextName = "btnNext";
    m_btnSkinNextName = "btnSkinNext";
    m_btnSkinPrevName = "btnSkinPrev";
    m_wndPriceName = "wndPrice";
}

ai::Workshop* WorkshopVehicleWnd::GetWorkshop() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

WorkshopVehicleWnd::~WorkshopVehicleWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* WorkshopVehicleWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(VehicleWnd);
}

int WorkshopVehicleWnd::GetWorkshopId() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* WorkshopVehicleWnd::CreateObject()
{
    return new WorkshopVehicleWnd;
}

void WorkshopVehicleWnd::OnRestoreStyles()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WorkshopVehicleWnd::SetupForWorkshop(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* WorkshopVehicleWnd::GetClass() const
{
    return RT_CLASS_LOCAL(WorkshopVehicleWnd);
}

m3d::Object* WorkshopVehicleWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WorkshopVehicleWnd::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WorkshopVehicleWnd::GameDataSetup()
{
    // TODO: implememnt WorkshopVehicleWnd::GameDataSetup
    // RETRUXX_NOT_IMPLEMENTED;
    return 1;
}

WorkshopVehicleWnd::WorkshopVehicleWnd(WorkshopVehicleWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

WorkshopVehicleWnd::WorkshopVehicleWnd()
{
    m_btnPrev = 0;
    m_btnNext = 0;
    m_btnSkinNext = 0;
    m_btnSkinPrev = 0;
    m_skinSwitcher = 0;
    m_wndPrice = 0;
    m_workshopId = -1;
}

void WorkshopVehicleWnd::LaunchNewVehicleInventoryWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WorkshopVehicleWnd::FullUpdate()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WorkshopVehicleWnd::GetNextObjId(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WorkshopVehicleWnd::OnFinishTrade(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WorkshopVehicleWnd::OnRepositoryChanged()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WorkshopVehicleWnd::UpdateNextPrevButtonsStates()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WorkshopVehicleWnd::AddInfoToEncyclopaedia()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WorkshopVehicleWnd::MakeVehiclesList()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WorkshopVehicleWnd::GetPrevObjId(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WorkshopVehicleWnd::OnAfterAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WorkshopVehicleWnd::OnBeforeAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WorkshopVehicleWnd::ShowNextVehicle()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WorkshopVehicleWnd::Hide()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WorkshopVehicleWnd::GameDataUpdate(void*, int)
{
    // TODO: implement GarageWnd::GameDataUpdate
    // RETRUXX_NOT_IMPLEMENTED;
    return 0;
}

void WorkshopVehicleWnd::SetVehicleId(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WorkshopVehicleWnd::ShowPrevVehicle()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WorkshopVehicleWnd::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::GeomRepository* WorkshopVehicleWnd::GetWorkshopRepository() const
{
    RETRUXX_NOT_IMPLEMENTED;
}
