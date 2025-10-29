#include "cinemapanel.h"

RT_CLASS_EXPORT_METHOD_DEFINE(CinemaPanel, AddMessage)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(CinemaPanel, ClearMessages)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORTS_BEGIN(CinemaPanel)
    RT_CLASS_EXPORT(CinemaPanel, m3d::METHOD, AddMessage, "", "", "")
    RT_CLASS_EXPORT(CinemaPanel, m3d::METHOD, ClearMessages, "", "", "")
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(CinemaPanel);

CinemaPanel::MessageTimeInfo::MessageTimeInfo(int, float)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CinemaPanel::AuxInfo::AuxInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* CinemaPanel::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Object* CinemaPanel::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CinemaPanel::AddMessage(int, float)
{
    RETRUXX_NOT_IMPLEMENTED;
}

unsigned CinemaPanel::GetTimeToTheEndOfMsg()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool CinemaPanel::MessageQueueIsEmpty() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CinemaPanel::OnHide()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CinemaPanel::SkipMessage()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool CinemaPanel::HasMsg()
{
    return !m_msgInfos.empty() || m_bIsShowingMessage;
}

void CinemaPanel::ClearMessages()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* CinemaPanel::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

CinemaPanel::~CinemaPanel()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* CinemaPanel::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CinemaPanel::Clear()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CinemaPanel::ClearNormal()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CinemaPanel::OnPaint(m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CinemaPanel::SetPanelTypeForMsg(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CinemaPanel::_SetMsg(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CinemaPanel::SetupPortrait(MsgInfo const*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CinemaPanel::ClearBase()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CinemaPanel::HideAllControls()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CinemaPanel::GetControlsByPanelType(PanelType, std::vector<m3d::ui::Wnd*>&, bool) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CinemaPanel::SetupTextScroll(MsgInfo const*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CinemaPanel::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CinemaPanel::GetAllControls(std::vector<m3d::ui::Wnd*>&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CinemaPanel::SetupSound(MsgInfo const*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CinemaPanel::DeleteAllControls()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CinemaPanel::GameDataSetup()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CinemaPanel::SetupTextNormal(MsgInfo const*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CinemaPanel::CinemaPanel(CinemaPanel const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CinemaPanel::CinemaPanel()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CinemaPanel::InitControlsForMsgScroll(MsgInfo const*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CinemaPanel::InitControlsForMsgNormal(MsgInfo const*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CinemaPanel::UpdateAnimation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CinemaPanel::SetupTime(MsgInfo const*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CinemaPanel::ShowControlsForPanelType(PanelType)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CinemaPanel::ClearScroll()
{
    RETRUXX_NOT_IMPLEMENTED;
}

CinemaPanel::PanelType CinemaPanel::GetPanelTypeByMsgType(MsgInfo::MsgType) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CinemaPanel::InitControlsForMsgBase(MsgInfo const*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CinemaPanel::SetupImagesScroll(MsgInfo const*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CinemaPanel::StopSound()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CinemaPanel::SetPanelType(PanelType)
{
    RETRUXX_NOT_IMPLEMENTED;
}
