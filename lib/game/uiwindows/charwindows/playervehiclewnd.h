#pragma once
#include "vehiclewnd.h"

class DamageInfoWnd;

class PlayerVehicleWnd :  public VehicleWnd
{
public:
    class PAuxInfo
    {
    public:
        PAuxInfo(void);
        CStr m_wndTabName;
        PointBase<float> m_tabBtnSz;
        float m_tabBtnSpace;
        CStr m_tabButtonNames[3];
    };

public:
    virtual m3d::Class * GetClass() const ;
    virtual ~PlayerVehicleWnd();
    static m3d::Object * CreateObject();
    virtual m3d::Object * Clone();
    static m3d::Class * GetBaseClass();

protected:
    void UpdateTabButtonsState();
    PlayerVehicleWnd();
    PlayerVehicleWnd(PlayerVehicleWnd const &);
    virtual int OnWndNotify(m3d::ui::Wnd *,unsigned int,unsigned int, m3d::AIParam const &);
    ai::Vehicle * GetHackedVehicle() const ;
    virtual void SetVehicleId(int);
    void DestroyHackedVehicle();
    int CreateHackedVehicle();
    virtual int GameDataUpdate(void *,int);
    void OnTabBtnWeaponGroupsClick();
    void OnTabBtnCBCharacteristicsClick();
    virtual void SetVehicleType(VehicleWnd::VehicleType);
    virtual void ShowTab(VehicleWnd::TabId);
    virtual int OnAfterAddToWndStation();
    void SelectTabButton(VehicleWnd::TabId,bool);
    int PrepareHackedVehicleForVehiclePart(int);
    void OnTabBtnVehicleCharacteristicsClick();
    virtual int GameDataSetup();
    int SetupForHackedVehiclePart(int);

public:
    RT_CLASS_DECLARE(PlayerVehicleWnd);

private:
    PlayerVehicleWnd::PAuxInfo m_paif;
    ref_ptr<DamageInfoWnd> m_wndDamageInfo;
    int m_hackedVehicleId;
    m3d::ui::ButtonWnd *m_tabButtons[3];
};
