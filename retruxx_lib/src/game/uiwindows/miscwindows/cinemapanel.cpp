#include "cinemapanel.h"

#include "autoscrolltextwnd.h"
#include "core/log.h"
#include "ui/image.h"

RT_CLASS_EXPORT_METHOD_DEFINE(CinemaPanel, AddMessage)
{
    auto panel = (CinemaPanel*)context->asObject(0, "CinemaPanel");
    int msgId = context->asInt(1);
    float delay = context->asFloat(2);
    panel->AddMessage(msgId, delay);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(CinemaPanel, ClearMessages)
{
    auto panel = (CinemaPanel*)context->asObject(0, "CinemaPanel");
    panel->ClearMessages();
    return 1;
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
    m_wndPortraitName = "wndPortrait_";
    m_wndTextName = "wndText";
    m_wndPortraitOverlayName = "wndPortraitOverlay";
    m_wndUpPanelName = "UpPanel";
    m_wndDownPanelName = "DownPanel";
    m_wndScrollImageName = "wndScrollImage";
    m_wndScrollImageUpOverlayName = "wndScrollImageUpOverlay";
    m_wndScrollImageDownOverlayName = "wndScrollImageDownOverlay";
    m_wndScrollTextName = "wndScrollText";
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
    // TODO: implement CinemaPanel::AddMessage
    //RETRUXX_NOT_IMPLEMENTED;
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
    // TODO: implement CinemaPanel::ClearMessages
    //RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* CinemaPanel::CreateObject()
{
    return new CinemaPanel;
}

CinemaPanel::~CinemaPanel()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* CinemaPanel::GetClass() const
{
    return RT_CLASS_LOCAL(CinemaPanel);
}

void CinemaPanel::Clear()
{
    // TODO: implement CinemaPanel::Clear
    // RETRUXX_NOT_IMPLEMENTED;
}

void CinemaPanel::ClearNormal()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CinemaPanel::OnPaint(m3d::ui::DrawInfo const&)
{
    // TODO: implement CinemaPanel::OnPaint
    // RETRUXX_NOT_IMPLEMENTED;
    return 1;
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
    // TODO: implement CinemaPanel::GameDataSetup
    return 1;
    if ((m_gameDataFlags & 2) == 0)
    {
        auto child = GetChildByName(m_aif.m_wndTextName);
        if (child && child->IsKindOf(&m3d::ui::Wnd::m_classWnd))
        {
            m_wndText = dynamic_cast<Wnd*>(child);
        }
        else
        {
            M3D_LOG_ERR("Get control error: control " + m_aif.m_wndTextName + " is not found or incorrect type");
        }

        child = GetChildByName(m_aif.m_wndUpPanelName);
        if (child && child->IsKindOf(&m3d::ui::Wnd::m_classWnd))
        {
            m_wndUpPanel = dynamic_cast<Wnd*>(child);
        }
        else
        {
            M3D_LOG_ERR("Get control error: control " + m_aif.m_wndUpPanelName + " is not found or incorrect type");
        }

        child = GetChildByName(m_aif.m_wndDownPanelName);
        if (child && child->IsKindOf(&m3d::ui::Wnd::m_classWnd))
        {
            m_wndDownPanel = dynamic_cast<Wnd*>(child);
        }
        else
        {
            M3D_LOG_ERR("Get control error: control " + m_aif.m_wndDownPanelName + " is not found or incorrect type");
        }

        child = GetChildByName(m_aif.m_wndScrollImageName);
        if (child && child->IsKindOf(&m3d::ui::ImageWnd::m_classWnd))
        {
            m_wndScrollImage = dynamic_cast<m3d::ui::ImageWnd*>(child);
        }
        else
        {
            M3D_LOG_ERR("Get control error: control " + m_aif.m_wndScrollImageName + " is not found or incorrect type");
        }

        child = GetChildByName(m_aif.m_wndScrollImageUpOverlayName);
        if (child && child->IsKindOf(&m3d::ui::ImageWnd::m_classWnd))
        {
            m_wndScrollImageUpOverlay = dynamic_cast<m3d::ui::ImageWnd*>(child);
        }
        else
        {
            M3D_LOG_ERR("Get control error: control " + m_aif.m_wndScrollImageUpOverlayName + " is not found or incorrect type");
        }

        child = GetChildByName(m_aif.m_wndScrollImageDownOverlayName);
        if (child && child->IsKindOf(&m3d::ui::ImageWnd::m_classWnd))
        {
            m_wndScrollImageDownOverlay = dynamic_cast<m3d::ui::ImageWnd*>(child);
        }
        else
        {
            M3D_LOG_ERR("Get control error: control " + m_aif.m_wndScrollImageDownOverlayName + " is not found or incorrect type");
        }

        child = GetChildByName(m_aif.m_wndPortraitOverlayName);
        if (child && child->IsKindOf(&m3d::ui::ImageWnd::m_classWnd))
        {
            m_wndPortraitOverlay = dynamic_cast<m3d::ui::ImageWnd*>(child);
        }
        else
        {
            M3D_LOG_ERR("Get control error: control " + m_aif.m_wndPortraitOverlayName + " is not found or incorrect type");
        }

        child = GetChildByName(m_aif.m_wndScrollTextName);
        if (child && child->IsKindOf(&m3d::ui::Wnd::m_classWnd))
        {
            auto autoScroll = (AutoScrollTextWnd*)M3D_KERNEL->New("AutoScrollTextWnd");
            if (autoScroll)
            {
                if (!autoScroll->CreateFromPattern(dynamic_cast<m3d::ui::Wnd*>(child), true))
                {
                    M3D_LOG_ERR("Make control error: cannot create " + m_aif.m_wndScrollTextName + " from pattern class");
                }
            }
            else
            {
                M3D_LOG_ERR("Make control error: cannot create " + m_aif.m_wndScrollTextName + " - cannot find rtti class AutoScrollTextWnd");
            }
        }
        else
        {
            M3D_LOG_ERR("Make control error: control " + m_aif.m_wndScrollTextName + " is not found or incorrect type");
        }

        // ....
    }

    return 1;
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
    m_soundTableId = -1;
    m_soundChannelId = -1;
    m_curMessageStartTime = 0;
    m_curMessageEndTime = 0;
    m_bIsShowingMessage = 0;
    m_bSkipMessage = 0;
    m_wndText = 0;
    m_panelType = PANELTYPE_NORMAL;
    m_wndUpPanel = 0;
    m_wndDownPanel = 0;
    m_wndScrollImage = 0;
    m_wndScrollImageUpOverlay = 0;
    m_wndScrollImageDownOverlay = 0;
    m_wndPortraitOverlay = 0;
    m_minTimeToExists = 10;
    m_wndsPortraits[0] = 0;
    m_wndsPortraits[1] = 0;
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
