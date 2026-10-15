#pragma once
#include <ui/ui.h>

namespace ai
{
    class Vehicle;
}

class ChildPanel : public m3d::ui::Wnd
{
public:
    virtual void SetVehicleId(int vehicleId);
    int GetVehicleId() const;
    ai::Vehicle* GetVehicle() const;
    virtual void OnRestoreStyles();

protected:
    virtual int GameDataUpdate(void* data, int dataType) override;
    virtual int GameDataClear(bool beforeContinuousMap) override;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data) override;
    virtual void OnExit();
    int m_vehicleId;
    ChildPanel();
    ChildPanel(ChildPanel const& rhs);

public:
    virtual ~ChildPanel() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* CreateObject();
    static m3d::Class* GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classChildPanel;
};
