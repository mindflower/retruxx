#include "towndlg.h"

RT_CLASS_EXPORT_METHOD_DEFINE(TownDlg, SetConditionalClosingForTown)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(TownDlg, IsTownWithConditionalClosing)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORTS_BEGIN(TownDlg)
	RT_CLASS_EXPORT(TownDlg, m3d::METHOD, SetConditionalClosingForTown, "", "", "")
	RT_CLASS_EXPORT(TownDlg, m3d::METHOD, IsTownWithConditionalClosing, "", "", "")
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(TownDlg);

TownDlg::AuxInfo::AuxInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int TownDlg::GameDataLoad(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

TownDlg::ConditionalClosingInfo* TownDlg::GetConditionalClosingInfoForTown(CStr const&, CStr const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* TownDlg::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void TownDlg::FullUpdate()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void TownDlg::ClearConditionalClosingInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* TownDlg::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int TownDlg::OnAfterAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void TownDlg::OnEnterBar(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool TownDlg::IsTownWithConditionalClosing(CStr const&, CStr const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int TownDlg::OnBeforeAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int TownDlg::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int TownDlg::GameDataSave(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void TownDlg::UpdateClanPicture()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void TownDlg::AddConditionalClosingInfo(CStr const&, CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int TownDlg::GameDataSetup()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void TownDlg::OnLeaveBar()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int TownDlg::GameDataUpdate(void*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* TownDlg::GetBaseClass()
{
    return RT_CLASS_LOCAL(ChildPanel);
}

void TownDlg::SetConditionalClosingForTown(CStr const&, CStr const&, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int TownDlg::SetUpForTown(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void TownDlg::ShowForTown(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* TownDlg::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Town* TownDlg::GetTown() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void TownDlg::RemoveConditionalClosingInfo(CStr const&, CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

TownDlg::~TownDlg()
{
    RETRUXX_NOT_IMPLEMENTED;
}

TownDlg::TownDlg()
{
    RETRUXX_NOT_IMPLEMENTED;
}

TownDlg::TownDlg(TownDlg const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}
