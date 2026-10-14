#pragma once
#include "vehiclewnd.h"

class DamageInfoWnd;

class PlayerVehicleWnd : public VehicleWnd
{
public:
    struct PAuxInfo
    {
        CStr m_wndTabName;
        PointBase<float> m_tabBtnSz;
        float m_tabBtnSpace;
        CStr m_tabButtonNames[3];
        PAuxInfo(PlayerVehicleWnd::PAuxInfo const&);
        PAuxInfo();
    };

protected:
    virtual int GameDataSetup() override;
    virtual int GameDataUpdate(void* data, int dataType) override;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data) override
       ;
    virtual void SetVehicleId(int vehicleId) override;
    virtual void SetVehicleType(VehicleWnd::VehicleType type) override;
    virtual int OnAfterAddToWndStation() override;
    int CreateHackedVehicle();
    void DestroyHackedVehicle();
    int PrepareHackedVehicleForVehiclePart(int hackedVpId);
    ai::Vehicle* GetHackedVehicle() const;
    int SetupForHackedVehiclePart(int vpId);
    void OnTabBtnVehicleCharacteristicsClick();
    void OnTabBtnCBCharacteristicsClick();
    void OnTabBtnWeaponGroupsClick();
    virtual void ShowTab(VehicleWnd::TabId tabId) override;
    void UpdateTabButtonsState();
    void SelectTabButton(VehicleWnd::TabId tabId, bool bSelect);
    PlayerVehicleWnd::PAuxInfo m_paif;
    ref_ptr<DamageInfoWnd> m_wndDamageInfo;
    int m_hackedVehicleId;
    m3d::ui::ButtonWnd* m_tabButtons[3];
    PlayerVehicleWnd();
    PlayerVehicleWnd(PlayerVehicleWnd const& rhs);

public:
    virtual ~PlayerVehicleWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classPlayerVehicleWnd;
};
