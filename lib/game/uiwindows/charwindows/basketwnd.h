#pragma once
#include "cbwnd.h"

class IzvratRepositoryWnd;

class BasketWnd : public CBWnd
{
public:
    struct AuxInfo
    {
        /* 0x0000 */ CStr m_wndInventoryName;

        AuxInfo(BasketWnd::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x000c */

protected:
    virtual int GameDataSetup() override /* 0x00 */;
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x10c */;
    virtual int GameDataClear(bool beforeContinuousLevel) override /* 0x108 */;
    virtual void UpdateOnMainPartChanged() override /* 0x130 */;
    virtual void SetupChildVehicleParts() override /* 0x134 */;
    virtual int OnAfterRemoveFromWndStation() override /* 0x00 */;
    void SetupInventory();
    void UpdateOnSaleTabChanged(void* data);
    virtual InventoryWnd::VehicleType GetVehicleTypeByGuiId(int guiId) const override /* 0x140 */;

    /* 0x0270 */ BasketWnd::AuxInfo m_aif;
    /* 0x027c */ ref_ptr<IzvratRepositoryWnd> m_wndInventory;

    BasketWnd();
    BasketWnd(BasketWnd const& rhs);

public:
    virtual ~BasketWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x00 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x00 */;
    static m3d::Class m_classBasketWnd;
}; /* size: 0x0280 */
