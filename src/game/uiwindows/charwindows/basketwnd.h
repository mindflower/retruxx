#pragma once

class BasketWnd :  public CBWnd
{
public:
    virtual class m3d::Object * Clone();
    static class m3d::Object * CreateObject();
    virtual struct m3d::Class * GetClass() const ;
    static struct m3d::Class * GetBaseClass();
    virtual ~BasketWnd();
protected:
    void SetupInventory();
    virtual int OnAfterRemoveFromWndStation();
    virtual void UpdateOnMainPartChanged();
    virtual int GameDataClear(bool);
    virtual void SetupChildVehicleParts();
    virtual int GameDataSetup();
    virtual int GameDataUpdate(void *,int);
    BasketWnd();
    BasketWnd(class BasketWnd const &);
    virtual enum InventoryWnd::VehicleType GetVehicleTypeByGuiId(int) const ;
    void UpdateOnSaleTabChanged(void *);
private:
    BasketWnd::AuxInfo m_aif;
    ref_ptr<IzvratRepositoryWnd> m_wndInventory;
};
