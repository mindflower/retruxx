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
        /* 0x0000 */ PointBase<float> m_cabinFirstGunOrigin;
        /* 0x0008 */ PointBase<float> m_cabinSecondGunOrigin;
        /* 0x0010 */ CStr m_gadgetCommonName;
        /* 0x001c */ CStr m_gadgetWeaponName;
        AuxInfo(const CabinWnd::AuxInfo&);
        AuxInfo();
    }; /* size: 0x0028 */

    using GadgetVector = std::vector<ref_ptr<GadgetWnd>, std::allocator<ref_ptr<GadgetWnd> > >;

protected:
    virtual int GameDataSetup() override /* 0x00 */;
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x10c */;
    virtual void UpdateOnMainPartChanged() override /* 0x130 */;
    virtual void SetupChildVehicleParts() override /* 0x134 */;
    void SetupGadgets();
    int CreateGadgetWnd(int id);
    ref_ptr<GadgetWnd> GetGadgetWndByGadgetSlotId(int slotId) const;
    virtual InventoryWnd::VehicleType GetVehicleTypeByGuiId(int guiId) const override /* 0x140 */;
    /* 0x0270 */ CabinWnd::AuxInfo m_aif;
    /* 0x0298 */ std::vector<ref_ptr<GadgetWnd>, std::allocator<ref_ptr<GadgetWnd> > > m_wndGadgets;
    CabinWnd();
    CabinWnd(const CabinWnd& rhs);

public:
    virtual  ~CabinWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x00 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x00 */;
    static m3d::Class m_classCabinWnd;
}; /* size: 0x02a8 */
