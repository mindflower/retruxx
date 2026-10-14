#pragma once
#include "cbwnd.h"

class IzvratRepositoryWnd;

class BasketWnd : public CBWnd
{
public:
    struct AuxInfo
    {
        CStr m_wndInventoryName;

        AuxInfo(BasketWnd::AuxInfo const&);
        AuxInfo();
    };

protected:
    virtual int GameDataSetup() override;
    virtual int GameDataUpdate(void* data, int dataType) override;
    virtual int GameDataClear(bool beforeContinuousLevel) override;
    virtual void UpdateOnMainPartChanged() override;
    virtual void SetupChildVehicleParts() override;
    virtual int OnAfterRemoveFromWndStation() override;
    void SetupInventory();
    void UpdateOnSaleTabChanged(void* data);
    virtual InventoryWnd::VehicleType GetVehicleTypeByGuiId(int guiId) const override;

    BasketWnd::AuxInfo m_aif;
    ref_ptr<IzvratRepositoryWnd> m_wndInventory;

    BasketWnd();
    BasketWnd(BasketWnd const& rhs);

public:
    virtual ~BasketWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classBasketWnd;
};
