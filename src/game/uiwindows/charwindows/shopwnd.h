#pragma once

class ShopWnd :  public SaleWnd
{
public:
    static struct m3d::Class * GetBaseClass();
    virtual class m3d::Object * Clone();
    static class m3d::Object * CreateObject();
    virtual struct m3d::Class * GetClass() const ;
    virtual ~ShopWnd();
protected:
    void OnDDItemQuickDrop(void *);
    virtual int GameDataSetup();
    virtual int GameDataUpdate(void *,int);
    virtual void SetupListOfTabTypes();
    void OnDDDrop(void *);
    virtual int SetUpForWorkshop(int);
    ShopWnd();
    ShopWnd(class ShopWnd const &);
    virtual int OnAfterAddToWndStation();
    virtual enum ai::BuildingType GetBuildingType();
private:
};
