#include "helpsimplemsgwnd.h"

RT_CLASS_EXPORTS_BEGIN(HelpSimpleMsgWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(HelpSimpleMsgWnd);

void HelpSimpleMsgWnd::SetupForHelp(CStr const&, bool)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* HelpSimpleMsgWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* HelpSimpleMsgWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(HelpWnd);
}

m3d::Object* HelpSimpleMsgWnd::CreateObject()
{
    throw std::logic_error("Not implemented");
}

HelpSimpleMsgWnd::~HelpSimpleMsgWnd()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* HelpSimpleMsgWnd::GetClass() const
{
    throw std::logic_error("Not implemented");
}

HelpSimpleMsgWnd::HelpSimpleMsgWnd()
{
    throw std::logic_error("Not implemented");
}

HelpSimpleMsgWnd::HelpSimpleMsgWnd(HelpSimpleMsgWnd const&)
{
    throw std::logic_error("Not implemented");
}

int HelpSimpleMsgWnd::GameDataSetup()
{
    throw std::logic_error("Not implemented");
}

int HelpSimpleMsgWnd::GameDataClear(bool)
{
    throw std::logic_error("Not implemented");
}
