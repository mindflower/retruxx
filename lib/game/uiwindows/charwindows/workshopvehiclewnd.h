#pragma once
#include "vehiclewnd.h"

#include <vector>

class WorkshopVehicleWnd : public VehicleWnd
{
public:
    int SetupForWorkshop(int workshopId);
    int GetWorkshopId() const;
    ai::Workshop* GetWorkshop() const;
    virtual void OnRestoreStyles() override /* 0x00 */;

    struct WAuxInfo
    {
        /* 0x0000 */ CStr m_btnPrevName;
        /* 0x000c */ CStr m_btnNextName;
        /* 0x0018 */ CStr m_btnSkinNextName;
        /* 0x0024 */ CStr m_btnSkinPrevName;
        /* 0x0030 */ CStr m_wndPriceName;
        WAuxInfo(WorkshopVehicleWnd::WAuxInfo const&);
        WAuxInfo();
    }; /* size: 0x003c */

protected:
    virtual int GameDataSetup() override /* 0x00 */;
    virtual int GameDataClear(bool beforeContinuousLevel) override /* 0x00 */;
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x10c */;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data) override
        /* 0x00 */;
    virtual int OnBeforeAddToWndStation() override /* 0x00 */;
    virtual int OnAfterAddToWndStation() override /* 0x00 */;
    virtual void SetVehicleId(int vehicleId) override /* 0x11c */;
    int MakeVehiclesList();
    void ShowNextVehicle();
    void ShowPrevVehicle();
    int GetNextObjId(int curVehicleObjId) const;
    int GetPrevObjId(int curVehicleObjId) const;
    void UpdateNextPrevButtonsStates();
    void LaunchNewVehicleInventoryWnd();
    ai::GeomRepository* GetWorkshopRepository() const;
    void OnRepositoryChanged();
    void FullUpdate();
    void AddInfoToEncyclopaedia();
    void Hide();
    void OnFinishTrade(void* data);

    /* 0x0290 */ WorkshopVehicleWnd::WAuxInfo m_waif;
    /* 0x02cc */ m3d::ui::ButtonWnd* m_btnPrev;
    /* 0x02d0 */ m3d::ui::ButtonWnd* m_btnNext;
    /* 0x02d4 */ int m_workshopId;
    /* 0x02d8 */ std::vector<int, std::allocator<int> > m_vehicleObjIdList;
    /* 0x02e8 */ m3d::ui::ButtonWnd* m_btnSkinNext;
    /* 0x02ec */ m3d::ui::ButtonWnd* m_btnSkinPrev;
    /* 0x02f0 */ int m_curSkin;
    /* 0x02f4 */ SkinSwitcher* m_skinSwitcher;
    /* 0x02f8 */ m3d::ui::Wnd* m_wndPrice;

    WorkshopVehicleWnd();
    WorkshopVehicleWnd(WorkshopVehicleWnd const& rhs);

public:
    virtual ~WorkshopVehicleWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    RT_CLASS_DECLARE(WorkshopVehicleWnd);
}; /* size: 0x02fc */
