#include "skinswnd.h"

RT_CLASS_EXPORTS_BEGIN(SkinsWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(SkinsWnd);

int SkinSwitcher::ApplySkin(int, ai::Vehicle*)
{
    throw std::logic_error("Not implemented");
}

SkinSwitcher::SkinSwitcher()
{
    throw std::logic_error("Not implemented");
}

void SkinSwitcher::ShowPrevSkin()
{
    throw std::logic_error("Not implemented");
}

int SkinSwitcher::GetMaxSkinCount(ai::Vehicle*)
{
    throw std::logic_error("Not implemented");
}

void SkinSwitcher::ShowNextSkin()
{
    throw std::logic_error("Not implemented");
}

bool SkinSwitcher::IsSkinApplicable(int, ai::Vehicle*)
{
    throw std::logic_error("Not implemented");
}

SkinSwitcher::~SkinSwitcher()
{
    throw std::logic_error("Not implemented");
}

int SkinSwitcher::GetVehicleSkin(ai::Vehicle*)
{
    throw std::logic_error("Not implemented");
}

void SkinSwitcher::Reset()
{
    throw std::logic_error("Not implemented");
}

int SkinSwitcher::GetCurSkin() const
{
    throw std::logic_error("Not implemented");
}

int SkinSwitcher::SetUp(int, m3d::ui::ButtonWnd*, m3d::ui::ButtonWnd*)
{
    throw std::logic_error("Not implemented");
}

ai::Vehicle* SkinSwitcher::GetVehicle() const
{
    throw std::logic_error("Not implemented");
}

int SkinSwitcher::GetPrevSkin() const
{
    throw std::logic_error("Not implemented");
}

int SkinSwitcher::GetNextSkin() const
{
    throw std::logic_error("Not implemented");
}

int SkinSwitcher::SetCurSkin(int)
{
    throw std::logic_error("Not implemented");
}

bool SkinSwitcher::IsValid() const
{
    throw std::logic_error("Not implemented");
}

void SkinSwitcher::UpdateSkinButtonsStates()
{
    throw std::logic_error("Not implemented");
}

SkinsWnd::AuxInfo::AuxInfo()
{
    throw std::logic_error("Not implemented");
}

int SkinsWnd::SetupForVehicle(int)
{
    throw std::logic_error("Not implemented");
}

SkinsWnd::~SkinsWnd()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* SkinsWnd::GetClass() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* SkinsWnd::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* SkinsWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Object* SkinsWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

SkinsWnd::SkinsWnd(SkinsWnd const&)
{
    throw std::logic_error("Not implemented");
}

SkinsWnd::SkinsWnd()
{
    throw std::logic_error("Not implemented");
}

void SkinsWnd::FullUpdate()
{
    throw std::logic_error("Not implemented");
}

void SkinsWnd::ShowPrevSkin()
{
    throw std::logic_error("Not implemented");
}

int SkinsWnd::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

ai::Vehicle* SkinsWnd::GetVehicle() const
{
    throw std::logic_error("Not implemented");
}

void SkinsWnd::OnChangeSkin()
{
    throw std::logic_error("Not implemented");
}

void SkinsWnd::ShowNextSkin()
{
    throw std::logic_error("Not implemented");
}

void SkinsWnd::BuySkin()
{
    throw std::logic_error("Not implemented");
}

int SkinsWnd::GameDataSetup()
{
    throw std::logic_error("Not implemented");
}

int SkinsWnd::OnBeforeAddToWndStation()
{
    throw std::logic_error("Not implemented");
}

int SkinsWnd::OnAfterRemoveFromWndStation()
{
    throw std::logic_error("Not implemented");
}

int SkinsWnd::GetSkinPrice(int, int) const
{
    throw std::logic_error("Not implemented");
}
