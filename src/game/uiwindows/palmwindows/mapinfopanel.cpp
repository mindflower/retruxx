#include "mapinfopanel.h"

RT_CLASS_DEFINE(MapInfoPanel);

MapInfoPanel::AuxInfo::AuxInfo()
{
    throw std::logic_error("Not implemented");
}

int MapInfoPanel::SetUpForObject(ObjectInfo*, bool)
{
    throw std::logic_error("Not implemented");
}

ObjectInfo* MapInfoPanel::GetObjectInfo() const
{
    throw std::logic_error("Not implemented");
}

int MapInfoPanel::SetUpForMode(LocalMapWnd::Mode)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* MapInfoPanel::GetClass() const
{
    throw std::logic_error("Not implemented");
}

MapInfoPanel::~MapInfoPanel()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* MapInfoPanel::CreateObject()
{
    throw std::logic_error("Not implemented");
}

int MapInfoPanel::SavePrices() const
{
    throw std::logic_error("Not implemented");
}

int MapInfoPanel::CreateFromPattern(m3d::ui::Wnd const*, bool)
{
    throw std::logic_error("Not implemented");
}

LocalMapWnd::Mode MapInfoPanel::GetCurrentMode() const
{
    throw std::logic_error("Not implemented");
}

m3d::Class* MapInfoPanel::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* MapInfoPanel::Clone()
{
    throw std::logic_error("Not implemented");
}

void MapInfoPanel::UpdateModeButtonsState()
{
    throw std::logic_error("Not implemented");
}

MapInfoPanel::MapInfoPanel()
{
    throw std::logic_error("Not implemented");
}

MapInfoPanel::MapInfoPanel(MapInfoPanel const&)
{
    throw std::logic_error("Not implemented");
}

int MapInfoPanel::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

int MapInfoPanel::Clear()
{
    throw std::logic_error("Not implemented");
}

int MapInfoPanel::GameDataClear(bool)
{
    throw std::logic_error("Not implemented");
}

int MapInfoPanel::OnBeforeAddToWndStation()
{
    throw std::logic_error("Not implemented");
}

void MapInfoPanel::ValidateMode()
{
    throw std::logic_error("Not implemented");
}

CStr MapInfoPanel::GetObjFullName() const
{
    throw std::logic_error("Not implemented");
}

int MapInfoPanel::SetupForClansMode()
{
    throw std::logic_error("Not implemented");
}

int MapInfoPanel::ShowClansModeControls(bool)
{
    throw std::logic_error("Not implemented");
}

int MapInfoPanel::Setup()
{
    throw std::logic_error("Not implemented");
}

int MapInfoPanel::ShowTradeModeControls(bool)
{
    throw std::logic_error("Not implemented");
}

int MapInfoPanel::SetupForTradeMode()
{
    throw std::logic_error("Not implemented");
}
