#pragma once

class TruxxImpulse :  public m3d::GameImpulse
{
public:
    virtual int HandleImpulse(struct m3d::AuxImpulseInfo const &,class m3d::ui::Wnd *);
    virtual int GetImpulseIdByName(class CStr const &);
    virtual int GetGameModeIdByName(class CStr const &);
    virtual class CStr GetImpulseNameById(int);
    virtual class CStr GetGameModeNameById(int);
    virtual struct m3d::Class * GetClass() const ;
    virtual class m3d::Object * Clone();
    virtual ~TruxxImpulse();
    static class m3d::Object * __fastcall CreateObject();
    static struct m3d::Class * __fastcall GetBaseClass();
    virtual class CStr GetProfileFolder();
protected:
    TruxxImpulse();
    TruxxImpulse(class TruxxImpulse const &);
private:
};
