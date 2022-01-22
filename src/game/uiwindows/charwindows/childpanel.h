#pragma once
#include <ui/ui.h>

namespace ai
{
    class Vehicle;
}

class ChildPanel :  public m3d::ui::Wnd
{
public:
    virtual m3d::Class * GetClass() const ;
    virtual void SetVehicleId(int);
    int GetVehicleId() const ;
    static m3d::Class * GetBaseClass();
    static m3d::Object * CreateObject();
    ai::Vehicle * GetVehicle() const ;
    virtual ~ChildPanel();
    virtual m3d::Object * Clone();
    virtual void OnRestoreStyles();

protected:
    ChildPanel();
    ChildPanel(ChildPanel const &);
    virtual int GameDataClear(bool);
    virtual int OnWndNotify(m3d::ui::Wnd *,unsigned int,unsigned int,m3d::AIParam const &);
    virtual int GameDataUpdate(void *,int);
    virtual void OnExit();

public:
    RT_CLASS_DECLARE(ChildPanel);

private:
    int m_vehicleId;
};
