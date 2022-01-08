#pragma once

class ChildPanel :  public m3d::ui::Wnd
{
public:
    virtual struct m3d::Class * GetClass() const ;
    virtual void SetVehicleId(int);
    int GetVehicleId() const ;
    static struct m3d::Class * GetBaseClass();
    static class m3d::Object * CreateObject();
    class ai::Vehicle * GetVehicle() const ;
    virtual ~ChildPanel();
    virtual class m3d::Object * Clone();
    virtual void OnRestoreStyles();
protected:
    ChildPanel();
    ChildPanel(class ChildPanel const &);
    virtual int GameDataClear(bool);
    virtual int OnWndNotify(class m3d::ui::Wnd *,unsigned int,unsigned int,class m3d::AIParam const &);
    virtual int GameDataUpdate(void *,int);
    virtual void OnExit();
private:
    int m_vehicleId;
};
