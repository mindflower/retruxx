#pragma once

class PlayerVehicleWnd :  public VehicleWnd
{
public:
    virtual struct m3d::Class * GetClass() const ;
    virtual ~PlayerVehicleWnd();
    static class m3d::Object * CreateObject();
    virtual class m3d::Object * Clone();
    static struct m3d::Class * GetBaseClass();
protected:
    void UpdateTabButtonsState();
    PlayerVehicleWnd();
    PlayerVehicleWnd(class PlayerVehicleWnd const &);
    virtual int OnWndNotify(class m3d::ui::Wnd *,unsigned int,unsigned int,class m3d::AIParam const &);
    class ai::Vehicle * GetHackedVehicle() const ;
    virtual void SetVehicleId(int);
    void DestroyHackedVehicle();
    int CreateHackedVehicle();
    virtual int GameDataUpdate(void *,int);
    void OnTabBtnWeaponGroupsClick();
    void OnTabBtnCBCharacteristicsClick();
    virtual void SetVehicleType(enum VehicleWnd::VehicleType);
    virtual void ShowTab(enum VehicleWnd::TabId);
    virtual int OnAfterAddToWndStation();
    void SelectTabButton(enum VehicleWnd::TabId,bool);
    int PrepareHackedVehicleForVehiclePart(int);
    void OnTabBtnVehicleCharacteristicsClick();
    virtual int GameDataSetup();
    int SetupForHackedVehiclePart(int);
private:
    GetClass();
    SetVehicleType(VehicleWnd::VehicleType);
    CreateObject();
    GameDataSetup();
    GetBaseClass();
    PlayerVehicleWnd::PAuxInfo m_paif;
    ref_ptr<DamageInfoWnd> m_wndDamageInfo;
    int m_hackedVehicleId;
    m3d::ui::ButtonWnd *m_tabButtons[3];
};
