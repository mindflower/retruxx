#include "maincursorwnd.h"

#include "maingameinterfacewnd.h"
#include "core/log.h"
#include "game/m3dgame.h"
#include "game/uimanager/uidefs.h"

RT_CLASS_EXPORTS_BEGIN(MainCursorWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(MainCursorWnd);

MainCursorWnd::AuxInfo::AuxInfo()
{
    m_texCapturingName = "CursorCapture";
    m_texCapturingSz.x = 32.0;
    m_texCapturingSz.y = 16.0;
    m_capturingRadius = 58.0;
    m_numCapturingSectors = 12;
}

bool MainCursorWnd::NeedShow() const
{
    const auto gameMode = (GameState)M3D_APP->GetCurGameMode();
    return M3D_APP->m_pInterfaceManager->IsGameModeValidForSmartCursor(gameMode) && M3D_APP->GetCursorShow0();
}

m3d::Class* MainCursorWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(SmartCursorWnd);
}

m3d::Object* MainCursorWnd::CreateObject()
{
    return new MainCursorWnd;
}

m3d::Class* MainCursorWnd::GetClass() const
{
    return RT_CLASS_LOCAL(MainCursorWnd);
}

MainCursorWnd::~MainCursorWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* MainCursorWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MainCursorWnd::OnShowCursor(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MainCursorWnd::CheckAndShow()
{
    using namespace m3d::ui;

    auto mainWnd = M3D_APP->m_pInterfaceManager->GetWindow(IW_WND_MAIN_GAME_INTERFACE);
    if (mainWnd)
    {
        if (IS_KIND_OF(mainWnd, MainGameInterfaceWnd))
        {
            auto* mainWndCasted = RT_DYNCAST(mainWnd.get(), MainGameInterfaceWnd);
            mainWndCasted->CheckAndShowMainCursorWnd();
        }
    }
}

void MainCursorWnd::FillCapturingDrawInfo(SmartCursorWnd::AuxDrawInfo*, int, bool) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MainCursorWnd::OnGameModeChanged(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MainCursorWnd::UpdateCapturingTex()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MainCursorWnd::OnNewFrame()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MainCursorWnd::GameDataSetup()
{
    if ((m_gameDataFlags & 2) == 0)
    {
        m_texCapturing = M3D_APP->m_pInterfaceManager->GetIcoByName(m_aif.m_texCapturingName, 0);
        M3D_RENDERER->ReferenceTexture(m_texCapturing);
        m_gameDataFlags |= 1u;
    }

    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }

    M3D_LOG_ERR("MainCursorWnd: error - fail to init because of a bad resource");
    return 0;
}

int MainCursorWnd::GameDataUpdate(void*, int dataType)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (dataType == 47 || dataType == 48)
    {
        CheckAndShow();
    }
    else if (dataType == 89)
    {
        OnNewFrame();
        return 1;
    }
    return 1;
}

MainCursorWnd::MainCursorWnd() = default;

MainCursorWnd::MainCursorWnd(MainCursorWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}
