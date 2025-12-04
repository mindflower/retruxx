#pragma once
#include "salewnd.h"

class ShopWnd :  public SaleWnd
{
public:
    static m3d::Class * GetBaseClass();
    virtual m3d::Object * Clone();
    static m3d::Object * CreateObject();
    virtual m3d::Class * GetClass() const ;
    virtual ~ShopWnd();

protected:
    void OnDDItemQuickDrop(void *);
    virtual int GameDataSetup();
    virtual int GameDataUpdate(void *,int);
    virtual void SetupListOfTabTypes();
    void OnDDDrop(void *);
    virtual int SetUpForWorkshop(int);
    ShopWnd();
    ShopWnd(ShopWnd const &);
    virtual int OnAfterAddToWndStation();
    virtual ai::BuildingType GetBuildingType();

public:
    RT_CLASS_DECLARE(ShopWnd);
};
