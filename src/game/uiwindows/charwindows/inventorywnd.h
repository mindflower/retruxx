#pragma once

class InventoryWnd :  public ChildPanel
{
public:
    virtual struct m3d::Class * GetClass() const ;
    virtual ~InventoryWnd();
    virtual void SetVehicleId(int);
    static struct m3d::Class * __fastcall GetBaseClass();
    static class m3d::Object * __fastcall CreateObject();
    void SetTradeVehicleId(int,enum ZnayuKakProdatWnd::TradeType);
    virtual class m3d::Object * Clone();
protected:
    InventoryWnd();
    InventoryWnd(class InventoryWnd const &);
    void RemoveCBWindows();
    virtual int GameDataUpdate(void *,int);
    enum VehicleType GetVehicleTypeByGuiId(int) const ;
    virtual int GameDataSetup();
    virtual int OnBeforeAddToWndStation();
    virtual int OnAfterRemoveFromWndStation();
    void OnPlayerVehicleChanged();
    void AddCBWindows();
private:
    InventoryWnd::AuxInfo m_aif;
    ref_ptr<CabinWnd> m_wndCabin;
    ref_ptr<BasketWnd> m_wndBasket;
    InventoryWnd::VehicleType m_vehicleType;
    InventoryWnd::ChildSaveInfo m_childSaveInfo;
    ZnayuKakProdatWnd::TradeType m_tradeType;
};
