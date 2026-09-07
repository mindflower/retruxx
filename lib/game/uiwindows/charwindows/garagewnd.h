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

class GarageWnd : public ChildPanel
{
public:
    struct AuxInfo
    {
        /* 0x0000 */ CStr m_btnCabinsName;
        /* 0x000c */ CStr m_btnBasketsName;
        /* 0x0018 */ CStr m_btnNewVehicleName;
        /* 0x0024 */ CStr m_btnSkinName;
        /* 0x0030 */ CStr m_btnRefuelAllName;
        /* 0x003c */ CStr m_btnRefuelListName;
        /* 0x0048 */ CStr m_wndRefuelPriceName;
        /* 0x0054 */ CStr m_btnRepairAllName;
        /* 0x0060 */ CStr m_btnRepairListName;
        /* 0x006c */ CStr m_wndRepairPriceName;
        /* 0x0078 */ CStr m_btnRechargeAllName;
        /* 0x0084 */ CStr m_btnRechargeListName;
        /* 0x0090 */ CStr m_wndRechargePriceName;
        /* 0x009c */ CStr m_btnBasketsTexName;
        /* 0x00a8 */ CStr m_btnCabinsTexName;
        /* 0x00b4 */ CStr m_btnSkinTexName;
        /* 0x00c0 */ CStr m_btnRefuelListTexName;
        /* 0x00cc */ CStr m_btnRepairListTexName;
        /* 0x00d8 */ CStr m_btnRechargeListTexName;
        /* 0x00e4 */ unsigned int m_colorYellow;
        /* 0x00e8 */ unsigned int m_colorGreen;
        /* 0x00ec */ unsigned int m_colorRed;
        /* 0x00f0 */ unsigned int m_colorTooltipRed;
        /* 0x00f4 */ unsigned int m_colorNormal;
        /* 0x00f8 */ CStr m_strIdTooltipRefuel;
        /* 0x0104 */ CStr m_strIdTooltipRefuelNotNeed;
        /* 0x0110 */ CStr m_strIdTooltipRefuelUnavailable;
        /* 0x011c */ CStr m_strIdTooltipRefuelPartial;
        /* 0x0128 */ CStr m_strIdTooltipRefuelFull;
        /* 0x0134 */ CStr m_strIdTooltipRepair;
        /* 0x0140 */ CStr m_strIdTooltipRepairNotNeed;
        /* 0x014c */ CStr m_strIdTooltipRepairUnavailable;
        /* 0x0158 */ CStr m_strIdTooltipRepairPartial;
        /* 0x0164 */ CStr m_strIdTooltipRepairFull;
        /* 0x0170 */ CStr m_strIdTooltipRecharge;
        /* 0x017c */ CStr m_strIdTooltipRechargeNotNeed;
        /* 0x0188 */ CStr m_strIdTooltipRechargeUnavailable;
        /* 0x0194 */ CStr m_strIdTooltipRechargePartial;
        /* 0x01a0 */ CStr m_strIdTooltipRechargeFull;
        /* 0x01ac */ CStr m_strIdState;
        /* 0x01b8 */ CStr m_strEnabled;
        /* 0x01c4 */ CStr m_strOverrolled;
        /* 0x01d0 */ CStr m_strPressed;
        /* 0x01dc */ CStr m_strDisabled;
        /* 0x01e8 */ CStr m_strSelected;
        AuxInfo(GarageWnd::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x01f4 */

    enum Service
    {
        SERVICE_CABINS = 0,
        SERVICE_BASKETS = 1,
        SERVICE_NEW_VEHICLE = 2,
        SERVICE_SKIN = 3,
        SERVICE_REFUEL = 4,
        SERVICE_REPAIR = 5,
        SERVICE_RECHARGE = 6,
    };

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
    static GarageWnd::AuxInfo m_aif;
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
