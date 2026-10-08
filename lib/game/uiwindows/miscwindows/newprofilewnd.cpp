#include "newprofilewnd.h"

#include <core/log.h>
#include <game/m3dgame.h>
#include <game/profile.h>
#include <game/uimanager/truxxuimanager.h>
#include <game/uimisc/guihelper.h>
#include <m3dapp.h>
#include <ui/button.h>
#include <ui/edit.h>

namespace
{
    // Control ids of the OK button and the name edit box.
    unsigned const ID_BTN_OK = 200000;
    unsigned const ID_EDIT_NAME = 200001;

    // Characters a profile name may not contain - it becomes a folder on disk.
    char const* const SERVICE_SYMBOLS = ".~!@#$%^&*|\\/\"<>?:";

    // Widest line the edit control is measured against before it starts
    // rejecting further input.
    float const EDIT_MEASURE_WIDTH = 1024.0f;
}  // namespace

RT_CLASS_EXPORTS_BEGIN(NewProfileWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(NewProfileWnd);

NewProfileWnd::AuxInfo::AuxInfo()
{
    // RVA 0x4C2080
    m_editNameName = "editName";
    m_btnCancelName = "btnCancel";
}

NewProfileWnd::NewProfileWnd()
{
    // RVA 0x4C2110
    m_editName = nullptr;
    m_btnCancel = nullptr;
}

NewProfileWnd::NewProfileWnd(NewProfileWnd const&)
{
    // RVA 0x4C21C0 - the shipped copy constructor copies nothing from rhs and,
    // unlike the default one, leaves the two control pointers uninitialised.
}

NewProfileWnd::~NewProfileWnd()
{
    // RVA 0x4C21E0 - only the aux info and the ModalWnd base need unwinding.
}

m3d::Object* NewProfileWnd::Clone()
{
    // RVA 0x4C1FD0
    return new NewProfileWnd(*this);
}

m3d::Object* NewProfileWnd::CreateObject()
{
    return new NewProfileWnd;
}

m3d::Class* NewProfileWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ModalWnd);
}

m3d::Class* NewProfileWnd::GetClass() const
{
    return RT_CLASS_LOCAL(NewProfileWnd);
}

int NewProfileWnd::GameDataSetup()
{
    // RVA 0x4C2250
    using namespace m3d::ui;
    auto res = 1;
    if ((m_gameDataFlags & 2) == 0)
    {
        auto editWnd = RT_DYNCAST(GetChildByName(m_aif.m_editNameName), EditWnd);
        if (editWnd && editWnd->IsKindOf(RT_CLASS_LOCAL(EditWnd)))
        {
            m_editName = editWnd;
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_editNameName + " is not found or incorrect type");
            res = 0;
        }

        auto btnCancel = RT_DYNCAST(GetChildByName(m_aif.m_btnCancelName), ButtonWnd);
        if (btnCancel && btnCancel->IsKindOf(RT_CLASS_LOCAL(ButtonWnd)))
        {
            m_btnCancel = btnCancel;
            if (res)
            {
                m_gameDataFlags |= 1u;
            }
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_btnCancelName + " is not found or incorrect type");
        }
    }
    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }
    M3D_LOG_INFO("NewProfileWnd: error - fail to init because of a bad resource");
    return 0;
}

int NewProfileWnd::OnBeforeAddToWndStation()
{
    // RVA 0x4C2530 - the very first profile cannot be cancelled out of, since
    // the game has nowhere to go back to.
    if ((m_gameDataFlags & 1) != 0)
    {
        m_btnCancel->EnableWindow(M3D_APP->GetProfileManager()->GetCurProfile() != nullptr);
        m_editName->SetText(M3D_APP->GetProfileManager()->GetDefaultProfileName());
    }
    return m3d::ui::Wnd::OnBeforeAddToWndStation();
}

int NewProfileWnd::OnWndNotify(m3d::ui::Wnd* from, unsigned id, unsigned msg, m3d::AIParam const& data)
{
    // RVA 0x4C25C0
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (!ModalWnd::OnWndNotify(from, id, msg, data))
    {
        if (id == ID_BTN_OK)
        {
            if (msg == 1 && CreateNewProfile())
            {
                int hideMode = 1;
                M3D_APP->m_pInterfaceManager->ShowWindow(m_guiId, false, false, false, false, &hideMode);
            }
        }
        else
        {
            if (id != ID_EDIT_NAME)
            {
                return 0;
            }
            if (msg == 5)
            {
                OnEditChange();
                return 1;
            }
        }
    }
    // NOTE: every path that gets this far reports the notification as handled,
    // including an ID_BTN_OK message that was not a click and an ID_EDIT_NAME
    // message that was not a text change.
    return 1;
}

int NewProfileWnd::CreateNewProfile()
{
    // RVA 0x4C2670
    if ((m_gameDataFlags & 1) == 0)
    {
        return 1;
    }

    CStr const name = m_editName->GetText();
    if (name.empty())
    {
        M3D_APP->RunMsgBoxDlg(CStr(), M3D_APP->GetStringByStringId0("EmptyProfileName"), m3d::ui::MBX_OK, false);
        return 0;
    }

    // A trailing space would survive into the folder name and confuse the
    // profile lookup, so it is rejected outright.
    if (name.c_str()[strlen(name.c_str()) - 1] == ' ')
    {
        M3D_APP->RunMsgBoxDlg(CStr(), M3D_APP->GetStringByStringId0("InvalidProfileName"), m3d::ui::MBX_OK, false);
        return 0;
    }

    if (name.findOneOf(SERVICE_SYMBOLS, 0) != -1)
    {
        // NOTE: the shipped code formats the message into itself, passing its
        // own buffer as the format string.
        CStr msg = M3D_APP->GetStringByStringId0("ServiceSymbolsUsed");
        msg.format(msg.c_str(), help::GetServiceSymbolsForVisualisation().c_str());
        M3D_APP->RunMsgBoxDlg(CStr(), msg, m3d::ui::MBX_OK, false);
        return 0;
    }

    auto* profileManager = M3D_APP->GetProfileManager();
    if (profileManager->GetProfileByName(name))
    {
        M3D_APP->RunMsgBoxDlg(CStr(), M3D_APP->GetStringByStringId0("ProfileExists"), m3d::ui::MBX_OK, false);
        return 0;
    }

    auto const* profile = profileManager->CreateNewProfile(name);
    if (!profile || !profileManager->SetCurProfile(profile->GetName()))
    {
        M3D_APP->RunMsgBoxDlg(CStr(), M3D_APP->GetStringByStringId0("ProfileCreateError"), m3d::ui::MBX_OK, false);
        // NOTE: unlike every validation failure above, a profile that could not
        // actually be created still reports success, so the caller closes the
        // window anyway.
    }
    return 1;
}

int NewProfileWnd::OnKey(unsigned short key, unsigned char scanCode, unsigned state)
{
    // RVA 0x4C2AC0
    if (state && (m_gameDataFlags & 1) != 0)
    {
        if ((key & 0xff) == m3d::KBD_ESC)
        {
            // Escape only backs out while the Cancel button is enabled, i.e.
            // while there is a profile to fall back on.
            if ((m_btnCancel->GetStyle() & m3d::ui::WS_DISABLE) == 0)
            {
                M3D_APP->m_pInterfaceManager->ShowWindow(m_guiId, false, false, false, false, nullptr);
            }
            return 1;
        }
        if ((key & 0xff) == m3d::KBD_ENTER)
        {
            if (CreateNewProfile())
            {
                int hideMode = 1;
                M3D_APP->m_pInterfaceManager->ShowWindow(m_guiId, false, false, false, false, &hideMode);
            }
            return 1;
        }
    }
    return ModalWnd::OnKey(key, scanCode, state);
}

void NewProfileWnd::OnEditChange()
{
    // RVA 0x4C2B60 - the profile name may not grow wider than its edit box, so
    // the symbol that overflowed it is taken straight back out again.
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    CStr const name = m_editName->GetText();
    auto const editClientB = m_editName->GetClientBounds();
    auto const nameSz =
        GetGfxServer()->MeasureText(name, m_editName->GetDefaultFont(), m3d::TW_NOWRAP, EDIT_MEASURE_WIDTH);

    if (nameSz.x > editClientB.width && !name.empty())
    {
        m_editName->DeleteSymbol(m_editName->GetMaxCursorPos() - 1);
        if (m_editName->GetCursorPosition() > m_editName->GetMaxCursorPos())
        {
            m_editName->DecrementCursorPosition();
        }
    }
}
