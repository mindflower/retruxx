#pragma once
#include "salewnd.h"

class ShopWnd : public SaleWnd
{
protected:
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x10c */;
    virtual int GameDataSetup() override /* 0x104 */;
    virtual int OnAfterAddToWndStation() override /* 0x6c */;
    virtual void SetupListOfTabTypes() override /* 0x13c */;
    virtual ai::BuildingType GetBuildingType() override /* 0x140 */;
    virtual int SetUpForWorkshop(int shopId) override /* 0x128 */;
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
}; /* size: 0x025c */
