#include "helpwnd.h"

RT_CLASS_EXPORTS_BEGIN(HelpWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(HelpWnd);

HelpWnd::AuxInfo::AuxInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* HelpWnd::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* HelpWnd::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* HelpWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ModalWnd);
}

m3d::Object* HelpWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

HelpWnd::~HelpWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void HelpWnd::AllowDisableAutoHelp(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int HelpWnd::OnAfterRemoveFromWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int HelpWnd::OnBeforeAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void HelpWnd::DisableAutoHelp(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void HelpWnd::CheckAndShowAutoHelpControls()
{
    RETRUXX_NOT_IMPLEMENTED;
}

HelpWnd::HelpWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

HelpWnd::HelpWnd(HelpWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int HelpWnd::GameDataSetup()
{
    RETRUXX_NOT_IMPLEMENTED;
}
