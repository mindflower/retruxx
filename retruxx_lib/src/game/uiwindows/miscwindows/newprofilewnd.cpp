#include "newprofilewnd.h"
#include <core/log.h>
#include <ui/button.h>
#include <ui/edit.h>

RT_CLASS_EXPORTS_BEGIN(NewProfileWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(NewProfileWnd);

NewProfileWnd::AuxInfo::AuxInfo()
{
}

m3d::Object* NewProfileWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* NewProfileWnd::GetClass() const
{
    return RT_CLASS_LOCAL(NewProfileWnd);
}

m3d::Object* NewProfileWnd::CreateObject()
{
    return new NewProfileWnd;
}

m3d::Class* NewProfileWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ModalWnd);
}

NewProfileWnd::~NewProfileWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int NewProfileWnd::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int NewProfileWnd::OnBeforeAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int NewProfileWnd::GameDataSetup()
{
    using namespace m3d::ui;
    auto res = 1;
    if ((m_gameDataFlags & 2) == 0)
    {
        auto editWnd = dynamic_cast<EditWnd*>(GetChildByName(m_aif.m_editNameName));
        if (editWnd && editWnd->IsKindOf(RT_CLASS_LOCAL(EditWnd)))
        {
            m_editName = editWnd;
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_editNameName + " is not found or incorrect type");
            res = 0;
        }

        auto btnCancel = dynamic_cast<ButtonWnd*>(GetChildByName(m_aif.m_btnCancelName));
        if (btnCancel && btnCancel->IsKindOf(RT_CLASS_LOCAL(ButtonWnd)))
        {
            m_btnCancel = btnCancel;
            if (res)
            {
                m_gameDataFlags |= 1;
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

void NewProfileWnd::OnEditChange()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int NewProfileWnd::OnKey(unsigned short, unsigned char, unsigned)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int NewProfileWnd::CreateNewProfile()
{
    RETRUXX_NOT_IMPLEMENTED;
}

NewProfileWnd::NewProfileWnd(NewProfileWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

NewProfileWnd::NewProfileWnd()
{
}
