#pragma once
#include "salewnd.h"

class ShopWnd : public SaleWnd
{
protected:
    virtual int GameDataUpdate(void* data, int dataType) override;
    virtual int GameDataSetup() override;
    virtual int OnAfterAddToWndStation() override;
    virtual void SetupListOfTabTypes() override;
    virtual ai::BuildingType GetBuildingType() override;
    virtual int SetUpForWorkshop(int shopId) override;
    void OnDDDrop(void* data);
    void OnDDItemQuickDrop(void* data);

    ShopWnd();
    ShopWnd(ShopWnd const& rhs);

public:
    virtual ~ShopWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(ShopWnd);
};
