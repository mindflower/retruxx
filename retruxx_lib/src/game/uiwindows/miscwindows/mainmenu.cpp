#include "mainmenu.h"
#include <game/m3dgame.h>
#include <core/log.h>

#include "config.h"
#include "video.h"
#include "game/profile.h"

RT_CLASS_EXPORTS_BEGIN(MainMenuUI)
    RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(MainMenuUI);

MainMenuUI::AuxInfo::AuxInfo()
{
}

m3d::Class* MainMenuUI::GetClass() const
{
    return RT_CLASS_LOCAL(MainMenuUI);
}

MainMenuUI::~MainMenuUI()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* MainMenuUI::CreateObject()
{
    return new MainMenuUI;
}

m3d::Class* MainMenuUI::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

void MainMenuUI::OnFinishVideoPlaying()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* MainMenuUI::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MainMenuUI::OnBeforeAddToWndStation()
{
    GetStation()->CaptureFocus(this);
    return Wnd::OnBeforeAddToWndStation();
}

MainMenuUI::MainMenuUI()
{
}

MainMenuUI::MainMenuUI(MainMenuUI const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MainMenuUI::OnNewGame()
{
    M3D_APP->m_pInterfaceManager->ShowWindow(
        172,
        true,
        true,
        true,
        true,
        nullptr);

    const auto cmd = "/map " + CStr(M3D_ENGINE_CFG.m_firstLevel.GetS());
    M3D_ENGINE_CFG.m_console->executeCommand(cmd);
}

int MainMenuUI::OnAfterRemoveFromWndStation()
{
    auto const res = Wnd::OnAfterRemoveFromWndStation();
    GetStation()->CaptureFocus(nullptr);
    return res;
}

int MainMenuUI::OnKey(unsigned short key, unsigned char scanCode, unsigned state)
{
    if (!state || key != 1 || M3D_APP->M3dVideoPlayer->IsVideoPlaing())
    {
        return m3d::ui::Wnd::OnKey(key, scanCode, state);
    }
    MainMenuUI::QuitToWindows();
    return 1;
}

int MainMenuUI::GameDataSetup()
{
    if ((m_gameDataFlags & 2) != 0)
    {
        return 1;
    }
    auto res = 1;
    auto version = GetChildByName(m_aif.m_wndVersionName);
    if (version && version->IsKindOf(RT_CLASS_LOCAL(Wnd)))
    {
        m_wndVersion = dynamic_cast<Wnd*>(version);
    }
    else
    {
        M3D_LOG_INFO("Get control error: control " + m_aif.m_wndVersionName + " is not found or incorrect type");
        res = 0;
    }
    auto profile = GetChildByName(m_aif.m_wndProfileName);
    if (!profile || !IsKindOf(RT_CLASS_LOCAL(Wnd)))
    {
        M3D_LOG_INFO("Get control error: control " + m_aif.m_wndProfileName + " is not found or incorrect type");
        return 1;
    }
    m_wndProfile = dynamic_cast<Wnd*>(profile);
    if (!res)
    {
        return 1;
    }
    auto title = m3d::Application::g_pApp->GetWindowTitle();
    m_wndVersion->SetText(title);
    m_gameDataFlags |= 1;
    return 1;
}

void MainMenuUI::OnStartVideoPlaying()
{
    if (IsChildOf(M3D_APP))
    {
        M3D_APP->CaptureMouse(this);
        if (M3D_APP->IsDXCursorEnabled())
        {
            M3D_APP->m_renderer->ShowDXCursor(false);
        }
    }
}

void MainMenuUI::QuitToWindows()
{
    if (M3D_APP->RunMsgBoxDlg("", M3D_APP->GetStringByStringId0("quitGame"), 2u, false) == m3d::ui::MBX_RET_OK)
    {
        M3D_APP->m_pInterfaceManager->ShowWindow(m_guiId, false, false, false, false, nullptr);
        M3D_APP->ClearViewportToBlack();
        M3D_APP->m_pInterfaceManager->ShowWindow(167, true, true, true, true, nullptr);
        M3D_APP->EnqueueMessage(1, 0, 0, 0, 0, {}, {});
    }

}

int MainMenuUI::OnWndNotify(m3d::ui::Wnd* from, unsigned id, unsigned msg, m3d::AIParam const& data)
{
    if ((m_style & 0x100000) != 0)
        ReflectChildNotifyToParent(from, id, msg, data);
    if (msg != 1)
        return 0;
    switch(id)
    {
    case 0x92824u:
    {
        QuitToWindows();
        return 1;
    }
    case 0x92825u:
    {
        OnNewGame();
        return 1;
    }
    case 0x92826u:
    {
        M3D_APP->EnqueueMessage(65645, 0, 0, 0, 0, {}, {});
        return 1;
    }
    case 0x92827u:
    {
        M3D_APP->m_pInterfaceManager->ShowWindow(148, true, true, true, false, nullptr);
        return 1;
    }
    case 0x92828u:
    {
        OnStartVideoPlaying();
        M3D_APP->StartPlayingVideo(M3D_KERNEL->GetEngineCfg().m_intro.GetS(), &CMiracle3d::OnFinishVideoPlaying);
        return 1;
    }
    case 0x92829u:
    {
        M3D_APP->m_pInterfaceManager->ShowWindow(165, true, true, true, false, nullptr);
        return 1;
    }
    case 0x9282Au:
    {
        M3D_APP->m_pInterfaceManager->ShowWindow(111, true, true, true, false, nullptr);
        return 1;
    }
    default:
        break;
    }
    return 0;
}

void MainMenuUI::OnCurProfileChanged()
{
    // TODO: implement MainMenuUI::OnCurProfileChanged
    //if ((m_gameDataFlags & 1) != 0)
    //{
    //    auto profile = M3D_APP->GetProfileManager()->GetCurProfile();
    //    if (profile)
    //    {
    //        
    //    }
    //}
   // RETRUXX_NOT_IMPLEMENTED;
}

int MainMenuUI::GameDataUpdate(void* data, int dataType)
{
    if ((this->m_gameDataFlags & 1) == 0)
        return 0;
    if (dataType == 40)
        OnCurProfileChanged();
    return 1;
}
