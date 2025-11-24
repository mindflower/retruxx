#include "skinswnd.h"

RT_CLASS_EXPORTS_BEGIN(SkinsWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(SkinsWnd);

int SkinSwitcher::ApplySkin(int, ai::Vehicle*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

SkinSwitcher::SkinSwitcher()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SkinSwitcher::ShowPrevSkin()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SkinSwitcher::GetMaxSkinCount(ai::Vehicle*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SkinSwitcher::ShowNextSkin()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool SkinSwitcher::IsSkinApplicable(int, ai::Vehicle*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

SkinSwitcher::~SkinSwitcher()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SkinSwitcher::GetVehicleSkin(ai::Vehicle*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SkinSwitcher::Reset()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SkinSwitcher::GetCurSkin() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SkinSwitcher::SetUp(int, m3d::ui::ButtonWnd*, m3d::ui::ButtonWnd*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Vehicle* SkinSwitcher::GetVehicle() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SkinSwitcher::GetPrevSkin() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SkinSwitcher::GetNextSkin() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SkinSwitcher::SetCurSkin(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool SkinSwitcher::IsValid() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SkinSwitcher::UpdateSkinButtonsStates()
{
    RETRUXX_NOT_IMPLEMENTED;
}

SkinsWnd::AuxInfo::AuxInfo()
{
    m_btnPrevName = "btnPrev";
    m_btnNextName = "btnNext";
    m_btnBuyName = "btnBuy";
    m_wndPriceName = "wndPrice";
}

int SkinsWnd::SetupForVehicle(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

SkinsWnd::~SkinsWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* SkinsWnd::GetClass() const
{
    return RT_CLASS_LOCAL(SkinsWnd);
}

m3d::Object* SkinsWnd::CreateObject()
{
    return new SkinsWnd;
}

m3d::Class* SkinsWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Object* SkinsWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

SkinsWnd::SkinsWnd(SkinsWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

SkinsWnd::SkinsWnd()
{
    m_skinSwitcher = 0;
    m_btnPrev = 0;
    m_btnNext = 0;
    m_btnBuy = 0;
    m_wndPrice = 0;
    m_vehicleId = -1;
    m_savedSkinId = -1;
}

void SkinsWnd::FullUpdate()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SkinsWnd::ShowPrevSkin()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SkinsWnd::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Vehicle* SkinsWnd::GetVehicle() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SkinsWnd::OnChangeSkin()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SkinsWnd::ShowNextSkin()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SkinsWnd::BuySkin()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SkinsWnd::GameDataSetup()
{
    // TODO: implement SkinsWnd::GameDataSetup
    // RETRUXX_NOT_IMPLEMENTED;
    return 1;
}

int SkinsWnd::OnBeforeAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SkinsWnd::OnAfterRemoveFromWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SkinsWnd::GetSkinPrice(int, int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}
