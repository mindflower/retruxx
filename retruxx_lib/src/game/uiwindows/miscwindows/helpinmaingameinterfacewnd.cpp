#include "helpinmaingameinterfacewnd.h"

RT_CLASS_EXPORTS_BEGIN(HelpInMainGameInterfaceWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(HelpInMainGameInterfaceWnd);

m3d::Class* HelpInMainGameInterfaceWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(HelpWnd);
}

m3d::Object* HelpInMainGameInterfaceWnd::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* HelpInMainGameInterfaceWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* HelpInMainGameInterfaceWnd::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

HelpInMainGameInterfaceWnd::~HelpInMainGameInterfaceWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int HelpInMainGameInterfaceWnd::GameDataSetup()
{
    RETRUXX_NOT_IMPLEMENTED;
}

HelpInMainGameInterfaceWnd::HelpInMainGameInterfaceWnd(HelpInMainGameInterfaceWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

HelpInMainGameInterfaceWnd::HelpInMainGameInterfaceWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int HelpInMainGameInterfaceWnd::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void HelpInMainGameInterfaceWnd::HideHelp()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void HelpInMainGameInterfaceWnd::ShowHelp(HelpManager::HelpId)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int HelpInMainGameInterfaceWnd::OnBeforeAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

HelpManager::HelpId HelpInMainGameInterfaceWnd::GetHelpIdByCtrlId(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}
