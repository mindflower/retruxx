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
    virtual void SetVehicleId(int vehicleId) override;
    void SetTradeVehicleId(int vehicleId, ZnayuKakProdatWnd::TradeType tradeType);

    struct AuxInfo
    {
        CStr m_wndCabinName;
        CStr m_wndBasketName;

        AuxInfo(InventoryWnd::AuxInfo const&);
        AuxInfo();
    };

    struct ChildSaveInfo
    {
        bool m_bBasketOnShowAnimationEnabled;
        bool m_bBasketOnHideAnimationEnabled;
        bool m_bCabinOnShowAnimationEnabled;
        bool m_bCabinOnHideAnimationEnabled;
        BoundsBase<float> m_basketB;
        BoundsBase<float> m_cabinB;

        ChildSaveInfo(InventoryWnd::ChildSaveInfo const&);
        ChildSaveInfo();
    };

protected:
    virtual int GameDataSetup() override;
    virtual int GameDataUpdate(void* data, int dataType) override;
    virtual int OnBeforeAddToWndStation() override;
    virtual int OnAfterRemoveFromWndStation() override;
    void OnPlayerVehicleChanged();
    InventoryWnd::VehicleType GetVehicleTypeByGuiId(int guiId) const;
    void AddCBWindows();
    void RemoveCBWindows();

    InventoryWnd::AuxInfo m_aif;
    ref_ptr<CabinWnd> m_wndCabin;
    ref_ptr<BasketWnd> m_wndBasket;
    InventoryWnd::VehicleType m_vehicleType;
    InventoryWnd::ChildSaveInfo m_childSaveInfo;
    ZnayuKakProdatWnd::TradeType m_tradeType;

    InventoryWnd();
    InventoryWnd(InventoryWnd const& rhs);

public:
    virtual ~InventoryWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classInventoryWnd;
};
