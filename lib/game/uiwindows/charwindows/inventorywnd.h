#pragma once
#include "znayukakprodatwnd.h"
#include <core/ref_ptr.h>
#include <math/bounds2d.h>

class BasketWnd;
class CabinWnd;

class InventoryWnd : public ChildPanel
{
public:
    enum VehicleType
    {
        VEHICLETYPE_PLAYER = 0,
        VEHICLETYPE_WORKSHOP = 1,
        VEHICLETYPE_INVALID = 2,
    };

public:
    virtual void SetVehicleId(int vehicleId) override /* 0x11c */;
    void SetTradeVehicleId(int vehicleId, ZnayuKakProdatWnd::TradeType tradeType);

    struct AuxInfo
    {
        /* 0x0000 */ CStr m_wndCabinName;
        /* 0x000c */ CStr m_wndBasketName;

        AuxInfo(InventoryWnd::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x0018 */

    struct ChildSaveInfo
    {
        /* 0x0000 */ bool m_bBasketOnShowAnimationEnabled;
        /* 0x0001 */ bool m_bBasketOnHideAnimationEnabled;
        /* 0x0002 */ bool m_bCabinOnShowAnimationEnabled;
        /* 0x0003 */ bool m_bCabinOnHideAnimationEnabled;
        /* 0x0004 */ BoundsBase<float> m_basketB;
        /* 0x0014 */ BoundsBase<float> m_cabinB;

        ChildSaveInfo(InventoryWnd::ChildSaveInfo const&);
        ChildSaveInfo();
    }; /* size: 0x0024 */

protected:
    virtual int GameDataSetup() override /* 0x00 */;
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x10c */;
    virtual int OnBeforeAddToWndStation() override /* 0x00 */;
    virtual int OnAfterRemoveFromWndStation() override /* 0x00 */;
    void OnPlayerVehicleChanged();
    InventoryWnd::VehicleType GetVehicleTypeByGuiId(int guiId) const;
    void AddCBWindows();
    void RemoveCBWindows();

    /* 0x0224 */ InventoryWnd::AuxInfo m_aif;
    /* 0x023c */ ref_ptr<CabinWnd> m_wndCabin;
    /* 0x0240 */ ref_ptr<BasketWnd> m_wndBasket;
    /* 0x0244 */ InventoryWnd::VehicleType m_vehicleType;
    /* 0x0248 */ InventoryWnd::ChildSaveInfo m_childSaveInfo;
    /* 0x026c */ ZnayuKakProdatWnd::TradeType m_tradeType;

    InventoryWnd();
    InventoryWnd(InventoryWnd const& rhs);

public:
    virtual ~InventoryWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classInventoryWnd;
}; /* size: 0x0270 */
