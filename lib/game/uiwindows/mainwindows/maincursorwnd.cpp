#include "maincursorwnd.h"

#include <cmath>

#include "maingameinterfacewnd.h"
#include "core/log.h"
#include "game/m3dgame.h"
#include "game/uimanager/uidefs.h"

#include "server/objects/base/globalproperties.h"
#include "server/objects/base/objcontainer.h"
#include "server/objects/player.h"
#include "server/objects/vehicle.h"

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
    auto const gameMode = (GameState)M3D_APP->GetCurGameMode();
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
    // RVA 0x128740 - the AuxInfo CStr and SmartCursorWnd base clean themselves
    // up automatically.
    M3D_RENDERER->ReleaseTexture(m_texCapturing);
}

m3d::Object* MainCursorWnd::Clone()
{
    // RVA 0x128530
    return new MainCursorWnd(*this);
}

void MainCursorWnd::OnShowCursor(void*)
{
    // RVA 0x128C30
    CheckAndShow();
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

void MainCursorWnd::FillCapturingDrawInfo(SmartCursorWnd::AuxDrawInfo* auxDi, int sectorId, bool bTransparent) const
{
    // RVA 0x128AC0
    if (!auxDi)
    {
        return;
    }

    auxDi->SetTexture(m_texCapturing);
    auxDi->m_sz = m_aif.m_texCapturingSz;
    auxDi->m_color = bTransparent ? 0x80FFFFFFu : 0xFFFFFFFFu;
    auxDi->m_angle = (static_cast<float>(sectorId) / static_cast<float>(m_aif.m_numCapturingSectors)) * 6.2831855f;

    float const radius = m_aif.m_capturingRadius;
    float const s = std::sin(auxDi->m_angle);
    float const c = std::cos(auxDi->m_angle);
    auxDi->m_coord.x = radius * s;
    auxDi->m_coord.y = -(radius * c);
}

void MainCursorWnd::OnGameModeChanged(void*)
{
    // RVA 0x128C40
    CheckAndShow();
}

void MainCursorWnd::UpdateCapturingTex()
{
    // RVA 0x1288D0
    if (!ai::thePlayer)
    {
        return;
    }
    ai::Vehicle* vehicle = ai::thePlayer->GetVehicle();
    if (!vehicle)
    {
        return;
    }

    int const lockedId = vehicle->GetToBeLockedObjId();
    if (lockedId < 0)
    {
        return;
    }

    ai::Obj* target = ai::theObjects->GetEntityByObjId(lockedId);
    if (!target)
    {
        return;
    }

    unsigned const flags = target->GetFlags();
    if ((flags & 8) != 0 || (flags & 2) != 0 || target->GetParentRepository())
    {
        return;
    }

    // Semi-transparent background ring - one sector for every m_numCapturingSectors.
    for (int i = 0; i < m_aif.m_numCapturingSectors; ++i)
    {
        auto* auxDi = new SmartCursorWnd::AuxDrawInfo();
        FillCapturingDrawInfo(auxDi, i, true);
        m_drawInfo.push_back(auxDi);
    }

    // Opaque progress fill - sector count proportional to lock-on progress.
    float const progress = vehicle->GetTimeToLockTarget() *
        (static_cast<float>(m_aif.m_numCapturingSectors) / ai::theGlobProp.m_lockTimeout);
    int numFilled = static_cast<int>(progress);
    if (numFilled < 0)
    {
        numFilled = 0;
    }
    if (numFilled > m_aif.m_numCapturingSectors)
    {
        numFilled = m_aif.m_numCapturingSectors;
    }
    for (int i = 0; i < numFilled; ++i)
    {
        auto* auxDi = new SmartCursorWnd::AuxDrawInfo();
        FillCapturingDrawInfo(auxDi, i, false);
        m_drawInfo.push_back(auxDi);
    }
}

void MainCursorWnd::OnNewFrame()
{
    ClearDrawInfo();
    UpdateCapturingTex();
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

MainCursorWnd::MainCursorWnd(MainCursorWnd const&) : MainCursorWnd()
{
    // Matches the original: the copy ctor default-constructs the base, the
    // AuxInfo, and an invalid capturing-texture handle; nothing is copied
    // from the source.
}
