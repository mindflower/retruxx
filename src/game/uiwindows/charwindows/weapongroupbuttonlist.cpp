#include "weapongroupbuttonlist.h"

RT_CLASS_DEFINE(WeaponGroupButton);
RT_CLASS_DEFINE(WeaponGroupButtonList);

WeaponGroupButton::AuxInfo::AuxInfo()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* WeaponGroupButton::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* WeaponGroupButton::GetClass() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* WeaponGroupButton::Clone()
{
    throw std::logic_error("Not implemented");
}

int WeaponGroupButton::GetGroupId() const
{
    throw std::logic_error("Not implemented");
}

void WeaponGroupButton::SetState(State)
{
    throw std::logic_error("Not implemented");
}

WeaponGroupButton::~WeaponGroupButton()
{
    throw std::logic_error("Not implemented");
}

int WeaponGroupButton::SetupForGroup(int)
{
    throw std::logic_error("Not implemented");
}

WeaponGroupButton::State WeaponGroupButton::GetState() const
{
    throw std::logic_error("Not implemented");
}

m3d::Class* WeaponGroupButton::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

void WeaponGroupButton::UpdateTextures()
{
    throw std::logic_error("Not implemented");
}

WeaponGroupButton::WeaponGroupButton()
{
    throw std::logic_error("Not implemented");
}

WeaponGroupButton::WeaponGroupButton(WeaponGroupButton const&)
{
    throw std::logic_error("Not implemented");
}

void WeaponGroupButton::GetTexturesByState(State, int, m3d::rend::TexHandle&, m3d::rend::TexHandle&, m3d::rend::TexHandle&) const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* WeaponGroupButtonList::Clone()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* WeaponGroupButtonList::GetClass() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* WeaponGroupButtonList::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* WeaponGroupButtonList::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

WeaponGroupButtonList::~WeaponGroupButtonList()
{
    throw std::logic_error("Not implemented");
}

int WeaponGroupButtonList::SetupForGunPart(CStr const&, int)
{
    throw std::logic_error("Not implemented");
}

ai::Vehicle const* WeaponGroupButtonList::GetVehicle() const
{
    throw std::logic_error("Not implemented");
}

int WeaponGroupButtonList::GetCurGroupId() const
{
    throw std::logic_error("Not implemented");
}

void WeaponGroupButtonList::UpdateCurGroupId()
{
    throw std::logic_error("Not implemented");
}

int WeaponGroupButtonList::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

int WeaponGroupButtonList::LoadPattern(m3d::ui::Wnd*)
{
    throw std::logic_error("Not implemented");
}

int WeaponGroupButtonList::CreateChildren()
{
    throw std::logic_error("Not implemented");
}

void WeaponGroupButtonList::OnVehiclePartChanged(void*)
{
    throw std::logic_error("Not implemented");
}

int WeaponGroupButtonList::CreateFromPattern()
{
    throw std::logic_error("Not implemented");
}

int WeaponGroupButtonList::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

void WeaponGroupButtonList::OnBtnWeaponGroupClick(m3d::ui::Wnd*, int, m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

WeaponGroupButtonList::WeaponGroupButtonList(WeaponGroupButtonList const&)
{
    throw std::logic_error("Not implemented");
}

WeaponGroupButtonList::WeaponGroupButtonList()
{
    throw std::logic_error("Not implemented");
}

bool WeaponGroupButtonList::IsSetForGun() const
{
    throw std::logic_error("Not implemented");
}

void WeaponGroupButtonList::AddWeaponToGroup(int)
{
    throw std::logic_error("Not implemented");
}

void WeaponGroupButtonList::UpdateButtonsState()
{
    throw std::logic_error("Not implemented");
}

void WeaponGroupButtonList::OnKeyBindingsChanged()
{
    throw std::logic_error("Not implemented");
}

ai::VehiclePart const* WeaponGroupButtonList::GetGun() const
{
    throw std::logic_error("Not implemented");
}

void WeaponGroupButtonList::UpdateButtonsTooltips()
{
    throw std::logic_error("Not implemented");
}

void WeaponGroupButtonList::OnWeaponGroupChanged()
{
    throw std::logic_error("Not implemented");
}
