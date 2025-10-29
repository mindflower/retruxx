#include "bardlg.h"

RT_CLASS_EXPORTS_BEGIN(BarDlg)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(BarDlg);

BarDlg::AuxInfo::AuxInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* BarDlg::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Bar* BarDlg::GetBar() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* BarDlg::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* BarDlg::GetBaseClass()
{
    return RT_CLASS_LOCAL(ChildPanel);
}

int BarDlg::SetUpForBar(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* BarDlg::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

BarDlg::~BarDlg()
{
    RETRUXX_NOT_IMPLEMENTED;
}

BarDlg::BarDlg(BarDlg const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

BarDlg::BarDlg()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BarDlg::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void BarDlg::FullUpdate()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BarDlg::GameDataSetup()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BarDlg::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BarDlg::OnAfterRemoveFromWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BarDlg::OnBeginConversation(ai::Npc*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BarDlg::OnAfterAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BarDlg::OnBeforeAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}
