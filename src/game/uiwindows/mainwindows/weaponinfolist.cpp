#include "weaponinfolist.h"

RT_CLASS_DEFINE(WeaponInfoList);

WeaponInfoList::AuxInfo::AuxInfo()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* WeaponInfoList::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* WeaponInfoList::CreateObject()
{
    throw std::logic_error("Not implemented");
}

int WeaponInfoList::CreateFromPattern(m3d::ui::Wnd*, bool)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* WeaponInfoList::Clone()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* WeaponInfoList::GetClass() const
{
    throw std::logic_error("Not implemented");
}

WeaponInfoList::~WeaponInfoList()
{
    throw std::logic_error("Not implemented");
}

void WeaponInfoList::UpdateOnKeyBindingsChanged()
{
    throw std::logic_error("Not implemented");
}

int WeaponInfoList::RecalcLayotInMainInterface()
{
    throw std::logic_error("Not implemented");
}

int WeaponInfoList::GameDataSetup()
{
    throw std::logic_error("Not implemented");
}

int WeaponInfoList::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

WeaponInfoList::WeaponInfoList()
{
    throw std::logic_error("Not implemented");
}

WeaponInfoList::WeaponInfoList(WeaponInfoList const&)
{
    throw std::logic_error("Not implemented");
}

void WeaponInfoList::GetItemsByGroup(int, std::vector<ref_ptr<WeaponInfoWnd>>&) const
{
    throw std::logic_error("Not implemented");
}

ai::Vehicle* WeaponInfoList::GetVehicle() const
{
    throw std::logic_error("Not implemented");
}

int WeaponInfoList::GetNumOfGroups() const
{
    throw std::logic_error("Not implemented");
}

int WeaponInfoList::CreateItems()
{
    throw std::logic_error("Not implemented");
}

int WeaponInfoList::RecalcLayot()
{
    throw std::logic_error("Not implemented");
}

int WeaponInfoList::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

int WeaponInfoList::AddGroupButton(int, BoundsBase<float> const&, WeaponInfoWnd const*, int)
{
    throw std::logic_error("Not implemented");
}

bool WeaponInfoList::CanGunShotToTarget(ai::Obj const*) const
{
    throw std::logic_error("Not implemented");
}

int WeaponInfoList::UpdateOnVehiclePartChanged(void*)
{
    throw std::logic_error("Not implemented");
}

void WeaponInfoList::UpdateOnWeaponGroupChanged()
{
    throw std::logic_error("Not implemented");
}

m3d::rend::TexHandle WeaponInfoList::GetGroupTextureForWeaponBtn(WeaponInfoWnd const*, int)
{
    throw std::logic_error("Not implemented");
}

int WeaponInfoList::RecalcLayotInCharacteristicWnd()
{
    throw std::logic_error("Not implemented");
}

int WeaponInfoList::GetGroupByVehiclePartName(CStr const&) const
{
    throw std::logic_error("Not implemented");
}

int WeaponInfoList::OnBeforeAddToWndStation()
{
    throw std::logic_error("Not implemented");
}

int WeaponInfoList::UpdateOnPlayerVehicleChanged()
{
    throw std::logic_error("Not implemented");
}

int WeaponInfoList::AddItem(int, int)
{
    throw std::logic_error("Not implemented");
}

void WeaponInfoList::InitGroupTextures()
{
    throw std::logic_error("Not implemented");
}

void WeaponInfoList::ReleaseGroupTextures()
{
    throw std::logic_error("Not implemented");
}

void WeaponInfoList::SetTooltipForGroupButton(m3d::ui::Wnd*)
{
    throw std::logic_error("Not implemented");
}

int WeaponInfoList::GameDataClear(bool)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_DEFINE(WeaponInfoWnd);

int WeaponInfoWnd::GetGroupId() const
{
    throw std::logic_error("Not implemented");
}

ai::Obj const* WeaponInfoWnd::GetGun() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* WeaponInfoWnd::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* WeaponInfoWnd::GetClass() const
{
    throw std::logic_error("Not implemented");
}

m3d::Class* WeaponInfoWnd::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

WeaponInfoWnd::~WeaponInfoWnd()
{
    throw std::logic_error("Not implemented");
}

int WeaponInfoWnd::SetupForGun(int, int, WeaponInfoList::Type)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* WeaponInfoWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

WeaponInfoWnd::WeaponInfoWnd()
{
    throw std::logic_error("Not implemented");
}

WeaponInfoWnd::WeaponInfoWnd(WeaponInfoWnd const&)
{
    throw std::logic_error("Not implemented");
}

m3d::rend::TexHandle WeaponInfoWnd::GetIco() const
{
    throw std::logic_error("Not implemented");
}

int WeaponInfoWnd::GetAmmo() const
{
    throw std::logic_error("Not implemented");
}

void WeaponInfoWnd::UpdateAmmo()
{
    throw std::logic_error("Not implemented");
}

void WeaponInfoWnd::UpdateCanShotBg()
{
    throw std::logic_error("Not implemented");
}

void WeaponInfoWnd::UpdateOnNewFrame()
{
    throw std::logic_error("Not implemented");
}

int WeaponInfoWnd::CreateFromPattern(WeaponInfoList::Type)
{
    throw std::logic_error("Not implemented");
}

m3d::rend::TexHandle WeaponInfoWnd::GetBarTexture(bool) const
{
    throw std::logic_error("Not implemented");
}

int WeaponInfoWnd::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

int WeaponInfoWnd::GameDataClear(bool)
{
    throw std::logic_error("Not implemented");
}

ai::DamageType WeaponInfoWnd::GetWeaponTypeForGun(ai::Obj const*) const
{
    throw std::logic_error("Not implemented");
}

void WeaponInfoWnd::UpdateCharge()
{
    throw std::logic_error("Not implemented");
}

void WeaponInfoWnd::ClearPattern(WeaponInfoList::Type)
{
    throw std::logic_error("Not implemented");
}

int WeaponInfoWnd::LoadPattern(m3d::ui::Wnd*, WeaponInfoList::Type)
{
    throw std::logic_error("Not implemented");
}

void WeaponInfoWnd::UpdateIco()
{
    throw std::logic_error("Not implemented");
}

bool WeaponInfoWnd::IsGunReady() const
{
    throw std::logic_error("Not implemented");
}

int WeaponInfoWnd::CreateChildren(WeaponInfoList::Type)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_DEFINE(WeaponGroupChoiceDlg);

WeaponGroupChoiceDlg::AuxInfo::AuxInfo()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* WeaponGroupChoiceDlg::Clone()
{
    throw std::logic_error("Not implemented");
}

CStr WeaponGroupChoiceDlg::CreateTooltipForWeaponGroup(int)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* WeaponGroupChoiceDlg::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* WeaponGroupChoiceDlg::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

WeaponGroupChoiceDlg::~WeaponGroupChoiceDlg()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* WeaponGroupChoiceDlg::GetClass() const
{
    throw std::logic_error("Not implemented");
}

WeaponGroupChoiceDlg::WeaponGroupChoiceDlg()
{
    throw std::logic_error("Not implemented");
}

WeaponGroupChoiceDlg::WeaponGroupChoiceDlg(WeaponGroupChoiceDlg const&)
{
    throw std::logic_error("Not implemented");
}

int WeaponGroupChoiceDlg::GameDataSetup()
{
    throw std::logic_error("Not implemented");
}

void WeaponGroupChoiceDlg::SelectButton(int)
{
    throw std::logic_error("Not implemented");
}

int WeaponGroupChoiceDlg::OnKey(unsigned short, unsigned char, unsigned)
{
    throw std::logic_error("Not implemented");
}

void WeaponGroupChoiceDlg::OnKeyBindingsChanged()
{
    throw std::logic_error("Not implemented");
}

int WeaponGroupChoiceDlg::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

int WeaponGroupChoiceDlg::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}
