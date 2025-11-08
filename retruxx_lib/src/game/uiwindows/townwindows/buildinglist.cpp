#include "buildinglist.h"

RT_CLASS_EXPORTS_BEGIN(BuildingButton)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(BuildingButton);

int BuildingButton::SetUpForBuilding(ai::Building const*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

BuildingButton::~BuildingButton()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* BuildingButton::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Building* BuildingButton::GetBuilding() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* BuildingButton::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* BuildingButton::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* BuildingButton::GetBaseClass()
{
    return RT_CLASS_LOCAL(ButtonWnd);
}

int BuildingButton::GetIdx() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void BuildingButton::SetIdx(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

BuildingButton::BuildingButton()
{
    RETRUXX_NOT_IMPLEMENTED;
}

BuildingButton::BuildingButton(BuildingButton const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BuildingButton::CreateChildren()
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr BuildingButton::GetNameForBuilding(ai::Building const*) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void BuildingButton::ClearPattern()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BuildingButton::CreateFromPattern()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::rend::TexHandle BuildingButton::GetIcoForBuilding(ai::Building const*) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BuildingButton::LoadPattern(m3d::ui::Wnd*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORTS_BEGIN(BuildingList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(BuildingList);

BuildingList::AuxInfo::AuxInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BuildingList::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* BuildingList::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

BuildingList::~BuildingList()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* BuildingList::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* BuildingList::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

void BuildingList::SetUpForTown(ai::Town*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BuildingList::CreateFromPattern(m3d::ui::Wnd*, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* BuildingList::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void BuildingList::RecalcLayot()
{
    RETRUXX_NOT_IMPLEMENTED;
}

BuildingList::BuildingList(BuildingList const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

BuildingList::BuildingList()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BuildingList::CreateItems()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void BuildingList::FullUpdate()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BuildingList::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Town const* BuildingList::GetTown() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void BuildingList::ClearItems()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BuildingList::OnAfterRemoveFromWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BuildingList::AddButton(BuildingButton*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void BuildingList::PlaceButton(BuildingButton*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BuildingList::OnBeforeAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}
