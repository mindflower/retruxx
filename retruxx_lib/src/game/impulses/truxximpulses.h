#pragma once
#include <engine/impulses/gameimpulse.h>

class TruxxImpulse : public m3d::GameImpulse
{
protected:
    TruxxImpulse();
    TruxxImpulse(const TruxxImpulse& rhs);

public:
    virtual  ~TruxxImpulse() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x00 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x00 */;
    static m3d::Class m_classTruxxImpulse;
    virtual int GetGameModeIdByName(const CStr& modeName) override /* 0x64 */;
    virtual CStr GetGameModeNameById(int modeId) override /* 0x68 */;
    virtual int GetImpulseIdByName(const CStr& impName) override /* 0x5c */;
    virtual CStr GetImpulseNameById(int impId) override /* 0x60 */;
    virtual CStr GetProfileFolder() override /* 0x84 */;
    virtual int HandleImpulse(const m3d::AuxImpulseInfo& impInfo, m3d::ui::Wnd* causeWnd) override /* 0x80 */;
}; /* size: 0x0080 */
