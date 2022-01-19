#include "truxximpulses.h"
#include <stdexcept>

RT_CLASS_DEFINE(TruxxImpulse);

int TruxxImpulse::HandleImpulse(m3d::AuxImpulseInfo const&, m3d::ui::Wnd*)
{
    throw std::logic_error("Not implemented");
}

int TruxxImpulse::GetImpulseIdByName(CStr const&)
{
    throw std::logic_error("Not implemented");
}

int TruxxImpulse::GetGameModeIdByName(CStr const&)
{
    throw std::logic_error("Not implemented");
}

CStr TruxxImpulse::GetImpulseNameById(int)
{
    throw std::logic_error("Not implemented");
}

CStr TruxxImpulse::GetGameModeNameById(int)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* TruxxImpulse::GetClass() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* TruxxImpulse::Clone()
{
    throw std::logic_error("Not implemented");
}

TruxxImpulse::~TruxxImpulse()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* TruxxImpulse::CreateObject()
{
    return new TruxxImpulse;
}

m3d::Class* TruxxImpulse::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

CStr TruxxImpulse::GetProfileFolder()
{
    throw std::logic_error("Not implemented");
}

TruxxImpulse::TruxxImpulse()
{
}

TruxxImpulse::TruxxImpulse(TruxxImpulse const&)
{
    throw std::logic_error("Not implemented");
}
