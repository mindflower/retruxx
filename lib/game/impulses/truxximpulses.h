#pragma once
#include <engine/impulses/gameimpulse.h>

class TruxxImpulse : public m3d::GameImpulse
{
protected:
    TruxxImpulse();
    TruxxImpulse(const TruxxImpulse& rhs);

public:
    virtual  ~TruxxImpulse() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classTruxxImpulse;
    virtual int GetGameModeIdByName(const CStr& modeName) override;
    virtual CStr GetGameModeNameById(int modeId) override;
    virtual int GetImpulseIdByName(const CStr& impName) override;
    virtual CStr GetImpulseNameById(int impId) override;
    virtual CStr GetProfileFolder() override;
    virtual int HandleImpulse(const m3d::AuxImpulseInfo& impInfo, m3d::ui::Wnd* causeWnd) override;
};
