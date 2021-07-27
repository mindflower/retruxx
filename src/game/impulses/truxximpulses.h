#pragma once

class TruxxImpulse :  public m3d::GameImpulse
{
public:
    virtual int HandleImpulse(struct m3d::AuxImpulseInfo const &,class m3d::ui::Wnd *);
    virtual int GetImpulseIdByName(CStr const &);
    virtual int GetGameModeIdByName(CStr const &);
    virtual CStr GetImpulseNameById(int);
    virtual CStr GetGameModeNameById(int);
    virtual struct m3d::Class * GetClass() const ;
    virtual class m3d::Object * Clone();
    virtual ~TruxxImpulse();
    static class m3d::Object * __fastcall CreateObject();
    static struct m3d::Class * __fastcall GetBaseClass();
    virtual CStr GetProfileFolder();
protected:
    TruxxImpulse();
    TruxxImpulse(class TruxxImpulse const &);
private:
};
