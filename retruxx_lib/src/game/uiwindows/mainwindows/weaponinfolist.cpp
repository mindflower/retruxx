#include "weaponinfolist.h"

RT_CLASS_EXPORTS_BEGIN(WeaponInfoList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(WeaponInfoList);

WeaponInfoList::AuxInfo::AuxInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* WeaponInfoList::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Object* WeaponInfoList::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponInfoList::CreateFromPattern(m3d::ui::Wnd*, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* WeaponInfoList::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* WeaponInfoList::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

WeaponInfoList::~WeaponInfoList()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponInfoList::UpdateOnKeyBindingsChanged()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponInfoList::RecalcLayotInMainInterface()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponInfoList::GameDataSetup()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponInfoList::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

WeaponInfoList::WeaponInfoList()
{
    RETRUXX_NOT_IMPLEMENTED;
}

WeaponInfoList::WeaponInfoList(WeaponInfoList const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponInfoList::GetItemsByGroup(int, std::vector<ref_ptr<WeaponInfoWnd>>&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Vehicle* WeaponInfoList::GetVehicle() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponInfoList::GetNumOfGroups() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponInfoList::CreateItems()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponInfoList::RecalcLayot()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponInfoList::GameDataUpdate(void*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponInfoList::AddGroupButton(int, BoundsBase<float> const&, WeaponInfoWnd const*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool WeaponInfoList::CanGunShotToTarget(ai::Obj const*) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponInfoList::UpdateOnVehiclePartChanged(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponInfoList::UpdateOnWeaponGroupChanged()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::rend::TexHandle WeaponInfoList::GetGroupTextureForWeaponBtn(WeaponInfoWnd const*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponInfoList::RecalcLayotInCharacteristicWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponInfoList::GetGroupByVehiclePartName(CStr const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponInfoList::OnBeforeAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponInfoList::UpdateOnPlayerVehicleChanged()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponInfoList::AddItem(int, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponInfoList::InitGroupTextures()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponInfoList::ReleaseGroupTextures()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponInfoList::SetTooltipForGroupButton(m3d::ui::Wnd*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponInfoList::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORTS_BEGIN(WeaponInfoWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(WeaponInfoWnd);

int WeaponInfoWnd::GetGroupId() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Obj const* WeaponInfoWnd::GetGun() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* WeaponInfoWnd::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* WeaponInfoWnd::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* WeaponInfoWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

WeaponInfoWnd::~WeaponInfoWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponInfoWnd::SetupForGun(int, int, WeaponInfoList::Type)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* WeaponInfoWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

WeaponInfoWnd::WeaponInfoWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

WeaponInfoWnd::WeaponInfoWnd(WeaponInfoWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::rend::TexHandle WeaponInfoWnd::GetIco() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponInfoWnd::GetAmmo() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponInfoWnd::UpdateAmmo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponInfoWnd::UpdateCanShotBg()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponInfoWnd::UpdateOnNewFrame()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponInfoWnd::CreateFromPattern(WeaponInfoList::Type)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::rend::TexHandle WeaponInfoWnd::GetBarTexture(bool) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponInfoWnd::GameDataUpdate(void*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponInfoWnd::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::DamageType WeaponInfoWnd::GetWeaponTypeForGun(ai::Obj const*) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponInfoWnd::UpdateCharge()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponInfoWnd::ClearPattern(WeaponInfoList::Type)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponInfoWnd::LoadPattern(m3d::ui::Wnd*, WeaponInfoList::Type)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponInfoWnd::UpdateIco()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool WeaponInfoWnd::IsGunReady() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponInfoWnd::CreateChildren(WeaponInfoList::Type)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORTS_BEGIN(WeaponGroupChoiceDlg)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(WeaponGroupChoiceDlg);

WeaponGroupChoiceDlg::AuxInfo::AuxInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* WeaponGroupChoiceDlg::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr WeaponGroupChoiceDlg::CreateTooltipForWeaponGroup(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* WeaponGroupChoiceDlg::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* WeaponGroupChoiceDlg::GetBaseClass()
{
    return RT_CLASS_LOCAL(ModalWnd);
}

WeaponGroupChoiceDlg::~WeaponGroupChoiceDlg()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* WeaponGroupChoiceDlg::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

WeaponGroupChoiceDlg::WeaponGroupChoiceDlg()
{
    RETRUXX_NOT_IMPLEMENTED;
}

WeaponGroupChoiceDlg::WeaponGroupChoiceDlg(WeaponGroupChoiceDlg const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponGroupChoiceDlg::GameDataSetup()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponGroupChoiceDlg::SelectButton(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponGroupChoiceDlg::OnKey(unsigned short, unsigned char, unsigned)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponGroupChoiceDlg::OnKeyBindingsChanged()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponGroupChoiceDlg::GameDataUpdate(void*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponGroupChoiceDlg::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}
