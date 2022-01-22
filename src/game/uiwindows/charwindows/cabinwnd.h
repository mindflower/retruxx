#pragma once
#include "cbwnd.h"

class CabinWnd :  public CBWnd
{
public:
    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        PointBase<float> m_cabinFirstGunOrigin;
        PointBase<float> m_cabinSecondGunOrigin;
        CStr m_gadgetCommonName;
        CStr m_gadgetWeaponName;
    };

public:
    virtual m3d::Object * Clone();
    static m3d::Object * CreateObject();
    static m3d::Class * GetBaseClass();
    virtual ~CabinWnd();
    virtual m3d::Class * GetClass() const ;

protected:
    CabinWnd();
    CabinWnd(CabinWnd const &);
    int CreateGadgetWnd(int);
    virtual InventoryWnd::VehicleType GetVehicleTypeByGuiId(int) const ;
    virtual int GameDataSetup();
    ref_ptr<class GadgetWnd> GetGadgetWndByGadgetSlotId(int) const ;
    void SetupGadgets();
    virtual void SetupChildVehicleParts();
    virtual void UpdateOnMainPartChanged();
    virtual int GameDataUpdate(void *,int);

public:
    RT_CLASS_DECLARE(CabinWnd);

private:
    CabinWnd::AuxInfo m_aif;
    std::vector<ref_ptr<GadgetWnd>> m_wndGadgets;
};
