#include "helpsimplemsgwnd.h"

RT_CLASS_EXPORTS_BEGIN(HelpSimpleMsgWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(HelpSimpleMsgWnd);

void HelpSimpleMsgWnd::SetupForHelp(CStr const&, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* HelpSimpleMsgWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* HelpSimpleMsgWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(HelpWnd);
}

m3d::Object* HelpSimpleMsgWnd::CreateObject()
{
    return new HelpSimpleMsgWnd;
}

HelpSimpleMsgWnd::~HelpSimpleMsgWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* HelpSimpleMsgWnd::GetClass() const
{
    return RT_CLASS_LOCAL(HelpSimpleMsgWnd);
}

HelpSimpleMsgWnd::HelpSimpleMsgWnd()
{
    m_wndHelpText = 0;
    m_wndTitle = 0;
}

HelpSimpleMsgWnd::HelpSimpleMsgWnd(HelpSimpleMsgWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int HelpSimpleMsgWnd::GameDataSetup()
{
    // TODO: implement HelpSimpleMsgWnd::GameDataSetup
    // RETRUXX_NOT_IMPLEMENTED;
    return 1;
}

int HelpSimpleMsgWnd::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}
