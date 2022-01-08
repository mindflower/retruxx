#pragma once

class GroundWnd :  public ChildPanel
{
public:
    virtual void SetVehicleId(int);
    static struct m3d::Class * GetBaseClass();
    void PickUpAll();
    virtual struct m3d::Class * GetClass() const ;
    static class m3d::Object * CreateObject();
    virtual class m3d::Object * Clone();
    virtual ~GroundWnd();
protected:
    virtual int OnAfterAddToWndStation();
    void SetupRepository();
    void OnRepositoryChanged(void *);
    void OnStartLevel();
    class ai::GeomRepository * GetGroundRepository() const ;
    virtual int GameDataClear(bool);
    GroundWnd();
    GroundWnd(class GroundWnd const &);
    void UpdatePickupButtonState();
    virtual int GameDataUpdate(void *,int);
    virtual int GameDataSetup();
    virtual int OnAfterRemoveFromWndStation();
    virtual int OnBeforeAddToWndStation();
    void UpdateGroundPicture();
    virtual int OnWndNotify(class m3d::ui::Wnd *,unsigned int,unsigned int,class m3d::AIParam const &);
private:
    GroundWnd::AuxInfo m_aif;
    ref_ptr<RepositoryWnd> m_wndRepository;
    m3d::ui::ButtonWnd *m_btnPickUpAll;
    m3d::ui::ImageWnd *m_wndGroundPicture;
};
