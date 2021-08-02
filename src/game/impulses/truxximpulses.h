#pragma once
#include <engine/impulses/gameimpulse.h>

class TruxxImpulse : public m3d::GameImpulse
{
public:
    static m3d::Class m_classTruxxImpulse;

public:
    virtual int HandleImpulse(m3d::AuxImpulseInfo const &, m3d::ui::Wnd *);
    virtual int GetImpulseIdByName(CStr const &);
    virtual int GetGameModeIdByName(CStr const &);
    virtual CStr GetImpulseNameById(int);
    virtual CStr GetGameModeNameById(int);
    virtual m3d::Class * GetClass() const ;
    virtual Object * Clone();
    virtual ~TruxxImpulse();
    static Object * CreateObject();
    static m3d::Class * GetBaseClass();
    virtual CStr GetProfileFolder();

protected:
    TruxxImpulse();
    TruxxImpulse(TruxxImpulse const &);
};
