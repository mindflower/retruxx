#include "demosplashwnd.h"

#include <windows.h>

#include <shellapi.h>

#include <config.h>
#include <core/console/cvar.h>
#include <core/kernel.h>
#include <core/log.h>
#include <game/m3dgame.h>
#include <m3dapp.h>
#include <ui/button.h>
#include <ui/wndstation.h>

namespace
{
    // Control ids of the three "visit our site" buttons and the plain OK one.
    unsigned const ID_BTN_EXMACHINA = 300100;
    unsigned const ID_BTN_BUKA = 300101;
    unsigned const ID_BTN_NIVAL = 300102;
    unsigned const ID_BTN_OK = 300110;

    // String ids the URLs are looked up under.
    CStr const EXMACHINA_URL("ExMachina URL");
    CStr const BUKA_URL("Buka URL");
    CStr const NIVAL_URL("Nival URL");
}  // namespace

RT_CLASS_EXPORTS_BEGIN(DemoSplashWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(DemoSplashWnd);

DemoSplashWnd::AuxInfo::AuxInfo()
{
    // RVA 0x4B40E0
    m_btnTargemName = "btnTargem";
}

DemoSplashWnd::DemoSplashWnd()
{
    // RVA 0x4B4130
    m_btnTargem = nullptr;
}

DemoSplashWnd::DemoSplashWnd(DemoSplashWnd const&)
{
    // RVA 0x4B41F0 - the shipped copy constructor is an assert(0); the splash is
    // a singleton owned by the interface manager and cannot be duplicated.
    RETRUXX_NOT_IMPLEMENTED;
}

DemoSplashWnd::~DemoSplashWnd()
{
    // RVA 0x4B4230 - the window owns nothing of its own.
}

m3d::Object* DemoSplashWnd::Clone()
{
    // RVA 0x4B4020 - allocates, runs the asserting copy constructor and returns
    // null; cloning this window is not supported.
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* DemoSplashWnd::CreateObject()
{
    return new DemoSplashWnd;
}

m3d::Class* DemoSplashWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ModalWnd);
}

m3d::Class* DemoSplashWnd::GetClass() const
{
    return RT_CLASS_LOCAL(DemoSplashWnd);
}

int DemoSplashWnd::GameDataSetup()
{
    // RVA 0x4B4270
    if ((m_gameDataFlags & 2) == 0)
    {
        m3d::Object* btnTargem = GetChildByName(m_aif.m_btnTargemName);
        if (btnTargem && btnTargem->IsKindOf(&m3d::ui::ButtonWnd::m_classButtonWnd))
        {
            m_gameDataFlags |= 1u;
            m_btnTargem = static_cast<m3d::ui::ButtonWnd*>(btnTargem);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_btnTargemName + " is not found or incorrect type");
        }
    }

    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }

    // NOTE: unlike its siblings this window logs the failure at error level.
    M3D_LOG_ERR("DemoSplashWnd: error - fail to init because of a bad resource");
    return 0;
}

int DemoSplashWnd::OnWndNotify(m3d::ui::Wnd* from, unsigned idFrom, unsigned message, m3d::AIParam const& data)
{
    // RVA 0x4B4430 - each publisher button opens its site in the browser and then
    // closes the splash; the plain OK button just closes it.
    if (ModalWnd::OnWndNotify(from, idFrom, message, data))
    {
        return 1;
    }
    if (message != 1)
    {
        return 0;
    }

    CStr url;
    switch (idFrom)
    {
    case ID_BTN_EXMACHINA:
        url = GetStation()->GetStringByStringId0(EXMACHINA_URL);
        break;
    case ID_BTN_BUKA:
        url = GetStation()->GetStringByStringId0(BUKA_URL);
        break;
    case ID_BTN_NIVAL:
        url = GetStation()->GetStringByStringId0(NIVAL_URL);
        break;
    case ID_BTN_OK:
        break;
    default:
        return 0;
    }

    if (!url.empty())
    {
        CStr const sUrl = url;
        ShellExecuteA(nullptr, "open", "iexplore", sUrl.c_str(), nullptr, SW_SHOWMAXIMIZED);
    }
    CloseModal(0);
    return 1;
}

int DemoSplashWnd::OnPaint(m3d::ui::DrawInfo const& di)
{
    // RVA 0x4B45C0 - folded onto the base painter in the shipped build.
    return ModalWnd::OnPaint(di);
}

int DemoSplashWnd::OnActivate(bool on)
{
    // RVA 0x4B45D0 - the splash comes up in silence.
    if (on)
    {
        M3D_APP->StopPlayingMusic();
        if (M3D_ENGINE_CFG.m_snd_Enable.GetB())
        {
            M3D_APP->m_sound->StopAllSounds();
        }
    }
    return Wnd::OnActivate(on);
}
