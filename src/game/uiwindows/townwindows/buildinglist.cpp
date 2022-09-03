#include "buildinglist.h"

RT_CLASS_EXPORTS_BEGIN(BuildingButton)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(BuildingButton);

int BuildingButton::SetUpForBuilding(ai::Building const*)
{
    throw std::logic_error("Not implemented");
}

BuildingButton::~BuildingButton()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* BuildingButton::GetClass() const
{
    throw std::logic_error("Not implemented");
}

ai::Building* BuildingButton::GetBuilding() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* BuildingButton::Clone()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* BuildingButton::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* BuildingButton::GetBaseClass()
{
    return RT_CLASS_LOCAL(ButtonWnd);
}

int BuildingButton::GetIdx() const
{
    throw std::logic_error("Not implemented");
}

void BuildingButton::SetIdx(int)
{
    throw std::logic_error("Not implemented");
}

BuildingButton::BuildingButton()
{
    throw std::logic_error("Not implemented");
}

BuildingButton::BuildingButton(BuildingButton const&)
{
    throw std::logic_error("Not implemented");
}

int BuildingButton::CreateChildren()
{
    throw std::logic_error("Not implemented");
}

CStr BuildingButton::GetNameForBuilding(ai::Building const*) const
{
    throw std::logic_error("Not implemented");
}

void BuildingButton::ClearPattern()
{
    throw std::logic_error("Not implemented");
}

int BuildingButton::CreateFromPattern()
{
    throw std::logic_error("Not implemented");
}

m3d::rend::TexHandle BuildingButton::GetIcoForBuilding(ai::Building const*) const
{
    throw std::logic_error("Not implemented");
}

int BuildingButton::LoadPattern(m3d::ui::Wnd*)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORTS_BEGIN(BuildingList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(BuildingList);

BuildingList::AuxInfo::AuxInfo()
{
    throw std::logic_error("Not implemented");
}

int BuildingList::GameDataClear(bool)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* BuildingList::GetClass() const
{
    throw std::logic_error("Not implemented");
}

BuildingList::~BuildingList()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* BuildingList::Clone()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* BuildingList::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

void BuildingList::SetUpForTown(ai::Town*)
{
    throw std::logic_error("Not implemented");
}

int BuildingList::CreateFromPattern(m3d::ui::Wnd*, bool)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* BuildingList::CreateObject()
{
    throw std::logic_error("Not implemented");
}

void BuildingList::RecalcLayot()
{
    throw std::logic_error("Not implemented");
}

BuildingList::BuildingList(BuildingList const&)
{
    throw std::logic_error("Not implemented");
}

BuildingList::BuildingList()
{
    throw std::logic_error("Not implemented");
}

int BuildingList::CreateItems()
{
    throw std::logic_error("Not implemented");
}

void BuildingList::FullUpdate()
{
    throw std::logic_error("Not implemented");
}

int BuildingList::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

ai::Town const* BuildingList::GetTown() const
{
    throw std::logic_error("Not implemented");
}

void BuildingList::ClearItems()
{
    throw std::logic_error("Not implemented");
}

int BuildingList::OnAfterRemoveFromWndStation()
{
    throw std::logic_error("Not implemented");
}

int BuildingList::AddButton(BuildingButton*)
{
    throw std::logic_error("Not implemented");
}

void BuildingList::PlaceButton(BuildingButton*)
{
    throw std::logic_error("Not implemented");
}

int BuildingList::OnBeforeAddToWndStation()
{
    throw std::logic_error("Not implemented");
}
