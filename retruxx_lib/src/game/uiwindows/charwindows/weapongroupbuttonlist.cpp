#include "weapongroupbuttonlist.h"

RT_CLASS_EXPORTS_BEGIN(WeaponGroupButton)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(WeaponGroupButton);

RT_CLASS_EXPORTS_BEGIN(WeaponGroupButtonList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(WeaponGroupButtonList);

WeaponGroupButton::AuxInfo::AuxInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* WeaponGroupButton::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* WeaponGroupButton::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* WeaponGroupButton::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponGroupButton::GetGroupId() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponGroupButton::SetState(State)
{
    RETRUXX_NOT_IMPLEMENTED;
}

WeaponGroupButton::~WeaponGroupButton()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponGroupButton::SetupForGroup(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

WeaponGroupButton::State WeaponGroupButton::GetState() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* WeaponGroupButton::GetBaseClass()
{
    return RT_CLASS_LOCAL(ButtonWnd);
}

void WeaponGroupButton::UpdateTextures()
{
    RETRUXX_NOT_IMPLEMENTED;
}

WeaponGroupButton::WeaponGroupButton()
{
    RETRUXX_NOT_IMPLEMENTED;
}

WeaponGroupButton::WeaponGroupButton(WeaponGroupButton const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponGroupButton::GetTexturesByState(State, int, m3d::rend::TexHandle&, m3d::rend::TexHandle&, m3d::rend::TexHandle&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* WeaponGroupButtonList::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* WeaponGroupButtonList::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* WeaponGroupButtonList::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* WeaponGroupButtonList::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

WeaponGroupButtonList::~WeaponGroupButtonList()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponGroupButtonList::SetupForGunPart(CStr const&, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Vehicle const* WeaponGroupButtonList::GetVehicle() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponGroupButtonList::GetCurGroupId() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponGroupButtonList::UpdateCurGroupId()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponGroupButtonList::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponGroupButtonList::LoadPattern(m3d::ui::Wnd*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponGroupButtonList::CreateChildren()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponGroupButtonList::OnVehiclePartChanged(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponGroupButtonList::CreateFromPattern()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponGroupButtonList::GameDataUpdate(void*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponGroupButtonList::OnBtnWeaponGroupClick(m3d::ui::Wnd*, int, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

WeaponGroupButtonList::WeaponGroupButtonList(WeaponGroupButtonList const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

WeaponGroupButtonList::WeaponGroupButtonList()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool WeaponGroupButtonList::IsSetForGun() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponGroupButtonList::AddWeaponToGroup(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponGroupButtonList::UpdateButtonsState()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponGroupButtonList::OnKeyBindingsChanged()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::VehiclePart const* WeaponGroupButtonList::GetGun() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponGroupButtonList::UpdateButtonsTooltips()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponGroupButtonList::OnWeaponGroupChanged()
{
    RETRUXX_NOT_IMPLEMENTED;
}
