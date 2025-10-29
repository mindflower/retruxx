#include "changeprofilewnd.h"
#include <core/log.h>
#include <ui/button.h>
#include <ui/listbox.h>

#include "game/m3dgame.h"
#include "game/profile.h"
#include "game/uimisc/guihelper.h"

RT_CLASS_EXPORTS_BEGIN(ChangeProfileWnd)
    RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(ChangeProfileWnd);

ChangeProfileWnd::AuxInfo::AuxInfo()
{
}

m3d::Object* ChangeProfileWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ChangeProfileWnd::~ChangeProfileWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
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
}

ChangeProfileWnd::ChangeProfileWnd(ChangeProfileWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
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

int ChangeProfileWnd::OnKey(unsigned short, unsigned char, unsigned)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ChangeProfileWnd::ChoseProfileAndExit()
{
    RETRUXX_NOT_IMPLEMENTED;
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
        m_wndProfileList->SetCurSel(0);
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
    RETRUXX_NOT_IMPLEMENTED;
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
    RETRUXX_NOT_IMPLEMENTED;
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
    RETRUXX_NOT_IMPLEMENTED;
}

void ChangeProfileWnd::FullUpdate()
{
    RETRUXX_NOT_IMPLEMENTED;
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
