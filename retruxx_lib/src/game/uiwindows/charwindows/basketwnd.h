#pragma once
#include "cbwnd.h"

class IzvratRepositoryWnd;

class BasketWnd :  public CBWnd
{
public:
    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        CStr m_wndInventoryName;
    };

public:
    virtual m3d::Object * Clone();
    static m3d::Object * CreateObject();
    virtual m3d::Class * GetClass() const ;
    static m3d::Class * GetBaseClass();
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
    BasketWnd(BasketWnd const &);
    virtual InventoryWnd::VehicleType GetVehicleTypeByGuiId(int) const ;
    void UpdateOnSaleTabChanged(void *);

public:
    RT_CLASS_DECLARE(BasketWnd);

private:
    BasketWnd::AuxInfo m_aif;
    ref_ptr<IzvratRepositoryWnd> m_wndInventory;
};
