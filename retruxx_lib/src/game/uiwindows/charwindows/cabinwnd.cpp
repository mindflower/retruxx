#include "cabinwnd.h"
#include "gadgetwnd.h"
#include "core/log.h"

RT_CLASS_EXPORTS_BEGIN(CabinWnd)
    RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(CabinWnd);

CabinWnd::AuxInfo::AuxInfo()
{
    m_cabinFirstGunOrigin.x = 368.0;
    m_cabinFirstGunOrigin.y = 70.0;
    m_cabinSecondGunOrigin.x = 144.0;
    m_cabinSecondGunOrigin.y = 112.0;
    m_gadgetCommonName = "wndGadgetCommonBg";
    m_gadgetWeaponName = "wndGadgetWeaponBg";
}

m3d::Object* CabinWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* CabinWnd::CreateObject()
{
    return new CabinWnd;
}

m3d::Class* CabinWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(CBWnd);
}

CabinWnd::~CabinWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* CabinWnd::GetClass() const
{
    return RT_CLASS_LOCAL(CabinWnd);
}

CabinWnd::CabinWnd()
{
    m_mainPartName = "CABIN";
    m_wndGadgets.resize(0xA);
}

CabinWnd::CabinWnd(CabinWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CabinWnd::CreateGadgetWnd(int)
{
    // TODO: implement CabinWnd::CabinWnd
    //RETRUXX_NOT_IMPLEMENTED;
    return 1;
}

InventoryWnd::VehicleType CabinWnd::GetVehicleTypeByGuiId(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CabinWnd::GameDataSetup()
{
    if (CBWnd::GameDataSetup())
    {
        auto res = 1;
        if ((this->m_gameDataFlags & 2) == 0)
        {
            for (int i = 0; i < 10; ++i)
                res &= CabinWnd::CreateGadgetWnd(i);
            if (!res)
                this->m_gameDataFlags &= ~1u;
        }
        if ((this->m_gameDataFlags & 1) != 0)
            return 1;

        M3D_LOG_ERR("CabinWnd: error - fail to init because of a bad resource");
    }
    return 0;
}

ref_ptr<GadgetWnd> CabinWnd::GetGadgetWndByGadgetSlotId(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CabinWnd::SetupGadgets()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CabinWnd::SetupChildVehicleParts()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CabinWnd::UpdateOnMainPartChanged()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CabinWnd::GameDataUpdate(void*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}
