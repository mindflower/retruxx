#include "bardlg.h"

RT_CLASS_EXPORTS_BEGIN(BarDlg)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(BarDlg);

BarDlg::AuxInfo::AuxInfo()
{
    m_lstNPCsPatternName = "wndNpcList";
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
    return new BarDlg;
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
    return RT_CLASS_LOCAL(BarDlg);
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
    m_barId = -1;
    m_lstNPCs = 0;
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
    // TODO: implement BarDlg::GameDataSetup
    // RETRUXX_NOT_IMPLEMENTED;
    return 1;
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
