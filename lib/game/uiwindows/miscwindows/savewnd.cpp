#include "savewnd.h"

#include <core/log.h>
#include <game/m3dgame.h>
#include <game/uimanager/truxxuimanager.h>
#include <game/uimisc/guihelper.h>
#include <game/uimisc/savesmanager.h>
#include <m3dapp.h>
#include <ui/edit.h>
#include <ui/image.h>

namespace
{
    // The list itself plus the three widgets this window adds to it.
    unsigned const ID_SAVE_LIST = 400002;
    unsigned const ID_BTN_SAVE = 400200;
    unsigned const ID_BTN_DELETE = 400201;
    unsigned const ID_EDIT_SAVE_NAME = 400202;

    // Characters a save name may not contain - they would end up in a folder
    // name on disk.
    char const* const SERVICE_SYMBOLS = ".~!@#$%^&*|\\/\"<>?:";

    // Widest line the edit control is measured against before it starts
    // rejecting further input.
    float const EDIT_MEASURE_WIDTH = 1024.0f;
}  // namespace

RT_CLASS_EXPORTS_BEGIN(SaveWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(SaveWnd);

SaveWnd::SAuxInfo::SAuxInfo()
{
    // RVA 0x4C5A70
    m_btnSaveName = "btnSave";
    m_btnDeleteName = "btnDelete";
    m_editSaveName = "editSaveName";
}

SaveWnd::SaveWnd()
{
    // RVA 0x4C5B50
    m_btnSave = nullptr;
    m_btnDelete = nullptr;
    m_editSaveName = nullptr;
    m_bNeedScreenshot = false;
}

SaveWnd::SaveWnd(SaveWnd const&)
{
    // RVA 0x4C5C50 - the shipped copy constructor copies nothing from rhs and,
    // unlike the default one, leaves the three control pointers and the
    // screenshot flag uninitialised.
}

SaveWnd::~SaveWnd()
{
    // RVA 0x4C5C70 - only the two aux infos and the LSWnd base need unwinding.
}

m3d::Object* SaveWnd::Clone()
{
    // RVA 0x4C59B0
    return new SaveWnd(*this);
}

m3d::Object* SaveWnd::CreateObject()
{
    return new SaveWnd;
}

m3d::Class* SaveWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(LSWnd);
}

m3d::Class* SaveWnd::GetClass() const
{
    return RT_CLASS_LOCAL(SaveWnd);
}

int SaveWnd::GameDataSetup()
{
    // RVA 0x4C5CA0
    using namespace m3d::ui;
    if (!LSWnd::GameDataSetup())
    {
        return 0;
    }

    auto res = 1;
    if ((m_gameDataFlags & 2) == 0)
    {
        auto btnSave = RT_DYNCAST(GetChildByName(m_saif.m_btnSaveName), ButtonWnd);
        if (btnSave && btnSave->IsKindOf(RT_CLASS_LOCAL(ButtonWnd)))
        {
            m_btnSave = btnSave;
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_saif.m_btnSaveName + " is not found or incorrect type");
            res = 0;
        }

        auto btnDelete = RT_DYNCAST(GetChildByName(m_saif.m_btnDeleteName), ButtonWnd);
        if (btnDelete && btnDelete->IsKindOf(RT_CLASS_LOCAL(ButtonWnd)))
        {
            m_btnDelete = btnDelete;
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_saif.m_btnDeleteName + " is not found or incorrect type");
            res = 0;
        }

        auto editSaveName = RT_DYNCAST(GetChildByName(m_saif.m_editSaveName), EditWnd);
        if (editSaveName && editSaveName->IsKindOf(RT_CLASS_LOCAL(EditWnd)))
        {
            m_editSaveName = editSaveName;
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_saif.m_editSaveName + " is not found or incorrect type");
            res = 0;
        }

        // The very same list class serves both windows; this is what makes it
        // grow the leading "new save" row.
        m_wndSaveList->SetListType(SaveList::TYPE_SAVE);
        if (res)
        {
            OnCurProfileChanged();
        }
        else
        {
            // The shipped code clears the ready bit LSWnd set rather than
            // tracking the failure separately.
            m_gameDataFlags &= ~1u;
        }
    }

    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }
    M3D_LOG_INFO("SaveWnd: error - fail to init because of a bad resource");
    return 0;
}

int SaveWnd::GameDataUpdate(void* data, int dataType)
{
    // RVA 0x4C5F90 - dataType 43 toggles the window.
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (dataType != 43)
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

int SaveWnd::OnWndNotify(m3d::ui::Wnd* from, unsigned id, unsigned msg, m3d::AIParam const& data)
{
    // RVA 0x4C6020
    if (LSWnd::OnWndNotify(from, id, msg, data))
    {
        return 1;
    }
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }

    switch (id)
    {
    case ID_SAVE_LIST:
        // A double click on a row saves over it.
        if (msg != 4)
        {
            return 0;
        }
        OnSave();
        return 1;
    case ID_BTN_SAVE:
        if (msg != 1)
        {
            return 0;
        }
        OnSave();
        return 1;
    case ID_BTN_DELETE:
        if (msg != 1)
        {
            return 0;
        }
        OnDelete();
        return 1;
    case ID_EDIT_SAVE_NAME:
        if (msg == 5)
        {
            OnEditChange();
        }
        // NOTE: the edit notification is deliberately reported as unhandled.
        return 0;
    default:
        break;
    }
    return 0;
}

void SaveWnd::OnSave()
{
    // RVA 0x4C6190
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    CStr const name = m_editSaveName->GetText();
    if (name.empty())
    {
        M3D_APP->RunMsgBoxDlg(CStr(), M3D_APP->GetStringByStringId0("SaveNameDemand"), m3d::ui::MBX_OK, false);
        return;
    }

    if (name.findOneOf(SERVICE_SYMBOLS, 0) != -1)
    {
        // NOTE: the shipped code formats the message into itself, passing its
        // own buffer as the format string.
        CStr msg = M3D_APP->GetStringByStringId0("ServiceSymbolsUsed");
        msg.format(msg.c_str(), help::GetServiceSymbolsForVisualisation().c_str());
        M3D_APP->RunMsgBoxDlg(CStr(), msg, m3d::ui::MBX_OK, false);
        return;
    }

    // Index 0 is the "new save" row, everything below it would be overwritten.
    // NOTE: a selection of -1 falls into the overwrite branch and then indexes
    // the item list with -1; the shipped code never checks for it because the
    // Save button is disabled while nothing is selected. OnEnter can still get
    // here, though.
    auto const curSel = m_wndSaveList->GetCurSel();
    if (curSel != 0)
    {
        auto const answer = M3D_APP->RunMsgBoxDlg(
            CStr(), M3D_APP->GetStringByStringId0("SaveGameExists"), m3d::ui::MBX_YES_NO, false);
        if (answer == m3d::ui::MBX_RET_NO)
        {
            return;
        }
    }

    auto* btn = m_wndSaveList->GetItem(curSel);
    if (!btn)
    {
        return;
    }

    auto* savesManager = M3D_APP->m_pInterfaceManager->GetSavesManager();
    CStr folderName;
    if (btn->GetSaveFolderName().empty())
    {
        folderName = savesManager->GetNewSaveFolderName();
    }
    else
    {
        folderName = btn->GetSaveFolderName();
    }

    savesManager->SaveGame(name, folderName, false);
    M3D_APP->m_pInterfaceManager->ShowWindow(m_guiId, false, false, false, false, nullptr);
    M3D_APP->EnqueueMessage(UM_GAME_MENU_MODE_EXIT, 0, 0, 0, 0, {}, {});
}

void SaveWnd::OnDelete()
{
    // RVA 0x4C65D0 - index 0 is the "new save" row and has nothing to delete.
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    auto const curSel = m_wndSaveList->GetCurSel();
    if (curSel == -1 || curSel == 0)
    {
        return;
    }
    auto* btn = m_wndSaveList->GetItem(curSel);
    if (!btn)
    {
        return;
    }

    auto const answer = M3D_APP->RunMsgBoxDlg(
        CStr(), M3D_APP->GetStringByStringId0("SaveGameDeleteWarning"), m3d::ui::MBX_YES_NO, false);
    if (answer == m3d::ui::MBX_RET_NO)
    {
        return;
    }

    if (M3D_APP->m_pInterfaceManager->GetSavesManager()->DeleteSaveGame(btn->GetSaveFolderName()))
    {
        m_wndSaveList->FullUpdate();
    }
    else
    {
        M3D_LOG_INFO("SaveWnd::OnDelete error - cannot delete game " + btn->GetSaveName());
    }
}

void SaveWnd::OnSaveSelectionChange()
{
    // RVA 0x4C6760
    LSWnd::OnSaveSelectionChange();
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    auto const curSel = m_wndSaveList->GetCurSel();
    SaveButton* btn = nullptr;
    if (curSel != -1)
    {
        btn = m_wndSaveList->GetItem(curSel);
    }

    m_btnSave->EnableWindow(curSel != -1);
    m_btnDelete->EnableWindow(curSel != -1 && curSel != 0);
    m_editSaveName->SetText(btn ? btn->GetSaveName() : CStr());
    m_editSaveName->SetQuickEraseState(true);
}

int SaveWnd::SetScreenshotForSave(int saveIdx)
{
    // RVA 0x4C6840 - the "new save" row previews the live game screenshot
    // instead of one read back from disk.
    if (saveIdx != 0)
    {
        return LSWnd::SetScreenshotForSave(saveIdx);
    }

    m_wndScreenshot->ShowWindow(true);
    auto const screenshot = M3D_APP->m_pInterfaceManager->GetSavesManager()->GetCurGameScreenshot();
    m_wndScreenshot->SetImage(screenshot);
    return screenshot.IsValid();
}

void SaveWnd::OnEditChange()
{
    // RVA 0x4C68A0 - the save name may not grow wider than its edit box, so the
    // symbol that overflowed it is taken straight back out again.
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    CStr const saveName = m_editSaveName->GetText();
    auto const editClientB = m_editSaveName->GetClientBounds();
    auto const saveNameSz =
        GetGfxServer()->MeasureText(saveName, m_editSaveName->GetDefaultFont(), m3d::TW_NOWRAP, EDIT_MEASURE_WIDTH);

    if (saveNameSz.x > editClientB.width && !saveName.empty())
    {
        m_editSaveName->DeleteSymbol(m_editSaveName->GetMaxCursorPos() - 1);
        if (m_editSaveName->GetCursorPosition() > m_editSaveName->GetMaxCursorPos())
        {
            m_editSaveName->DecrementCursorPosition();
        }
    }
}

void SaveWnd::OnEnter()
{
    // RVA 0x4C6990
    OnSave();
}
