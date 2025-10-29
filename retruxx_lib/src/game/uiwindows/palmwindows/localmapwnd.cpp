#include "localmapwnd.h"
#include "saveselllist.h"
#include "mapinfopanel.h"
#include "localchartwnd.h"
#include "mapcombobox.h"

RT_CLASS_EXPORTS_BEGIN(LocalMapWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(LocalMapWnd);

LocalMapWnd::AuxInfo::AuxInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int LocalMapWnd::SetUpForLevel(CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* LocalMapWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr LocalMapWnd::GetCurrentLevelName() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* LocalMapWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ScreenWnd);
}

m3d::Class* LocalMapWnd::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool LocalMapWnd::IsObjectInfoValidForPrices(ObjectInfo*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* LocalMapWnd::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

LocalMapWnd::~LocalMapWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ObjectInfo* LocalMapWnd::GetCurrentObjectInfo() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

LocalMapWnd::Mode LocalMapWnd::GetCurrentMode() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int LocalMapWnd::GameDataUpdate(void*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int LocalMapWnd::GameDataSetup()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void LocalMapWnd::OnBtnEnableObjectNamesClick(m3d::ui::Wnd*, unsigned, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void LocalMapWnd::UpdateBtnAddTownToSavedListState()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool LocalMapWnd::IsObjectInfoSavedList(ObjectInfo*) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void LocalMapWnd::OnNavPointAdded(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int LocalMapWnd::SavePricesForCurrentObjectInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int LocalMapWnd::GetUserLocationNavPointId() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void LocalMapWnd::OnBtnNavPointClick(m3d::ui::Wnd*, unsigned, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int LocalMapWnd::AddNavPoint(CVector const&, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int LocalMapWnd::AddNavPoint(ObjectInfo*, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void LocalMapWnd::UpdateSavedListSelection()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool LocalMapWnd::CanUserLocationNavPointBeAddedFromBtn() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void LocalMapWnd::UpdateBtnEnableObjectNamesState()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ObjectInfo* LocalMapWnd::GetInfoForNearestVisibleObject() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void LocalMapWnd::ShowInfo(ObjectInfo*, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int LocalMapWnd::OnAfterAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void LocalMapWnd::GetBtnEnableObjectNamesTexNames(bool, CStr&, CStr&, CStr&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void LocalMapWnd::GetBtnAddTownToSavedListTexNames(bool, CStr&, CStr&, CStr&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool LocalMapWnd::CanUserLocationNavPointBeDeletedFromBtn() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void LocalMapWnd::EnsureShowInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int LocalMapWnd::RemoveSavedPricesForCurrentObjectInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void LocalMapWnd::UpdateBtnNavPointState()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool LocalMapWnd::IsObjectNamesEnabled() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void LocalMapWnd::GetBtnNavPointTexNames(bool, CStr&, CStr&, CStr&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void LocalMapWnd::OnObjectInfoChanged()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void LocalMapWnd::LaunchGlobalMap()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int LocalMapWnd::DeleteNavPoint(int, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void LocalMapWnd::EnableObjectNames(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void LocalMapWnd::OnNavPointDeleted(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void LocalMapWnd::ValidateSavedObjectInfos()
{
    RETRUXX_NOT_IMPLEMENTED;
}

LocalMapWnd::LocalMapWnd(LocalMapWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

LocalMapWnd::LocalMapWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void LocalMapWnd::OnBtnAddTownToSavedListClick(m3d::ui::Wnd*, unsigned, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int LocalMapWnd::OnBeforeAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int LocalMapWnd::ChangeMap()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int LocalMapWnd::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}
