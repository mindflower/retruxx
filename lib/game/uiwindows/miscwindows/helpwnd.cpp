#include "helpwnd.h"

RT_CLASS_EXPORTS_BEGIN(HelpWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(HelpWnd);

HelpWnd::AuxInfo::AuxInfo()
{
    m_checkDisableAutoHelpName = "checkDisableAutoHelp";
    m_wndCheckDisableAutoHelpEmbossName = "checkDisableAutoHelp_emboss";
    m_lblDisableAutoHelpName = "lblDisableAutoHelp";
}

m3d::Object* HelpWnd::CreateObject()
{
    return new HelpWnd;
}

m3d::Class* HelpWnd::GetClass() const
{
    return RT_CLASS_LOCAL(HelpWnd);
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
    m_checkDisableAutoHelp = 0;
    m_wndCheckDisableAutoHelpEmboss = 0;
    m_lblDisableAutoHelp = 0;
    m_bAllowDisableAutoHelp = 0;
}

HelpWnd::HelpWnd(HelpWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int HelpWnd::GameDataSetup()
{
    // TODO: implement HelpWnd::GameDataSetup
    // RETRUXX_NOT_IMPLEMENTED;
    return 1;
}
