#include "changeprofilewnd.h"
#include <core/log.h>
#include <ui/button.h>
#include <ui/listbox.h>

RT_CLASS_DEFINE(ChangeProfileWnd);

ChangeProfileWnd::AuxInfo::AuxInfo()
{
}

m3d::Object* ChangeProfileWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

ChangeProfileWnd::~ChangeProfileWnd()
{
    throw std::logic_error("Not implemented");
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
    throw std::logic_error("Not implemented");
}

ChangeProfileWnd::ChangeProfileWnd()
{
}

ChangeProfileWnd::ChangeProfileWnd(ChangeProfileWnd const&)
{
    throw std::logic_error("Not implemented");
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
    throw std::logic_error("Not implemented");
}

void ChangeProfileWnd::OnChangeProfileSelection()
{
    throw std::logic_error("Not implemented");
}

int ChangeProfileWnd::OnKey(unsigned short, unsigned char, unsigned)
{
    throw std::logic_error("Not implemented");
}

int ChangeProfileWnd::ChoseProfileAndExit()
{
    throw std::logic_error("Not implemented");
}

void ChangeProfileWnd::OnProfilesListChanged()
{
    throw std::logic_error("Not implemented");
}

int ChangeProfileWnd::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

int ChangeProfileWnd::ChoseProfile()
{
    throw std::logic_error("Not implemented");
}

void ChangeProfileWnd::OnCurProfileChanged()
{
    throw std::logic_error("Not implemented");
}

int ChangeProfileWnd::DeleteProfile()
{
    throw std::logic_error("Not implemented");
}

void ChangeProfileWnd::Clear()
{
    throw std::logic_error("Not implemented");
}

int ChangeProfileWnd::CreateProfile()
{
    throw std::logic_error("Not implemented");
}

void ChangeProfileWnd::FullUpdate()
{
    throw std::logic_error("Not implemented");
}

int ChangeProfileWnd::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}
