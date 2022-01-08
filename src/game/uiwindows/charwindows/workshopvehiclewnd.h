#pragma once

class WorkshopVehicleWnd :  public VehicleWnd
{
public:
    class ai::Workshop * GetWorkshop() const ;
    virtual ~WorkshopVehicleWnd();
    static struct m3d::Class * GetBaseClass();
    int GetWorkshopId() const ;
    static class m3d::Object * CreateObject();
    virtual void OnRestoreStyles();
    int SetupForWorkshop(int);
    virtual struct m3d::Class * GetClass() const ;
    virtual class m3d::Object * Clone();
protected:
    virtual int OnWndNotify(class m3d::ui::Wnd *,unsigned int,unsigned int,class m3d::AIParam const &);
    virtual int GameDataSetup();
    WorkshopVehicleWnd(class WorkshopVehicleWnd const &);
    WorkshopVehicleWnd();
    void LaunchNewVehicleInventoryWnd();
    void FullUpdate();
    int GetNextObjId(int) const ;
    void OnFinishTrade(void *);
    void OnRepositoryChanged();
    void UpdateNextPrevButtonsStates();
    void AddInfoToEncyclopaedia();
    int MakeVehiclesList();
    int GetPrevObjId(int) const ;
    virtual int OnAfterAddToWndStation();
    virtual int OnBeforeAddToWndStation();
    void ShowNextVehicle();
    void Hide();
    virtual int GameDataUpdate(void *,int);
    virtual void SetVehicleId(int);
    void ShowPrevVehicle();
    virtual int GameDataClear(bool);
    class ai::GeomRepository * GetWorkshopRepository() const ;
private:
    GameDataUpdate(void *,int);
    Clone();
    WorkshopVehicleWnd::WAuxInfo m_waif;
    m3d::ui::ButtonWnd *m_btnPrev;
    m3d::ui::ButtonWnd *m_btnNext;
    int m_workshopId;
    std::vector<int> m_vehicleObjIdList;
    m3d::ui::ButtonWnd *m_btnSkinNext;
    m3d::ui::ButtonWnd *m_btnSkinPrev;
    int m_curSkin;
    SkinSwitcher *m_skinSwitcher;
    m3d::ui::Wnd *m_wndPrice;
};
