#pragma once
#include "cbwnd.h"
#include "gadgetwnd.h"

class CabinWnd : public CBWnd
{
public:
    enum GadgetId
    {
        GADGET_COMMON_MIN = 0,
        GADGET_COMMON_MAX = 4,
        GADGET_WEAPON_MIN = 5,
        GADGET_WEAPON_MAX = 9,
        GADGET_NUM_GADGETS = 10,
    };
    
    struct AuxInfo
    {
        PointBase<float> m_cabinFirstGunOrigin;
        PointBase<float> m_cabinSecondGunOrigin;
        CStr m_gadgetCommonName;
        CStr m_gadgetWeaponName;
        AuxInfo(const CabinWnd::AuxInfo&);
        AuxInfo();
    };

    using GadgetVector = std::vector<ref_ptr<GadgetWnd>, std::allocator<ref_ptr<GadgetWnd> > >;

protected:
    virtual int GameDataSetup() override;
    virtual int GameDataUpdate(void* data, int dataType) override;
    virtual void UpdateOnMainPartChanged() override;
    virtual void SetupChildVehicleParts() override;
    void SetupGadgets();
    int CreateGadgetWnd(int id);
    ref_ptr<GadgetWnd> GetGadgetWndByGadgetSlotId(int slotId) const;
    virtual InventoryWnd::VehicleType GetVehicleTypeByGuiId(int guiId) const override;
    CabinWnd::AuxInfo m_aif;
    std::vector<ref_ptr<GadgetWnd>, std::allocator<ref_ptr<GadgetWnd> > > m_wndGadgets;
    CabinWnd();
    CabinWnd(const CabinWnd& rhs);

public:
    virtual  ~CabinWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classCabinWnd;
};
