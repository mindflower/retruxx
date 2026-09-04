#pragma once
#include "vehiclewnd.h"

class DamageInfoWnd;

class PlayerVehicleWnd : public VehicleWnd
{
public:
    struct PAuxInfo
    {
        /* 0x0000 */ CStr m_wndTabName;
        /* 0x000c */ PointBase<float> m_tabBtnSz;
        /* 0x0014 */ float m_tabBtnSpace;
        /* 0x0018 */ CStr m_tabButtonNames[3];
        PAuxInfo(PlayerVehicleWnd::PAuxInfo const&);
        PAuxInfo();
    }; /* size: 0x003c */

protected:
    virtual int GameDataSetup() override /* 0x00 */;
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x10c */;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data) override
        /* 0x00 */;
    virtual void SetVehicleId(int vehicleId) override /* 0x11c */;
    virtual void SetVehicleType(VehicleWnd::VehicleType type) override /* 0x12c */;
    virtual int OnAfterAddToWndStation() override /* 0x00 */;
    int CreateHackedVehicle();
    void DestroyHackedVehicle();
    int PrepareHackedVehicleForVehiclePart(int hackedVpId);
    ai::Vehicle* GetHackedVehicle() const;
    int SetupForHackedVehiclePart(int vpId);
    void OnTabBtnVehicleCharacteristicsClick();
    void OnTabBtnCBCharacteristicsClick();
    void OnTabBtnWeaponGroupsClick();
    virtual void ShowTab(VehicleWnd::TabId tabId) override /* 0x134 */;
    void UpdateTabButtonsState();
    void SelectTabButton(VehicleWnd::TabId tabId, bool bSelect);
    /* 0x0290 */ PlayerVehicleWnd::PAuxInfo m_paif;
    /* 0x02cc */ ref_ptr<DamageInfoWnd> m_wndDamageInfo;
    /* 0x02d0 */ int m_hackedVehicleId;
    /* 0x02d4 */ m3d::ui::ButtonWnd* m_tabButtons[3];
    PlayerVehicleWnd();
    PlayerVehicleWnd(PlayerVehicleWnd const& rhs);

public:
    virtual ~PlayerVehicleWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classPlayerVehicleWnd;
}; /* size: 0x02e0 */
