#include "changeprofilewnd.h"
#include <core/log.h>
#include <m3dapp.h>
#include <ui/button.h>
#include <ui/listbox.h>
#include <ui/msgbox.h>
#include <ui/wndstation.h>

#include "game/m3dgame.h"
#include "game/profile.h"
#include "game/uimanager/truxxuimanager.h"
#include "game/uimanager/uidefs.h"
#include "game/uimisc/guihelper.h"

RT_CLASS_EXPORTS_BEGIN(ChangeProfileWnd)
    RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(ChangeProfileWnd);

ChangeProfileWnd::AuxInfo::AuxInfo()
{
    // RVA 0x4AACB0
    m_wndProfileListName = "wndProfileList";
    m_btnChoseName = "btnChose";
    m_btnDeleteName = "btnDelete";
    m_lblCurProfileName = "lblCurProfileName";
    m_strIdCurProfile = "CurProfile";
    m_curProfileNameColor = 0xFF904040;
}

m3d::Object* ChangeProfileWnd::Clone()
{
    // RVA 0x4AABF0
    return new ChangeProfileWnd(*this);
}

ChangeProfileWnd::~ChangeProfileWnd()
{
    // RVA 0x4AAF30 - only the aux info and the ModalWnd base need unwinding.
}

m3d::Object* ChangeProfileWnd::CreateObject()
{
    return new ChangeProfileWnd;
}

m3d::Class* ChangeProfileWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ModalWnd);
}

m3d::Class* ChangeProfileWnd::GetClass() const
{
    return RT_CLASS_LOCAL(ChangeProfileWnd);
}

CStr ChangeProfileWnd::GetSelectedProfileName() const
{
    if ((m_gameDataFlags & 1) != 0)
    {
        auto const curSel = m_wndProfileList->GetCurSel();
        if (curSel != -1)
        {
            return m_wndProfileList->GetItem(curSel);
        }
    }
    return {};
}

ChangeProfileWnd::ChangeProfileWnd()
{
    // RVA 0x4AAE00
    m_wndProfileList = nullptr;
    m_btnChose = nullptr;
    m_btnDelete = nullptr;
    m_lblCurProfile = nullptr;
}

ChangeProfileWnd::ChangeProfileWnd(ChangeProfileWnd const&)
{
    // RVA 0x4AAF10 - the shipped copy constructor copies nothing from rhs and,
    // unlike the default one, leaves the four child window pointers
    // uninitialised.
}

int ChangeProfileWnd::GameDataSetup()
{
    using namespace m3d::ui;
    auto res = 1;
    if ((m_gameDataFlags & 2) == 0)
    {
        auto wndProfileList = dynamic_cast<StringsListBoxWnd*>(GetChildByName(m_aif.m_wndProfileListName));
        if (wndProfileList && wndProfileList->IsKindOf(RT_CLASS_LOCAL(StringsListBoxWnd)))
        {
            m_wndProfileList = wndProfileList;
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_wndProfileListName + " is not found or incorrect type");
            res = 0;
        }

        auto btnChose = dynamic_cast<ButtonWnd*>(GetChildByName(m_aif.m_btnChoseName));
        if (btnChose && btnChose->IsKindOf(RT_CLASS_LOCAL(ButtonWnd)))
        {
            m_btnChose = btnChose;
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_btnChoseName + " is not found or incorrect type");
            res = 0;
        }

        auto btnDelete = dynamic_cast<ButtonWnd*>(GetChildByName(m_aif.m_btnDeleteName));
        if (btnDelete && btnDelete->IsKindOf(RT_CLASS_LOCAL(ButtonWnd)))
        {
            m_btnDelete = btnDelete;
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_btnDeleteName + " is not found or incorrect type");
            res = 0;
        }

        auto lblCurProfile = dynamic_cast<Wnd*>(GetChildByName(m_aif.m_lblCurProfileName));
        if (lblCurProfile && lblCurProfile->IsKindOf(RT_CLASS_LOCAL(Wnd)))
        {
            m_lblCurProfile = lblCurProfile;
            if (res)
            {
                m_gameDataFlags |= 1u;
            }
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_lblCurProfileName + " is not found or incorrect type");
        }
    }
    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }
    M3D_LOG_INFO("ChangeProfileWnd: error - fail to init because of a bad resource");
    return 0;
}

int ChangeProfileWnd::OnBeforeAddToWndStation()
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return Wnd::OnBeforeAddToWndStation();
    }
    Clear();
    OnProfilesListChanged();
    OnCurProfileChanged();
    return Wnd::OnBeforeAddToWndStation();
}

void ChangeProfileWnd::OnChangeProfileSelection()
{
    if ((m_gameDataFlags & 1) != 0)
    {
        if (!GetSelectedProfileName().empty())
        {
            m_btnChose->EnableWindow(true);
            m_btnDelete->EnableWindow(true);
        }
        else
        {
            m_btnChose->EnableWindow(false);
            m_btnDelete->EnableWindow(false);
        }
    }
}

int ChangeProfileWnd::OnKey(unsigned short key, unsigned char scanCode, unsigned state)
{
    // RVA 0x4ABD90 - Enter picks the highlighted profile, but only when there is
    // one highlighted.
    if (static_cast<unsigned char>(key) != 4 || !state)
    {
        return ModalWnd::OnKey(key, scanCode, state);
    }
    if (GetSelectedProfileName().empty())
    {
        return ModalWnd::OnKey(key, scanCode, state);
    }
    ChoseProfileAndExit();
    return 1;
}

int ChangeProfileWnd::ChoseProfileAndExit()
{
    // RVA 0x4AB730 - the window closes whether or not the switch succeeded.
    int const res = ChoseProfile();
    M3D_APP->m_pInterfaceManager->ShowWindow(m_guiId, false, false, false, false, nullptr);
    return res;
}

void ChangeProfileWnd::OnProfilesListChanged()
{
    if ((m_gameDataFlags & 1) != 0)
    {
        m_wndProfileList->RemoveAllItems();
        auto const names = M3D_APP->GetProfileManager()->GetProfilesNames();
        for (auto const& name : names)
        {
            m_wndProfileList->AddItem(name);
        }
        // The shipped code re-reads the list box's own item count here; with no
        // public accessor for it, the source vector is the same thing.
        if (!names.empty())
        {
            m_wndProfileList->SetCurSel(0);
        }
    }
}

int ChangeProfileWnd::GameDataUpdate(void* data, int dataType)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (dataType == 40)
    {
        if (IsChildOf(M3D_APP))
        {
            OnCurProfileChanged();
        }
    }
    else if (dataType == 41)
    {
        if (IsChildOf(M3D_APP))
        {
            OnProfilesListChanged();
            return 1;
        }
    }
    return 1;
}

int ChangeProfileWnd::ChoseProfile()
{
    // RVA 0x4AB600
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }

    CStr const selProfileName = GetSelectedProfileName();
    if (M3D_APP->GetProfileManager()->SetCurProfile(selProfileName))
    {
        return 1;
    }

    // NOTE: the shipped code really does report the *create* error string when
    // switching to an existing profile fails.
    M3D_APP->RunMsgBoxDlg(CStr(), M3D_APP->GetStringByStringId0(CStr("ProfileCreateError")), 1u, false);
    return 0;
}

void ChangeProfileWnd::OnCurProfileChanged()
{
    if ((m_gameDataFlags & 1) != 0)
    {
        if (auto const profile = M3D_APP->GetProfileManager()->GetCurProfile())
        {
            auto const text = M3D_APP->GetStringByStringId0(m_aif.m_strIdCurProfile) + ": " +
                help::Color2Str(m_aif.m_curProfileNameColor) + profile->GetName();
            m_lblCurProfile->SetText(text);
        }
        else
        {
            m_lblCurProfile->SetText(M3D_APP->GetStringByStringId0(m_aif.m_strIdCurProfile));
        }
    }
}

int ChangeProfileWnd::DeleteProfile()
{
    // RVA 0x4AB760
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }

    CStr const selProfileName = GetSelectedProfileName();
    if (M3D_APP->RunMsgBoxDlg(CStr(), M3D_APP->GetStringByStringId0(CStr("DeleteProfileWarning")), 2u, false) !=
        m3d::ui::MBX_RET_YES)
    {
        return 1;
    }
    if (M3D_APP->GetProfileManager()->DeleteProfile(selProfileName))
    {
        return 1;
    }

    M3D_APP->RunMsgBoxDlg(CStr(), M3D_APP->GetStringByStringId0(CStr("ProfileDeleteError")), 1u, false);
    return 0;
}

void ChangeProfileWnd::Clear()
{
    if ((m_gameDataFlags & 1) != 0)
    {
        m_wndProfileList->RemoveAllItems();
        m_lblCurProfile->SetText(M3D_APP->GetStringByStringId0(m_aif.m_strIdCurProfile));
    }
}

int ChangeProfileWnd::CreateProfile()
{
    // RVA 0x4AB950 - hands over to the modal "new profile" dialog and closes
    // this window when it reports success.
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }

    int ret = 3;
    M3D_APP->m_pInterfaceManager->ShowWindow(IW_WND_NEW_PROFILE, true, true, true, true, &ret);
    if (ret == 1)
    {
        M3D_APP->m_pInterfaceManager->ShowWindow(m_guiId, false, false, false, false, nullptr);
    }
    return 1;
}

void ChangeProfileWnd::FullUpdate()
{
    // RVA 0x4ABAD0
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    Clear();
    OnProfilesListChanged();
    OnCurProfileChanged();
}

int ChangeProfileWnd::OnWndNotify(m3d::ui::Wnd* from, unsigned id, unsigned msg, m3d::AIParam const& data)
{
    if (ModalWnd::OnWndNotify(from, id, msg, data))
        return 1;
    if ((m_gameDataFlags & 1) == 0)
        return 0;
    switch (id)
    {
    case 0x493E0u:
        if (msg != 1)
            return 0;
        ChoseProfileAndExit();
        return 1;
    case 0x493E1u:
        if (msg != 1)
            return 0;
        DeleteProfile();
        return 1;
    case 0x493E2u:
        if (msg != 1)
            return 0;
        CreateProfile();
        return 1;
    case 0x493E3u:
        if (msg == 4)
        {
            ChoseProfileAndExit();
            return 1;
        }
        if (msg != 5)
            return 0;
        OnChangeProfileSelection();
        break;
    default:
        return 0;
    }
    return 1;
}
