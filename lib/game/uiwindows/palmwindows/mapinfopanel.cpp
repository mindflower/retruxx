#include "mapinfopanel.h"

RT_CLASS_EXPORTS_BEGIN(MapInfoPanel)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(MapInfoPanel);

MapInfoPanel::AuxInfo::AuxInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MapInfoPanel::SetUpForObject(ObjectInfo*, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ObjectInfo* MapInfoPanel::GetObjectInfo() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MapInfoPanel::SetUpForMode(LocalMapWnd::Mode)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* MapInfoPanel::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

MapInfoPanel::~MapInfoPanel()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* MapInfoPanel::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MapInfoPanel::SavePrices() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MapInfoPanel::CreateFromPattern(m3d::ui::Wnd const*, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

LocalMapWnd::Mode MapInfoPanel::GetCurrentMode() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* MapInfoPanel::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Object* MapInfoPanel::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MapInfoPanel::UpdateModeButtonsState()
{
    RETRUXX_NOT_IMPLEMENTED;
}

MapInfoPanel::MapInfoPanel()
{
    RETRUXX_NOT_IMPLEMENTED;
}

MapInfoPanel::MapInfoPanel(MapInfoPanel const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MapInfoPanel::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MapInfoPanel::Clear()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MapInfoPanel::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MapInfoPanel::OnBeforeAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MapInfoPanel::ValidateMode()
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr MapInfoPanel::GetObjFullName() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MapInfoPanel::SetupForClansMode()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MapInfoPanel::ShowClansModeControls(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MapInfoPanel::Setup()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MapInfoPanel::ShowTradeModeControls(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MapInfoPanel::SetupForTradeMode()
{
    RETRUXX_NOT_IMPLEMENTED;
}
