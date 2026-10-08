#include "loadwnd.h"

#include <core/log.h>
#include <game/m3dgame.h>
#include <game/uimanager/truxxuimanager.h>
#include <game/uimisc/savesmanager.h>
#include <m3dapp.h>

namespace
{
    // The list itself and the Load button.
    unsigned const ID_SAVE_LIST = 400002;
    unsigned const ID_BTN_LOAD = 400100;
}  // namespace

RT_CLASS_EXPORTS_BEGIN(LoadWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(LoadWnd);

LoadWnd::LAuxInfo::LAuxInfo()
{
    // RVA 0x4B9BD0
    m_btnLoadName = "btnLoad";
}

LoadWnd::LoadWnd()
{
    // RVA 0x4B9C20
    m_btnLoad = nullptr;
}

LoadWnd::LoadWnd(LoadWnd const&)
{
    // RVA 0x4B9CB0 - the shipped copy constructor copies nothing from rhs and,
    // unlike the default one, leaves m_btnLoad uninitialised.
}

LoadWnd::~LoadWnd()
{
    // RVA 0x4B9CD0 - only the two aux infos and the LSWnd base need unwinding.
}

m3d::Object* LoadWnd::Clone()
{
    // RVA 0x4B9B20
    return new LoadWnd(*this);
}

m3d::Object* LoadWnd::CreateObject()
{
    return new LoadWnd;
}

m3d::Class* LoadWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(LSWnd);
}

m3d::Class* LoadWnd::GetClass() const
{
    return RT_CLASS_LOCAL(LoadWnd);
}

int LoadWnd::GameDataSetup()
{
    // RVA 0x4B9D20
    using namespace m3d::ui;
    if (!LSWnd::GameDataSetup())
    {
        return 0;
    }
    if ((m_gameDataFlags & 2) == 0)
    {
        auto btnLoad = RT_DYNCAST(GetChildByName(m_laif.m_btnLoadName), ButtonWnd);
        if (btnLoad && btnLoad->IsKindOf(RT_CLASS_LOCAL(ButtonWnd)))
        {
            m_btnLoad = btnLoad;
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_laif.m_btnLoadName + " is not found or incorrect type");
            // The shipped code clears the ready bit LSWnd set rather than
            // tracking the failure separately.
            m_gameDataFlags &= ~1u;
        }
    }
    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }
    M3D_LOG_INFO("LoadWnd: error - fail to init because of a bad resource");
    return 0;
}

int LoadWnd::GameDataUpdate(void* data, int dataType)
{
    // RVA 0x4B9E70 - dataType 44 toggles the window.
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (dataType != 44)
    {
        return LSWnd::GameDataUpdate(data, dataType);
    }
    if (!IsChildOf(M3D_APP))
    {
        M3D_APP->m_pInterfaceManager->ShowWindow(m_guiId, true, true, true, true, nullptr);
    }
    else
    {
        M3D_APP->m_pInterfaceManager->ShowWindow(m_guiId, false, false, false, false, nullptr);
    }
    return 1;
}

int LoadWnd::OnWndNotify(m3d::ui::Wnd* from, unsigned id, unsigned msg, m3d::AIParam const& data)
{
    // RVA 0x4B9F00 - double-clicking a save loads it, as does the Load button.
    if (LSWnd::OnWndNotify(from, id, msg, data))
    {
        return 1;
    }
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }

    auto res = false;
    if (id == ID_SAVE_LIST)
    {
        res = msg == 4;
    }
    else
    {
        if (id != ID_BTN_LOAD)
        {
            return 0;
        }
        res = msg == 1;
    }
    if (!res)
    {
        return 0;
    }
    OnLoad();
    return 1;
}

void LoadWnd::OnLoad()
{
    // RVA 0x4B9F60
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    auto const curSel = m_wndSaveList->GetCurSel();
    if (curSel == -1)
    {
        return;
    }
    auto* btn = m_wndSaveList->GetItem(curSel);
    if (!btn)
    {
        return;
    }

    M3D_APP->EnqueueMessage(UM_GAME_MENU_MODE_EXIT, 0, 0, 0, 0, {}, {});
    M3D_APP->m_pInterfaceManager->ShowWindow(m_guiId, false, false, false, false, nullptr);
    M3D_APP->m_pInterfaceManager->GetSavesManager()->LoadGame(btn->GetSaveFolderName());
}

void LoadWnd::OnSaveSelectionChange()
{
    // RVA 0x4BA070
    LSWnd::OnSaveSelectionChange();
    if ((m_gameDataFlags & 1) != 0)
    {
        m_btnLoad->EnableWindow(m_wndSaveList->GetCurSel() != -1);
    }
}

void LoadWnd::OnEnter()
{
    // RVA 0x4BA0B0
    if ((m_gameDataFlags & 1) != 0 && m_wndSaveList->GetCurSel() != -1)
    {
        OnLoad();
    }
}
