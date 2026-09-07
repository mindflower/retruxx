#pragma once
#include <ui/ui.h>

namespace ai
{
    class Vehicle;
}

class ChildPanel : public m3d::ui::Wnd
{
public:
    virtual void SetVehicleId(int vehicleId) /* 0x11c */;
    int GetVehicleId() const;
    ai::Vehicle* GetVehicle() const;
    virtual void OnRestoreStyles() /* 0x120 */;

protected:
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x10c */;
    virtual int GameDataClear(bool beforeContinuousMap) override /* 0x108 */;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data) override
        /* 0xc8 */;
    virtual void OnExit() /* 0x124 */;
    /* 0x0220 */ int m_vehicleId;
    ChildPanel();
    ChildPanel(ChildPanel const& rhs);

public:
    virtual ~ChildPanel() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classChildPanel;
}; /* size: 0x0224 */
