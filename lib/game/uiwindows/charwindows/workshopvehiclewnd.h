#pragma once
#include "vehiclewnd.h"

#include <vector>

class WorkshopVehicleWnd : public VehicleWnd
{
public:
    int SetupForWorkshop(int workshopId);
    int GetWorkshopId() const;
    ai::Workshop* GetWorkshop() const;
    virtual void OnRestoreStyles() override;

    struct WAuxInfo
    {
        CStr m_btnPrevName;
        CStr m_btnNextName;
        CStr m_btnSkinNextName;
        CStr m_btnSkinPrevName;
        CStr m_wndPriceName;
        WAuxInfo(WorkshopVehicleWnd::WAuxInfo const&);
        WAuxInfo();
    };

protected:
    virtual int GameDataSetup() override;
    virtual int GameDataClear(bool beforeContinuousLevel) override;
    virtual int GameDataUpdate(void* data, int dataType) override;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data) override;
    virtual int OnBeforeAddToWndStation() override;
    virtual int OnAfterAddToWndStation() override;
    virtual void SetVehicleId(int vehicleId) override;
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

    WorkshopVehicleWnd::WAuxInfo m_waif;
    m3d::ui::ButtonWnd* m_btnPrev;
    m3d::ui::ButtonWnd* m_btnNext;
    int m_workshopId;
    std::vector<int, std::allocator<int>> m_vehicleObjIdList;
    m3d::ui::ButtonWnd* m_btnSkinNext;
    m3d::ui::ButtonWnd* m_btnSkinPrev;
    int m_curSkin;
    SkinSwitcher* m_skinSwitcher;
    m3d::ui::Wnd* m_wndPrice;

    WorkshopVehicleWnd();
    WorkshopVehicleWnd(WorkshopVehicleWnd const& rhs);

public:
    virtual ~WorkshopVehicleWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(WorkshopVehicleWnd);
};
