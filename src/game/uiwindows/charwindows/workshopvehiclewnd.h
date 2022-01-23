#pragma once
#include "vehiclewnd.h"

class WorkshopVehicleWnd : public VehicleWnd
{
public:
    class WAuxInfo
    {
    public:
        WAuxInfo();

    private:
        CStr m_btnPrevName;
        CStr m_btnNextName;
        CStr m_btnSkinNextName;
        CStr m_btnSkinPrevName;
        CStr m_wndPriceName;
    };

public:
    ai::Workshop* GetWorkshop() const;
    virtual ~WorkshopVehicleWnd();
    static m3d::Class* GetBaseClass();
    int GetWorkshopId() const;
    static m3d::Object* CreateObject();
    virtual void OnRestoreStyles();
    int SetupForWorkshop(int);
    virtual m3d::Class* GetClass() const;
    virtual m3d::Object* Clone();
protected:

    virtual int OnWndNotify(m3d::ui::Wnd*, unsigned int, unsigned int, m3d::AIParam const&);
    virtual int GameDataSetup();
    WorkshopVehicleWnd(WorkshopVehicleWnd const&);
    WorkshopVehicleWnd();
    void LaunchNewVehicleInventoryWnd();
    void FullUpdate();
    int GetNextObjId(int) const;
    void OnFinishTrade(void*);
    void OnRepositoryChanged();
    void UpdateNextPrevButtonsStates();
    void AddInfoToEncyclopaedia();
    int MakeVehiclesList();
    int GetPrevObjId(int) const;
    virtual int OnAfterAddToWndStation();
    virtual int OnBeforeAddToWndStation();
    void ShowNextVehicle();
    void Hide();
    virtual int GameDataUpdate(void*, int);
    virtual void SetVehicleId(int);
    void ShowPrevVehicle();
    virtual int GameDataClear(bool);
    ai::GeomRepository* GetWorkshopRepository() const;

public:
    RT_CLASS_DECLARE(WorkshopVehicleWnd);

private:
    WorkshopVehicleWnd::WAuxInfo m_waif;
    m3d::ui::ButtonWnd* m_btnPrev;
    m3d::ui::ButtonWnd* m_btnNext;
    int m_workshopId;
    std::vector<int> m_vehicleObjIdList;
    m3d::ui::ButtonWnd* m_btnSkinNext;
    m3d::ui::ButtonWnd* m_btnSkinPrev;
    int m_curSkin;
    SkinSwitcher* m_skinSwitcher;
    m3d::ui::Wnd* m_wndPrice;
};
