#pragma once
#include "childpanel.h"
#include <core/ref_ptr.h>
#include <game/uimisc/guihelper.h>
#include <server/objects/workshop.h>

class RechargeList;
class RepairList;
class RefuelList;
class SkinsWnd;
class BasketList;
class CabinList;

namespace m3d
{
    namespace ui
    {
        class ButtonWnd;
    }
}

class GarageWnd :  public ChildPanel
{
public:
    static m3d::Class * GetBaseClass();
    int SetupForWorkshop(int);
    bool IsListOpen() const ;
    ai::Workshop * GetWorkshop() const ;
    static help::Color __fastcall GetValueColor(int,int);
    virtual m3d::Object * Clone();
    int GetWorkshopId() const ;
    virtual ~GarageWnd();
    static m3d::Object * CreateObject();
    virtual m3d::Class * GetClass() const ;

protected:
    void OnNewFrame();
    void OnRepairList();
    void UpdateAllPricePrevValues();
    unsigned int EnumColor2Color(help::Color) const ;
    int GetUnitsToRecharge() const ;
    void OnCabins();
    void Refuel(int);
    void MaxRefuel();
    void OnRepairAll();
    void UpdateRefuelAllTooltip();
    void OnPlayerVehicleChanged();
    void UpdateRefuelAllPriceControls(bool);
    void OnRepositoryChanged(void *);
    void UpdateNewVehicleActiveState();
    void UpdateRefuelActiveState();
    void OnBaskets();
    void MaxRecharge();
    void OnSkin();
    void OpenList(ref_ptr<m3d::ui::Wnd>, m3d::ui::ButtonWnd *, m3d::ui::ButtonWnd *, ai::eGameEvent);
    void OnRechargeList();
    void OnFinishTrade(void *);
    void UpdateServiceActiveState(std::vector<m3d::ui::Wnd *, std::allocator<m3d::ui::Wnd *> > const &,bool, ref_ptr<m3d::ui::Wnd>);
    ai::GeomRepository * GetWorkshopRepositoryByType(ai::WorkshopRepositoryType) const ;
    void UpdateRechargeAllTooltip();
    void GetPossibleRepair(int &,int &) const ;
    void OnRechargeAll();
    int GetUnitsToRepair() const ;
    virtual int OnAfterAddToWndStation();
    void Recharge(int);
    void FullUpdate();
    float GetHealthPriceForOneUnit() const ;
    void MaxRepair();
    void UpdateSkinActiveState();
    virtual int OnWndNotify(m3d::ui::Wnd *,unsigned int,unsigned int, m3d::AIParam const &);
    void OnRefuelList();
    virtual int OnBeforeAddToWndStation();
    GarageWnd(GarageWnd const &);
    GarageWnd();
    virtual int OnAfterRemoveFromWndStation();
    int GetShellPrice(int) const ;
    void Repair(int);
    void GetPossibleRecharge(int &,int &) const ;
    void UpdateActiveStates();
    void UpdateRepairActiveState();
    void UpdateRepairAllPriceControls(bool);
    void UpdateRepairAllTooltip();
    void OnRefuelAll();
    void UpdateAllPriceControls(bool);
    void OnBuySkin();
    void GetPossibleRefuel(int &,int &) const ;
    int GetTownId() const ;
    void CloseCurrentList();
    int GetUnitsToRefuel() const ;
    void UpdateBasketsActiveState();
    int GetFuelPriceForOneUnit() const ;
    void UpdateServiceSelectState(std::vector<m3d::ui::Wnd *, std::allocator<m3d::ui::Wnd *> > const &,bool);
    void UpdateRechargeAllPriceControls(bool);
    virtual int GameDataSetup();
    virtual int GameDataUpdate(void *,int);
    void UpdateCabinsActiveState();
    void OnNewVehicle();
    void UpdateRechargeActiveState();
    void UpdateAllPriceValues();

public:
    RT_CLASS_DECLARE(GarageWnd);

private:
    m3d::ui::ButtonWnd *m_btnCabins;
    m3d::ui::ButtonWnd *m_btnBaskets;
    m3d::ui::ButtonWnd *m_btnNewVehicle;
    m3d::ui::ButtonWnd *m_btnSkin;
    m3d::ui::ButtonWnd *m_btnRefuelAll;
    m3d::ui::ButtonWnd *m_btnRefuelList;
    m3d::ui::Wnd *m_wndRefuelPrice;
    m3d::ui::ButtonWnd *m_btnRepairAll;
    m3d::ui::ButtonWnd *m_btnRepairList;
    m3d::ui::Wnd *m_wndRepairPrice;
    m3d::ui::ButtonWnd *m_btnRechargeAll;
    m3d::ui::ButtonWnd *m_btnRechargeList;
    m3d::ui::Wnd *m_wndRechargePrice;
    ref_ptr<CabinList> m_wndCabinsList;
    ref_ptr<BasketList> m_wndBasketsList;
    ref_ptr<SkinsWnd> m_wndSkinList;
    ref_ptr<RefuelList> m_wndRefuelList;
    ref_ptr<RepairList> m_wndRepairList;
    ref_ptr<RechargeList> m_wndRechargeList;
    ref_ptr<m3d::ui::Wnd> m_curList;
    m3d::ui::ButtonWnd *m_curListButton;
    m3d::ui::ButtonWnd *m_curAllButton;
    int m_workshopId;
    int m_unitsToRepair;
    int m_maxPossibleUnitsToRepair;
    int m_repairPrice;
    int m_prevUnitsToRepair;
    int m_prevMaxPossibleUnitsToRepair;
    int m_prevRepairPrice;
    int m_unitsToRefuel;
    int m_maxPossibleUnitsToRefuel;
    int m_refuelPrice;
    int m_prevUnitsToRefuel;
    int m_prevMaxPossibleUnitsToRefuel;
    int m_prevRefuelPrice;
    int m_unitsToRecharge;
    int m_maxPossibleUnitsToRecharge;
    int m_rechargePrice;
    int m_prevUnitsToRecharge;
    int m_prevMaxPossibleUnitsToRecharge;
    int m_prevRechargePrice;
};
