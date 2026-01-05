#pragma once
#include "znayukakprodatwnd.h"
#include <core/ref_ptr.h>
#include <math/bounds2d.h>

class BasketWnd;
class CabinWnd;

class InventoryWnd :  public ChildPanel
{
public:
    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        CStr m_wndCabinName;
        CStr m_wndBasketName;
    };

    enum VehicleType
    {
        VEHICLETYPE_PLAYER = 0x0,
        VEHICLETYPE_WORKSHOP = 0x1,
        VEHICLETYPE_INVALID = 0x2,
    };

    class ChildSaveInfo
    {
    public:
        ChildSaveInfo();
    public:
        bool m_bBasketOnShowAnimationEnabled;
        bool m_bBasketOnHideAnimationEnabled;
        bool m_bCabinOnShowAnimationEnabled;
        bool m_bCabinOnHideAnimationEnabled;
        BoundsBase<float> m_basketB;
        BoundsBase<float> m_cabinB;
    };

public:
    virtual m3d::Class * GetClass() const ;
    virtual ~InventoryWnd();
    virtual void SetVehicleId(int);
    static m3d::Class * GetBaseClass();
    static m3d::Object * CreateObject();
    void SetTradeVehicleId(int,ZnayuKakProdatWnd::TradeType);
    virtual m3d::Object * Clone();

protected:
    InventoryWnd();
    InventoryWnd(InventoryWnd const &);
    void RemoveCBWindows();
    virtual int GameDataUpdate(void *,int);
    VehicleType GetVehicleTypeByGuiId(int) const ;
    virtual int GameDataSetup();
    virtual int OnBeforeAddToWndStation();
    virtual int OnAfterRemoveFromWndStation();
    void OnPlayerVehicleChanged();
    void AddCBWindows();

public:
    RT_CLASS_DECLARE(InventoryWnd);

private:
    InventoryWnd::AuxInfo m_aif;
    ref_ptr<CabinWnd> m_wndCabin;
    ref_ptr<BasketWnd> m_wndBasket;
    InventoryWnd::VehicleType m_vehicleType;
    InventoryWnd::ChildSaveInfo m_childSaveInfo;
    ZnayuKakProdatWnd::TradeType m_tradeType;
};
