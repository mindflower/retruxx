#pragma once

class CabinWnd :  public CBWnd
{
public:
    virtual class m3d::Object * Clone();
    static class m3d::Object * __fastcall CreateObject();
    static struct m3d::Class * __fastcall GetBaseClass();
    virtual ~CabinWnd();
    virtual struct m3d::Class * GetClass() const ;
protected:
    CabinWnd();
    CabinWnd(class CabinWnd const &);
    int CreateGadgetWnd(int);
    virtual enum InventoryWnd::VehicleType GetVehicleTypeByGuiId(int) const ;
    virtual int GameDataSetup();
    class ref_ptr<class GadgetWnd> GetGadgetWndByGadgetSlotId(int) const ;
    void SetupGadgets();
    virtual void SetupChildVehicleParts();
    virtual void UpdateOnMainPartChanged();
    virtual int GameDataUpdate(void *,int);
private:
    CabinWnd::AuxInfo m_aif;
    std::vector<ref_ptr<GadgetWnd>> m_wndGadgets;
};
