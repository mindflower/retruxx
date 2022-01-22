#include "localmapwnd.h"
#include "saveselllist.h"
#include "mapinfopanel.h"
#include "localchartwnd.h"
#include "mapcombobox.h"

RT_CLASS_DEFINE(LocalMapWnd);

LocalMapWnd::AuxInfo::AuxInfo()
{
    throw std::logic_error("Not implemented");
}

int LocalMapWnd::SetUpForLevel(CStr const&)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* LocalMapWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

CStr LocalMapWnd::GetCurrentLevelName() const
{
    throw std::logic_error("Not implemented");
}

m3d::Class* LocalMapWnd::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* LocalMapWnd::GetClass() const
{
    throw std::logic_error("Not implemented");
}

bool LocalMapWnd::IsObjectInfoValidForPrices(ObjectInfo*)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* LocalMapWnd::CreateObject()
{
    throw std::logic_error("Not implemented");
}

LocalMapWnd::~LocalMapWnd()
{
    throw std::logic_error("Not implemented");
}

ObjectInfo* LocalMapWnd::GetCurrentObjectInfo() const
{
    throw std::logic_error("Not implemented");
}

LocalMapWnd::Mode LocalMapWnd::GetCurrentMode() const
{
    throw std::logic_error("Not implemented");
}

int LocalMapWnd::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

int LocalMapWnd::GameDataSetup()
{
    throw std::logic_error("Not implemented");
}

void LocalMapWnd::OnBtnEnableObjectNamesClick(m3d::ui::Wnd*, unsigned, m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

void LocalMapWnd::UpdateBtnAddTownToSavedListState()
{
    throw std::logic_error("Not implemented");
}

bool LocalMapWnd::IsObjectInfoSavedList(ObjectInfo*) const
{
    throw std::logic_error("Not implemented");
}

void LocalMapWnd::OnNavPointAdded(void*)
{
    throw std::logic_error("Not implemented");
}

int LocalMapWnd::SavePricesForCurrentObjectInfo()
{
    throw std::logic_error("Not implemented");
}

int LocalMapWnd::GetUserLocationNavPointId() const
{
    throw std::logic_error("Not implemented");
}

void LocalMapWnd::OnBtnNavPointClick(m3d::ui::Wnd*, unsigned, m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

int LocalMapWnd::AddNavPoint(CVector const&, bool)
{
    throw std::logic_error("Not implemented");
}

int LocalMapWnd::AddNavPoint(ObjectInfo*, bool)
{
    throw std::logic_error("Not implemented");
}

void LocalMapWnd::UpdateSavedListSelection()
{
    throw std::logic_error("Not implemented");
}

bool LocalMapWnd::CanUserLocationNavPointBeAddedFromBtn() const
{
    throw std::logic_error("Not implemented");
}

void LocalMapWnd::UpdateBtnEnableObjectNamesState()
{
    throw std::logic_error("Not implemented");
}

ObjectInfo* LocalMapWnd::GetInfoForNearestVisibleObject() const
{
    throw std::logic_error("Not implemented");
}

void LocalMapWnd::ShowInfo(ObjectInfo*, bool)
{
    throw std::logic_error("Not implemented");
}

int LocalMapWnd::OnAfterAddToWndStation()
{
    throw std::logic_error("Not implemented");
}

void LocalMapWnd::GetBtnEnableObjectNamesTexNames(bool, CStr&, CStr&, CStr&) const
{
    throw std::logic_error("Not implemented");
}

void LocalMapWnd::GetBtnAddTownToSavedListTexNames(bool, CStr&, CStr&, CStr&) const
{
    throw std::logic_error("Not implemented");
}

bool LocalMapWnd::CanUserLocationNavPointBeDeletedFromBtn() const
{
    throw std::logic_error("Not implemented");
}

void LocalMapWnd::EnsureShowInfo()
{
    throw std::logic_error("Not implemented");
}

int LocalMapWnd::RemoveSavedPricesForCurrentObjectInfo()
{
    throw std::logic_error("Not implemented");
}

void LocalMapWnd::UpdateBtnNavPointState()
{
    throw std::logic_error("Not implemented");
}

bool LocalMapWnd::IsObjectNamesEnabled() const
{
    throw std::logic_error("Not implemented");
}

void LocalMapWnd::GetBtnNavPointTexNames(bool, CStr&, CStr&, CStr&) const
{
    throw std::logic_error("Not implemented");
}

void LocalMapWnd::OnObjectInfoChanged()
{
    throw std::logic_error("Not implemented");
}

void LocalMapWnd::LaunchGlobalMap()
{
    throw std::logic_error("Not implemented");
}

int LocalMapWnd::DeleteNavPoint(int, bool)
{
    throw std::logic_error("Not implemented");
}

void LocalMapWnd::EnableObjectNames(bool)
{
    throw std::logic_error("Not implemented");
}

void LocalMapWnd::OnNavPointDeleted(void*)
{
    throw std::logic_error("Not implemented");
}

void LocalMapWnd::ValidateSavedObjectInfos()
{
    throw std::logic_error("Not implemented");
}

LocalMapWnd::LocalMapWnd(LocalMapWnd const&)
{
    throw std::logic_error("Not implemented");
}

LocalMapWnd::LocalMapWnd()
{
    throw std::logic_error("Not implemented");
}

void LocalMapWnd::OnBtnAddTownToSavedListClick(m3d::ui::Wnd*, unsigned, m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

int LocalMapWnd::OnBeforeAddToWndStation()
{
    throw std::logic_error("Not implemented");
}

int LocalMapWnd::ChangeMap()
{
    throw std::logic_error("Not implemented");
}

int LocalMapWnd::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}
